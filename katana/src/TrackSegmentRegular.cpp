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
// |  C++ Module  :       "./TrackSegmentRegular.cpp"                |
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
#include "katana/TrackSegmentRegular.h"
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
// Class  :  "TrackSegmentRegular".


  TrackSegmentRegular::TrackSegmentRegular ( AutoSegment* segment, Track* track )
    : Super(segment,track)
  {
    // cdebug_log(155,0) << "CTOR TrackSegmentRegular " << (void*)this    << ":" << this    << endl;
    // cdebug_log(155,0) << "                    over " << (void*)segment << ":" << segment << endl;
  }


  void  TrackSegmentRegular::_postCreate ()
  {
    Super::_postCreate();
  }


  TrackSegmentRegular::~TrackSegmentRegular ()
  {
  }


  void  TrackSegmentRegular::_preDestroy ()
  {
    Super::_preDestroy();
  }


  size_t  TrackSegmentRegular::getTrackSpan () const { return 1; }


  void  TrackSegmentRegular::addOverlapCost ( TrackCost& cost ) const
  {
    uint32_t  depth           = Session::getRoutingGauge()->getLayerDepth(getLayer());
    bool      inLocalDepth    = (depth < 3);
    bool      isOneLocalTrack = (isLocal()) and (base()->getAutoSource()->getGCell()->getGlobalsCount(depth) >= 9.0);
    Track*    track           = cost.getTrack();

    if (not track) return;

    cost.setFlags( (isLocal() and (depth >= 3)) ? TrackCost::LocalAndTopDepth : 0 );
    cost.setFlags( (isAnalog()) ? TrackCost::Analog : 0 );

    track->addOverlapCost( cost );

    cost.setDistanceToFixed();
    cost.incAxisWeight     ( getDataNegociate()->getRoutingEvent()->getAxisWeight(track->getAxis()) );
    cost.incDeltaPerpand   ( getDataNegociate()->getWiringDelta(track->getAxis()) );
    if (isGlobal()) cost.setForGlobal();

    if ( inLocalDepth and (cost.getDataState() == DataNegociate::MaximumSlack) )
      cost.setInfinite();

    if ( isOneLocalTrack
       and  cost.isOverlapGlobal()
       and (cost.getDataState() >= DataNegociate::ConflictSolveByHistory) )
        cost.setInfinite();
  }


}  // Katana namespace.
