
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2013, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./TrackMarker.cpp"                        |
// +-----------------------------------------------------------------+


#include <iomanip>
#include <sstream>
#include "hurricane/Bug.h"
#include "hurricane/Warning.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Net.h"
#include "hurricane/Name.h"
#include "crlcore/RoutingGauge.h"
#include "katabatic/GCell.h"
#include "kite/TrackMarker.h"
#include "kite/Track.h"
#include "kite/RoutingPlane.h"
#include "kite/Session.h"
#include "kite/RoutingEvent.h"
#include "kite/KiteEngine.h"


namespace Kite {

  using std::cerr;
  using std::endl;
  using std::ostringstream;
  using std::setprecision;
  using Hurricane::Bug;
  using CRL::RoutingGauge;


  TrackMarker* TrackMarker::create ( RoutingPad* rp, size_t depth )
  {
    TrackMarker* segment = new TrackMarker ( rp, depth );
    return segment;
  }


  void  TrackMarker::destroy ()
  {
    if ( !--_refcount ) delete this;
  }


  TrackMarker::TrackMarker ( RoutingPad* pad, size_t depth )
    : _routingPad    (pad)
    , _sourcePosition(0)
    , _targetPosition(0)
    , _track         (NULL)
    , _weight        (0)
    , _refcount      (0)
  {
    Point         sourcePoint  = pad->getSourcePosition();
    Point         targetPoint  = pad->getTargetPosition();
    RoutingGauge* rg           = Session::getKiteEngine()->getRoutingGauge();
    RoutingPlane* rp           = Session::getKiteEngine()->getRoutingPlaneByIndex(depth);
    unsigned int  rpDirection  = rg->getLayerDirection(depth);
    Interval      trackSpan;

    if ( rpDirection == Constant::Horizontal ) {
      _sourcePosition = sourcePoint.getX();
      _targetPosition = targetPoint.getX();
      trackSpan       = Interval ( sourcePoint.getY(), targetPoint.getY() );
    } else {
      _sourcePosition = sourcePoint.getY();
      _targetPosition = targetPoint.getY();
      trackSpan       = Interval ( sourcePoint.getX(), targetPoint.getX() );
    }

    if ( rpDirection xor rg->getLayerDirection(rg->getLayerDepth(pad->getLayer())) ) {
      _weight = (unsigned int)(( 5.0 / (5.0+DbU::getLambda(trackSpan.getSize())) ) * 100.0) ;
    } else {
      _weight = (unsigned int)( (5.0 + DbU::getLambda(trackSpan.getSize())) * 20.0 );
    }

    Track* track = rp->getTrackByPosition ( trackSpan.getVMin() );
    while ( track && (track->getAxis() <= trackSpan.getVMax()) ) {
      Session::addInsertEvent ( this, track );
      track = track->getNextTrack();
      _refcount++;
    }
  }


  Net* TrackMarker::getNet () const
  { return _routingPad->getNet(); }


  string  TrackMarker::_getTypeName () const
  { return "TrackMarker"; }


  string  TrackMarker::_getString () const
  {
    ostringstream s;
    s << "<"   << _getTypeName()
      << " "   << getNet()->getName()
      << " ["  << DbU::getValueString(_sourcePosition)
      << ":"   << DbU::getValueString(_targetPosition)
      << " "   << setprecision(3) << ((double)_weight)/100.0
      << ">";
    return s.str();
  }


  Record* TrackMarker::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    record->add ( getSlot ( "_routingPad"    ,  _routingPad     ) );
    record->add ( getSlot ( "_sourcePosition",  _sourcePosition ) );
    record->add ( getSlot ( "_targetPosition",  _targetPosition ) );
    record->add ( getSlot ( "_track"         ,  _track          ) );
    record->add ( getSlot ( "_weight"        ,  _weight         ) );

    return record;
  }


} // Kite namespace.
