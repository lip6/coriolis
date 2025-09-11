// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2025-2025.
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./TrackMarkerSpacing.cpp"                 |
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
#include "katana/TrackMarkerSpacing.h"
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
  using Hurricane::ParallelSpacings;
  using CRL::RoutingGauge;


  TrackMarkerSpacing* TrackMarkerSpacing::create ( Segment* segment )
  {
    TrackMarkerSpacing* marker = new TrackMarkerSpacing ( segment );
    return marker;
  }


  TrackMarkerSpacing::TrackMarkerSpacing ( Segment* segment )
    : TrackMarkerBase()
    , _wideSegment   (segment)
  {
    DebugSession::open( segment->getNet(), 159, 160 );
    cdebug_log(159,1) << "TrackMarkerSpacing::TrackMarkerSpacing() " << segment << endl;

    const Layer*     layer          = segment->getLayer();
    RoutingGauge*    rg             = Session::getRoutingGauge();
    RoutingPlane*    plane          = Session::getKatanaEngine()->getRoutingPlaneByLayer( layer );
    Flags            planeDirection = rg->getLayerDirection( plane->getDepth() );
    bool             isHorizontal   = (planeDirection == Constant::Horizontal);
    Interval         trackSpan;
    Box              bb             = segment->getBoundingBox();
    ParallelSpacings spacings       = layer->getParallelSpacings( bb, isHorizontal );
    cdebug_log(159,0) << "bb=" << bb << endl;
    if (isHorizontal) {
      setSourceU( bb.getXMin() );
      setTargetU( bb.getXMax() );
      trackSpan = Interval( bb.getYMin(), bb.getYMax() );
    } else {
      setSourceU( bb.getYMin() );
      setTargetU( bb.getYMax() );
      trackSpan = Interval( bb.getXMin(), bb.getXMax() );
    }
    cdebug_log(159,0) << "trackSpan=" << trackSpan << endl;
    
    DbU::Unit wireWidth    = rg->getWireWidth(layer) / 2;
    DbU::Unit spacingBound = trackSpan.getVMin() - spacings.maxSpacing() - wireWidth;
    Track*    track        = plane->getTrackByPosition( trackSpan.getVMin(), Constant::Inferior );
    cdebug_log(159,0) << "Inferior: " << track << endl;
    while ( track and (track->getAxis() > spacingBound)) {
      cdebug_log(159,0) << "| spacing=" << DbU::getValueString(spacings.maxSpacing())
                        <<  " " << track << endl;
      Session::addInsertEvent ( this, track );
      incRefcount();
      track = track->getPreviousTrack();
    }
    
    spacingBound = trackSpan.getVMax() + spacings.maxSpacing() + wireWidth;
    track        = plane->getTrackByPosition( trackSpan.getVMax(), Constant::Superior );
    cdebug_log(159,0) << "Superior: " << track << endl;
    while ( track and (track->getAxis() < spacingBound)) {
      cdebug_log(159,0) << "| spacing=" << DbU::getValueString(spacings.maxSpacing())
                        <<  " " << track << endl;
      Session::addInsertEvent ( this, track );
      incRefcount();
      track = track->getNextTrack();
    }

    cdebug_tabw(159,-1);
    DebugSession::close();
  }


  Net* TrackMarkerSpacing::getNet () const
  { return _wideSegment->getNet(); }


  DbU::Unit  TrackMarkerSpacing::_getDistance ( DbU::Unit trackAxis, ParallelSpacings& spacings ) const
  {
    const Layer*     layer          = _wideSegment->getLayer();
    RoutingGauge*    rg             = Session::getRoutingGauge();
    RoutingPlane*    plane          = Session::getKatanaEngine()->getRoutingPlaneByLayer( layer );
    Flags            planeDirection = rg->getLayerDirection( plane->getDepth() );
    bool             isHorizontal   = (planeDirection == Constant::Horizontal);
    Box              bb             = _wideSegment->getBoundingBox();
    spacings = _wideSegment->getLayer()->getParallelSpacings( bb, isHorizontal );

    DbU::Unit wireWidth = rg->getWireWidth(layer) / 2;
    DbU::Unit distance  = 0;
    if (isHorizontal) {
      if (trackAxis > bb.getYMax()) distance = trackAxis - bb.getYMax() - wireWidth;
      else                          distance = bb.getYMin() - trackAxis - wireWidth;
    } else {
      if (trackAxis > bb.getXMax()) distance = trackAxis - bb.getXMax() - wireWidth;
      else                          distance = bb.getXMin() - trackAxis - wireWidth;
    }

    cdebug_log(159,0) << "distance="  << DbU::getValueString(distance ) << endl;
    return distance;
  }


  bool  TrackMarkerSpacing::isValidSpacing ( const Interval& span, DbU::Unit trackAxis ) const
  { 
    cdebug_log(159,1) << "TrackMarkerSpacing::isValidSpacing() " << _wideSegment << endl;
    cdebug_log(159,0) << "span=" << span << " on axis=" << DbU::getValueString(trackAxis) << endl;

    ParallelSpacings spacings;
    DbU::Unit distance  = _getDistance( trackAxis, spacings );
    DbU::Unit runLength = Interval( getSourceU(), getTargetU() ).getIntersection( span ).getSize();
    cdebug_log(159,0) << "runLength=" << DbU::getValueString(runLength) << endl;
    for ( size_t i=0 ; i<spacings.size() ; ++i ) {
      if ((runLength < spacings.parallelLength(i)) and (distance >= spacings.spacing(i))) {
        cdebug_log(159,0) << "Minimum spacing OK" << endl;
        cdebug_tabw(159,-1);
        return true;
      }
    }

    cdebug_log(159,0) << "Minimum spacing failed" << endl;
    cdebug_tabw(159,-1);
    return false;
  }

  
  DbU::Unit  TrackMarkerSpacing::getMaxParallelLength ( DbU::Unit trackAxis ) const
  {
    ParallelSpacings spacings;
    DbU::Unit distance  = _getDistance( trackAxis, spacings );
    for ( size_t i=0 ; i<spacings.size() ; ++i ) {
      if (distance >= spacings.spacing(i)) {
        return spacings.parallelLength(i);
      }
    }
    return -1;
  }


  string  TrackMarkerSpacing::_getTypeName () const
  { return "TrackMarkerSpacing"; }


  string  TrackMarkerSpacing::_getString () const
  {
    return Super::_getString();
  }


  Record* TrackMarkerSpacing::_getRecord () const
  {
    Record* record = Super::_getRecord();
    record->add( getSlot( "_wideSegment", _wideSegment ));
    return record;
  }


} // Katana namespace.
