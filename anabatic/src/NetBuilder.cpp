// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |         A n a b a t i c  -  Routing Toolbox                     |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./NetBuilder.cpp"                         |
// +-----------------------------------------------------------------+


#include <cstdlib>
#include <sstream>
#include "hurricane/Bug.h"
#include "hurricane/Breakpoint.h"
#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/DebugSession.h"
#include "hurricane/Layer.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/RegularLayer.h"
#include "hurricane/Technology.h"
#include "hurricane/DataBase.h"
#include "hurricane/Net.h"
#include "hurricane/NetExternalComponents.h"
#include "hurricane/NetRoutingProperty.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/RoutingPads.h"
#include "hurricane/Pad.h"
#include "hurricane/Plug.h"
#include "hurricane/Cell.h"
#include "hurricane/Instance.h"
#include "hurricane/Vertical.h"
#include "hurricane/Horizontal.h"
#include "crlcore/AllianceFramework.h"
#include "crlcore/RoutingGauge.h"
#include "anabatic/AutoContactTerminal.h"
#include "anabatic/AutoContactTurn.h"
#include "anabatic/AutoContactHTee.h"
#include "anabatic/AutoContactVTee.h"
#include "anabatic/AutoSegment.h"
#include "anabatic/NetBuilder.h"
#include "anabatic/NetBuilderHV.h"
#include "anabatic/AnabaticEngine.h"


namespace {

  using Anabatic::AutoContactTerminal;
  using Hurricane::Breakpoint;


  using namespace std;
  using namespace CRL;
  using namespace Hurricane;
  using namespace Anabatic;


  // ---------------------------------------------------------------
  // Local Variables.

  const char* invalidGCell =
    "Anabatic::NetBuilder () :\n\n"
    "    No GCell under point.\n";

  const char* mismatchGCell =
    "Anabatic::NetBuilder () :\n\n"
    "    Contacts under two different GCells.\n";

  const char* missingGCell =
    "Anabatic::NetBuilder () :\n\n"
    "    No Contact in GCell.\n";


  
// -------------------------------------------------------------------
// Class :  "NetBuilder::SortRpByX".

  class SortRpByX {
    public:
      inline       SortRpByX  ( uint64_t flags );
      inline bool  operator() ( RoutingPad* rp1, RoutingPad* rp2 );
    private:
      uint64_t  _flags;
  };


  inline  SortRpByX::SortRpByX ( uint64_t flags )
    : _flags(flags)
  { }


  inline bool  SortRpByX::operator() ( RoutingPad* rp1, RoutingPad* rp2 )
  {
    DbU::Unit x1 = rp1->getCenter().getX();
    DbU::Unit x2 = rp2->getCenter().getX();

    if (x1 == x2) return false;
    return (_flags & NetBuilder::SortDecreasing) xor (x1 < x2);
  }


// -------------------------------------------------------------------
// Class  :  "NetBuilder::SortRpByY".

  class SortRpByY {
    public:
      inline       SortRpByY  ( uint64_t flags );
      inline bool  operator() ( RoutingPad* rp1, RoutingPad* rp2 );
    protected:
      uint64_t  _flags;
  };


  inline  SortRpByY::SortRpByY  ( uint64_t flags )
    : _flags(flags)
  { }


  inline bool  SortRpByY::operator() ( RoutingPad* rp1, RoutingPad* rp2 )
  {
    DbU::Unit y1 = rp1->getCenter().getY();
    DbU::Unit y2 = rp2->getCenter().getY();

    if (y1 == y2) return false;
    return (_flags & NetBuilder::SortDecreasing) xor (y1 < y2);
  }


// -------------------------------------------------------------------
// Class :  "NetBuilder::SortHookByX".
  
  class SortHookByX {
    public:
      inline       SortHookByX  ( uint64_t flags );
      inline bool  operator()   ( Hook* h1, Hook* h2 );
    protected:
      uint64_t  _flags;
  };


  inline  SortHookByX::SortHookByX ( uint64_t flags )
    : _flags(flags)
  { }


  inline bool  SortHookByX::operator() ( Hook* h1, Hook* h2 )
  {
    DbU::Unit   x1  = 0;
    DbU::Unit   x2  = 0;
    Horizontal* hh1 = dynamic_cast<Horizontal*>(h1->getComponent());
    Horizontal* hh2 = dynamic_cast<Horizontal*>(h2->getComponent());
    Vertical*   vv1 = dynamic_cast<Vertical*>  (h1->getComponent());
    Vertical*   vv2 = dynamic_cast<Vertical*>  (h2->getComponent());

    if      (hh1) x1 = std::min( hh1->getSource()->getX(), hh1->getTarget()->getX() );
    else if (vv1) x1 = vv1->getX();
    else          x1 = h1->getComponent()->getCenter().getX();

    if      (hh2) x2 = std::min( hh2->getSource()->getX(), hh2->getTarget()->getX() );
    else if (vv2) x2 = vv2->getX();
    else          x2 = h2->getComponent()->getCenter().getX();

    if (x1 == x2) return false;
    return (_flags & NetBuilder::SortDecreasing) xor (x1 < x2);
  }


// -------------------------------------------------------------------
// Class :  "NetBuilder::SortHookByY".

  class SortHookByY {
    public:
      inline       SortHookByY  ( uint64_t flags );
      inline bool  operator()   ( Hook* h1, Hook* h2 );
    protected:
      uint64_t  _flags;
  };


  inline  SortHookByY::SortHookByY ( uint64_t flags )
    : _flags(flags)
  { }


  inline bool  SortHookByY::operator() ( Hook* h1, Hook* h2 )
  {
    DbU::Unit   y1  = 0;
    DbU::Unit   y2  = 0;
    Horizontal* hh1 = dynamic_cast<Horizontal*>(h1->getComponent());
    Horizontal* hh2 = dynamic_cast<Horizontal*>(h2->getComponent());
    Vertical*   vv1 = dynamic_cast<Vertical*>  (h1->getComponent());
    Vertical*   vv2 = dynamic_cast<Vertical*>  (h2->getComponent());

    if      (vv1) y1 = std::min( vv1->getSource()->getY(), vv1->getTarget()->getY() );
    else if (hh1) y1 = hh1->getY();
    else          y1 = h1->getComponent()->getCenter().getX();

    if      (vv2) y2 = std::min( vv2->getSource()->getY(), vv2->getTarget()->getY() );
    else if (hh2) y2 = hh2->getY();
    else          y2 = h2->getComponent()->getCenter().getY();

    if (y1 == y2) return false;
    return (_flags & NetBuilder::SortDecreasing) xor (y1 < y2);
  }


}  // Anonymous namespace.


namespace Anabatic {

  using Hurricane::Name;
  using Hurricane::DebugSession;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Bug;

  
// -------------------------------------------------------------------
// Class  :  "NetBuilder".


  void  NetBuilder::getPositions ( Component* anchor, Point& source, Point& target )
  {
    Segment* segment = dynamic_cast<Segment*>( anchor );
    if (segment) {
      source = segment->getSourcePosition();
      target = segment->getTargetPosition();
      return;
    }

    RoutingPad* rp = dynamic_cast<RoutingPad*>( anchor );
    if (rp) {
      source = rp->getSourcePosition();
      target = rp->getTargetPosition();
      return;
    }

    source = anchor->getPosition();
    target = anchor->getPosition();
  }


  uint64_t  NetBuilder::checkRoutingPadSize ( Component* anchor )
  {
    Point  source;
    Point  target;

    size_t anchorDepth = Session::getLayerDepth( anchor->getLayer() );
    if (anchorDepth == 0) ++anchorDepth;

    NetBuilder::getPositions( anchor, source, target );

    DbU::Unit width  = abs( target.getX() - source.getX() );
    DbU::Unit height = abs( target.getY() - source.getY() );

    uint64_t flags = 0;
    flags |= (width  < 3*Session::getPitch(anchorDepth)) ? NetBuilder::HSmall   : 0;
    flags |= (height < 3*Session::getPitch(anchorDepth)) ? NetBuilder::VSmall   : 0;
    flags |= ((width == 0) && (height == 0))             ? NetBuilder::Punctual : 0;

    cdebug_log(145,0) << "::checkRoutingPadSize(): pitch[" << anchorDepth << "]:"
               << DbU::getValueString(Session::getPitch(anchorDepth)) << " "
               << ((flags & NetBuilder::HSmall) ? "HSmall " : " ")
               << ((flags & NetBuilder::VSmall) ? "VSmall " : " ")
               << endl;

    return flags;
  }


  Hook* NetBuilder::getSegmentOppositeHook ( Hook* hook )
  {
    Segment* segment = static_cast<Segment*>( hook->getComponent() );
    return segment->getOppositeHook( hook );
  }


  uint64_t  NetBuilder::getSegmentHookType ( Hook* hook )
  {
    Horizontal* horizontal = dynamic_cast<Horizontal*>( hook->getComponent() );
    if (horizontal) {
      if (horizontal->getSourceX() > horizontal->getTargetX())
        cerr << Warning("Bad orientation of %s",getString(horizontal).c_str()) << endl;

      if (dynamic_cast<Segment::SourceHook*>(hook))
        return NetBuilder::EastBound;
      return NetBuilder::WestBound;
    } else {
      Vertical* vertical = dynamic_cast<Vertical*>( hook->getComponent() );
      if (vertical) {
        if (vertical->getSourceY() > vertical->getTargetY())
          cerr << Warning("Bad orientation of %s",getString(vertical).c_str()) << endl;

        if (dynamic_cast<Segment::SourceHook*>(hook))
          return NetBuilder::NorthBound;
      } else {
        cerr << Warning("Unmanaged Hook %s",getString(hook).c_str()) << endl;
      }
    }
    return NetBuilder::SouthBound;
  }


  void  NetBuilder::sortRpByX ( vector<RoutingPad*>& rps, uint64_t flags )
  { sort( rps.begin(), rps.end(), SortRpByX(flags) ); }
  

  void  NetBuilder::sortRpByY ( vector<RoutingPad*>& rps, uint64_t flags )
  { sort( rps.begin(), rps.end(), SortRpByY(flags) ); }
  

  void  NetBuilder::sortHookByX ( vector<Hook*>& hooks, uint64_t flags )
  { sort( hooks.begin(), hooks.end(), SortHookByX(flags) ); }
  

  void  NetBuilder::sortHookByY ( vector<Hook*>& hooks, uint64_t flags )
  { sort( hooks.begin(), hooks.end(), SortHookByY(flags) ); }


  NetBuilder::NetBuilder ()
    : _anabatic              (NULL)
    , _forks                 ()
    , _connexity             ()
    , _topology              (0)
    , _gcell                 (NULL)
    , _sourceContact         (NULL)
    , _southWestContact      (NULL)
    , _northEastContact      (NULL)
    , _fromHook              (NULL)
    , _easts                 ()
    , _wests                 ()
    , _norths                ()
    , _souths                ()
    , _routingPads           ()
    , _routingPadAutoSegments()
    , _toFixSegments         ()
    , _degree                (0)
    , _isTwoMetals           (false)
  { }


  NetBuilder::~NetBuilder ()
  { }


  void  NetBuilder::clear ()
  {
    _connexity.connexity = 0;
    _topology            = 0;
    _gcell               = NULL;
    _sourceContact       = NULL;
    _southWestContact    = NULL;
    _northEastContact    = NULL;
    _fromHook            = NULL;
    _easts                 .clear();
    _wests                 .clear();
    _norths                .clear();
    _souths                .clear();
    _routingPads           .clear();
    _toFixSegments         .clear();
    _routingPadAutoSegments.clear();
  }
  

  void  NetBuilder::fixSegments ()
  {
    for ( size_t i=0 ; i<_toFixSegments.size() ; ++i )
      _toFixSegments[i]->setFlags( AutoSegment::SegFixed );
    _toFixSegments.clear();
  }


