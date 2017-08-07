// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./TrackSegmentWide.cpp"                   |
// +-----------------------------------------------------------------+


#include <sstream>
#include <limits>
#include "hurricane/Bug.h"
#include "hurricane/Warning.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Net.h"
#include "hurricane/Name.h"
#include "hurricane/RoutingPad.h"
#include "anabatic/AutoContact.h"
#include "anabatic/GCell.h"
#include "crlcore/RoutingGauge.h"
#include "katana/DataNegociate.h"
#include "katana/TrackSegmentWide.h"
#include "katana/Track.h"
#include "katana/Session.h"
#include "katana/RoutingEvent.h"
#include "katana/NegociateWindow.h"
#include "katana/KatanaEngine.h"


namespace Katana {

  using namespace std;
  using Hurricane::tab;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::BasicLayer;
  using Hurricane::Net;
  using Hurricane::Name;
  using Hurricane::RoutingPad;
  using Anabatic::AutoSegment;
  using Anabatic::perpandicularTo;


// -------------------------------------------------------------------
// Class  :  "TrackSegmentWide".


  TrackSegmentWide::TrackSegmentWide ( AutoSegment* segment, Track* track, size_t trackSpan )
    : Super(segment,track)
    , _trackSpan (trackSpan)
    , _trackCount(0)
  {
    cdebug_log(155,0) << "CTOR TrackSegmentWide " << (void*)this    << ":" << this    << endl;
    cdebug_log(155,0) << "                 over " << (void*)segment << ":" << segment << endl;

    if (not _trackSpan) {
      DbU::Unit mWidth = std::max( Session::getWireWidth(getLayer()), Session::getViaWidth(getLayer()) );
      if (segment->getWidth() < mWidth) {
        _trackSpan = 1;
      } else {
        DbU::Unit pitch = Session::getPitch(segment->getLayer());
        DbU::Unit width = segment->getWidth() - mWidth;

        _trackSpan = (size_t)(width/pitch) + 1 + ((width%pitch) ? 1 : 0);
      }
    }
  }


  void  TrackSegmentWide::_postCreate ()
  {
    Super::_postCreate();
  }


  TrackSegmentWide::~TrackSegmentWide ()
  {
  }


  void  TrackSegmentWide::_preDestroy ()
  {
    Super::_preDestroy();
  }


  size_t    TrackSegmentWide::getTrackSpan  () const { return _trackSpan; }
  uint32_t  TrackSegmentWide::getTrackCount () const { return _trackCount; }


  void  TrackSegmentWide::addTrackCount ( int32_t count )
  {
    if (count > 0) _trackCount += count;
    else {
      if (-count > (int32_t)_trackCount) _trackCount = 0;
      _trackCount -= -count;
    }
  }


  void  TrackSegmentWide::addOverlapCost ( TrackCost& cost ) const
  {
    uint32_t  depth        = Session::getRoutingGauge()->getLayerDepth(getLayer());
    bool      inLocalDepth = (depth < 3);
    Track*    track        = cost.getTrack();

    if (not track) return;

    cost.setFlags( (isLocal() and (depth >= 3)) ? TrackCost::LocalAndTopDepth : 0 );
    cost.setFlags( (isAnalog()) ? TrackCost::Analog : 0 );
    cost.setDistanceToFixed();
    cost.incAxisWeight  ( getDataNegociate()->getRoutingEvent()->getAxisWeight( track->getAxis() ) );
    cost.incDeltaPerpand( getDataNegociate()->getWiringDelta( track->getAxis() ) );

    cdebug_log(155,0) << "incAxisWeight:" << DbU::getValueString(track->getAxis())
                      << " of " << DbU::getValueString(getDataNegociate()->getRoutingEvent()->getAxisWeight( track->getAxis() ))
                      << " (sum:" << DbU::getValueString(cost.getAxisWeight()) << ")" 
                      << endl;

    for ( size_t span=0 ; (span < _trackSpan) and (track != NULL) ; ++span ) {
      track->addOverlapCost( cost );
    // Todo: have to choose here wether we go *next* or *previous* according
    //       to the symmetry kind.
      track = track->getNextTrack();
      cost.selectNextTrack();
    }

    if (isGlobal()) cost.setForGlobal();

    if ( inLocalDepth and (cost.getDataState() == DataNegociate::MaximumSlack) )
      cost.setInfinite();

    cost.select( 0, TrackCost::NoFlags );
  }


}  // Katana namespace.
