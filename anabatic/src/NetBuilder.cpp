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
#include "hurricane/Rectilinear.h"
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
  using Hurricane::Rectilinear;

  
// -------------------------------------------------------------------
// Class  :  "NetBuilder".


  string  NetBuilder::functionFlagsToStr ( uint32_t flags ) 
  {
    string s;
    s += ((flags & SortDecreasing  ) ? 'd' : '-');
    s += ((flags & HAccess         ) ? 'h' : '-');
    s += ((flags & HAccessEW       ) ? 'H' : '-');
    s += ((flags & VLarge          ) ? 'V' : '-');
    s += ((flags & VSmall          ) ? 'v' : '-');
    s += ((flags & HSmall          ) ? 'h' : '-');
    s += ((flags & Punctual        ) ? 'p' : '-');
    s += ((flags & M1Offgrid       ) ? 'o' : '-');
    s += ((flags & HCollapse       ) ? 'h' : '-');
    s += ((flags & VCollapse       ) ? 'v' : '-');
    s += ((flags & Terminal        ) ? 't' : '-');
    s += ((flags & DoSourceContact ) ? 'S' : '-');
    s += ((flags & DoTargetContact ) ? 'T' : '-');
    s += ((flags & SouthBound      ) ? 'S' : '-');
    s += ((flags & NorthBound      ) ? 'N' : '-');
    s += ((flags & WestBound       ) ? 'W' : '-');
    s += ((flags & EastBound       ) ? 'E' : '-');
    s += ((flags & Middle          ) ? 'm' : '-');
    s += ((flags & UseNonPref      ) ? 'n' : '-');
    s += ((flags & NoProtect       ) ? 'p' : '-');
    s += ((flags & ToUpperRouting  ) ? 'u' : '-');
    s += ((flags & AddHNonPref     ) ? 'h' : '-');
    return s;
  }

  
  uint64_t  NetBuilder::checkRoutingPadSize ( RoutingPad* rp )
  {
    uint32_t rpFlags = rp->getFlags();
    uint32_t flags   = 0;
    flags |= (rpFlags & RoutingPad::VLarge   ) ? VLarge    : 0;
    flags |= (rpFlags & RoutingPad::VSmall   ) ? VSmall    : 0;
    flags |= (rpFlags & RoutingPad::HSmall   ) ? HSmall    : 0;
    flags |= (rpFlags & RoutingPad::Punctual ) ? Punctual  : 0;
    flags |= (rpFlags & RoutingPad::M1Offgrid) ? M1Offgrid : 0;
    cdebug_log(145,0) << "::checkRoutingPadSize(): "
                      << ((flags & NetBuilder::HSmall  ) ? "HSmall "   : " ")
                      << ((flags & NetBuilder::VSmall  ) ? "VSmall "   : " ")
                      << ((flags & NetBuilder::Punctual) ? "Punctual " : " ")
                      << rp << endl;
    return flags;
  }


  Hook* NetBuilder::getSegmentOppositeHook ( Hook* hook )
  {
    Segment* segment = static_cast<Segment*>( hook->getComponent() );
    return segment->getOppositeHook( hook );
  }


  uint64_t  NetBuilder::getSegmentHookType ( Hook* hook )
  {
    if (not hook) return 0;
    
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
    : _anabatic              (nullptr)
    , _routingStyle          (StyleFlags::NoStyle)
    , _forks                 ()
    , _connexity             ()
    , _topology              (0)
    , _net                   (nullptr)
    , _netData               (nullptr)
    , _gcell                 (nullptr)
    , _sourceContact         (nullptr)
    , _eastContact           (nullptr)
    , _westContact           (nullptr)
    , _northContact          (nullptr)
    , _southContact          (nullptr)
    , _fromHook              (nullptr)
    , _easts                 ()
    , _wests                 ()
    , _norths                ()
    , _souths                ()
    , _routingPads           ()
    , _routingPadAutoSegments()
    , _toFixSegments         ()
    , _degree                (0)
    , _isStrictChannel       (false)
    , _sourceFlags           (0)
    , _flags                 (0)
  { }


  NetBuilder::~NetBuilder ()
  { }


  void  NetBuilder::clear ()
  {
    _connexity.connexity = 0;
    _sourceFlags         = 0;
    _flags               = 0;
    _topology            = 0;
    _net                 = nullptr;
    _netData             = nullptr;
    _gcell               = nullptr;
    _sourceContact       = nullptr;
    _eastContact         = nullptr;
    _westContact         = nullptr;
    _northContact        = nullptr;
    _southContact        = nullptr;
    _southContact        = nullptr;
    _fromHook            = nullptr;
    _easts                 .clear();
    _wests                 .clear();
    _norths                .clear();
    _souths                .clear();
    _routingPads           .clear();
    _routingPadAutoSegments.clear();
    _toFixSegments         .clear();
  }
  

  void  NetBuilder::fixSegments ()
  {
    for ( size_t i=0 ; i<_toFixSegments.size() ; ++i )
      _toFixSegments[i]->setFlags( AutoSegment::SegFixed );
    _toFixSegments.clear();
  }


  NetBuilder& NetBuilder::setStartHook ( AnabaticEngine* anbt
                                       , Hook*           fromHook
                                       , AutoContact*    sourceContact
                                       , uint64_t        sourceFlags )
  {
    clear();

    _isStrictChannel = anbt->isChannelStyle() and not anbt->isHybridStyle();
    _sourceFlags     = sourceFlags;
    _sourceContact   = sourceContact;
    _fromHook        = fromHook;

    cdebug_log(145,1) << "NetBuilder::setStartHook()" << endl;
    cdebug_log(145,0) << "* _fromHook:     " << fromHook << endl;
    cdebug_log(145,0) << "* _sourceContact:" << sourceContact << endl;
    cdebug_log(145,0) << "_isStrictChannel:" << _isStrictChannel << endl;

    if (not _fromHook) {
      cdebug_tabw(145,-1);
      return *this;
    }

    Segment* fromSegment = static_cast<Segment*>( _fromHook->getComponent() );
    _net     = fromSegment->getNet();
    _netData = anbt->getNetData( _net );

    cdebug_log(145,0) << "For Hooks from fromHook" << endl;
    for ( Hook* hook : fromHook->getHooks() ) {
      cdebug_log(145,0) << "Hook: " << hook << endl;
      cdebug_log(145,0) << "Topology [" << _connexity.connexity << "] = "
                        << "["  << (int)_connexity.fields.globals
                        << "+"  << (int)_connexity.fields.M1     
                        << "+"  << (int)_connexity.fields.M2     
                        << "+"  << (int)_connexity.fields.M3
                        << "+"  << (int)_connexity.fields.M4
                        << "+"  << (int)_connexity.fields.M5
                        << "+"  << (int)_connexity.fields.M6
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

            if (  (_gcell->getDensity( Session::getDHorizontalDepth() ) > 0.9)
               or (_gcell->getDensity( Session::getDVerticalDepth  () ) > 0.9)) {
              cdebug_log(145,0) << "Base layers blockeds, moving up" << endl;
              _flags |= ToUpperRouting;
            }

            if (not _gcell->isMatrix()) {
              cdebug_log(145,0) << "* Non-matrix GCell under: " << contact << endl;
              cdebug_log(145,0) << "| " << gcell << endl;
            }
          } else {
            if (rp and AllianceFramework::get()->isPad(rp->_getEntityAs<Component>()->getCell())) {
              _connexity.fields.Pad++;
            } else {
              const Layer* layer = anchor->getLayer();
              cdebug_log(145,0) << "rp: " << rp << endl;

              if (not rp)
                throw Error ( "NetBuilder::setStartHook(): Null RP" );

              if (not layer) {
                cerr << Error( "RoutingPad is still on it's Plug, routing will be incomplete.\n"
                               "        %s"
                             , getString(anchor).c_str() )
                     << endl;
                continue;
              }

              bool   isPin   = (dynamic_cast<Pin*>( rp->getOccurrence().getEntity() ) != nullptr);
              size_t rpDepth = 0;
              for ( size_t depth=0 ; depth < Session::getRoutingGauge()->getDepth() ; ++depth ) {
                if (layer->getMask() == Session::getRoutingLayer(depth)->getMask()) {
                  rpDepth = depth;
                  break;
                }
              }
              if (rpDepth >= Session::getRoutingGauge()->getFirstRoutingLayer())
                rpDepth -= Session::getRoutingGauge()->getFirstRoutingLayer();
              else
                cerr << Error( "Terminal layer \"%s\" of %s is below first usable routing layer."
                               , getString(layer->getName()).c_str()
                               , getString(anchor).c_str() )
                     << endl;
              if ((rpDepth > 0) and not isPin
                                and not Session::getRoutingGauge()->isSuperPitched()) {
                _flags |= ToUpperRouting;
                cdebug_log(145,0) << "ToUpperRouting set, getFlags():" << getFlags() << endl;
              }

              if      (rpDepth == 0) _connexity.fields.M1++; // M1 V
              else if (rpDepth == 1) _connexity.fields.M2++; // M2 H
              else if (rpDepth == 2) _connexity.fields.M3++; // M3 V
              else if (rpDepth == 3) _connexity.fields.M4++; // M4 H
              else if (rpDepth == 4) _connexity.fields.M5++; // M5 V
              else {
                cerr << Warning( "Terminal layer \"%s\" of %s is not managed yet (ignored)."
                               , getString(layer->getName()).c_str()
                               , getString(anchor).c_str() )
                     << endl;
              //continue;
              }

              if (isPin) _connexity.fields.Pin++; 
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

    cdebug_log(145,0) << "Topology [" << _connexity.connexity << "] = "
                      << "["  << (int)_connexity.fields.globals
                      << "+"  << (int)_connexity.fields.M1     
                      << "+"  << (int)_connexity.fields.M2     
                      << "+"  << (int)_connexity.fields.M3
                      << "+"  << (int)_connexity.fields.Pin
                      << "+"  << (int)_connexity.fields.Pad
                      << "] " << _gcell
                      << endl;
    return *this;
  }
    

  bool  NetBuilder::push ( Hook* toHook, AutoContact* contact, uint64_t flags )
  {
    cdebug_log(145,0) << "NetBuilder::push()" << endl;
    cdebug_log(145,0) << "* toHook:   " << toHook << endl;
    cdebug_log(145,0) << "* _fromHook:" << _fromHook << endl;
    cdebug_log(145,0) << "* flags:" << flags << endl;


    if (not toHook or (toHook == _fromHook)) {
      if (contact) {
        if ( (flags & EastBound) and not _eastContact ) {
          cdebug_log(145,0) << "Setting _eastContact:" << contact << endl;
          _eastContact = contact;
        }
        if ( (flags & WestBound) and not _westContact ) {
          cdebug_log(145,0) << "Setting _westContact:" << contact << endl;
          _westContact = contact;
        }
        if ( (flags & SouthBound) and not _southContact ) {
          cdebug_log(145,0) << "Setting _southContact:" << contact << endl;
          _southContact = contact;
        }
        if ( (flags & NorthBound) and not _northContact ) {
          cdebug_log(145,0) << "Setting _northContact:" << contact << endl;
          _northContact = contact;
        }
      }
      return false;
    }

    Hook* toHookOpposite = getSegmentOppositeHook( toHook );
    cdebug_log(145,0) << "Pushing (to)   " << getString(toHook) << endl;
    cdebug_log(145,0) << "Pushing (from) " << contact << endl;
    _forks.push( toHookOpposite, contact, getFlags() );

    return true;
  }


  bool  NetBuilder::isInsideBlockage ( GCell* gcell, Component* rp ) const
  {
    cdebug_log(145,1) << getTypeName() << "::isInsideBlockage() " << endl;
    cdebug_log(145,0) << rp << endl;
    cdebug_log(145,0) << rp->getLayer()->getMask() << endl;

    size_t rpDepth = Session::getLayerDepth( rp->getLayer() );
    if (gcell->getDensity(rpDepth) < 0.5) {
      cdebug_tabw(145,-1);
      return false;
    }
    
    Box         rpBb   = rp->getBoundingBox();
    Layer::Mask rpMask = rp->getLayer()->getBlockageLayer()->getMask();
    cdebug_log(145,0) << "rpBb: " << rpBb << endl;
    for ( Occurrence occurrence : getAnabatic()->getCell()->getOccurrencesUnder(rpBb) ) {
      cdebug_log(145,0) << "| " << occurrence.getEntity() << endl;
      Component* component = dynamic_cast<Component*>( occurrence.getEntity() );
      if (not component) continue;

      const Layer* blockageLayer = component->getLayer();
      Box          blockageBb    = component->getBoundingBox();
      cdebug_log(145,0) << "  Mask: " << blockageLayer->getMask() << endl;
      if (    blockageLayer->isBlockage()
         and (blockageLayer->getMask() == rpMask)) {
        occurrence.getPath().getTransformation().applyOn( blockageBb );
        cdebug_log(145,0) << "  Bb: " << blockageBb << endl;
        if (blockageBb.contains(rpBb)) {
          cdebug_log(145,-1) << "* Inside " << component << endl;
          return true;
        }
        if (blockageBb.intersect(rpBb)) {
          cerr << Warning( "NetBuilder::isInsideBlockage(): RoutingPad is only partially inside blocked area.\n"
                           "           * %s\n"
                           "           * %s"
                         , getString(rp).c_str()
                         , getString(component).c_str()
                         ) << endl; 
          cdebug_log(145,-1) << "* Partially inside " << component << endl;
          return true;
        }
      }
    }
    cdebug_tabw(145,-1);
    return false;
  }
  

  void  NetBuilder::construct ()
  {
    cdebug_log(145,1) << "NetBuilder::construct() [" << _connexity.connexity << "] in " << _gcell << endl;
    cdebug_log(145,0) << "Topology [" << _connexity.connexity << "] = "
                      << "["  << (int)_connexity.fields.globals
                      << "+"  << (int)_connexity.fields.M1     
                      << "+"  << (int)_connexity.fields.M2     
                      << "+"  << (int)_connexity.fields.M3
                      << "+"  << (int)_connexity.fields.M4
                      << "+"  << (int)_connexity.fields.M5
                      << "+"  << (int)_connexity.fields.M6
                      << "+"  << (int)_connexity.fields.Pin
                      << "+"  << (int)_connexity.fields.Pad
                      << "] " << _gcell
                      << endl;
    cdebug_log(145,0) << "getSourceFlags():" << getSourceFlags()
                      << " getFlags():" << getFlags() << endl;

    if (not isStrictChannel()) {
      _eastContact  = nullptr;
      _westContact  = nullptr;
      _northContact = nullptr;
      _southContact = nullptr;
    }

    if (not _gcell->isAnalog()) {
      if (isStrictChannel() and not _sourceContact) {
        cdebug_log(145,0) << "No _sourceContact, resetting _fromHook" << endl;
        _fromHook = NULL;
      }
      
      switch ( _connexity.connexity ) {
        case Conn_1G_1Pad:
        case Conn_2G_1Pad:
        case Conn_3G_1Pad: _do_xG_1Pad  (); break;
        // End xG_1Pad cascaded cases.

        case Conn_1G_1M1:  if (_do_1G_1M1()) break;
        case Conn_1G_2M1:
        case Conn_1G_3M1:
        case Conn_1G_4M1:
        case Conn_1G_5M1:
        case Conn_1G_6M1:
        case Conn_1G_7M1:
        case Conn_1G_8M1:
        case Conn_1G_9M1:   _do_1G_xM1(); break;
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
        case Conn_2G_6M1:
        case Conn_2G_7M1:
        case Conn_2G_8M1:
        case Conn_2G_9M1:  _do_xG_xM1_xM3(); break;
        case Conn_3G_1M1:  if (_do_xG_1M1()) break;
        case Conn_3G_2M1:
        case Conn_3G_3M1:
        case Conn_3G_4M1:
        case Conn_3G_5M1:
        case Conn_3G_6M1:
        case Conn_3G_7M1:
        case Conn_3G_8M1:
        case Conn_3G_9M1:
        case Conn_3G_2M3:
        case Conn_3G_3M3:
        case Conn_3G_4M3:  _do_xG_xM1_xM3(); break;
        case Conn_4G_1M1:  if (_do_xG_1M1()) break;
        case Conn_4G_2M1:
        case Conn_4G_3M1:
        case Conn_4G_4M1:
        case Conn_4G_5M1:
        case Conn_4G_6M1:
        case Conn_4G_7M1:
        case Conn_4G_8M1:
        case Conn_4G_9M1:  _do_xG_xM1_xM3(); break;
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
        case Conn_1G_1M4:
        case Conn_2G_1M4:
        case Conn_3G_1M4:  _do_xG_1M4(); break;
        // End xG_1M4 cascaded cases.
        case Conn_1G_1M5:  _do_1G_1M5(); break;
        case Conn_2G_1M5:  _do_2G_1M5(); break;

        case Conn_2G:      if (_do_2G()) break;
        case Conn_3G:
        case Conn_4G:     _do_xG(); break;
        // End xG cascaded cases.
        // Optimized specific cases.
        case Conn_1G_1PinM1:      _do_1G_1PinM1    (); break;
        case Conn_2G_1PinM1:      _do_2G_1PinM1    (); break;
        case Conn_1G_1PinM4:
        case Conn_1G_1PinM2:      _do_1G_1PinM2    (); break;
        case Conn_2G_1PinM2:                       
        case Conn_3G_1PinM2:
        case Conn_2G_1PinM4:                       
        case Conn_3G_1PinM4:      _do_xG_1PinM2    (); break;
        case Conn_1G_1PinM3:      _do_1G_1PinM3    (); break;
        case Conn_2G_1PinM3:                       
        case Conn_3G_1PinM3:      _do_xG_1PinM3    (); break;
        case Conn_1G_2M1_1PinM1:  _do_1G_xM1_1PinM1(); break;
        case Conn_1G_1M1_1PinM3:  _do_1G_1M1_1PinM3(); break;
        case Conn_1G_1M1_1PinM2:
        case Conn_1G_2M1_1PinM2:
        case Conn_1G_3M1_1PinM2:
        case Conn_1G_4M1_1PinM2:
        case Conn_1G_5M1_1PinM2:
        case Conn_1G_1M1_1PinM4:
        case Conn_1G_2M1_1PinM4:
        case Conn_1G_3M1_1PinM4:
        case Conn_1G_4M1_1PinM4:
        case Conn_1G_5M1_1PinM4:  _do_1G_xM1_1PinM2(); break;
        case Conn_2G_1M1_1PinM2:
        case Conn_2G_2M1_1PinM2:
        case Conn_2G_1M1_1PinM4:
        case Conn_2G_2M1_1PinM4:  _do_2G_xM1_1PinM2(); break;
        case Conn_2G_1M1_1PinM3:
        case Conn_1G_2M1_1PinM3:
        case Conn_2G_2M1_1PinM3:
        case Conn_2G_3M1_1PinM3:  _do_2G_xM1_1PinM3(); break;
        case Conn_3G_1M1_1PinM3:
        case Conn_3G_2M1_1PinM3:
        case Conn_3G_3M1_1PinM3:  _do_3G_xM1_1PinM3(); break;
        case Conn_1G_1M1_1M2:     _do_xG_1M1_1M2   (); break;
        case Conn_1G_1M1_1M3:     _do_1G_xM1       (); break;
        case Conn_2G_1M1_1M2:     _do_xG_1M1_1M2   (); break;
        default:
        //if (not isStrictChannel())
            throw Bug( "Unmanaged Configuration [%d] = [%d+%d+%d+%d+%d,%d+%d] %s in %s\n"
                     "      The global routing seems to be defective."
                     , _connexity.connexity
                     , _connexity.fields.globals
                     , _connexity.fields.M1     
                     , _connexity.fields.M2     
                     , _connexity.fields.M3
                     , _connexity.fields.M4
                     , _connexity.fields.Pin
                     , _connexity.fields.Pad
                     , _net->_getString().c_str()
                     , getString(_gcell).c_str()
                     );
          _do_xG();
      }

      cdebug_log(145,0) << "East:  " << _eastContact << endl;
      cdebug_log(145,0) << "West:  " << _westContact << endl;
      cdebug_log(145,0) << "South: " << _southContact << endl;
      cdebug_log(145,0) << "North: " << _northContact << endl;

      if (not _do_globalSegment()) {
        cdebug_log(145,0) << "No global generated, finish." << endl;
        cdebug_tabw(145,-1);
        return;
      }
      cdebug_log(145,0) << "Proceed to next GCell." << endl;
    } else {
      cdebug_log(145,0) << "Analog mode" << endl;
      if (not _sourceContact) _fromHook = nullptr;

      if      (_gcell->isDevice  ()) _doDevice  ();
      else if (_gcell->isHChannel()) _doHChannel();
      else if (_gcell->isVChannel()) _doVChannel();
      else if (_gcell->isHRail   ()) _doHRail   ();
      else if (_gcell->isVRail   ()) _doVRail   ();
      else if (_gcell->isStrut   ()) _doStrut   ();
      else if (_gcell->isIoPad   ()) _doIoPad   ();
      else
        throw Bug( "Unmanaged GCell type: %s in %s\n"
                   "      The global routing seems to be defective."
                 , getString(_gcell).c_str()
                 , getString(_net).c_str()
                 );


      cdebug_log(145,0) << "* East:  " << _eastContact << endl;
      cdebug_log(145,0) << "* West:  " << _westContact << endl;
      cdebug_log(145,0) << "* South: " << _southContact << endl;
      cdebug_log(145,0) << "* North: " << _northContact << endl;
      cdebug_log(145,0) << "Making global segment from the previous GCell" << endl;
      cdebug_log(145,0) << "* getSourceContact(): " << getSourceContact() << endl;
      cdebug_log(145,0) << "hookType=" << functionFlagsToStr(getSegmentHookType(getFromHook())) << endl;

      if (getSourceContact()) {
        AutoContact* toContact = nullptr;
        switch ( getSegmentHookType(getFromHook()) ) {
          case EastBound:  toContact = getEastContact (); break;
          case WestBound:  toContact = getWestContact (); break;
          case NorthBound: toContact = getNorthContact(); break;
          case SouthBound: toContact = getSouthContact(); break;
        }
        if (not toContact) {
          throw Error( "NetBuilderHV::construct(): NULL *from* contact while building\n"
                       "        %s\n"
                       "        in %s"
                     , getString(getNet()).c_str()
                     , getString(getGCell()).c_str() );
        }
        cdebug_log(145,0) << "* toContact: " << toContact << endl;

        Segment*     baseSegment   = static_cast<Segment*>( getFromHook()->getComponent() );
        AutoSegment* globalSegment = AutoSegment::create( getSourceContact()
                                                        , toContact
                                                        , baseSegment
                                                        );
        globalSegment->setFlags( AutoSegment::SegNoMoveUp );
        cdebug_log(145,0) << "Create global segment: " << globalSegment << endl;
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


  void  NetBuilder::doRp_StairCaseH ( GCell* gcell, RoutingPad* rp1, RoutingPad* rp2 )
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
    AutoSegment::create( subContact2     , rp2ContactSource, Flags::Horizontal );
  }


  void  NetBuilder::doRp_StairCaseV ( GCell* gcell, RoutingPad* rp1, RoutingPad* rp2 )
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
    AutoSegment::create( subContact2     , rp2ContactSource, Flags::Vertical   );
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
    throw Error( "%s::_do_xG_1PinM2() method *not* reimplemented from base class.\n"
                 "        On %s"
               , getTypeName().c_str(), getString(getNet()).c_str() );
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

  
  bool  NetBuilder::_do_xG_1M4 ()
  {
    throw Error ( "%s::_do_2G_1M4() method *not* reimplemented from base class.", getTypeName().c_str() );
    return false;
  }

  
  bool  NetBuilder::_do_1G_1M5 ()
  {
    throw Error ( "%s::_do_1G_1M5() method *not* reimplemented from base class.", getTypeName().c_str() );
    return false;
  }

  
  bool  NetBuilder::_do_2G_1M5 ()
  {
    throw Error ( "%s::_do_2G_1M5() method *not* reimplemented from base class.", getTypeName().c_str() );
    return false;
  }


  bool  NetBuilder::_do_1G_1M1_1PinM3 ()
  {
    throw Error ( "%s::_do_1G_1M1_1PinM3() method *not* reimplemented from base class.\n"
                  "        On %s"
                , getTypeName().c_str()
                , getString(getNet()).c_str() );
    return false;
  }


  bool  NetBuilder::_do_1G_xM1_1PinM2 ()
  {
    throw Error ( "%s::_do_1G_xM1_1PinM2() method *not* reimplemented from base class.", getTypeName().c_str() );
    return false;
  }


  bool  NetBuilder::_do_2G_xM1_1PinM2 ()
  {
    throw Error ( "%s::_do_2G_xM1_1PinM2() method *not* reimplemented from base class.", getTypeName().c_str() );
    return false;
  }


  bool  NetBuilder::_do_2G_xM1_1PinM3 ()
  {
    throw Error ( "%s::_do_2G_xM1_1PinM3() method *not* reimplemented from base class.", getTypeName().c_str() );
    return false;
  }


  bool  NetBuilder::_do_1G_xM1_1PinM1 ()
  {
    throw Error ( "%s::_do_1G_xM1_1PinM1() method *not* reimplemented from base class.", getTypeName().c_str() );
    return false;
  }


  bool  NetBuilder::_do_3G_xM1_1PinM3 ()
  {
    throw Error ( "%s::_do_3G_xM1_1PinM3() method *not* reimplemented from base class.", getTypeName().c_str() );
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
    RoutingPad*          rpM2 = NULL;

    for ( RoutingPad* rp : net->getRoutingPads() ) {
      if (      Session::getRoutingGauge()->getLayerDepth(rp->getLayer())
         == 1 + Session::getRoutingGauge()->getFirstRoutingLayer())
        rpM2 = rp;
      else
        rpM1s.push_back( rp );
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
  

  void  NetBuilder::_doDevice ()
  {
    cdebug_log(145,1) << "NetBuilder::_doDevice()" << _gcell << endl;

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

    RoutingPad* rpNE = nullptr;
    RoutingPad* rpSW = nullptr;
    _southContact = nullptr;
    _westContact  = nullptr;

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

    AutoContact* acRp = nullptr;
    if (rpNE and rpSW) {
      if (east()) {
        cdebug_log(145,0) << "East access" << endl;
        acRp = doRp_AccessAnalog( _gcell, rpNE, NoFlags );
        push( east(), acRp, EastBound );
      } 
      if (west()) {
        cdebug_log(145,0) << "West access" << endl;
        acRp = doRp_AccessAnalog( _gcell, rpSW, NoFlags );
        push( west(), acRp, WestBound );
      }
      if (south()) {
        cdebug_log(145,0) << "South access" << endl;
        acRp = doRp_AccessAnalog( _gcell, rpSW, NoFlags );
        push( south(), acRp, SouthBound );
      }
      if (north()){
        cdebug_log(145,0) << "North access" << endl;
        acRp = doRp_AccessAnalog( _gcell, rpNE, NoFlags );
        push( north(), acRp, NorthBound );
      }
    }
    cdebug_log(145,0) << "doDevice done" << endl;
    cdebug_tabw(145,-1);

  //return acRp;
  }


  void  NetBuilder::_do_2G_Analog ()
  {
    cdebug_log(145,1) << "NetBuilder::_do_2G_Analog()" << _gcell << endl;
    if (getEasts().size() + getWests().size() == 0) {
      cdebug_log(145,0) << "Any combination of North + South case" << endl;
      AutoContact* turns [2] = { nullptr, nullptr };
      turns[0] = AutoContactTurn::create( getGCell(), getNet(), Session::getContactLayer(1) );
      turns[1] = AutoContactTurn::create( getGCell(), getNet(), Session::getContactLayer(1) );
      AutoSegment* h = AutoSegment::create( turns[0], turns[1], Flags::Horizontal );
      size_t iturn = 0;
      if (north(0)) push( north(0), turns[iturn++], NorthBound );
      if (north(1)) push( north(1), turns[iturn++], NorthBound );
      if (south(0)) push( south(0), turns[iturn++], SouthBound );
      if (south(1)) push( south(1), turns[iturn++], SouthBound );
    } else if (getNorths().size() + getSouths().size() == 0) {
      cdebug_log(145,0) << "Any combination of East + West case" << endl;
      AutoContact* turns [2] = { nullptr, nullptr };
      turns[0] = AutoContactTurn::create( getGCell(), getNet(), Session::getContactLayer(1) );
      turns[1] = AutoContactTurn::create( getGCell(), getNet(), Session::getContactLayer(1) );
      AutoSegment* v = AutoSegment::create( turns[0], turns[1], Flags::Vertical );
      size_t iturn = 0;
      if (west(0)) push( west(0), turns[iturn++], WestBound );
      if (west(1)) push( west(1), turns[iturn++], WestBound );
      if (east(0)) push( east(0), turns[iturn++], EastBound );
      if (east(1)) push( east(1), turns[iturn++], EastBound );
    } else {
      cdebug_log(145,0) << "Turning case" << endl;
      AutoContact* turn1 = AutoContactTurn::create( getGCell(), getNet(), Session::getContactLayer(1) );
      if (east ()) push( east (), turn1,  EastBound );
      if (west ()) push( west (), turn1,  WestBound );
      if (north()) push( north(), turn1, NorthBound );
      if (south()) push( south(), turn1, SouthBound );
    }
    cdebug_tabw(145,-1);
  }


  void  NetBuilder::_doHChannel ()
  {
    cdebug_log(145,1) << "NetBuilder::_doHChannel()" << _gcell << endl;

    if (getConnexity().fields.globals < 2) {
      cdebug_tabw(145,-1);
      throw Error( "NetBuilder::_doHChannel(): Less than two connecting wires in %s."
                 , getString( getGCell() ).c_str() );
    }

    if (getConnexity().fields.globals == 2) {
      _do_2G_Analog();
      cdebug_tabw(145,-1);
      return;
    }

    cdebug_log(145,0) << "General case." << endl;
    vector<Hook*> verticalHooks ( getNorths() );
    verticalHooks.insert( verticalHooks.end(), getSouths().begin(), getSouths().end() );
    sortHookByY( verticalHooks, NoFlags );

    AutoContact* htee1 = nullptr;
    AutoContact* htee2 = nullptr;
    AutoSegment* h     = nullptr;
    if (west()) {
      htee1 = AutoContactHTee::create( getGCell(), getNet(), Session::getContactLayer(1) );
      push( west(), htee1, WestBound );
    } else
      htee1 = AutoContactTurn::create( getGCell(), getNet(), Session::getContactLayer(1) );
    switch ( getSegmentHookType(verticalHooks[0]) ) {
      case SouthBound: push( verticalHooks[0], htee1, SouthBound ); break;
      case NorthBound: push( verticalHooks[0], htee1, NorthBound ); break;
    }

    if (verticalHooks.size() < 2) {
      push( east(), htee1, EastBound );
    } else {
      for ( size_t i=1 ; i < verticalHooks.size() ; ++i ) {
        if (east() or (i+1 < verticalHooks.size())) {
          htee2 = AutoContactHTee::create( getGCell(), getNet(), Session::getContactLayer(1) );
          push( east(), htee2, EastBound );
        } else
          htee2 = AutoContactTurn::create( getGCell(), getNet(), Session::getContactLayer(1) );
        h = AutoSegment::create( htee1, htee2, Flags::Horizontal );
        switch ( getSegmentHookType(verticalHooks[i]) ) {
          case SouthBound: push( verticalHooks[i], htee2, SouthBound ); break;
          case NorthBound: push( verticalHooks[i], htee2, NorthBound ); break;
        }
        htee1 = htee2;
      }
    }
    
    cdebug_tabw(145,-1);
  }


  void  NetBuilder::_doVChannel ()
  {
    cdebug_log(145,1) << "NetBuilder::_doVChannel()" << _gcell << endl;

    if (getConnexity().fields.globals < 2) {
      cdebug_tabw(145,-1);
      throw Error( "NetBuilder::_doVChannel(): Less than two connecting wires in %s."
                 , getString( getGCell() ).c_str() );
    }

    if (getConnexity().fields.globals == 2) {
      _do_2G_Analog();
      cdebug_tabw(145,-1);
      return;
    }

    cdebug_log(145,0) << "General case." << endl;
    vector<Hook*> horizontalHooks ( getEasts() );
    horizontalHooks.insert( horizontalHooks.end(), getWests().begin(), getWests().end() );
    sortHookByY( horizontalHooks, NoFlags );

    for ( size_t i=0 ; i<horizontalHooks.size() ; ++i ) {
      cdebug_log(145,0) << "[" << i << "] " << horizontalHooks[i] << endl;
    }

    AutoContact* vtee1 = nullptr;
    AutoContact* vtee2 = nullptr;
    AutoSegment* v     = nullptr;
    if (south()) {
      vtee1 = AutoContactVTee::create( getGCell(), getNet(), Session::getContactLayer(1) );
      push( south(), vtee1, SouthBound );
    } else
      vtee1 = AutoContactTurn::create( getGCell(), getNet(), Session::getContactLayer(1) );
    cdebug_log(145,0) << "[" << 0 << "] " << horizontalHooks[0] << endl;
    switch ( getSegmentHookType(horizontalHooks[0]) ) {
      case WestBound: push( horizontalHooks[0], vtee1, WestBound ); break;
      case EastBound: push( horizontalHooks[0], vtee1, EastBound ); break;
    }
    cdebug_log(145,0) << "First E/W done" << endl;

    if (horizontalHooks.size() < 2) {
      if (north()) push( north(), vtee1, NorthBound );
    } else {
      for ( size_t i=1 ; i < horizontalHooks.size() ; ++i ) {
        if (north() or (i+1 < horizontalHooks.size())) {
          vtee2 = AutoContactVTee::create( getGCell(), getNet(), Session::getContactLayer(1) );
          push( north(), vtee2, NorthBound );
        } else
          vtee2 = AutoContactTurn::create( getGCell(), getNet(), Session::getContactLayer(1) );
        v = AutoSegment::create( vtee1, vtee2, Flags::Vertical );
        switch ( getSegmentHookType(horizontalHooks[i]) ) {
          case WestBound: push( horizontalHooks[i], vtee2, WestBound ); break;
          case EastBound: push( horizontalHooks[i], vtee2, EastBound ); break;
        }
        vtee1 = vtee2;
      }
    }
    
    cdebug_tabw(145,-1);
  }

  
  void  NetBuilder::_doHRail ()
  {
    cdebug_log(145,1) << "NetBuilder::_doHRail() " << _gcell << endl;

    cdebug_log(145,0) << "rp: " << _routingPads[0] << endl;
    AutoContact* ac = doRp_AccessAnalog( _gcell, _routingPads[0], NoFlags );
    push( east (), ac,  EastBound );
    push( west (), ac,  WestBound );
    push( north(), ac, NorthBound );
    push( south(), ac, SouthBound );

    cdebug_tabw(145,-1);
  }

  
  void  NetBuilder::_doVRail ()
  {
    cdebug_log(145,1) << "NetBuilder::_doVRail() " << _gcell << endl;

    cdebug_log(145,0) << "rp: " << _routingPads[0] << endl;
    AutoContact* ac = doRp_AccessAnalog( _gcell, _routingPads[0], NoFlags );
    push( east (), ac,  WestBound );
    push( west (), ac,  EastBound );
    push( north(), ac, SouthBound );
    push( south(), ac, NorthBound );

    cdebug_tabw(145,-1);
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


  void  NetBuilder::_doStrut ()
  {
    cdebug_log(145,1) << "NetBuilder::_doStrut() " << _gcell << endl;

    if (getConnexity().fields.globals < 2) {
      cdebug_tabw(145,-1);
      throw Error( "NetBuilder::_doStrut(): Less than two connecting wires in %s."
                 , getString( getGCell() ).c_str() );
    }

    if (getConnexity().fields.globals == 2) {
      _do_2G_Analog();
      cdebug_tabw(145,-1);
      return;
    }

    if (getConnexity().fields.globals == 3) {
      cdebug_log(145,0) << "Three hooks (globals)" << endl;
      if (not east() or not west()) {
        cdebug_log(145,0) << "No east or west case" << endl;
        AutoContact* turn1 = AutoContactTurn::create( getGCell(), getNet(), Session::getContactLayer(1) );
        AutoContact* htee1 = AutoContactHTee::create( getGCell(), getNet(), Session::getContactLayer(1) );
        AutoSegment::create( turn1, htee1, Flags::Horizontal );
        if (east()) push( east (), htee1,  EastBound );
        if (west()) push( west (), htee1,  WestBound );
        push( north(), turn1, NorthBound );
        push( south(), htee1, SouthBound );
      } else {
        cdebug_log(145,0) << "No north or south case" << endl;
        AutoContact* turn1 = AutoContactTurn::create( getGCell(), getNet(), Session::getContactLayer(1) );
        AutoContact* vtee1 = AutoContactVTee::create( getGCell(), getNet(), Session::getContactLayer(1) );
        AutoSegment::create( turn1, vtee1, Flags::Vertical );
        push( east (), turn1,  EastBound );
        push( west (), vtee1,  WestBound );
        if (north()) push( north(), vtee1, NorthBound );
        if (south()) push( south(), vtee1, SouthBound );
      }
      cdebug_tabw(145,-1);
      return;
    }

    cdebug_log(145,0) << "Four hooks (globals)" << endl;
    AutoContact* tee1 = nullptr;
    AutoContact* tee2 = nullptr;
    if (getGCell()->getWidth() >= getGCell()->getHeight()) {
      tee1 = AutoContactHTee::create( getGCell(), getNet(), Session::getContactLayer(1) );
      tee2 = AutoContactHTee::create( getGCell(), getNet(), Session::getContactLayer(1) );
      AutoSegment::create( tee1, tee2, Flags::Horizontal );
    } else {
      tee1 = AutoContactVTee::create( getGCell(), getNet(), Session::getContactLayer(1) );
      tee2 = AutoContactVTee::create( getGCell(), getNet(), Session::getContactLayer(1) );
      AutoSegment::create( tee1, tee2, Flags::Vertical );
    }
    push( east (), tee2,  EastBound );
    push( west (), tee1,  WestBound );
    push( north(), tee2, NorthBound );
    push( south(), tee1, SouthBound );
    
    cdebug_tabw(145,-1);
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

    _anabatic     = anabatic;
    _routingStyle = anabatic->getConfiguration()->getRoutingStyle();
    cdebug_log(149,0) << "_routingStyle=" << _routingStyle.asString() << endl;
    

    Hook*        sourceHook    = NULL;
    AutoContact* sourceContact = NULL;
    uint64_t     sourceFlags   = NoFlags;
    RoutingPads  routingPads   = net->getRoutingPads();
    size_t       degree        = routingPads.getSize();

    if (degree == 0) {
      if (not net->isBlockage()) {
        cmess2 << Warning( "Net \"%s\" do not have any RoutingPad (ignored)."
                         , getString(net->getName()).c_str() ) << endl;
      }
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

    Box bbNet;
    cdebug_log(145,0) << "Start RoutingPad Ring" << endl;
    for ( RoutingPad* startRp : routingPads ) {
      bbNet.merge( startRp->getBoundingBox() );
      
      bool segmentFound = false;

      cdebug_log(145,0) << "startRp  : " << startRp << endl;
      cdebug_log(145,0) << "startHook: " << startHook << endl;
      for ( Hook* hook : startRp->getBodyHook()->getHooks() ) {
        cdebug_log(145,0) << "Component " << hook->getComponent() << endl;
        Segment* segment = dynamic_cast<Segment*>( hook->getComponent() );

        if (segment) {
          ++connecteds;
          segmentFound = true;

          setStartHook( anabatic, hook, NULL, NoFlags );
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

    if (startHook == NULL) { setStartHook(anabatic,NULL,NULL,NoFlags).singleGCell(anabatic,net); cdebug_tabw(145,-1); return; }

    setStartHook( anabatic, startHook, NULL, NoFlags );
    cdebug_log(145,0) << endl;
    cdebug_log(145,0) << "--------~~~~=={o}==~~~~--------" << endl;
    cdebug_log(145,0) << endl;
    cdebug_log(145,0) << "Start building from:" << getGCell() << endl;
    construct();

    sourceHook    = _forks.getFrom   ();
    sourceContact = _forks.getContact();
    sourceFlags   = _forks.getFlags  ();
    _forks.pop();

    while ( sourceHook ) {
      setStartHook( anabatic, sourceHook, sourceContact, sourceFlags );
      construct();

      sourceHook    = _forks.getFrom   ();
      sourceContact = _forks.getContact();
      sourceFlags   = _forks.getFlags  ();
      _forks.pop();

      cdebug_log(145,0) << "Popping (from)  " << sourceHook << endl;
      cdebug_log(145,0) << "Popping (to)    " << sourceContact << endl;
      cdebug_log(145,0) << "Popping (flags) " << sourceFlags << endl;
    }

    if (   (bbNet.getWidth () < Session::getSmallNetWidth ())
       and (bbNet.getHeight() < Session::getSmallNetHeight())) {
      for ( Segment* segment : net->getComponents().getSubSet<Segment*>() ) {
        AutoSegment* autoSegment = Session::lookup( segment );
        if (autoSegment)
          autoSegment->setFlags( AutoSegment::SegUnbreakable );
      }
    }

    Session::revalidate();
  //Breakpoint::stop( 0, "After construct" );
    
#if THIS_IS_DISABLED
    set<AutoSegment*>::iterator iover = overconstraineds.begin();
    for ( ; iover != overconstraineds.end() ; ++iover ) {
      (*iover)->makeDogLeg( (*iover)->getAutoSource()->getGCell(), true );
    }
    Session::revalidate();
#endif

    fixSegments();
    Session::revalidate();
    cdebug_tabw(145,-1);

  //DebugSession::close();
  }


  string  NetBuilder::getTypeName () const
  { return "NetBuilder"; }


} // Anabatic namespace.