  NetBuilder& NetBuilder::setStartHook ( AnabaticEngine* anbt, Hook* fromHook, AutoContact* sourceContact )
  {
    clear();

    _isTwoMetals   = anbt->getConfiguration()->isTwoMetals();
    _sourceContact = sourceContact;
    _fromHook      = fromHook;

    cdebug_log(145,1) << "NetBuilder::setStartHook()" << endl;
    cdebug_log(145,0) << "* _fromHook:     " << fromHook << endl;
    cdebug_log(145,0) << "* _sourceContact:" << sourceContact << endl;

    if (not _fromHook) {
      cdebug_tabw(145,-1);
      return *this;
    }

    Segment* fromSegment = static_cast<Segment*>( _fromHook->getComponent() );
    _net = fromSegment->getNet();

    cdebug_log(145,0) << "For Hooks from fromHook" << endl;
    for ( Hook* hook : fromHook->getHooks() ) {
      cdebug_log(145,0) << "Hook: " << hook << endl;
      cdebug_log(145,0) << "Topology [" << _connexity.connexity << "] = "
                 << "["  << (int)_connexity.fields.globals
                 << "+"  << (int)_connexity.fields.M1     
                 << "+"  << (int)_connexity.fields.M2     
                 << "+"  << (int)_connexity.fields.M3
                 << "+"  << (int)_connexity.fields.Pin
                 << "+"  << (int)_connexity.fields.Pad
                 << "] " << _gcell
                 << endl;

      Segment* toSegment = dynamic_cast<Segment*>( hook->getComponent() );
      if (toSegment) {
        switch ( getSegmentHookType(hook) ) {
          case WestBound:  _wests .push_back( hook ); break;
          case EastBound:  _easts .push_back( hook ); break;
          case SouthBound: _souths.push_back( hook ); break;
          case NorthBound: _norths.push_back( hook ); break;
        }

        _connexity.fields.globals++;
      } else {
        Component*  anchor = hook->getComponent();
        RoutingPad* rp     = dynamic_cast<RoutingPad*>( anchor );

        cdebug_log(145,0) << "| Looking for Anchor:" << anchor << endl;

        if (anchor) {
          Contact* contact = dynamic_cast<Contact*>( anchor );
          cdebug_log(145,0) << "Contact:" << contact << endl;
          if (contact
             and (  Session::getAnabatic()->getConfiguration()->isGContact( anchor->getLayer() )
                 or Session::getAnabatic()->getConfiguration()->isGMetal  ( anchor->getLayer() )) ) {
          // Global routing articulation contact are in <gmetalh> not <gcut> ?
            GCell* gcell = anbt->getGCellUnder( contact->getCenter() );
            cdebug_log(145,0) << "* Global Routing Articulation: " << contact << endl;
            cdebug_log(145,0) << "| " << gcell << endl;
            if (gcell == NULL)
              throw Error( invalidGCell );
            if (_gcell == NULL) _gcell = gcell;
            else if (_gcell != gcell) {
              throw Error( mismatchGCell );
            }

            if (not _gcell->isMatrix()) {
              cdebug_log(145,0) << "* Non-matrix GCell under: " << contact << endl;
              cdebug_log(145,0) << "| " << gcell << endl;
            }
          } else {
            if (rp and AllianceFramework::get()->isPad(rp->_getEntityAsComponent()->getCell())) {
              _connexity.fields.Pad++;
            } else {
              const Layer* layer = anchor->getLayer();
              cdebug_log(145,0) << "rp: " << rp << endl;

              if (not layer) {
                cerr << Error( "RoutingPad is still on it's Plug, routing will be incomplete.\n"
                               "        %s"
                             , getString(anchor).c_str() )
                     << endl;
                continue;
              }

              if      (layer->getMask() == Session::getRoutingLayer(0)->getMask()) _connexity.fields.M1++; // M1 V
              else if (layer->getMask() == Session::getRoutingLayer(1)->getMask()) _connexity.fields.M2++; // M2 H
              else if (layer->getMask() == Session::getRoutingLayer(2)->getMask()) _connexity.fields.M3++; // M3 V
              else if (layer->getMask() == Session::getRoutingLayer(3)->getMask()) _connexity.fields.M2++; // M4 H
              else if (layer->getMask() == Session::getRoutingLayer(4)->getMask()) _connexity.fields.M3++; // M5 V
              else {
                cerr << Warning( "Terminal layer \"%s\" of %s is not managed yet (ignored)."
                               , getString(layer->getName()).c_str()
                               , getString(anchor).c_str() )
                     << endl;
              //continue;
              }

              if (dynamic_cast<Pin*>(rp->getOccurrence().getEntity())) _connexity.fields.Pin++; 
            }

            cdebug_log(145,0) << "| Component to connect: " << anchor << endl;
            _routingPads.push_back( rp );
          }
        }
      }
    }

    if (_gcell->isMatrix()) {
      cdebug_log(145,0) << "east: " << east () << endl;
      cdebug_log(145,0) << "west: " << west () << endl;
      cdebug_log(145,0) << "north:" << north() << endl;
      cdebug_log(145,0) << "south:" << south() << endl;

      if (_connexity.fields.globals == 1) {
        if ( north() or south() ) _topology |= Global_Vertical_End;
        else                      _topology |= Global_Horizontal_End;
      } else if (_connexity.fields.globals == 2) {
        if      ( east () and west () ) _topology |= Global_Horizontal;
        else if ( north() and south() ) _topology |= Global_Vertical;
        else                            _topology |= Global_Turn;
      } else {
        _topology |= Global_Fork;
      }
    }

    cdebug_tabw(145,-1);

    if (_gcell == NULL) throw Error( missingGCell );

    return *this;
  }
    

  bool  NetBuilder::push ( Hook* toHook, AutoContact* contact, uint64_t flags )
  {
    cdebug_log(145,0) << "NetBuilder::push()" << endl;
    cdebug_log(145,0) << "* toHook:   " << toHook << endl;
    cdebug_log(145,0) << "* _fromHook:" << _fromHook << endl;

    if (not toHook or (toHook == _fromHook)) {
      if (contact) {
        if ( (flags & SouthWest) and not _southWestContact ) {
          cdebug_log(145,0) << "Setting _southWestContact:" << contact << endl;
          _southWestContact = contact;
        }
        if ( (flags & NorthEast) and not _northEastContact ) {
          cdebug_log(145,0) << "Setting _northEastContact:" << contact << endl;
          _northEastContact = contact;
        }
      }
      return false;
    }

    Hook* toHookOpposite = getSegmentOppositeHook( toHook );
    cdebug_log(145,0) << "Pushing (to)   " << getString(toHook) << endl;
    cdebug_log(145,0) << "Pushing (from) " << contact << endl;
    _forks.push( toHookOpposite, contact );

    return true;
  }


  void  NetBuilder::construct ()
  {
    cdebug_log(145,1) << "NetBuilder::construct() [" << _connexity.connexity << "] in " << _gcell << endl;

    if (not isTwoMetals()) {
      _southWestContact = NULL;
      _northEastContact = NULL;
    }

    if (not _gcell->isAnalog()) {
      if (isTwoMetals() and not _sourceContact) _fromHook = NULL;
      
      switch ( _connexity.connexity ) {
        case Conn_1G_1Pad:
        case Conn_2G_1Pad:
        case Conn_3G_1Pad: _do_xG_1Pad  (); break;
        // End xG_1Pad cascaded cases.

        case Conn_1G_1M1:  if (_do_1G_1M1()) break;
        case Conn_1G_2M1:
        case Conn_1G_3M1:
        case Conn_1G_4M1:
        case Conn_1G_5M1:   _do_1G_xM1(); break;
        // End 1G_xM1 cascaded cases.

        case Conn_1G_1M2:
        case Conn_1G_2M2:
        case Conn_1G_3M2:
        case Conn_1G_4M2:  _do_xG_xM2(); break;
        // End 1G_xM2 cascaded cases.

        case Conn_1G_1M3:  if (_do_1G_1M3()) break;
        case Conn_1G_2M3:
        case Conn_1G_3M3:
        case Conn_1G_4M3:  _do_xG_xM3(); break;
        // End 1G_xM3 cascaded cases.

        case Conn_2G_1M1:  if (_do_2G_1M1()) break;
        case Conn_2G_2M1:
        case Conn_2G_3M1:
        case Conn_2G_4M1:
        case Conn_2G_5M1:
        case Conn_3G_1M1:  if (_do_xG_1M1()) break;
        case Conn_3G_2M1:
        case Conn_3G_3M1:
        case Conn_3G_4M1:
        case Conn_3G_5M1:
        case Conn_3G_2M3:
        case Conn_3G_3M3:
        case Conn_3G_4M3:
        case Conn_4G_1M1:  if (_do_xG_1M1()) break;
        case Conn_4G_2M1:
        case Conn_4G_3M1:
        case Conn_4G_4M1:  _do_xG_xM1_xM3(); break;
        // End xG_xM1_xM3 cascaded cases.

        case Conn_4G_1M2:  if (_do_4G_1M2()) break;
        case Conn_2G_1M2:
        case Conn_2G_2M2:
        case Conn_2G_3M2:
        case Conn_2G_4M2:
        case Conn_3G_1M2:
        case Conn_3G_2M2:  _do_xG_xM2(); break;
        // End xG_xM2 cascaded cases.

        case Conn_2G_1M3:
        case Conn_2G_2M3:
        case Conn_2G_3M3:
        case Conn_2G_4M3:
        case Conn_3G_1M3:  _do_xG_xM3(); break;
        // End xG_xM3 cascaded cases.

        case Conn_2G:      if (_do_2G()) break;
        case Conn_3G:
        case Conn_4G:     _do_xG(); break;
        // End xG cascaded cases.
        // Optimized specific cases.
        case Conn_1G_1PinM1:  _do_1G_1PinM1 (); break;
        case Conn_2G_1PinM1:  _do_2G_1PinM1 (); break;
        case Conn_1G_1PinM2:  _do_1G_1PinM2 (); break;
        case Conn_2G_1PinM2:
        case Conn_3G_1PinM2:  _do_xG_1PinM2 (); break;
        case Conn_1G_1PinM3:  _do_1G_1PinM3 (); break;
        case Conn_2G_1PinM3:
        case Conn_3G_1PinM3:  _do_xG_1PinM3 (); break;
        case Conn_1G_1M1_1M2: _do_xG_1M1_1M2(); break;
        case Conn_1G_1M1_1M3: _do_1G_xM1    (); break;
        case Conn_2G_1M1_1M2: _do_xG_1M1_1M2(); break;
        default:
          if (not isTwoMetals())
            throw Bug( "Unmanaged Configuration [%d] = [%d+%d+%d+%d,%d+%d] %s in %s\n"
                     "      The global routing seems to be defective."
                     , _connexity.connexity
                     , _connexity.fields.globals
                     , _connexity.fields.M1     
                     , _connexity.fields.M2     
                     , _connexity.fields.M3
                     , _connexity.fields.Pin
                     , _connexity.fields.Pad
                     , _net->_getString().c_str()
                     , getString(_gcell).c_str()
                     );
          _do_xG();
      }

      if (not _do_globalSegment()) {
        cdebug_log(145,0) << "No global generated, finish." << endl;
        cdebug_tabw(145,-1);
        return;
      }
      cdebug_log(145,0) << "Proceed to next GCell." << endl;
    } else {
      AutoContact* targetContact = NULL;
      if (not _sourceContact) _fromHook = NULL;

      if      (_gcell->isDevice ())  targetContact = _doDevice  ();
      else if (_gcell->isHChannel()) targetContact = _doHChannel();
      else if (_gcell->isVChannel()) targetContact = _doVChannel();
      else if (_gcell->isHRail   ()) targetContact = _doHRail();
      else if (_gcell->isVRail   ()) targetContact = _doVRail();
      else if (_gcell->isStrut  ())  targetContact = _doStrut   ();
      else if (_gcell->isIoPad  ())  _doIoPad();
      else
        throw Bug( "Unmanaged GCell type: %s in %s\n"
                   "      The global routing seems to be defective."
                 , getString(_gcell).c_str()
                 , getString(_net).c_str()
                 );

      if ( (_sourceContact) && (targetContact) ){
        AutoSegment* globalSegment = AutoSegment::create( _sourceContact
                                                        , targetContact
                                                        , static_cast<Segment*>( _fromHook->getComponent() )
                                                        );
        cdebug_log(145,0) << "[Create global segment (1)]: " << globalSegment << endl;
      }     
    }

    cdebug_log(145,0) << endl;
    cdebug_tabw(145,-1);
  }


