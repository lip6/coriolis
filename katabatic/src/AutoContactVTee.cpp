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
// |  C++ Module  :       "./AutoContactVTee.cpp"                    |
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
#include  "hurricane/UpdateSession.h"
#include  "hurricane/DebugSession.h"
#include  "crlcore/RoutingGauge.h"
#include  "katabatic/AutoContactVTee.h"
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
// Class  :  "Katabatic::AutoContactVTee".


  AutoContactVTee* AutoContactVTee::create ( GCell* gcell, Net* net, const Layer* layer )
  {
    DbU::Unit viaSide = Session::getViaWidth( layer );
    Contact*  contact = Contact::create( net
                                       , layer
                                       , gcell->getCenter().getX()
                                       , gcell->getCenter().getY()
                                       , viaSide
                                       , viaSide
                                       );
    AutoContactVTee* autoContact = new AutoContactVTee( gcell, contact );

    autoContact->_postCreate();
    autoContact->unsetFlags( CntInCreationStage );

    ltrace(90) << "create(net*) " << autoContact << endl;
    return autoContact;
  }


  AutoContactVTee::AutoContactVTee ( GCell* gcell, Contact* contact )
    : AutoContact(gcell,contact)
    , _horizontal1(NULL)
    , _vertical1  (NULL)
    , _vertical2  (NULL)
  {
    setFlags( CntVTee );
  }


  AutoContactVTee::~AutoContactVTee ()
  { }


  AutoSegment* AutoContactVTee::getOpposite ( const AutoSegment* from ) const
  {
    if (from == _vertical1) return _vertical2;
    if (from == _vertical2) return _vertical1;
    return NULL;
  }


  AutoSegment* AutoContactVTee::getSegment ( unsigned int index ) const
  {
    switch ( index ) {
      case 0: return _horizontal1; 
      case 2: return _vertical1;
      case 3: return _vertical2;
    }
    return NULL;
  }


  void  AutoContactVTee::_invalidate ( unsigned int )
  {
    unsigned int flags = KbPropagate;
    if (_vertical1 and _vertical2) {
      if (_vertical1->isInvalidated() xor _vertical2->isInvalidated()) 
        flags = KbNoFlags;
    }

    if (_vertical1  ) _vertical1  ->invalidate( flags );
    if (_vertical2  ) _vertical2  ->invalidate( flags );
    if (_horizontal1) _horizontal1->invalidate();
  }


  void  AutoContactVTee::cacheDetach ( AutoSegment* segment )
  {
    if      (segment == _horizontal1) _horizontal1 = NULL;
    else if (segment == _vertical1)   _vertical1   = NULL;
    else if (segment == _vertical2)   _vertical2   = NULL;
    else return;

    setFlags( CntInvalidatedCache );
  }


  void  AutoContactVTee::cacheAttach ( AutoSegment* segment )
  {
    if (segment->getDirection() == KbVertical) {
      if      (not _vertical1) _vertical1 = static_cast<AutoVertical*>(segment);
      else if (not _vertical2) _vertical2 = static_cast<AutoVertical*>(segment);
      else {
        cerr << Bug( "%s::cacheAttach() On %s,\n"
                     "      v1 & v2 cache have not been cleared first, cancelling."
                   , _getTypeName().c_str(), getString(this).c_str()
                   ) << endl;
        return;
      }
    } else if (segment->getDirection() == KbHorizontal) {
      if (_horizontal1) {
        cerr << Bug( "%s::cacheAttach() On %s,\n"
                     "      h1 cache has not been cleared first, cancelling."
                   , _getTypeName().c_str(), getString(this).c_str()
                   ) << endl;
        return;
      }
      _horizontal1 = static_cast<AutoHorizontal*>(segment);
    }

    if (_vertical1 and _vertical2 and _horizontal1)
      unsetFlags( CntInvalidatedCache  );
  }


  void  AutoContactVTee::updateCache ()
  {
    DebugSession::open( getNet(), 80 );

    ltrace(110) << "AutoContactVTee::updateCache() " << this << endl;
    ltracein(110);

    Component*   anchor;
    Horizontal** horizontals = new Horizontal* [3];
    Vertical**   verticals   = new Vertical*   [3];

    _getTopology ( anchor, horizontals, verticals, 3 );

    _horizontal1 = static_cast<AutoHorizontal*>( Session::lookup(horizontals[0]) );
    _vertical1   = static_cast<AutoVertical  *>( Session::lookup(verticals  [0]) );
    _vertical2   = static_cast<AutoVertical  *>( Session::lookup(verticals  [1]) );

    string message;
    if      (verticals  [0] == NULL) message = "VTee has less than two vertical segments.";
    else if (verticals  [1] == NULL) message = "VTee has less than two vertical segments.";
    else if (verticals  [2] != NULL) message = "VTee has more than two vertical segments.";
    else if (horizontals[0] == NULL) message = "VTee is missing mandatory horizontal segment.";
    else if (horizontals[1] != NULL) message = "VTee has more than one horizontal segment.";
    else if (    (not _vertical1->isCreated() and not _vertical2->isCreated())
             and (_vertical1->getY() != _vertical2->getY()) )
      message = "VTee has misaligned vertical segments";
    if (not message.empty() ) {
      showTopologyError( message );
      setFlags( CntBadTopology );
    }
    unsetFlags( CntInvalidatedCache );

    ltrace(110) << "h1:" << _horizontal1 << endl;
    ltrace(110) << "v1:" << _vertical1 << endl;
    ltrace(110) << "v2:" << _vertical2 << endl;

    delete [] horizontals;
    delete [] verticals;

    ltraceout(110);
    DebugSession::close();
  }


  void  AutoContactVTee::updateGeometry ()
  {
    DebugSession::open( getNet(), 80 );

    ltrace(110) << "AutoContactVTee::updateGeometry() " << this << endl;
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


  void  AutoContactVTee::updateTopology ()
  {
    DebugSession::open ( getNet(), 80 );

    ltrace(110) << "AutoContactVTee::updateTopology() " << this << endl;
    ltracein(110);

    if (isInvalidatedCache()) updateCache();
    if (isInvalidatedCache()) {
      cerr << Error( "%s::updateGeometry() %s: Unable to restore cache."
                   , _getTypeName().c_str(), getString(this).c_str() ) << endl;
      ltraceout(110);
      return;
    }

    if (not hasBadTopology()) {
      RoutingGauge* rg           = Session::getRoutingGauge();
      size_t        depthV1      = rg->getLayerDepth( getVertical1  ()->getLayer() );
      size_t        depthV2      = rg->getLayerDepth( getVertical2  ()->getLayer() );
      size_t        depthH1      = rg->getLayerDepth( getHorizontal1()->getLayer() );
      size_t        minDepth     = std::min( depthH1, std::min(depthV1,depthV2) );
      size_t        maxDepth     = std::max( depthH1, std::max(depthV1,depthV2) );
      size_t        delta        = maxDepth - minDepth;

      ltrace(110) << "minDepth:" << minDepth << endl;
      ltrace(110) << "maxDepth:" << maxDepth << endl;
      ltrace(110) << "delta:"    << delta << endl;

      if ( maxDepth - minDepth > 3 ) {
        showTopologyError( "Sheared VTee, layer delta exceed 3." );
        setFlags( CntBadTopology );
      } else {
        if (depthV1 == depthV2) {
          ltrace(110) << "depthV1 == depthV2 (" << depthV1 << ")" << endl;
        // Dogleg on the horizontal.
          switch ( delta ) {
            case 0: setLayer( rg->getRoutingLayer(minDepth) ); break;
            case 1: setLayer( rg->getContactLayer(minDepth) ); break;
            default:
              ltrace(110) << "Restore connectivity: dogleg on h1." << endl;
              setLayer( rg->getContactLayer( depthV1 + ((depthV1==minDepth)?0:-1) ) );
              _horizontal1 = static_cast<AutoHorizontal*>( _horizontal1->makeDogleg(this) );
              break;
          }
        } else {
        // Dogleg on the vertical with the greater gap (should be equal to +/-2).
          int deltaV1 = (int)depthV1 - (int)depthH1;
          int deltaV2 = (int)depthV2 - (int)depthH1;

          if (std::abs(deltaV1) > std::abs(deltaV2)) {
            setLayer( rg->getContactLayer( depthV2 + ((depthV2<depthH1)?0:-1) )  );
          //_vertical1 = static_cast<AutoVertical*>( _vertical1->makeDogleg(this) );
            _vertical1->makeDogleg(this);
          } else {
            setLayer( rg->getContactLayer( depthV1 + ((depthV1<depthH1)?0:-1) )  );
          //_vertical2 = static_cast<AutoVertical*>( _vertical2->makeDogleg(this) );
            _vertical2->makeDogleg(this);
          }
        }
      }
    }

    ltraceout(110);
    DebugSession::close ();
  }


  string  AutoContactVTee::_getTypeName () const
  { return "ContactVTee"; }


} // Katabatic namespace.
