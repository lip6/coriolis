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
    TrackMarker* marker = new TrackMarker ( rp, depth );
    return marker;
  }


  TrackMarker::TrackMarker ( RoutingPad* rp, size_t depth )
    : TrackMarkerBase()
    , _routingPad    (rp)
    , _weight        (0)
  {
    DebugSession::open( rp->getNet(), 159, 160 );
    cdebug_log(159,1) << "TrackMarker::TrackMarker() depth=" << depth << " " << rp << endl;

    RoutingGauge* rg             = Session::getRoutingGauge();
    RoutingPlane* plane          = Session::getKatanaEngine()->getRoutingPlaneByIndex( depth );
    DbU::Unit     pitch          = Session::getPitch( depth );
    Flags         planeDirection = rg->getLayerDirection( depth );
    Interval      trackSpan;

    if (rg->isSymbolic()) {
      Point sourcePoint = rp->getSourcePosition();
      Point targetPoint = rp->getTargetPosition();
      if ( planeDirection == Constant::Horizontal ) {
        setSourceU( sourcePoint.getX() );
        setTargetU( targetPoint.getX() );
        trackSpan = Interval( sourcePoint.getY(), targetPoint.getY() );
      } else {
        setSourceU( sourcePoint.getY() );
        setTargetU( targetPoint.getY() );
        trackSpan = Interval( sourcePoint.getX(), targetPoint.getX() );
      }
    } else {
      Box bb = rp->getBoundingBox();
      cdebug_log(159,0) << "bb=" << bb << endl;
      RoutingLayerGauge* rlg1         = Session::getLayerGauge( (size_t)0 );
      const Layer*       viaLayer1    = Session::getBuildContactLayer( 0 );
      DbU::Unit          viaEnclosure = rlg1->getHalfViaWidth()
                                      + viaLayer1->getBottomEnclosure( Layer::EnclosureV );
      if (planeDirection == Constant::Horizontal) {
        setSourceU( bb.getXMin() );
        setTargetU( bb.getXMax() );
        trackSpan = Interval( bb.getYMin(), bb.getYMax() );
      } else {
        setSourceU( bb.getYMin() );
        setTargetU( bb.getYMax() );
        trackSpan = Interval( bb.getXMin(), bb.getXMax() );
      }
      trackSpan.inflate( -viaEnclosure );
    }
    cdebug_log(159,0) << "trackSpan=" << trackSpan << endl;
    
    if (planeDirection xor (uint64_t)rg->getLayerDirection(rg->getLayerDepth(rp->getLayer()))) {
      if (not rg->isSymbolic())
        _weight = (uint32_t)( 100.0 / (1.0 + (trackSpan.getSize()/pitch)) );
    } else {
      _weight = (uint32_t)( (1.0 + trackSpan.getSize()/pitch) * 20.0 );
    }

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
        incRefcount();
      }
    } else {
      cdebug_log(159,0) << "| offgrid weight=" << _weight << " " << track << endl;
      Session::addInsertEvent ( this, track );
      track = track->getNextTrack();
      incRefcount();
    }

    cdebug_tabw(159,-1);
    DebugSession::close();
  }


  Net* TrackMarker::getNet () const
  { return _routingPad->getNet(); }


  uint32_t  TrackMarker::getWeight ( const Track* track ) const
  { return _weight; }


  string  TrackMarker::_getTypeName () const
  { return "TrackMarker"; }


  string  TrackMarker::_getString () const
  {
    ostringstream os;
    os << " " << setprecision(3) << ((double)_weight)/100.0;
    string s = Super::_getString();
    s.insert( s.size()-1, os.str() );
    return s;
  }


  Record* TrackMarker::_getRecord () const
  {
    Record* record = Super::_getRecord();
    record->add ( getSlot ( "_routingPad"    ,  _routingPad     ) );
    record->add ( getSlot ( "_weight"        ,  _weight         ) );
    return record;
  }


} // Katana namespace.