  AutoContact* NetBuilder::doRp_AccessPad ( RoutingPad* rp, uint64_t flags )
  {
    cdebug_log(145,1) << "doRp_AccessPad()" << endl;
    cdebug_log(145,0) << rp << endl;

  // Hardcoded: H access is METAL2 (depth=1), V access is METAL3 (depth=2).
    size_t  accessDepth = (flags & HAccess) ? 1 : 2 ;
    size_t  padDepth    = Session::getLayerDepth(rp->getLayer());
    if (padDepth > Session::getAllowedDepth()) {
      cerr << Error( "NetBuilder::doRp_AccessPad(): Pad RoutingPad %s\n"
                     "        has a layer unreachable by the router (top layer is: %s)"
                   , getString(rp).c_str()
                   , getString(Session::getRoutingLayer(Session::getAllowedDepth())).c_str()
                   ) << endl;
      padDepth = Session::getAllowedDepth();
    }

    rp->getBodyHook()->detach();

    Point rpPosition = rp->getCenter();
    Point position   = rp->getCenter();
    Box   rpbb       = rp->getBoundingBox();
    if (  (rpbb.getWidth () > 2*Session::getWireWidth(padDepth)) 
       or (rpbb.getHeight() > 2*Session::getWireWidth(padDepth)) ) {
    //cerr << "doRp_AccessPad(): connecting to non-punctual connector (RoutingPad).\n"
    //     << "                  " << rp->getNet() << "pad:" << rp->getOccurrence().getMasterCell() << endl;

      Transformation transf = rp->getOccurrence().getPath().getTransformation();
      switch ( transf.getOrientation() ) {
        case Transformation::Orientation::ID: position.setY( rpbb.getYMin() ); break;
        case Transformation::Orientation::MY: position.setY( rpbb.getYMax() ); break;
        case Transformation::Orientation::YR:
        case Transformation::Orientation::R3: position.setX( rpbb.getXMin() ); break;
        case Transformation::Orientation::R1: position.setX( rpbb.getXMax() ); break;
        default:
          break;
      }
    }

    GCell*     gcell  = Session::getAnabatic()->getGCellUnder(position);
    Component* anchor = rp;

    if (padDepth != accessDepth) {
      if (padDepth > accessDepth) {
      // Go *down* from the pad's RoutingPad.
        --padDepth;

        Contact*   target   = NULL;
        Contact*   source   = Contact::create ( rp
                                              , Session::getContactLayer(padDepth)
                                              , position.getX() - rpPosition.getX()
                                              , position.getY() - rpPosition.getY()
                                              , Session::getViaWidth(padDepth)
                                              , Session::getViaWidth(padDepth)
                                              );
      
        for ( size_t depth = padDepth ; depth >= accessDepth ; --depth ) {
          const Layer* segmentLayer = Session::getRoutingLayer(depth);
          const Layer* targetLayer  = (depth == accessDepth) ? segmentLayer
                                                             : Session::getContactLayer(depth-1);
          DbU::Unit    targetSide   = (depth == accessDepth) ? Session::getWireWidth(depth)
                                                             : Session::getViaWidth (depth-1);

          target = Contact::create( rp->getNet()
                                  , targetLayer
                                  , position.getX()
                                  , position.getY()
                                  , targetSide
                                  , targetSide
                                  );
          if (Session::getDirection(depth) == Flags::Horizontal) {
            anchor = Horizontal::create( source
                                       , target
                                       , segmentLayer
                                       , position.getY()
                                       , Session::getWireWidth(depth)
                                       );
          } else {
            anchor = Vertical::create( source
                                     , target
                                     , segmentLayer
                                     , position.getX()
                                     , Session::getWireWidth(depth)
                                     );
          }
          cdebug_log(145,0) << "Pad strap: " << anchor << endl;
          source = target;
        }
      } else {
      // Go *up* from the pad's RoutingPad.
        Contact*   target   = NULL;
        Contact*   source   = Contact::create ( rp
                                              , Session::getContactLayer(padDepth)
                                              , 0
                                              , 0
                                              , Session::getViaWidth(padDepth)
                                              , Session::getViaWidth(padDepth)
                                              );
      
        for ( size_t depth = padDepth ; depth <= accessDepth ; ++depth ) {
          const Layer* segmentLayer = Session::getRoutingLayer(depth);
          const Layer* targetLayer  = (depth == accessDepth) ? segmentLayer
                                                             : Session::getContactLayer(depth);
          DbU::Unit    targetSide   = (depth == accessDepth) ? Session::getWireWidth(depth)
                                                             : Session::getViaWidth (depth);

          target = Contact::create( rp->getNet()
                                  , targetLayer
                                  , position.getX()
                                  , position.getY()
                                  , targetSide
                                  , targetSide
                                  );
          if (Session::getDirection(depth) == Flags::Horizontal) {
            anchor = Horizontal::create( source
                                       , target
                                       , segmentLayer
                                       , position.getY()
                                       , Session::getWireWidth(depth)
                                       );
          } else {
            anchor = Vertical::create( source
                                     , target
                                     , segmentLayer
                                     , position.getX()
                                     , Session::getWireWidth(depth)
                                     );
          }
          cdebug_log(145,0) << "Pad strap: " << anchor << endl;
          source = target;
        }
      }
    }

    AutoContact* autoSource
      = AutoContactTerminal::create ( gcell
                                    , anchor
                                    , Session::getRoutingLayer(accessDepth)
                                    , position
                                    , Session::getWireWidth(accessDepth)
                                    , Session::getWireWidth(accessDepth)
                                    );
    cdebug_tabw(145,-1);
    return autoSource;
  }


  AutoContact* NetBuilder::doRp_AccessAnalog ( GCell* gcell, RoutingPad* rp, uint64_t flags )
  {
    cdebug_log(145,1) << "doRp_AccessAnalog()" << endl;
    cdebug_log(145,0) << rp << endl;

    const Layer* rpLayer  = rp->getLayer();
    size_t       rpDepth  = Session::getLayerDepth( rpLayer );
    DbU::Unit    viaSide  = Session::getViaWidth  ( rpDepth );
    Point        position = rp->getCenter();
    position.translate( -Session::getDVerticalOffset(), -Session::getDHorizontalOffset() );

    Point        onGrid   = Session::getNearestGridPoint( position, gcell->getConstraintBox() );
    AutoContact* contact  = AutoContactTerminal::create( gcell, rp, rpLayer, position, viaSide, viaSide );

    if (position != onGrid) {
      cerr << Bug( "NetBuilder::doRp_AccessAnalog(): RoutingPad is not under any grid point.\n"
                   "      %s\n"
                   "      %s shifted to nearest grid point %s"
                 , getString(rp).c_str()
                 , getString(position).c_str()
                 , getString(onGrid).c_str()
                 ) << endl;
      contact->forceOnGrid( onGrid );
    }

    if (rpDepth != Session::getDHorizontalDepth()) {
      cerr << Bug( "NetBuilder::doRp_AccessAnalog(): RoutingPad must be in %s layer.\n"
                   "      %s"
                 , getString(Session::getDHorizontalLayer()->getName()).c_str()
                 , getString(rp).c_str()
                 ) << endl;
    }

    cdebug_log(145,0) << contact << endl;
    cdebug_tabw(145,-1);
    return contact;
  }


  void  NetBuilder::doRp_StairCaseH ( GCell* gcell, Component* rp1, Component* rp2 )
  {
    cdebug_log(145,0) << "doRp_StairCaseH()" << endl;

    if (rp1->getCenter().getX() > rp2->getCenter().getX()) swap( rp1, rp2 );

    AutoContact* rp1ContactSource = NULL;
    AutoContact* rp1ContactTarget = NULL;
    AutoContact* rp2ContactSource = NULL;
    AutoContact* rp2ContactTarget = NULL;
    const Layer* viaLayer         = NULL;

    doRp_AutoContacts( gcell, rp1, rp1ContactSource, rp1ContactTarget, DoTargetContact );
    doRp_AutoContacts( gcell, rp2, rp2ContactSource, rp2ContactTarget, DoSourceContact );

    if (rp1ContactTarget->getY() == rp2ContactSource->getY()) {
      cdebug_log(145,0) << "Aligned horizontal routing pads : straight wire" << endl;

      viaLayer = rp1->getLayer();
      AutoSegment::create( rp1ContactTarget, rp2ContactSource, Flags::Horizontal );
      return;
    }

    viaLayer = Session::getContactLayer(1);

    AutoContact* subContact1 = AutoContactTurn::create( gcell, rp1->getNet(), viaLayer );
    AutoContact* subContact2 = AutoContactTurn::create( gcell, rp1->getNet(), viaLayer );

    AutoSegment::create( rp1ContactTarget, subContact1     , Flags::Horizontal );
    AutoSegment::create( subContact1     , subContact2     , Flags::Vertical   );
    AutoSegment::create( subContact1     , rp2ContactSource, Flags::Horizontal );
  }


  void  NetBuilder::doRp_StairCaseV ( GCell* gcell, Component* rp1, Component* rp2 )
  {
    cdebug_log(145,0) << "doRp_StairCaseV()" << endl;

    if (rp1->getCenter().getY() > rp2->getCenter().getY()) swap( rp1, rp2 );

    AutoContact* rp1ContactSource = NULL;
    AutoContact* rp1ContactTarget = NULL;
    AutoContact* rp2ContactSource = NULL;
    AutoContact* rp2ContactTarget = NULL;
    const Layer* viaLayer         = NULL;

    doRp_AutoContacts( gcell, rp1, rp1ContactSource, rp1ContactTarget, DoTargetContact );
    doRp_AutoContacts( gcell, rp2, rp2ContactSource, rp2ContactTarget, DoSourceContact );

    if (rp1ContactTarget->getX() == rp2ContactSource->getX()) {
      cdebug_log(145,0) << "Aligned vertical routing pads : straight wire" << endl;

      viaLayer = rp1->getLayer();
      AutoSegment::create( rp1ContactTarget, rp2ContactSource, Flags::Vertical );
      return;
    }

    viaLayer = Session::getContactLayer(1);

    AutoContact* subContact1 = AutoContactTurn::create( gcell, rp1->getNet(), viaLayer );
    AutoContact* subContact2 = AutoContactTurn::create( gcell, rp1->getNet(), viaLayer );

    AutoSegment::create( rp1ContactTarget, subContact1     , Flags::Vertical   );
    AutoSegment::create( subContact1     , subContact2     , Flags::Horizontal );
    AutoSegment::create( subContact1     , rp2ContactSource, Flags::Vertical   );
  }


