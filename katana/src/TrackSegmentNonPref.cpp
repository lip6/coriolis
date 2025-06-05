// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Universite 2019-2019, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./TrackSegmentNonPref.cpp"                |
// +-----------------------------------------------------------------+


#include <sstream>
#include <limits>
#include "hurricane/DebugSession.h"
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
#include "katana/TrackSegmentNonPref.h"
#include "katana/Track.h"
#include "katana/RoutingPlane.h"
#include "katana/Session.h"
#include "katana/RoutingEvent.h"
#include "katana/NegociateWindow.h"
#include "katana/KatanaEngine.h"


namespace Katana {

  using namespace std;
  using Hurricane::tab;
  using Hurricane::DebugSession;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::BasicLayer;
  using Hurricane::Net;
  using Hurricane::Name;
  using Hurricane::RoutingPad;
  using Anabatic::AutoSegment;
  using Anabatic::perpandicularTo;


// -------------------------------------------------------------------
// Class  :  "TrackSegmentNonPref".


  TrackSegmentNonPref::TrackSegmentNonPref ( AutoSegment* segment )
    : Super(segment,NULL)
    , _trackSpan (0)
    , _trackCount(0)
  {
    cdebug_log(159,1) << "CTOR TrackSegmentNonPref " << /*(void*)this    << ":" <<*/ this    << endl;
    cdebug_log(159,0) <<   "                  over " << /*(void*)segment << ":" <<*/ segment << endl;

    updateTrackSpan();

    cdebug_tabw(159,-1);
  }


  void  TrackSegmentNonPref::_postCreate ()
  {
    Super::_postCreate();
  }


  TrackSegmentNonPref::~TrackSegmentNonPref ()
  {
  }


  void  TrackSegmentNonPref::_preDestroy ()
  {
    cdebug_log(160,1) << "TrackSegmentNonPref::_preDestroy() " << this << endl;
    Super::_preDestroy();
    cdebug_tabw(160,-1);
  }


  bool      TrackSegmentNonPref::isNonPref     () const { return true; }
  size_t    TrackSegmentNonPref::getTrackSpan  () const { return _trackSpan; }
  uint32_t  TrackSegmentNonPref::getTrackCount () const { return _trackCount; }


  bool  TrackSegmentNonPref::canPromoteToPref ( Flags flags ) const
  {
    cdebug_log(159,0) << "canPromoteToPref() " << this << endl;
    cdebug_log(159,0) << "_segment->getDirection() " << getDirection() << endl;
    cdebug_log(159,0) << "_segment->isForOffgrid() " << isForOffgrid() << endl;
    cdebug_log(159,0) << "Session::getDirection() " << Session::getDirection(getLayer()) << endl;
    if (Session::getStage() != Anabatic::StageNegociate) return false;
    if (isForOffgrid()) return false;
    if (getDirection() == Session::getDirection(getLayer())) return false;
    if (flags & Flags::IgnoreRipupState) return true;
    if (getDataNegociate() and (getDataNegociate()->getState() < DataNegociate::MaximumSlack))
      return false;
    return true;
  }


  void  TrackSegmentNonPref::updateTrackSpan ()
  {
    DebugSession::open( getNet(), 150, 160 );
    cdebug_log(159,1) << "TrackSegmentNonPref::updateTrackSpan() " << /*(void*)this    << ":" <<*/ this << endl;

    RoutingPlane* plane       = Session::getKatanaEngine()->getRoutingPlaneByLayer(_base->getLayer());
    Interval      newAxisSpan = _base->getNonPrefSpan();
  //newAxisSpan.inflate( base()->getExtensionCap( Anabatic::Flags::NoFlags ));
    Track*        ntrack      = plane->getTrackByPosition( newAxisSpan.getVMin(), Constant::Superior );

    cdebug_log(159,0) << "getSourceU():        " << DbU::getValueString( _base->getSourceU() ) << endl;
    cdebug_log(159,0) << "getTargetU():        " << DbU::getValueString( _base->getTargetU() ) << endl;
    cdebug_log(159,0) << "getSourcePosition(): " << DbU::getValueString( _base->getSourcePosition() ) << endl;
    cdebug_log(159,0) << "getTargetPosition(): " << DbU::getValueString( _base->getTargetPosition() ) << endl;
    cdebug_log(159,0) << "new Axis span: " << newAxisSpan << endl;
    
    if (ntrack) {
      cdebug_log(159,0) << "+ " << ntrack << endl;
      _trackSpan = 0;
      for ( ; ntrack and newAxisSpan.contains(ntrack->getAxis())
            ; ntrack = ntrack->getNextTrack(), _trackSpan++ ) {
        cdebug_log(159,0) << "| " << ntrack << endl;
      }
    }
    if (not _trackSpan) _trackSpan = 1;
    cdebug_log(159,0) << "_trackSpan: " <<  _trackSpan << endl;

    cdebug_tabw(159,-1);
    DebugSession::close();
  }


  void  TrackSegmentNonPref::addTrackCount ( int32_t count )
  {
    if (count > 0) _trackCount += count;
    else {
      if (-count > (int32_t)_trackCount) _trackCount = 0;
      _trackCount -= -count;
    }
  }


  void  TrackSegmentNonPref::addOverlapCost ( TrackCost& cost ) const
  {
    uint32_t  depth        = Session::getRoutingGauge()->getLayerDepth(getLayer());
    bool      inLocalDepth = (depth < 3);
    Track*    track        = cost.getTrack();

    if (not track) return;
    DbU::Unit axisCandidate = cost.getRefCandidateAxis();

    cost.setFlags( (isLocal() and (depth >= 3)) ? TrackCost::LocalAndTopDepth : 0 );
    cost.setFlags( (isAnalog()) ? TrackCost::Analog : 0 );
    cost.setDistanceToFixed();
    cost.incAxisWeight  ( getDataNegociate()->getRoutingEvent()->getAxisWeight( axisCandidate ) );
    cost.incDeltaPerpand( getDataNegociate()->getWiringDelta( axisCandidate ) );

    cdebug_log(155,0) << "incAxisWeight:" << DbU::getValueString(axisCandidate)
                      << " of " << DbU::getValueString(getDataNegociate()->getRoutingEvent()->getAxisWeight( axisCandidate ))
                      << " (sum:" << DbU::getValueString(cost.getAxisWeight()) << ")" 
                      << endl;

    for ( size_t span=0 ; (span < _trackSpan) and (track != NULL) ; ++span ) {
      cdebug_log(155,0) << "Crossed: " << track << endl;
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


  void  TrackSegmentNonPref::invalidate ()
  {
    if (isInvalidated()) return;

    Super::invalidate();

    if (getTrack()) reschedule( 0 );
  }


}  // Katana namespace.
