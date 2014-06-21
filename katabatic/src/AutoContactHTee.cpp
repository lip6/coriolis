// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2012-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./AutoContactHTee.cpp"                    |
// +-----------------------------------------------------------------+


#include  <cstdlib>
#include  <climits>
#include  <sstream>
#include  "hurricane/Bug.h"
#include  "hurricane/Error.h"
#include  "hurricane/Warning.h"
#include  "hurricane/Layer.h"
#include  "hurricane/ViaLayer.h"
#include  "hurricane/BasicLayer.h"
#include  "hurricane/Technology.h"
#include  "hurricane/Net.h"
#include  "hurricane/Plug.h"
#include  "hurricane/Vertical.h"
#include  "hurricane/Horizontal.h"
#include  "hurricane/DebugSession.h"
#include  "crlcore/RoutingGauge.h"
#include  "katabatic/AutoContactHTee.h"
#include  "katabatic/AutoVertical.h"
#include  "katabatic/AutoHorizontal.h"
#include  "katabatic/Session.h"


namespace Katabatic {

  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::DebugSession;
  using Hurricane::ltracein;
  using Hurricane::ltraceout;


// -------------------------------------------------------------------
// Class  :  "Katabatic::AutoContactHTee".


  AutoContactHTee* AutoContactHTee::create ( GCell* gcell, Net* net, const Layer* layer )
  {
    DbU::Unit viaSide = Session::getViaWidth( layer );
    Contact*  contact = Contact::create ( net
                                        , layer
                                        , gcell->getCenter().getX()
                                        , gcell->getCenter().getY()
                                        , viaSide
                                        , viaSide
                                        );
    AutoContactHTee* autoContact = new AutoContactHTee ( gcell, contact );

    autoContact->_postCreate();
    autoContact->unsetFlags( CntInCreationStage );

    ltrace(90) << "create(net*) " << autoContact << endl;
    return autoContact;
  }


  AutoContactHTee::AutoContactHTee ( GCell* gcell, Contact* contact )
    : AutoContact (gcell,contact)
    , _horizontal1(NULL)
    , _horizontal2(NULL)
    , _vertical1  (NULL)
  {
    setFlags( CntHTee );
  }


  AutoContactHTee::~AutoContactHTee ()
  { }


  AutoSegment* AutoContactHTee::getOpposite ( const AutoSegment* from ) const
  {
    if (from == _horizontal1) return _horizontal2;
    if (from == _horizontal2) return _horizontal1;
    return NULL;
  }


  AutoSegment* AutoContactHTee::getSegment ( unsigned int index ) const
  {
    AutoSegment* segment = NULL;
    switch ( index ) {
      case 0: return _horizontal1; 
      case 1: return _horizontal2; 
      case 2: return _vertical1;
    }
  //if (not segment)
  //  cerr << Error( "In %s:\n        No cached segment at index %d"
  //               , getString(this).c_str(), index ) << endl;
    return segment;
  }


  void  AutoContactHTee::_invalidate ( unsigned int )
  {
    unsigned int flags = KbPropagate;
    if (_horizontal1 and _horizontal2) {
      if (_horizontal1->isInvalidated() xor _horizontal2->isInvalidated()) 
        flags = KbNoFlags;
    }

    if (_horizontal1) _horizontal1->invalidate( flags );
    if (_horizontal2) _horizontal2->invalidate( flags );
    if (_vertical1  ) _vertical1  ->invalidate();
  }


  void  AutoContactHTee::cacheDetach ( AutoSegment* segment )
  {
    ltrace(110) << _getTypeName() << "::_cacheDetach() " << this << endl;

    if      (segment == _horizontal1) _horizontal1 = NULL;
    else if (segment == _horizontal2) _horizontal2 = NULL;
    else if (segment == _vertical1)   _vertical1   = NULL;
    else return;

    setFlags( CntInvalidatedCache );
  }


  void  AutoContactHTee::cacheAttach ( AutoSegment* segment )
  {
    ltrace(110) << _getTypeName() << "::cacheAttach() " << this << endl;
    ltracein(110);
    ltrace(110) << "Attaching: " << segment << endl;

    if (segment->getDirection() == KbHorizontal) {
      if      (not _horizontal1) _horizontal1 = static_cast<AutoHorizontal*>(segment);
      else if (not _horizontal2) _horizontal2 = static_cast<AutoHorizontal*>(segment);
      else {
        cerr << Bug( "%s::cacheAttach() On %s,\n"
                     "      h1 & h2 cache have not been cleared first, cancelled."
                   , _getTypeName().c_str(), getString(this).c_str()
                   ) << endl;
        ltraceout(110);
        return;
      }
    } else if (segment->getDirection() == KbVertical) {
      if (_vertical1) {
        cerr << Bug( "%s::cacheAttach() On %s,\n"
                     "      v1 cache has not been cleared first, cancelled."
                   , _getTypeName().c_str(), getString(this).c_str()
                   ) << endl;
        ltraceout(110);
        return;
      }
      _vertical1 = static_cast<AutoVertical*>(segment);
    }

    if (_horizontal1 and _horizontal2 and _vertical1)
      unsetFlags( CntInvalidatedCache  );

    ltraceout(110);
  }