  bool  NetBuilder::_do_xG ()
  {
    throw Error( "%::_do_xG() method *not* reimplemented from base class.", getTypeName().c_str() );
    return false;
  }

  
  bool  NetBuilder::_do_2G ()
  {
    throw Error ( "%s::_do_2G() method *not* reimplemented from base class.", getTypeName().c_str() );
    return false;
  }

  
  bool  NetBuilder::_do_xG_1Pad ()
  {
    throw Error ( "%s::_do_xG_1Pad() method *not* reimplemented from base class.", getTypeName().c_str() );
    return false;
  }

  
  bool  NetBuilder::_do_1G_1PinM2 ()
  {
    throw Error ( "%s::_do_1G_1PinM2() method *not* reimplemented from base class.", getTypeName().c_str() );
    return false;
  }

  
  bool  NetBuilder::_do_1G_1PinM1 ()
  {
    throw Error ( "%s::_do_1G_1PinM1() method *not* reimplemented from base class.", getTypeName().c_str() );
    return false;
  }

  
  bool  NetBuilder::_do_2G_1PinM1 ()
  {
    throw Error ( "%s::_do_2G_1PinM1() method *not* reimplemented from base class.", getTypeName().c_str() );
    return false;
  }

  
  bool  NetBuilder::_do_xG_1PinM2 ()
  {
    throw Error ( "%s::_do_xG_1PinM2() method *not* reimplemented from base class.", getTypeName().c_str() );
    return false;
  }

  
  bool  NetBuilder::_do_1G_1PinM3 ()
  {
    throw Error ( "%s::_do_1G_1PinM3() method *not* reimplemented from base class.", getTypeName().c_str() );
    return false;
  }

  
  bool  NetBuilder::_do_xG_1PinM3 ()
  {
    throw Error( "%s::_do_xG_1PinM3() method *not* reimplemented from base class.", getTypeName().c_str() );
    return false;
  }

  
  bool  NetBuilder::_do_1G_1M1 ()
  {
    throw Error ( "%s::_do_1G_1M1() method *not* reimplemented from base class.", getTypeName().c_str() );
    return false;
  }

  
  bool  NetBuilder::_do_2G_1M1 ()
  {
    throw Error ( "%s::_do_2G_1M1() method *not* reimplemented from base class.", getTypeName().c_str() );
    return false;
  }

  
  bool  NetBuilder::_do_1G_xM1 ()
  {
    throw Error ( "%s::_do_1G_xM1() method *not* reimplemented from base class.", getTypeName().c_str() );
    return false;
  }


  
  bool  NetBuilder::_do_xG_1M1 ()
  {
    return _do_xG_xM1_xM3();
  }

  
  bool  NetBuilder::_do_xG_xM1_xM3 ()
  {
    throw Error ( "%s::_do_xG_xM1_xM3() method *not* reimplemented from base class.", getTypeName().c_str() );
    return false;
  }

  
  bool  NetBuilder::_do_xG_1M1_1M2 ()
  {
    throw Error ( "%s::_do_xG_1M1_1M2() method *not* reimplemented from base class.", getTypeName().c_str() );
    return false;
  }

  
  bool  NetBuilder::_do_4G_1M2 ()
  {
    throw Error ( "%s::_do_4G_1M2() method *not* reimplemented from base class.", getTypeName().c_str() );
    return false;
  }

  
  bool  NetBuilder::_do_xG_xM2 ()
  {
    throw Error ( "%s::_do_xG_xM2() method *not* reimplemented from base class.", getTypeName().c_str() );
    return false;
  }

  
  bool  NetBuilder::_do_1G_1M3 ()
  {
    throw Error ( "%s::_do_1G_1M3() method *not* reimplemented from base class.", getTypeName().c_str() );
    return false;
  }

  
  bool  NetBuilder::_do_xG_xM3 ()
  {
    throw Error ( "%s::_do_xG_xM3() method *not* reimplemented from base class.", getTypeName().c_str() );
    return false;
  }

  
  bool  NetBuilder::_do_globalSegment ()
  {
    throw Error ( "%s::_do_globalSegment() method *not* reimplemented from base class.", getTypeName().c_str() );
    return false;
  }


  void  NetBuilder::singleGCell ( AnabaticEngine* anbt, Net* net )
  {
    cdebug_log(145,1) << "NetBuilder::singleGCell() " << net << endl;

    vector<RoutingPad*>  rpM1s;
    Component*           rpM2 = NULL;

    forEach ( RoutingPad*, irp, net->getRoutingPads() ) {
      if (Session::getRoutingGauge()->getLayerDepth(irp->getLayer()) == 1)
        rpM2 = *irp;
      else
        rpM1s.push_back( *irp );
    }

    if ((rpM1s.size() < 2) and not rpM2) {
      cerr << Error( "For %s, less than two Plugs/Pins (%d)."
                   , getString(net).c_str()
                   , rpM1s.size() ) << endl;
      cdebug_tabw(145,-1);
      return;
    }

    if (rpM1s.empty()) {
      cerr << Error( "No METAL1 in Single GCell for Net \"%s\".", getString(net->getName()).c_str() ) << endl;
      cdebug_tabw(145,-1);
      return;
    }

    sortRpByX( rpM1s, NetBuilder::NoFlags ); // increasing X.

    GCell* gcell1 = anbt->getGCellUnder( (*rpM1s.begin ())->getCenter() );
    GCell* gcell2 = anbt->getGCellUnder( (*rpM1s.rbegin())->getCenter() );

    if (not gcell1) {
      cerr << Error( "No GCell under %s.", getString(*(rpM1s.begin())).c_str() ) << endl;
      cdebug_tabw(145,-1);
      return;
    }
    if (gcell1 != gcell2) {
      cerr << Error( "Not under a single GCell %s.", getString(*(rpM1s.rbegin())).c_str() ) << endl;
      cdebug_tabw(145,-1);
      return;
    }

    cdebug_log(145,0) << "singleGCell " << gcell1 << endl;

    AutoContact* turn1  = NULL;
    AutoContact* turn2  = NULL;
    AutoContact* source = NULL;
    AutoContact* target = NULL;

    for ( size_t irp=1 ; irp<rpM1s.size() ; ++irp ) {
      doRp_AutoContacts( gcell1, rpM1s[irp-1], source, turn1, DoSourceContact );
      doRp_AutoContacts( gcell1, rpM1s[irp  ], target, turn1, DoSourceContact );

      if (source->getUConstraints(Flags::Vertical).intersect(target->getUConstraints(Flags::Vertical))) {
        uint64_t flags = checkRoutingPadSize( rpM1s[irp-1] );
        if ((flags & VSmall) or Session::getConfiguration()->isVH()) {
          if (Session::getConfiguration()->isHV()) {
            turn1  = AutoContactTurn::create( gcell1, rpM1s[irp]->getNet(), Session::getDContactLayer() );
            AutoSegment::create( source, turn1, Flags::Horizontal   );
            source = turn1;
          }
          turn1  = AutoContactTurn::create( gcell1, rpM1s[irp]->getNet(), Session::getDContactLayer() );
          AutoSegment::create( source, turn1 , Flags::Vertical   );
          source = turn1;
        }
        flags = checkRoutingPadSize( rpM1s[irp] );
        if ((flags & VSmall) or Session::getConfiguration()->isVH()) {
          if (Session::getConfiguration()->isHV()) {
            turn1  = AutoContactTurn::create( gcell1, rpM1s[irp]->getNet(), Session::getDContactLayer() );
            AutoSegment::create( target, turn1, Flags::Horizontal   );
            target = turn1;
          }
          turn1  = AutoContactTurn::create( gcell1, rpM1s[irp]->getNet(), Session::getDContactLayer() );
          AutoSegment::create( target, turn1 , Flags::Vertical   );
          target = turn1;
        }
        AutoSegment::create( source, target, Flags::Horizontal );
      } else {
        turn1 = AutoContactTurn::create( gcell1, rpM1s[irp]->getNet(), Session::getDContactLayer() );
        turn2 = AutoContactTurn::create( gcell1, rpM1s[irp]->getNet(), Session::getDContactLayer() );
        AutoSegment::create( source, turn1 , Flags::Horizontal );
        AutoSegment::create( turn1 , turn2 , Flags::Vertical   );
        AutoSegment::create( turn2 , target, Flags::Horizontal );
      }
    }

    if (rpM2) {
      doRp_AutoContacts( gcell1, rpM1s[0], source, turn1, DoSourceContact );
      doRp_AutoContacts( gcell1, rpM2    , target, turn1, DoSourceContact );
      turn1 = AutoContactTurn::create( gcell1, rpM2->getNet(), Session::getContactLayer(1) );
      AutoSegment::create( source, turn1 , Flags::Horizontal );
      AutoSegment::create( turn1 , target, Flags::Vertical );
    }

    cdebug_tabw(145,-1);
  }


#if THIS_IS_DISABLED
  bool isVertical ( RoutingPad* rp )
  {
    return (  (rp->getSourcePosition().getX() == rp->getTargetPosition().getX()) 
           && (rp->getSourcePosition().getY() != rp->getTargetPosition().getY()) 
           );
  }


  bool isHorizontal ( RoutingPad* rp )
  {
    return (  (rp->getSourcePosition().getY() == rp->getTargetPosition().getY()) 
           && (rp->getSourcePosition().getX() != rp->getTargetPosition().getX()) 
           );
  }
#endif


  RoutingPad* returnSW ( GCell* gcell, RoutingPad* rp1, RoutingPad* rp2 )
  {
    DbU::Unit c1SW = (rp1->getSourcePosition().getX() - gcell->getXMin()) + (rp1->getSourcePosition().getY() - gcell->getYMin());
    DbU::Unit c2SW = (rp2->getSourcePosition().getX() - gcell->getXMin()) + (rp2->getSourcePosition().getY() - gcell->getYMin());

    if ( c1SW < c2SW ){
      return rp1;
    } else {
      return rp2;
    }
  }


  RoutingPad* returnNE ( GCell* gcell, RoutingPad* rp1, RoutingPad* rp2 )
  {
    DbU::Unit c1NE = (gcell->getXMax() - rp1->getTargetPosition().getX()) + (gcell->getYMax() - rp1->getTargetPosition().getY());
    DbU::Unit c2NE = (gcell->getXMax() - rp2->getTargetPosition().getX()) + (gcell->getYMax() - rp2->getTargetPosition().getY());

    if ( c1NE < c2NE ){
      return rp1;
    } else {
      return rp2;
    }
  }
  

  AutoContact* NetBuilder::_doDevice ()
  {
    cdebug_log(145,1) << "void  NetBuilder::_doDevice ()" << _gcell << endl;

    for ( size_t i=0; i<_routingPads.size() ; i++ ) {
      if (   (_routingPads[i]->getSourcePosition().getX() == _routingPads[i]->getTargetPosition().getX())
         and (_routingPads[i]->getSourcePosition().getY() == _routingPads[i]->getTargetPosition().getY()) ) {
        throw Error( "NetBuilder::_doDevice() Some RoutingPads are not set to a component.\n"
                     "        On: %s."
                   , getString(_gcell).c_str() );
      }
    }
    cdebug_log(145,0) << "FromHook: " << _fromHook << endl;
    cdebug_log(145,0) << "North   : " << north()   << endl;
    cdebug_log(145,0) << "East    : " << east()    << endl;
    cdebug_log(145,0) << "South   : " << south()   << endl;
    cdebug_log(145,0) << "West    : " << west()    << endl;
    cdebug_log(145,0) << "_routingPads.size(): " << _routingPads.size() << endl;

    RoutingPad*  rpNE = NULL;
    RoutingPad*  rpSW = NULL;
    _southWestContact = NULL;

    if (_routingPads.size() > 1) {
      cdebug_log(145,0) << "Case _routingPads.size() > 1 "<< endl;
      for ( RoutingPad* rp : _routingPads )
        cdebug_log(145,0) << rp << endl;
      
    // #1: Find RoutingPads to use for AutoContacts NE+SW
      rpNE = _routingPads[0];
      rpSW = _routingPads[0];

      for ( size_t i=1 ; i<_routingPads.size() ; i++ ) {
        rpNE = returnNE( _gcell, rpNE, _routingPads[i] );
        rpSW = returnSW( _gcell, rpSW, _routingPads[i] );
      }
      
      cdebug_log(145,0) << "rpNE: " << rpNE << endl;
      cdebug_log(145,0) << "rpSW: " << rpSW << endl;
    } else if (_routingPads.size() == 0) {
      cdebug_log(145,0) << "Case _routingPads.size() = 0 "<< endl;
      throw Error( "NetBuilder::_doDevice() No RoutingPads found.\n"
                   "        On: %s."
                 , getString(_gcell).c_str() );
    } else {
      cdebug_log(145,0) << "Case _routingPads.size() = 1 "<< endl;
      rpNE = rpSW = _routingPads[0];
    }
    cdebug_log(145,0) << "rp NE: " << rpNE << endl;
    cdebug_log(145,0) << "rp SW: " << rpSW << endl;

    if (rpNE and rpSW) {
      if (east()) {
        cdebug_log(145,0) << "East"  << endl;
        AutoContact* ac = doRp_AccessAnalog( _gcell, rpNE, NoFlags );
        push( east(), ac, SouthWest );
      } 
      if (west()) {
        cdebug_log(145,0) << "West"  << endl;
        AutoContact* ac = doRp_AccessAnalog( _gcell, rpSW, NoFlags );
        push( west(), ac, SouthWest );
      }
      if (south()) {
        cdebug_log(145,0) << "South"  << endl;
        AutoContact* ac = doRp_AccessAnalog( _gcell, rpSW, NoFlags );
        push( south(), ac, SouthWest );
      }
      if (north()){
        cdebug_log(145,0) << "North"  << endl;
        AutoContact* ac = doRp_AccessAnalog( _gcell, rpNE, NoFlags );
        push( north(), ac, SouthWest );
      }
    }
    cdebug_log(145,0) << "doDevice done" << endl;
    cdebug_tabw(145,-1);

    return _southWestContact;
  }


