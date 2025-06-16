// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
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
#include "hurricane/DebugSession.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Net.h"
#include "hurricane/Name.h"
#include "crlcore/RoutingGauge.h"
#include "anabatic/GCell.h"
#include "katana/TrackMarker.h"
#include "katana/Track.h"
#include "katana/RoutingPlane.h"
#include "katana/Session.h"
#include "katana/RoutingEvent.h"
#include "katana/KatanaEngine.h"


namespace Katana {

  using std::cerr;
  using std::endl;
  using std::ostringstream;
  using std::setprecision;
  using Hurricane::Bug;
  using Hurricane::DebugSession;
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


  TrackMarker::TrackMarker ( RoutingPad* rp, size_t depth )
    : _routingPad    (rp)
    , _sourcePosition(0)
    , _targetPosition(0)
    , _track         (NULL)
    , _weight        (0)
    , _refcount      (0)
  {
    DebugSession::open( rp->getNet(), 159, 160 );

    RoutingGauge* rg             = Session::getRoutingGauge();
    RoutingPlane* plane          = Session::getKatanaEngine()->getRoutingPlaneByIndex( depth );
    DbU::Unit     pitch          = Session::getPitch( depth );
    Flags         planeDirection = rg->getLayerDirection( depth );
    Interval      trackSpan;

    if (rg->isSymbolic()) {
      Point sourcePoint = rp->getSourcePosition();
      Point targetPoint = rp->getTargetPosition();
      if ( planeDirection == Constant::Horizontal ) {
        _sourcePosition = sourcePoint.getX();
        _targetPosition = targetPoint.getX();
        trackSpan       = Interval ( sourcePoint.getY(), targetPoint.getY() );
      } else {
        _sourcePosition = sourcePoint.getY();
        _targetPosition = targetPoint.getY();
        trackSpan       = Interval ( sourcePoint.getX(), targetPoint.getX() );
      }
    } else {
      Box bb = rp->getBoundingBox();
      cdebug_log(159,0) << "bb=" << bb << endl;
      RoutingLayerGauge* rlg1         = Session::getLayerGauge( (size_t)0 );
      const Layer*       viaLayer1    = Session::getBuildContactLayer( 0 );
      DbU::Unit          viaEnclosure = rlg1->getHalfViaWidth()
                                      + viaLayer1->getBottomEnclosure( Layer::EnclosureV );
      if (planeDirection == Constant::Horizontal) {
        _sourcePosition = bb.getXMin();
        _targetPosition = bb.getXMax();
        trackSpan = Interval( bb.getYMin(), bb.getYMax() );
        trackSpan.inflate( 0, -viaEnclosure );
      } else {
        _sourcePosition = bb.getYMin();
        _targetPosition = bb.getYMax();
        trackSpan = Interval( bb.getXMin(), bb.getXMax() );
        trackSpan.inflate( -viaEnclosure, 0 );
      }
    }
    
    if (planeDirection xor (uint64_t)rg->getLayerDirection(rg->getLayerDepth(rp->getLayer()))) {
      if (not rg->isSymbolic())
        _weight = (uint32_t)( 100.0 / (1.0 + (trackSpan.getSize()/pitch)) );
    } else {
      _weight = (uint32_t)( (1.0 + trackSpan.getSize()/pitch) * 20.0 );
    }

    cdebug_log(159,1) << "TrackMarker::TrackMarker() depth=" << depth << " " << rp << endl;
    cdebug_log(159,0) << "trackSpan=" << trackSpan << endl;

    Track* track = plane->getTrackByPosition ( trackSpan.getVMin() );
    cdebug_log(159,0) << "Nearest: " << track << endl;
    if (trackSpan.contains(track->getAxis())
       or (track->getNextTrack() and trackSpan.contains(track->getNextTrack()->getAxis()))) {
      if (track->getAxis() < trackSpan.getVMin())
        track = track->getNextTrack();
      while ( track && (track->getAxis() <= trackSpan.getVMax()) ) {
        cdebug_log(159,0) << "| weight=" << _weight << " " << track << endl;
        Session::addInsertEvent ( this, track );
        track = track->getNextTrack();
        _refcount++;
      }
    } else {
      cdebug_log(159,0) << "| offgrid weight=" << _weight << " " << track << endl;
      Session::addInsertEvent ( this, track );
      track = track->getNextTrack();
      _refcount++;
    }

    cdebug_tabw(159,-1);
    DebugSession::close();
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
      << "] "   << setprecision(3) << ((double)_weight)/100.0
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


} // Katana namespace.