  void  AutoContactHTee::updateCache ()
  {
    DebugSession::open( getNet(), 80 );

    ltrace(110) << _getTypeName() << "::updateCache() " << this << endl;
    ltracein(110);

    Component*   anchor;
    Horizontal** horizontals = new Horizontal* [3];
    Vertical**   verticals   = new Vertical*   [3];

    _getTopology( base(), anchor, horizontals, verticals, 3 );

    _horizontal1 = static_cast<AutoHorizontal*>( Session::lookup(horizontals[0]) );
    _horizontal2 = static_cast<AutoHorizontal*>( Session::lookup(horizontals[1]) );
    _vertical1   = static_cast<AutoVertical  *>( Session::lookup(verticals  [0]) );

    string message;
    if      (horizontals[0] == NULL) message = "HTee has less than two horizontal segments.";
    else if (horizontals[1] == NULL) message = "HTee has less than two horizontal segments.";
    else if (horizontals[2] != NULL) message = "HTee has more than two horizontal segments.";
    else if (verticals  [0] == NULL) message = "HTee is missing mandatory vertical segment.";
    else if (verticals  [1] != NULL) message = "HTee has more than one vertical segment.";
    else if (_horizontal1   == NULL) message = "AutoSegment lookup failed on first horizontal segment.";
    else if (_horizontal2   == NULL) message = "AutoSegment lookup failed on second horizontal segment.";
    else if (_vertical1     == NULL) message = "AutoSegment lookup failed on vertical segment.";
    else if (   (not _horizontal1->isCreated() and not _horizontal2->isCreated())
            and (_horizontal1->getY() != _horizontal2->getY()) ) {
      message = "HTee has misaligned horizontal segments";
      message += " h1:" + getString(_horizontal1->getY());
      message += " h2:" + getString(_horizontal2->getY());
    }
    if (not message.empty()) {
      showTopologyError( message );
      setFlags( CntBadTopology );
    }
    unsetFlags( CntInvalidatedCache );

    ltrace(110) << "h1:" << _horizontal1 << endl;
    ltrace(110) << "h2:" << _horizontal2 << endl;
    ltrace(110) << "v1:" << _vertical1 << endl;

    delete [] horizontals;
    delete [] verticals;

    ltraceout(110);
    DebugSession::close();
  }


  void  AutoContactHTee::updateGeometry ()
  {
    DebugSession::open( getNet(), 80 );

    ltrace(110) << _getTypeName() << "::updateGeometry() " << this << endl;
    ltracein(110);

    if (isInvalidatedCache()) updateCache();
    if (isInvalidatedCache()) {
      cerr << Error( "%s::updateGeometry() %s: Unable to restore cache."
                   , _getTypeName().c_str(), getString(this).c_str() ) << endl;
      ltraceout(110);
      return;
    }

    base()->invalidate( false );
    unsetFlags( CntInvalidated );

    if (not hasBadTopology()) {
      setX( getVertical1  ()->getX() );
      setY( getHorizontal1()->getY() );
    }

    ltraceout(110);
    DebugSession::close();
  }


  void  AutoContactHTee::updateTopology ()
  {
    DebugSession::open( getNet(), 80 );

    ltrace(110) << _getTypeName() << "::updateTopology() " << this << endl;
    ltracein(110);

    if (isInvalidatedCache()) updateCache();
    if (isInvalidatedCache()) {
      cerr << Error( "%s::updateGeometry() %s: Unable to restore cache."
                   , _getTypeName().c_str(), getString(this).c_str() ) << endl;
      ltraceout(110);
      return;
    }

    if (not hasBadTopology()) {
      RoutingGauge* rg       = Session::getRoutingGauge();
      size_t        depthH1  = rg->getLayerDepth( getHorizontal1()->getLayer() );
      size_t        depthH2  = rg->getLayerDepth( getHorizontal2()->getLayer() );
      size_t        depthV1  = rg->getLayerDepth( getVertical1  ()->getLayer() );
      size_t        minDepth = std::min( depthV1, std::min(depthH1,depthH2) );
      size_t        maxDepth = std::max( depthV1, std::max(depthH1,depthH2) );
      size_t        delta    = maxDepth - minDepth;

      ltrace(110) << "delta:" << delta << endl;

      if (maxDepth - minDepth > 3) {
        showTopologyError( "Sheared HTee, layer delta exceed 3." );
        setFlags( CntBadTopology );
      } else {
        if (depthH1 == depthH2) {
        // Dogleg on the vertical.
          switch ( delta ) {
            case 0: setLayer( rg->getRoutingLayer(minDepth) ); break;
            case 1: setLayer( rg->getContactLayer(minDepth) ); break;
            default:
              setLayer( rg->getContactLayer( depthH1 + ((depthH1==minDepth)?0:-1) ) );
              _vertical1 = static_cast<AutoVertical*>( _vertical1->makeDogleg(this) );
              break;
          }
        } else {
        // Dogleg on the horizontal with the greater gap (should be equal to +/-2).
          int deltaH1 = (int)depthH1 - (int)depthV1;
          int deltaH2 = (int)depthH2 - (int)depthV1;

          if (std::abs(deltaH1) > std::abs(deltaH2)) {
            setLayer( rg->getContactLayer( depthH2 + ((depthH2<depthV1)?0:-1) )  );
          //_horizontal1 = static_cast<AutoHorizontal*>( _horizontal1->makeDogleg(this) );
            _horizontal1->makeDogleg(this);
            ltrace(110) << "New h1:" << _horizontal1 << endl;
          } else {
            setLayer( rg->getContactLayer( depthH1 + ((depthH1<depthV1)?0:-1) )  );
          //_horizontal2 = static_cast<AutoHorizontal*>( _horizontal2->makeDogleg(this) );
            _horizontal2->makeDogleg(this);
            ltrace(110) << "New h2:" << _horizontal2 << endl;
          }
        }
      }
    }
    _horizontal1->invalidate( this );
    _horizontal2->invalidate( this );
    _vertical1  ->invalidate( this );

    ltraceout(110);
    DebugSession::close();
  }


  string  AutoContactHTee::_getTypeName () const
  { return "ContactHTee"; }


} // Katabatic namespace.