  AutoContact*  NetBuilder::_doHChannel ()
  {
    cdebug_log(145,1) << "void  NetBuilder::_doHChannel() " << _gcell << endl;
    
    vector<Hook*>  hooks;
    Hook*          firsthhook = NULL;
    Hook*          lasthhook  = NULL;
    AutoContact*   targetContact = NULL;
    
  // Save segments only
    cdebug_log(145,0) << "fromHook: "  << _fromHook << endl;
    for ( Hook* toHook : _fromHook->getHooks() ) {
      cdebug_log(145,0) << "toHook: "  << toHook << endl;

      Segment* s = dynamic_cast<Segment*>( toHook->getComponent() );
      if (s) {
        Horizontal* h = dynamic_cast<Horizontal*>(s);
        if (h) {
          if      (h->getSource()->getX() <= _gcell->getXMin()) firsthhook = toHook;
          else if (h->getTarget()->getX() >= _gcell->getXMax()) lasthhook  = toHook;
          else    cdebug_log(145,0) << "Error(AutoContact*  NetBuilder::_doHChannel ( ForkStack& forks )): This case should not happen " << endl;
        } else hooks.push_back(toHook);
      }
    }

    cdebug_log(145,0) << "Sorted hooks:" << endl;
    sortHookByX( hooks, NoFlags );
    
    if (firsthhook) hooks.insert   (hooks.begin(), firsthhook);
    if (lasthhook ) hooks.push_back(lasthhook                );

    for (vector<Hook*>::iterator it = hooks.begin(); it != hooks.end(); it++){
      cdebug_log(145,0) << "toHook: "  << (*it) << endl;
    }
    size_t i = 0;

  // More than 2 AutoContacts to create
    cdebug_log(145,0) << "Number of hooks: " <<  hooks.size() << endl;
    if ( hooks.size() > 2 ){
      cdebug_log(145,0) << "Number of hooks > 2 : " <<  hooks.size() << endl;
      vector<AutoContact*> autoContacts;
      bool                 firstH      = false; 

      for (vector<Hook*>::iterator it = hooks.begin(); it != hooks.end(); it++){
      
        Horizontal* h = dynamic_cast<Horizontal*>((*it)->getComponent());
        Vertical*   v = dynamic_cast<Vertical*>  ((*it)->getComponent());
        AutoContact* ac = NULL;
        cdebug_log(145,0) << "index: " << i << endl;
        cdebug_log(145,0) << "h    : " << h << endl;
        cdebug_log(145,0) << "v    : " << v << endl;

        if (i == 0){
          if        (h){
            cdebug_log(145,0) << "case i=0, h" <<  endl;
            ac = AutoContactHTee::create( _gcell, _net, Session::getContactLayer(2) );
            ac->setX(_gcell->getXMin());
            ac->setY(_gcell->getYMin() + _gcell->getHeight()/2);
            firstH = true;
          } else if (v){
            cdebug_log(145,0) << "case i=0, v" <<  endl;
            ac = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
            ac->setX(v->getX());
            ac->setY(_gcell->getYMin() + _gcell->getHeight()/2);
          }
        } else if (i == hooks.size()-1){
          if        (h){
            cdebug_log(145,0) << "case i=last, h" <<  endl;
            ac = autoContacts[i-1];
          } else if (v){
            cdebug_log(145,0) << "case i=last, v" <<  endl;
            ac = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
            ac->setX(v->getX());
            ac->setY(_gcell->getYMin() + _gcell->getHeight()/2);
          }
        } else {
          if ((i == 1)&&(firstH)){
            cdebug_log(145,0) << "case i=1, first was H" <<  endl;
            ac = autoContacts[i-1];
            if (v) ac->setX(v->getX());
            else {
              cerr << Warning( "Something is wrong with the number of globals in this HChannel."
                             , getString(_gcell).c_str() )
                   << endl;
            }
          } else {
            cdebug_log(145,0) << "case i=1, first was V" <<  endl;
            ac = AutoContactHTee::create( _gcell, _net, Session::getContactLayer(2) );
            ac->setX(v->getX());
            ac->setY(_gcell->getYMin() + _gcell->getHeight()/2);
          }
        } 
        cdebug_log(145,0) << "[Create AutoContact]: " << ac << endl;
        autoContacts.push_back(ac);
      
        cdebug_log(145,0) << "FromHook: " << _fromHook << endl;
        if (_fromHook->getComponent() == (*it)->getComponent()){
          cdebug_log(145,0) << "Found from:" << (*it)->getComponent() << endl;
          targetContact = ac;
        } else {
          push( (*it), ac );
        }
        i++;
      }   
      cdebug_log(145,0) << "Chain contacts: "  << endl;
      for (size_t j=1; j < autoContacts.size(); j++){
        if (autoContacts[j-1] != autoContacts[j]){
          AutoSegment* segment = AutoSegment::create( autoContacts[j-1] , autoContacts[j], Flags::Horizontal );
          cdebug_log(145,0) << "[Create global segment (2)]: " << segment << endl;
        }
      }
    // There are only 2 AutoContacts to create
    } else if (hooks.size() == 2){
      cdebug_log(145,0) << "Number of hooks == 2 : " <<  hooks.size() << endl;
      Horizontal*  h0  = dynamic_cast<Horizontal*>(hooks[0]->getComponent());
      Vertical*    v0  = dynamic_cast<Vertical*>  (hooks[0]->getComponent());
      Horizontal*  h1  = dynamic_cast<Horizontal*>(hooks[1]->getComponent());
      Vertical*    v1  = dynamic_cast<Vertical*>  (hooks[1]->getComponent());
      AutoContact* source = NULL;
      AutoContact* target = NULL;
      cdebug_log(145,0) << "h0: " << h0 << endl;
      cdebug_log(145,0) << "v0: " << v0 << endl;
      cdebug_log(145,0) << "h1: " << h1 << endl;
      cdebug_log(145,0) << "v1: " << v1 << endl;
        
      if ((v0 != NULL) && (v1 != NULL)){
        cdebug_log(145,0) << "case 2V" << endl;
        source = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
        source->setX(v0->getX());
        source->setY(_gcell->getYMin() + _gcell->getHeight()/2);
        target = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
        target->setX(v1->getX());
        target->setY(_gcell->getYMin() + _gcell->getHeight()/2);

        cdebug_log(145,0) << "[Create AutoContact Source]: " << source << endl;
        cdebug_log(145,0) << "[Create AutoContact Target]: " << target << endl;
        AutoSegment* globalSegment = AutoSegment::create( source, target, Flags::Horizontal );
        cdebug_log(145,0) << "[Create global segment (3)]: " << globalSegment << endl;
        
        if        (_fromHook->getComponent() == hooks[0]->getComponent()){
          cdebug_log(145,0) << "Found from:" << hooks[0]->getComponent() << endl;
          targetContact = source;
          push( hooks[1], target );
          
        } else if (_fromHook->getComponent() == hooks[1]->getComponent()){
          cdebug_log(145,0) << "Found from:" << hooks[1]->getComponent() << endl;
          targetContact = target;
          push( hooks[0], source );
        }

      } else if (((h0 != NULL) && (v1 != NULL)) || ((v0 != NULL) && (h1 != NULL))){
        cdebug_log(145,0) << "case 1V and 1H" << endl;
        source = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
        cdebug_log(145,0) << "[Create AutoContact]: " << source << endl;
        if (h0 && v1){
          source->setX(v1->getX());
        } else {
          source->setX(v0->getX());
        }
        source->setY(_gcell->getYMin() + _gcell->getHeight()/2);
        targetContact = source;

        if        (_fromHook->getComponent() == hooks[0]->getComponent()){
          cdebug_log(145,0) << "Found from:" << hooks[0]->getComponent() << endl;
          push( hooks[1], source );
          
        } else if (_fromHook->getComponent() == hooks[1]->getComponent()){
          cdebug_log(145,0) << "Found from:" << hooks[1]->getComponent() << endl;
          push( hooks[0], source );
        }
      } else if ((h0 != NULL) && (h1 != NULL)){
        cdebug_log(145,0) << "case 2H" << endl;
        source = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
        source->setX(_gcell->getXMin() + _gcell->getWidth()/2);
        source->setY(_gcell->getYMin() + _gcell->getHeight()/2);
        target = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
        target->setX(_gcell->getXMin() + _gcell->getWidth()/2);
        target->setY(_gcell->getYMin() + _gcell->getHeight()/2);

        cdebug_log(145,0) << "[Create AutoContact Source]: " << source << endl;
        cdebug_log(145,0) << "[Create AutoContact Target]: " << target << endl;

        AutoSegment* globalSegment = AutoSegment::create( source, target, Flags::Vertical );
        cdebug_log(145,0) << "[Create global segment (4)]: " << globalSegment << endl;

        if        (_fromHook->getComponent() == hooks[0]->getComponent()){
          cdebug_log(145,0) << "Found from:" << hooks[0]->getComponent() << endl;
          targetContact = source;
          push( hooks[1], target );
          
        } else if (_fromHook->getComponent() == hooks[1]->getComponent()){
          cdebug_log(145,0) << "Found from:" << hooks[1]->getComponent() << endl;
          targetContact = target;
          push( hooks[0], source );
        }
      }
    }
    cdebug_log(145,0) << "doHChannel done" << endl;
    cdebug_tabw(145,-1);

    return targetContact;
  }


  AutoContact*  NetBuilder::_doVChannel ()
  {
    cdebug_log(145,1) << "void  NetBuilder::_doVChannel ()" << _gcell << endl;
    
  
    vector<Hook*>  hooks;
    Hook*          firstvhook = NULL;
    Hook*          lastvhook  = NULL;
    AutoContact*   targetContact   = NULL;
    
  // Save segments only
    cdebug_log(145,0) << "fromHook: "  << _fromHook << endl;

    
    for ( Hook* toHook : _fromHook->getHooks() ) {
      cdebug_log(145,0) << "toHook: "  << toHook << endl;

      Segment* s = dynamic_cast<Segment*>( toHook->getComponent() );
      if (s) {
        Vertical* v = dynamic_cast<Vertical*>(s);
        if (v) {
          if      (v->getSource()->getY() <= _gcell->getYMin()) firstvhook = toHook;
          else if (v->getTarget()->getY() >= _gcell->getYMax()) lastvhook  = toHook;
          else    cdebug_log(145,0) << "Error(AutoContact*  NetBuilder::_doVChannel()): This case should not happen " << endl;
        } else hooks.push_back(toHook);
      }
    }
    
    cdebug_log(145,0) << "Sorted hooks:" << endl;
    sortHookByY( hooks, NoFlags );

    if (firstvhook) hooks.insert   (hooks.begin(), firstvhook);
    if (lastvhook ) hooks.push_back(lastvhook                );

    for (vector<Hook*>::iterator it = hooks.begin(); it != hooks.end(); it++){
      cdebug_log(145,0) << "toHook: "  << (*it) << endl;
    }
    size_t i = 0;

  // More than 2 AutoContacts to create
    cdebug_log(145,0) << "Number of hooks: " <<  hooks.size() << endl;
    if ( hooks.size() > 2 ){
      cdebug_log(145,0) << "Number of hooks > 2 : " <<  hooks.size() << endl;
      vector<AutoContact*> autoContacts;
      bool                 firstV      = false; 

      for (vector<Hook*>::iterator it = hooks.begin(); it != hooks.end(); it++){
      
        Horizontal* h = dynamic_cast<Horizontal*>((*it)->getComponent());
        Vertical*   v = dynamic_cast<Vertical*>  ((*it)->getComponent());
        AutoContact* ac = NULL;
        cdebug_log(145,0) << "index: " << i << endl;
        cdebug_log(145,0) << "h    : " << h << endl;
        cdebug_log(145,0) << "v    : " << v << endl;

        if (i == 0){
          if        (v){
            cdebug_log(145,0) << "case i=0, v"<< endl;
            ac = AutoContactVTee::create( _gcell, _net, Session::getContactLayer(1) );
            ac->setX(_gcell->getXMin() + _gcell->getWidth()/2);
            ac->setY(_gcell->getYMin());
            firstV = true;
          } else if (h){
            cdebug_log(145,0) << "case i=0, h"<< endl;
            ac = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
            ac->setX(_gcell->getXMin() + _gcell->getWidth()/2);
            ac->setY(h->getY());
          }
        } else if (i == hooks.size()-1){
          if        (v){
            cdebug_log(145,0) << "case i=last, v"<< endl;
            ac = autoContacts[i-1];
          } else if (h){
            cdebug_log(145,0) << "case i=last, h"<< endl;
            ac = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
            ac->setX(_gcell->getXMin() + _gcell->getWidth()/2);
            ac->setY(h->getY());
          }
        } else {
          if ((i == 1)&&(firstV)){
            cdebug_log(145,0) << "case i=X, first was V"<< endl;
            ac = autoContacts[i-1];
            if (h) ac->setY(h->getY());
            else {
              cerr << Warning( "Something is wrong with the number of globals in this VChannel."
                             , getString(_gcell).c_str() )
                   << endl;
            }
          } else {
            cdebug_log(145,0) << "case i=X, first was H"<< endl;
            ac = AutoContactVTee::create( _gcell, _net, Session::getContactLayer(1) );
            ac->setX(_gcell->getXMin() + _gcell->getWidth()/2);
            ac->setY(h->getY());
          }
        } 
        cdebug_log(145,0) << "[Create AutoContact]: " << ac << endl;
        autoContacts.push_back(ac);
      
        cdebug_log(145,0) << "FromHook: " << _fromHook << endl << endl;
        if (_fromHook->getComponent() == (*it)->getComponent()){
          cdebug_log(145,0) << "Found from:" << (*it)->getComponent() << endl;
          targetContact = ac;
        } else {
          push( (*it), ac );
        }
        i++;
      }   
      cdebug_log(145,0) << "Chain contacts: "  << endl;
      for (size_t j=1; j < autoContacts.size(); j++){
        if (autoContacts[j-1] != autoContacts[j]){
          AutoSegment* segment = AutoSegment::create( autoContacts[j-1] , autoContacts[j], Flags::Vertical );
          if (not segment->isGlobal()) segment->setFlags( AutoSegment::SegLongLocal );
          cdebug_log(145,0) << "[Create global segment (5)]: " << segment << endl;
        }
      }
    // There are only 2 AutoContacts to create
    } else if (hooks.size() == 2){
      cdebug_log(145,0) << "Number of hooks == 2 : " <<  hooks.size() << endl;
      Horizontal*  h0  = dynamic_cast<Horizontal*>(hooks[0]->getComponent());
      Vertical*    v0  = dynamic_cast<Vertical*>  (hooks[0]->getComponent());
      Horizontal*  h1  = dynamic_cast<Horizontal*>(hooks[1]->getComponent());
      Vertical*    v1  = dynamic_cast<Vertical*>  (hooks[1]->getComponent());
      AutoContact* source = NULL;
      AutoContact* target = NULL;
      cdebug_log(145,0) << "h0: " << h0 << endl;
      cdebug_log(145,0) << "v0: " << v0 << endl;
      cdebug_log(145,0) << "h1: " << h1 << endl;
      cdebug_log(145,0) << "v1: " << v1 << endl;
          
      if ((h0 != NULL) && (h1 != NULL)){
        cdebug_log(145,0) << "case 2H" << endl;
        source = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
        source->setX(_gcell->getXMin() + _gcell->getWidth()/2);
        source->setY(h0->getY());

        target = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
        target->setX(_gcell->getXMin() + _gcell->getWidth()/2);
        target->setY(h1->getY());

        cdebug_log(145,0) << "[Create AutoContact Source]: " << source << endl;
        cdebug_log(145,0) << "[Create AutoContact Target]: " << target << endl;

        AutoSegment* globalSegment = AutoSegment::create( source, target, Flags::Vertical );
        if (not globalSegment->isGlobal()) globalSegment->setFlags( AutoSegment::SegLongLocal );
        cdebug_log(145,0) << "[Create global segment (6)]: " << globalSegment << endl;

        if        (_fromHook->getComponent() == hooks[0]->getComponent()){
          cdebug_log(145,0) << "Found from:" << hooks[0]->getComponent() << endl;
          targetContact = source;
          push( hooks[1], target );
          
        } else if (_fromHook->getComponent() == hooks[1]->getComponent()){
          cdebug_log(145,0) << "Found from:" << hooks[1]->getComponent() << endl;
          targetContact = target;
          push( hooks[0], source );
        }

      } else if (((v0 != NULL) && (h1 != NULL)) || ((h0 != NULL) && (v1 != NULL))){
        cdebug_log(145,0) << "case 1V and 1H" << endl;
        source = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
        if (v0 && h1){
          source->setY(h1->getY());
        } else {
          source->setY(h0->getY());
        }
        source->setX(_gcell->getXMin() + _gcell->getWidth()/2);
        targetContact = source;

        cdebug_log(145,0) << "[Create AutoContact]: " << source << endl;

        if        (_fromHook->getComponent() == hooks[0]->getComponent()){
          cdebug_log(145,0) << "Found from:" << hooks[0]->getComponent() << endl;
          push( hooks[1], source );
          
        } else if (_fromHook->getComponent() == hooks[1]->getComponent()){
          cdebug_log(145,0) << "Found from:" << hooks[1]->getComponent() << endl;
          push( hooks[0], source );
        }
      } else if ((v0 != NULL) && (v1 != NULL)){
        cdebug_log(145,0) << "case 2V" << endl;
        source = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
        source->setX(_gcell->getXMin() + _gcell->getWidth()/2);
        source->setY(_gcell->getYMin() + _gcell->getHeight()/2);
        target = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
        target->setX(_gcell->getXMin() + _gcell->getWidth()/2);
        target->setY(_gcell->getYMin() + _gcell->getHeight()/2);

        cdebug_log(145,0) << "[Create AutoContact Source]: " << source << endl;
        cdebug_log(145,0) << "[Create AutoContact Target]: " << target << endl;

        AutoSegment* globalSegment = AutoSegment::create( source, target, Flags::Horizontal );
        if (not globalSegment->isGlobal()) globalSegment->setFlags( AutoSegment::SegLongLocal );
        cdebug_log(145,0) << "[Create global segment (7)]: " << globalSegment << endl;

        if        (_fromHook->getComponent() == hooks[0]->getComponent()){
          cdebug_log(145,0) << "Found from:" << hooks[0]->getComponent() << endl;
          targetContact = source;
          push( hooks[1], target );
          
        } else if (_fromHook->getComponent() == hooks[1]->getComponent()){
          cdebug_log(145,0) << "Found from:" << hooks[1]->getComponent() << endl;
          targetContact = target;
          push( hooks[0], source );
        }
      }
    }
    cdebug_log(145,0) << "doVChannel done" << endl;
    cdebug_tabw(145,-1);

    return targetContact;
  }

  
  AutoContact* NetBuilder::_doHRail ()
  {
    cdebug_log(145,1) << "void  NetBuilder::_doHRail ()" << _gcell << endl;
    cdebug_log(145,0) << "FromHook: " << _fromHook << endl;
    cdebug_log(145,0) << "North   : " << north()   << endl;
    cdebug_log(145,0) << "East    : " << east()    << endl;
    cdebug_log(145,0) << "South   : " << south()   << endl;
    cdebug_log(145,0) << "West    : " << west()    << endl;
    cdebug_log(145,0) << "_routingPads.size(): " << _routingPads.size() << endl;

    RoutingPad*  rpNE = NULL;
    RoutingPad*  rpSW = NULL;
    _southWestContact = NULL;
    if (_routingPads.size() == 1){
      rpNE = rpSW = _routingPads[0];
    } else {
      cdebug_log(145,0) << "Case _routingPads.size() != 1 "<< endl;
      throw Error( "NetBuilder::_doHRail() Unexpected case.\n"
                   "        On: %s."
                 , getString(_gcell).c_str() );
    }
    cdebug_log(145,0) << "rp: " << _routingPads[0] << endl;
    AutoContact* ac = doRp_AccessAnalog( _gcell, rpNE, NoFlags );
    if (east()) {
      cdebug_log(145,0) << "East"  << endl;
      push( east(), ac, SouthWest );
    } 
    if (west()) {
      cdebug_log(145,0) << "West"  << endl;
      push( west(), ac, SouthWest );
    }
    if (south()) {
      cdebug_log(145,0) << "South"  << endl;
      push( south(), ac, SouthWest );
    }
    if (north()){
      cdebug_log(145,0) << "North"  << endl;
      push( north(), ac, SouthWest );
    }
    
    cdebug_log(145,0) << "doHRail done" << endl;
    cdebug_tabw(145,-1);
    
    return _southWestContact;
  }

  
  AutoContact* NetBuilder::_doVRail ()
  {
    cdebug_log(145,1) << "void  NetBuilder::_doVRail ()" << _gcell << endl;
    cdebug_log(145,0) << "FromHook: " << _fromHook << endl;
    cdebug_log(145,0) << "North   : " << north()   << endl;
    cdebug_log(145,0) << "East    : " << east()    << endl;
    cdebug_log(145,0) << "South   : " << south()   << endl;
    cdebug_log(145,0) << "West    : " << west()    << endl;
    cdebug_log(145,0) << "_routingPads.size(): " << _routingPads.size() << endl;

    RoutingPad*  rpNE = NULL;
    RoutingPad*  rpSW = NULL;
    _southWestContact = NULL;
    if (_routingPads.size() == 1){
      rpNE = rpSW = _routingPads[0];
    } else {
      cdebug_log(145,0) << "Case _routingPads.size() != 1 "<< endl;
      throw Error( "NetBuilder::_doVRail() Unexpected case.\n"
                   "        On: %s."
                 , getString(_gcell).c_str() );
    }
    cdebug_log(145,0) << "rp: " << _routingPads[0] << endl;
    AutoContact* ac = doRp_AccessAnalog( _gcell, rpNE, NoFlags );
    if (east()) {
      cdebug_log(145,0) << "East"  << endl;
      push( east(), ac, SouthWest );
    } 
    if (west()) {
      cdebug_log(145,0) << "West"  << endl;
      push( west(), ac, SouthWest );
    }
    if (south()) {
      cdebug_log(145,0) << "South"  << endl;
      push( south(), ac, SouthWest );
    }
    if (north()){
      cdebug_log(145,0) << "North"  << endl;
      push( north(), ac, SouthWest );
    }

    cdebug_log(145,0) << "doVRail done" << endl;
    cdebug_tabw(145,-1);
    
    return _southWestContact;
  }


  unsigned int NetBuilder::getNumberGlobals ()
  {
    unsigned int i = 0;
    if (north()) i++;
    if (south()) i++;
    if (east() ) i++;
    if (west() ) i++;
    return i;
  }


  unsigned int NetBuilder::getDeviceNeighbourBound()
  {
    unsigned int bound = 0;
    if (north()){
      if (_gcell->getNorth()->isDevice()) bound = NorthBound;
    } else if (south()){
      if (_gcell->getSouth()->isDevice()) bound = SouthBound;
    } else if (east()){
      if (_gcell->getEast()->isDevice() ) bound = EastBound;
    } else if (west()){
      if (_gcell->getWest()->isDevice() ) bound = WestBound;
    }
    return bound;
  }


  AutoContact*  NetBuilder::_doStrut ()
  {
    cdebug_log(145,1) << "void  NetBuilder::_doStrut ()" << _gcell << endl;

    AutoContact* targetContact   = NULL; // Contact for fromHook segment
    cdebug_log(145,0) << "FromHook: " << _fromHook  << endl;
    cdebug_log(145,0) << "North   : " << north()     << endl;
    cdebug_log(145,0) << "East    : " << east()      << endl;
    cdebug_log(145,0) << "South   : " << south()     << endl;
    cdebug_log(145,0) << "West    : " << west()      << endl;

  // Determine NE and SW contacts
    if ( getNumberGlobals() == 2 ){
      cdebug_log(145,0) << "Case 2 globals: " <<  getNumberGlobals() << endl;

      AutoContact* source = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
      targetContact = source;
      if (  ((north() != NULL) && (west() != NULL)) 
         || ((north() != NULL) && (east() != NULL))
         || ((south() != NULL) && (west() != NULL))
         || ((south() != NULL) && (east() != NULL))
         ){
        if        ((north() != NULL) && (west() != NULL)) {
          cdebug_log(145,0) << "North: " << north() << endl;
          cdebug_log(145,0) << "West : " << west() << endl;
          source->setX(north()->getComponent()->getX());
          source->setY(west()->getComponent ()->getY());
        } else if ((north() != NULL) && (east() != NULL)) {
          cdebug_log(145,0) << "North: " << north() << endl;
          cdebug_log(145,0) << "East : " << east() << endl;
          source->setX(north()->getComponent()->getX());
          source->setY(east()->getComponent ()->getY());
        } else if ((south() != NULL) && (west() != NULL)) {
          cdebug_log(145,0) << "South: " << south() << endl;
          cdebug_log(145,0) << "West : " << west() << endl;
          source->setX(south()->getComponent()->getX());
          source->setY(west()->getComponent ()->getY());
        } else if ((south() != NULL) && (east() != NULL)) {
          cdebug_log(145,0) << "South: " << south() << endl;
          cdebug_log(145,0) << "East: "  << east() << endl;
          source->setX(south()->getComponent()->getX());
          source->setY(east()->getComponent ()->getY());
        }

        cdebug_log(145,0) << "[Create AutoContact]: " << source << endl;
        push( east() , source );
        push( west() , source );
        push( north(), source );
        push( south(), source );

      } else if ((north() != NULL) && (south() != NULL)) {
        cdebug_log(145,0) << "Case NS" <<  endl;
        AutoContact* target = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
        source->setX(north()->getComponent()->getX());
        source->setY(_gcell->getYMin() + _gcell->getHeight()/2);
        target->setX(south()->getComponent()->getX());
        target->setY(_gcell->getYMin() + _gcell->getHeight()/2);

        if (north()->getComponent()->getX() > south()->getComponent()->getX()) {
          swap( source, target );
          if ( north() and (_fromHook != north()) ){
            push( north(), target );
          } else if ( south() and (_fromHook != south()) ){
            push( south(), source );
            targetContact = target;
          } else {
            cerr << Warning( "Something is wrong with the globals and the fromHook in this Strut 1."
                           , getString(_gcell).c_str() )
                 << endl;
          }
        } else {
          if ( north() and (_fromHook != north()) ){ 
            push( north(), source );
            targetContact = target;
          } else if ( south() and (_fromHook != south()) ){
            push( south(), target );
          } else {
            cerr << Warning( "Something is wrong with the globals and the fromHook in this Strut 2."
                           , getString(_gcell).c_str() )
                 << endl;
          }
        }
        cdebug_log(145,0) << "[Create AutoContact]: " << source << endl;
        cdebug_log(145,0) << "[Create AutoContact]: " << target << endl;

        AutoSegment* globalSegment = AutoSegment::create( source, target, Flags::Horizontal );
        if (not globalSegment->isGlobal()) globalSegment->setFlags( AutoSegment::SegLongLocal );
        cdebug_log(145,0) << "[Create global segment (8)]: " << globalSegment << endl;
      
      } else if ((east() != NULL) && (west() != NULL) ) {
        cdebug_log(145,0) << "Case EW" <<  endl;
        AutoContact* target = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
        source->setX(_gcell->getXMin() + _gcell->getWidth()/2);
        source->setY(east()->getComponent()->getY());
        target->setX(_gcell->getXMin() + _gcell->getWidth()/2);
        target->setY(west()->getComponent()->getY());
        cdebug_log(145,0) << "1" <<  endl;
        
        if (east()->getComponent()->getY() > west()->getComponent()->getY()){
          cdebug_log(145,0) << "2.1" <<  endl;
          swap( source, target );

          cdebug_log(145,0) << "3.1" <<  endl;
          if ( east() and (_fromHook != east()) ){
            push( east(), target );
          } else if ( west() and (_fromHook != west()) ){
            push( west(), source );
            targetContact = target;
          } else {
            cerr << Warning( "Something is wrong with the globals and the fromHook in this Strut. 3"
                           , getString(_gcell).c_str() )
                 << endl;
          }
        } else {
          cdebug_log(145,0) << "2.2" <<  endl;
          if ( east() and (_fromHook != east()) ){ 
            push( east(), source );
            targetContact = target;
          } else if ( west() and (_fromHook != west()) ){
            push( west(), target );
          } else {
            cerr << Warning( "Something is wrong with the globals and the fromHook in this Strut. 4"
                           , getString(_gcell).c_str() )
                 << endl;
          }
        }
        cdebug_log(145,0) << "[Create AutoContact]: " << source << endl;
        cdebug_log(145,0) << "[Create AutoContact]: " << target << endl;

        AutoSegment* globalSegment = AutoSegment::create( source, target, Flags::Vertical );
        if (not globalSegment->isGlobal()) globalSegment->setFlags( AutoSegment::SegLongLocal );
        cdebug_log(145,0) << "[Create global segment (9)]: " << globalSegment << endl;

      } else {
        cerr << Warning( "Something is wrong with the globals in this Strut. 5 "
                       , getString(_gcell).c_str() )
             << endl;
      }
    } else if ( getNumberGlobals() == 3 ){
      cdebug_log(145,0) << "Case 3 globals: " <<  getNumberGlobals() << endl;

      AutoContact* turn = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
      AutoContact* xtee = NULL;
    //xtee = AutoContactHTee::create( _gcell, _net, Session::getContactLayer(2) );

      if        ((north() != NULL) && (south() != NULL) && (east() != NULL)){ 
        cdebug_log(145,0) << "Case NSE " << endl;
        xtee = AutoContactHTee::create( _gcell, _net, Session::getContactLayer(1) );
        if (north()->getComponent()->getX() < south()->getComponent()->getX()){
          turn->setX(north()->getComponent()->getX());
          xtee->setX(south()->getComponent()->getX());
          if ( north() and (_fromHook != north()) ) push( north(), turn );
          else                                      targetContact = turn;
          if ( south() and (_fromHook != south()) ) push( south(), xtee );
          else                                      targetContact = xtee;

        } else {
          turn->setX(south()->getComponent()->getX());
          xtee->setX(north()->getComponent()->getX());
          if ( north() and (_fromHook != north()) ) push( north(), xtee );
          else                                      targetContact = xtee;
          if ( south() and (_fromHook != south()) ) push( south(), turn );
          else                                      targetContact = turn;
        }
        turn->setY(east()->getComponent()->getY());
        xtee->setY(east()->getComponent()->getY());
        if ( east() and (_fromHook != east()) ) push( east(), xtee );
        else                                    targetContact = xtee;
     
        cdebug_log(145,0) << "[Create AutoContact]: " << turn << endl;
        cdebug_log(145,0) << "[Create AutoContact]: " << xtee << endl;
        AutoSegment* globalSegment = AutoSegment::create( turn, xtee, Flags::Horizontal );
        if (not globalSegment->isGlobal()) globalSegment->setFlags( AutoSegment::SegLongLocal );
        cdebug_log(145,0) << "[Create global segment (10)]: " << globalSegment << endl;

      } else if ((north() != NULL) && (south() != NULL) && (west() != NULL)){ 
        cdebug_log(145,0) << "Case NSW " << endl;
        xtee = AutoContactHTee::create( _gcell, _net, Session::getContactLayer(1) );

        if (north()->getComponent()->getX() < south()->getComponent()->getX()){
          xtee->setX(north()->getComponent()->getX());
          turn->setX(south()->getComponent()->getX());
          if ( north() and (_fromHook != north()) ) push( north(), xtee );
          else                                      targetContact = xtee;
          if ( south() and (_fromHook != south()) ) push( south(), turn );
          else                                      targetContact = turn;

        } else {
          xtee->setX(south()->getComponent()->getX());
          turn->setX(north()->getComponent()->getX());
          if ( north() and (_fromHook != north()) ) push( north(), turn );
          else                                      targetContact = turn;
          if ( south() and (_fromHook != south()) ) push( south(), xtee );
          else                                      targetContact = xtee;
        }
        turn->setY(west()->getComponent()->getY());
        xtee->setY(west()->getComponent()->getY());
        if ( west() and (_fromHook != west()) ) push( west(), xtee );
        else                                    targetContact = xtee;
     
        cdebug_log(145,0) << "[Create AutoContact]: " << xtee << endl;
        cdebug_log(145,0) << "[Create AutoContact]: " << turn << endl;
        AutoSegment* globalSegment = AutoSegment::create( xtee, turn, Flags::Horizontal );
        if (not globalSegment->isGlobal()) globalSegment->setFlags( AutoSegment::SegLongLocal );
        cdebug_log(145,0) << "[Create global segment (11)]: " << globalSegment << endl;


      } else if ((east() != NULL)  && (north() != NULL) && (west() != NULL)){
        cdebug_log(145,0) << "Case EWN " << endl;
        xtee = AutoContactVTee::create( _gcell, _net, Session::getContactLayer(2) );

        if (east()->getComponent()->getY() < west()->getComponent()->getY()){
          turn->setY(east()->getComponent()->getY());
          xtee->setY(west()->getComponent()->getY());
          if ( east() and (_fromHook != east()) ) push( east(), turn );
          else                                    targetContact = turn;
          if ( west() and (_fromHook != west()) ) push( west(), xtee );
          else                                    targetContact = xtee;

        } else {
          turn->setY(west()->getComponent()->getY());
          xtee->setY(east()->getComponent()->getY());
          if ( east() and (_fromHook != east()) ) push( east(), xtee );
          else                                    targetContact = xtee;
          if ( west() and (_fromHook != west()) ) push( west(), turn );
          else                                    targetContact = turn;
        }
        turn->setX(north()->getComponent()->getX());
        xtee->setX(north()->getComponent()->getX());
        if ( north() and (_fromHook != north()) ) push( north(), xtee );
        else                                      targetContact = xtee;
     
        cdebug_log(145,0) << "[Create AutoContact]: " << turn << endl;
        cdebug_log(145,0) << "[Create AutoContact]: " << xtee << endl;
        AutoSegment* globalSegment = AutoSegment::create( turn, xtee, Flags::Vertical );
        if (not globalSegment->isGlobal()) globalSegment->setFlags( AutoSegment::SegLongLocal );
        cdebug_log(145,0) << "[Create global segment (12)]: " << globalSegment << endl;

      } else if ((east() != NULL) && (south() != NULL) && (west() != NULL)){
        cdebug_log(145,0) << "Case EWS " << endl;
        xtee = AutoContactVTee::create( _gcell, _net, Session::getContactLayer(2) );

        if (east()->getComponent()->getY() < west()->getComponent()->getY()){
          xtee->setY(east()->getComponent()->getY());
          turn->setY(west()->getComponent()->getY());
          if ( east() and (_fromHook != east()) ) push( east(), xtee );
          else                                    targetContact = xtee;
          if ( west() and (_fromHook != west()) ) push( west(), turn );
          else                                    targetContact = turn;

        } else {
          xtee->setY(west()->getComponent()->getY());
          turn->setY(east()->getComponent()->getY());
          if ( east() and (_fromHook != east()) ) push( east(), turn );
          else                                    targetContact = turn;
          if ( west() and (_fromHook != west()) ) push( west(), xtee );
          else                                    targetContact = xtee;
        }
        turn->setX(south()->getComponent()->getX());
        xtee->setX(south()->getComponent()->getX());
        if ( south() and (_fromHook != south()) ) push( south(), xtee );
        else                                      targetContact = xtee;
     
        cdebug_log(145,0) << "[Create AutoContact]: " << xtee << endl;
        cdebug_log(145,0) << "[Create AutoContact]: " << turn << endl;
        AutoSegment* globalSegment = AutoSegment::create( xtee, turn, Flags::Vertical );
        if (not globalSegment->isGlobal()) globalSegment->setFlags( AutoSegment::SegLongLocal );
        cdebug_log(145,0) << "[Create global segment (13)]: " << globalSegment << endl;

      } else {
        cerr << Warning( "Something is wrong with the globals in this Strut. 6"
                       , getString(_gcell).c_str() )
             << endl;
      }
    } else if ( getNumberGlobals() == 4 ){
      cdebug_log(145,0) << "Case 4 globals: " <<  getNumberGlobals() << endl;
      AutoContact* turn  = AutoContactTurn::create( _gcell, _net, Session::getContactLayer(1) );
      AutoContact* hteeh = AutoContactHTee::create( _gcell, _net, Session::getContactLayer(2) );
      AutoContact* vteev = AutoContactVTee::create( _gcell, _net, Session::getContactLayer(1) );

      if (  (north()->getComponent()->getX() < south()->getComponent()->getX() )
         && (east()->getComponent ()->getY() < west()->getComponent ()->getY() )
         ) { 
        cdebug_log(145,0) << "(N.X < S.X) & (E.Y < W.Y)" <<  endl;
        turn->setX (north()->getComponent()->getX());
        turn->setY (east()->getComponent ()->getY());
        hteeh->setX(south()->getComponent()->getX());
        hteeh->setY(east()->getComponent ()->getY());
        vteev->setX(north()->getComponent()->getX());
        vteev->setY(west()->getComponent ()->getY());

        if ( east() and (_fromHook != east()) ) push( east(), hteeh );
        else targetContact = hteeh;
        if ( west() and (_fromHook != west()) ) push( west(), vteev );
        else targetContact = vteev;
        if ( north() and (_fromHook != north()) ) push( north(), vteev );
        else targetContact = vteev;
        if ( south() and (_fromHook != south()) ) push( south(), hteeh );
        else targetContact = hteeh;

        cdebug_log(145,0) << "[Create AutoContact]: " << hteeh << endl;
        cdebug_log(145,0) << "[Create AutoContact]: " << vteev << endl;
        cdebug_log(145,0) << "[Create AutoContact]: " << turn << endl;
        AutoSegment* globalSegment1 = AutoSegment::create( turn, hteeh, Flags::Horizontal );
        AutoSegment* globalSegment2 = AutoSegment::create( turn, hteeh, Flags::Vertical );
        cdebug_log(145,0) << "[Create global segment (14.1)]: " << globalSegment1 << endl;
        cdebug_log(145,0) << "[Create global segment (14.2)]: " << globalSegment2 << endl;

      } else if (  (north()->getComponent()->getX() > south()->getComponent()->getX() )
                && (east()->getComponent ()->getY() < west()->getComponent ()->getY() )
                ) {
        cdebug_log(145,0) << "(N.X > S.X) & (E.Y < W.Y)" <<  endl;
        turn->setX (south()->getComponent()->getX());
        turn->setY (west()->getComponent ()->getY());
        hteeh->setX(north()->getComponent()->getX());
        hteeh->setY(east()->getComponent ()->getY());
        vteev->setX(south()->getComponent()->getX());
        vteev->setY(east()->getComponent ()->getY());

        if ( east() and (_fromHook != east()) ) push( east(), hteeh );
        else targetContact = hteeh;
        if ( west() and (_fromHook != west()) ) push( west(), turn );
        else targetContact = turn;
        if ( north() and (_fromHook != north()) ) push( north(), hteeh );
        else targetContact = hteeh;
        if ( south() and (_fromHook != south()) ) push( south(), vteev );
        else targetContact = vteev;

        cdebug_log(145,0) << "[Create AutoContact]: " << hteeh << endl;
        cdebug_log(145,0) << "[Create AutoContact]: " << vteev << endl;
        cdebug_log(145,0) << "[Create AutoContact]: " << turn << endl;
        AutoSegment* globalSegment1 = AutoSegment::create( vteev, hteeh, Flags::Horizontal );
        AutoSegment* globalSegment2 = AutoSegment::create( vteev, turn, Flags::Vertical );
        cdebug_log(145,0) << "[Create global segment (15.1)]: " << globalSegment1 << endl;
        cdebug_log(145,0) << "[Create global segment (15.2)]: " << globalSegment2 << endl;

      } else if (  (north()->getComponent()->getX() < south()->getComponent()->getX() )
                && (east()->getComponent ()->getY() > west()->getComponent ()->getY() )
                ) {
        cdebug_log(145,0) << "(N.X < S.X) & (E.Y > W.Y)" <<  endl;
        turn->setX (north()->getComponent()->getX());
        turn->setY (east()->getComponent ()->getY());
        hteeh->setX(south()->getComponent()->getX());
        hteeh->setY(east()->getComponent ()->getY());
        vteev->setX(south()->getComponent()->getX());
        vteev->setY(west()->getComponent ()->getY());

        if ( east() and (_fromHook != east()) ) push( east(), hteeh );
        else targetContact = hteeh;
        if ( west() and (_fromHook != west()) ) push( west(), vteev );
        else targetContact = vteev;
        if ( north() and (_fromHook != north()) ) push( north(), turn );
        else targetContact = turn;
        if ( south() and (_fromHook != south()) ) push( south(), vteev );
        else targetContact = vteev;

        cdebug_log(145,0) << "[Create AutoContact]: " << hteeh << endl;
        cdebug_log(145,0) << "[Create AutoContact]: " << vteev << endl;
        cdebug_log(145,0) << "[Create AutoContact]: " << turn << endl;
        AutoSegment* globalSegment1 = AutoSegment::create( turn, hteeh, Flags::Horizontal );
        AutoSegment* globalSegment2 = AutoSegment::create( vteev, hteeh, Flags::Vertical );
        cdebug_log(145,0) << "[Create global segment (16.1)]: " << globalSegment1 << endl;
        cdebug_log(145,0) << "[Create global segment (16.2)]: " << globalSegment2 << endl;

      } else {
        cdebug_log(145,0) << "(N.X > S.X) & (E.Y > W.Y)" <<  endl;
        turn->setX (south()->getComponent()->getX());
        turn->setY (east()->getComponent ()->getY());
        hteeh->setX(north()->getComponent()->getX());
        hteeh->setY(east()->getComponent ()->getY());
        vteev->setX(south()->getComponent()->getX());
        vteev->setY(west()->getComponent ()->getY());

        if ( east() and (_fromHook != east()) ) push( east(), hteeh );
        else targetContact = hteeh;
        if ( west() and (_fromHook != west()) ) push( west(), vteev );
        else targetContact = vteev;
        if ( north() and (_fromHook != north()) ) push( north(), hteeh );
        else targetContact = hteeh;
        if ( south() and (_fromHook != south()) ) push( south(), vteev );
        else targetContact = vteev;

        cdebug_log(145,0) << "[Create AutoContact]: " << hteeh << endl;
        cdebug_log(145,0) << "[Create AutoContact]: " << vteev << endl;
        cdebug_log(145,0) << "[Create AutoContact]: " << turn << endl;
        AutoSegment* globalSegment1 = AutoSegment::create( turn, hteeh, Flags::Horizontal );
        AutoSegment* globalSegment2 = AutoSegment::create( vteev, turn, Flags::Vertical );
        cdebug_log(145,0) << "[Create global segment (17.1)]: " << globalSegment1 << endl;
        cdebug_log(145,0) << "[Create global segment (17.2)]: " << globalSegment2 << endl;
      }
    } else { 
      cerr << Warning( "Something is wrong with the number of globals in this Strut. 7"
                     , getString(_gcell).c_str() )
           << endl;
    }
    cdebug_log(145,0) << "doStrut done" << endl;
    cdebug_tabw(145,-1);

    return targetContact;
  }


  void  NetBuilder::_doIoPad ()
  {
    throw Error( "NetBuilder::_doIoPad() Unimplemented, blame goes to J.-P. Chaput." );
  }


  void  NetBuilder::_load ( AnabaticEngine* anabatic, Net* net )
  {
  //DebugSession::open( 145, 150 );

    cdebug_log(149,0) << "NetBuilder::load( " << net << " )" << endl;
    cdebug_tabw(145,1);

    _anabatic = anabatic;

    Hook*        sourceHook    = NULL;
    AutoContact* sourceContact = NULL;
    RoutingPads  routingPads   = net->getRoutingPads();
    size_t       degree        = routingPads.getSize();

    if (degree == 0) {
      cmess2 << Warning("Net \"%s\" do not have any RoutingPad (ignored)."
                       ,getString(net->getName()).c_str()) << endl;
      cdebug_tabw(145,-1);
      return;
    }
    if (degree < 2) {
      cdebug_tabw(145,-1);
      return;
    }

    cdebug_tabw(145,1);
    Hook*  startHook    = NULL;
    GCell* lowestGCell  = NULL;
    size_t unconnecteds = 0;
    size_t connecteds   = 0;

    setDegree( degree );

    cdebug_log(145,0) << "Start RoutingPad Ring" << endl;
    for ( RoutingPad* startRp : routingPads ) {
      bool segmentFound = false;

      cdebug_log(145,0) << "startRp  : " << startRp << endl;
      cdebug_log(145,0) << "startHook: " << startHook << endl;
      for ( Hook* hook : startRp->getBodyHook()->getHooks() ) {
        cdebug_log(145,0) << "Component " << hook->getComponent() << endl;
        Segment* segment = dynamic_cast<Segment*>( hook->getComponent() );

        if (segment) {
          ++connecteds;
          segmentFound = true;

          setStartHook( anabatic, hook, NULL );
          if (getStateG() == 1) {
            if ( (lowestGCell == NULL) or (*getGCell() < *lowestGCell) ) {
              cdebug_log(145,0) << "Potential starting GCell " << getGCell() << endl;
              lowestGCell = getGCell();
              startHook   = hook;
            }
            break;
          }
        }
      }

      unconnecteds += (segmentFound) ? 0 : 1;
      if ( (unconnecteds > 10) and (connecteds == 0) ) {
        cerr << Warning("More than 10 unconnected RoutingPads (%u) on %s, missing global routing?"
                       ,unconnecteds, getString(net->getName()).c_str() ) << endl;

        NetRoutingExtension::create( net )->setFlags  ( NetRoutingState::Excluded );
        NetRoutingExtension::create( net )->unsetFlags( NetRoutingState::AutomaticGlobalRoute );
        cdebug_tabw(145,-1);
        return;
      }
    // Uncomment the next line to disable the lowest GCell search.
    // (takes first GCell with exactly one global).
    //if (startHook) break;
    }
    cdebug_tabw(145,-1);

    if (startHook == NULL) { setStartHook(anabatic,NULL,NULL).singleGCell(anabatic,net); cdebug_tabw(145,-1); return; }

    setStartHook( anabatic, startHook, NULL );
    cdebug_log(145,0) << endl;
    cdebug_log(145,0) << "--------~~~~=={o}==~~~~--------" << endl;
    cdebug_log(145,0) << endl;
    cdebug_log(145,0) << "Start building from:" << getGCell() << endl;
    construct();

    sourceHook    = _forks.getFrom   ();
    sourceContact = _forks.getContact();
    _forks.pop();

    while ( sourceHook ) {
      setStartHook( anabatic, sourceHook, sourceContact );
      construct();

      sourceHook    = _forks.getFrom();
      sourceContact = _forks.getContact();
      _forks.pop();

      cdebug_log(145,0) << "Popping (from) " << sourceHook << endl;
      cdebug_log(145,0) << "Popping (to)   " << sourceContact << endl;
    }

    Session::revalidate();
  //Breakpoint::stop( 0, "After construct" );
    
#if THIS_IS_DISABLED
    set<AutoSegment*>::iterator iover = overconstraineds.begin();
    for ( ; iover != overconstraineds.end() ; ++iover ) {
      (*iover)->makeDogLeg( (*iover)->getAutoSource()->getGCell(), true );
    }
#endif

    Session::revalidate();
    fixSegments();
    cdebug_tabw(145,-1);

  //DebugSession::close();
  }


  string  NetBuilder::getTypeName () const
  { return "NetBuilder"; }


} // Anabatic namespace.
