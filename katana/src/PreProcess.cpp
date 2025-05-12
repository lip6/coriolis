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
// |  C++ Module  :       "./PreProcess.cpp"                         |
// +-----------------------------------------------------------------+


#include <sstream>
#include "hurricane/DebugSession.h"
#include "hurricane/Bug.h"
#include "hurricane/Warning.h"
#include "hurricane/Net.h"
#include "hurricane/Name.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Horizontal.h"
#include "anabatic/AutoContactTerminal.h"
#include "katana/DataNegociate.h"
#include "katana/TrackFixedSpanRp.h"
#include "katana/Track.h"
#include "katana/RoutingPlane.h"
#include "katana/NegociateWindow.h"
#include "katana/Session.h"
#include "katana/KatanaEngine.h"


namespace {

  using namespace std;
  using namespace Hurricane;
  using namespace CRL;
  using namespace Katana;
  using Anabatic::perpandicularTo;
  using Anabatic::AutoSegment;
  using Anabatic::AutoContactTerminal;
  using Anabatic::AutoSegments_OnContact;
  using Anabatic::AutoSegments;


  class SortAcByXY {
    public:
      inline bool  operator() ( AutoContactTerminal* contact1, AutoContactTerminal* contact2 ) const;
  };


  inline bool  SortAcByXY::operator() ( AutoContactTerminal* contact1, AutoContactTerminal* contact2 ) const
  {
    DbU::Unit x1 = contact1->getX();
    DbU::Unit x2 = contact2->getX();

    if (x1 == x2) {
      DbU::Unit y1 = contact1->getY();
      DbU::Unit y2 = contact2->getY();

      if (y1 == y2) return false;
      return (y1 < y2);
    }
    return (x1 < x2);
  }


  void  getPerpandiculars ( TrackElement*           startSegment
                          , Anabatic::AutoContact*  from
                          , Flags                   direction
                          , int                     maxDepth
                          , vector<TrackElement*>&  perpandiculars
                          )
  {
    vector< pair<TrackElement*, int> > stack;
    stack.push_back( make_pair( startSegment, maxDepth ));
    
    while ( not stack.empty() ) {
      TrackElement* segment = stack.back().first;
      int           depth   = stack.back().second;
      stack.pop_back();
    
      TrackElement* perpandicular;
      for( Segment* osegment : segment->base()->getAutoSource()->getSlaveComponents().getSubSet<Segment*>() ) {
        perpandicular = Session::lookup ( osegment );
        cdebug_log(159,0) << "S " << perpandicular << endl;
      
        if (not perpandicular) continue;
        if (perpandicular->getDirection() == direction) {
          if (depth > 0)
            stack.push_back( make_pair( perpandicular, depth-1 ));
          continue;
        }
        perpandiculars.push_back( perpandicular );
      }

      for( Segment* osegment : segment->base()->getAutoTarget()->getSlaveComponents().getSubSet<Segment*>() ) {
        perpandicular = Session::lookup ( osegment );
        cdebug_log(159,0) << "T " << perpandicular << endl;
      
        if (not perpandicular) continue;
        if (perpandicular->getDirection() == direction) {
          if (depth > 0)
            stack.push_back( make_pair( perpandicular, depth-1 ));
          continue;
        }
        perpandiculars.push_back( perpandicular );
      }
    }
  }


  void  findFailedPerpandiculars ( RoutingPad* rp, Flags direction, set<TrackElement*>& faileds )
  {
    cdebug_log(159,0) << "Find failed caging: " << rp << endl;

    TrackElement* parallel;
    for( Segment* osegment : rp->getSlaveComponents().getSubSet<Segment*>() ) {
      parallel = Session::lookup ( osegment );
      cdebug_log(159,0) << "* " << parallel << endl;

      if ( parallel->isFixed () ) continue;
      if ( parallel->getDirection() != direction ) continue;

      Anabatic::AutoContact* contact = parallel->base()->getAutoSource();
      if ( contact->base()->getAnchor() != rp ) contact = NULL;

      if ( contact == NULL ) contact = parallel->base()->getAutoTarget();
      if ( contact->base()->getAnchor() != rp ) continue;

    //parallel->makeDogLeg ( contact->getGCell() );
      faileds.insert ( parallel );
    }
  }


  void  propagateCagedConstraints ( KatanaEngine*       katana
                                  , TrackElement*       segment
                                  , size_t              isegment
                                  , set<TrackElement*>& faileds )
  {
    DebugSession::open( segment->getNet(), 150, 160 );
    cdebug_log(159,0) << "propagateCagedConstraints(): " << segment << endl;

    if (not segment->isFixed() or segment->isFixedAxis()) {
      DebugSession::close();
      return;
    }

    Track*                 track         = segment->getTrack();
    Flags                  direction     = segment->getDirection();
    Anabatic::AutoContact* source        = nullptr;
    GCell*                 gcell         = nullptr;
    RoutingPad*            rp            = nullptr;
    vector<TrackElement*>  perpandiculars;

    if (segment->isFixedSpanRp()) {
      rp    = dynamic_cast<TrackFixedSpanRp*>( segment )->getRoutingPad();
      gcell = katana->getGCellUnder( rp->getCenter() );
    } else {
      if (not segment->base()) {
        DebugSession::close();
        return;
      }

      source = segment->base()->getAutoSource();
      rp     = dynamic_cast<RoutingPad*>(source->getAnchor());
      gcell  = source->getGCell();
      if (not rp) {
        cerr << Bug( "%s is not anchored on a <RoutingPad>\n       (%s)"
                   , getString(source).c_str()
                   , getString(source->getAnchor()).c_str() ) << endl;
        DebugSession::close();
        return;
      }
    }
    Interval  uside         = gcell->getSide( direction );
    DbU::Unit minConstraint = uside.getVMin();
    DbU::Unit maxConstraint = uside.getVMax();

    if ( not track ) {
      cerr << Bug( "%s is not inserted in a <Track>", getString(segment).c_str() ) << endl;
      DebugSession::close();
      return;
    }

  // Computing constraints from fixed only TrackElements (caging).
    TrackElement* parallel = nullptr;
    size_t i = isegment;
    while ( (i != Track::npos) and (i > 0) ) {
      parallel = track->getSegment( --i );
      if (not parallel) continue;
      if (parallel->getTargetU() < uside.getVMin()) break;
      if (parallel->getNet() == segment->getNet()) continue;
      if (not parallel->isFixed()) continue;

      cdebug_log(159,0) << "Min Constraint from: " << parallel << endl;
      minConstraint = max( minConstraint, parallel->getTargetU() );
    }

    i = isegment;
    while ( i < track->getSize()-1 ) {
      parallel = track->getSegment( ++i );
      if (not parallel) continue;
      if (parallel->getSourceU() > uside.getVMax()) break;
      if (parallel->getNet() == segment->getNet()) continue;
      if (not parallel->isFixed()) continue;

      cdebug_log(159,0) << "Max Constraint from: " << parallel << endl;
      maxConstraint = min( maxConstraint, parallel->getSourceU() );
    }

    if (minConstraint > maxConstraint) {
      cerr << Bug( "%s have too tight caging constraints.", getString(segment).c_str() ) << endl;
      cdebug_tabw(159,-1);
      DebugSession::close();
      return;
    }
    if ( (minConstraint <= uside.getVMin()) and (maxConstraint >= uside.getVMax()) ) {
      cdebug_log(159,0) << "No constraints [" << DbU::getValueString(minConstraint)
                  << ":"                << DbU::getValueString(maxConstraint)
                  << " vs. " << uside << endl;
      DebugSession::close();
      return;
    }

  // Finding perpandiculars, by way of the source & target RoutingPad.
    for( Segment* osegment : rp->getSlaveComponents().getSubSet<Segment*>() ) {
      parallel = Session::lookup( osegment );
      if (not parallel) continue;

      int depth = (parallel->isForOffgrid()) ? 1 : 0;
      cdebug_log(159,0) << "* " << parallel << " depth=" << depth << endl;
      if (parallel->isFixed ()) continue;
      if (parallel->isGlobal()) continue;
      getPerpandiculars( parallel, parallel->base()->getAutoSource(), direction, depth, perpandiculars );
      getPerpandiculars( parallel, parallel->base()->getAutoTarget(), direction, depth, perpandiculars );
    }

  // Apply caging constraints to perpandiculars.
    cdebug_tabw(159,1);
    if (perpandiculars.size() == 0) {
      cdebug_log(159,0) << "No perpandiculars to " << segment << endl;
      cdebug_tabw(159,-1);
      DebugSession::close();
      return;
    }

    Interval constraints ( minConstraint, maxConstraint );
    for ( size_t iperpand=0 ; iperpand<perpandiculars.size() ; iperpand++ ) {
      cdebug_log(159,0) << "Caged: " << constraints << " " << perpandiculars[iperpand] << endl;
      perpandiculars[iperpand]->base()->mergeUserConstraints( constraints );
      Interval ppConstraints = perpandiculars[iperpand]->base()->getUserConstraints();
      if (ppConstraints.isEmpty()) {
        cdebug_log(159,0) << "Cumulative caged constraints are too tight on " << perpandiculars[iperpand] << endl;
        findFailedPerpandiculars( rp, direction, faileds );
      }
      if (perpandiculars[ iperpand ]->isHorizontal())
        perpandiculars[ iperpand ]->setAxis( rp->getPosition().getY() );
      else
        perpandiculars[ iperpand ]->setAxis( rp->getPosition().getX() );
    }

    cdebug_tabw(159,-1);
    DebugSession::close();
  }


#if THIS_IS_DISABLED
  void  moveUpCaged ( TrackElement* segment )
  {
    DebugSession::open( segment->getNet(), 150, 160 );
    cdebug_log(159,1) << "::moveUpCaged() " << segment << endl;

  //Configuration* configuration = Session::getConfiguration();
  //const Layer*   metal2        = configuration->getRoutingLayer( 1 );
  //const Layer*   metal3        = configuration->getRoutingLayer( 2 );

    Anabatic::AutoContact* support = segment->base()->getAutoSource();
    RoutingPad*             rp      = dynamic_cast<RoutingPad*>(support->getAnchor());

    for( Component* component : rp->getSlaveComponents() ) {
      Horizontal*   baseSegment   = dynamic_cast<Horizontal*>( component );
      TrackElement* accessSegment = Session::lookup( baseSegment );

      if (accessSegment and not accessSegment->isFixed()) {
        accessSegment->moveUp( Flags::NoFlags );
      }
    }

    cdebug_tabw(159,-1);
    DebugSession::close();
  }
#endif


  void  protectCagedTerminals ( Track* track )
  {
    cdebug_log(159,1) << "protectCagedTerminals() " << track << endl;

    DbU::Unit      lastMovedUp   = track->getMin();
    Configuration* configuration = Session::getConfiguration();
    const Layer*   metal2        = configuration->getRoutingLayer( 1 );
    Net*           neighborNet   = NULL;
    RoutingPlane*  metal3plane   = track->getRoutingPlane()->getTop();

    if (track->getLayer() != metal2) {
      cdebug_tabw(159,-1);
      return;
    }

    for ( size_t i=0 ; i<track->getSize() ; ++i ) {
      TrackElement* segment = track->getSegment(i);
      if (not segment or segment->isRouted()) continue;
      if (segment and (segment->isFixed() or segment->isFixedAxis()) and segment->isTerminal()) {
        DbU::Unit  ppitch = segment->getPPitch();

        if (  ((segment->getSourceU() - track->getMin()) < 2*ppitch)
           or ((track->getMax() - segment->getTargetU()) < 2*ppitch) ) continue;

        Interval freeInterval = track->getFreeInterval( segment->getSourceU(), segment->getNet() );

        if (  (segment->getSourceU() - freeInterval.getVMin() < ppitch*3)
           or (freeInterval.getVMax() - segment->getTargetU() < ppitch*3) ) {
          cparanoid << "[INFO] Caged terminal: " << segment << endl;

          Anabatic::AutoContact* support     = NULL;
          Anabatic::AutoContact* turn        = NULL;
          if (segment->base()->isSourceTerminal()) {
            support = segment->base()->getAutoSource();
            turn    = segment->base()->getAutoTarget();
          } else {
            support = segment->base()->getAutoTarget();
            turn    = segment->base()->getAutoSource();
          }

          DbU::Unit supportLength = support->getConstraintBox().getWidth();
          cdebug_log(159,0) << "Support length (rp): " << DbU::getValueString(supportLength) << endl;

          if (  (segment->getLayer () != metal2)
             or (supportLength        >= ppitch)
             or (segment->getNet   () == neighborNet) ) {
            neighborNet = segment->getNet();
            continue;
          }

          cdebug_log(159,0) << "Protect " << segment << endl;

          RoutingPad* rp          = dynamic_cast<RoutingPad*>(support->getAnchor());
          Track*      metal3track = metal3plane->getTrackByPosition( rp->getSourcePosition().getX() );

          turn->restrictConstraintBox( freeInterval.getVMin()
                                     , freeInterval.getVMax()
                                     , Flags::Vertical );

          if (metal3track->getFreeInterval(segment->getAxis(),segment->getNet()).isEmpty()) {
            cparanoid << "[INFO]   Cannot protect caged terminal because top layer (metal3) is obstructed." << endl;
            continue;
          }
          lastMovedUp = segment->getSourceU();
        }

        neighborNet = segment->getNet();
      }
    }

    cdebug_tabw(159,-1);
  }


} // End of local namespace.


namespace Katana {


  using Hurricane::Bug;
  using Hurricane::Net;
  using Hurricane::Name;
  using Anabatic::AutoSegmentLut;


  void  KatanaEngine::preProcess ()
  {
  //DebugSession::open( 145, 150 );

    for ( size_t i=0 ; i<_routingPlanes.size() ; ++i ) {
      RoutingPlane* plane = _routingPlanes[i];

      Track* track = plane->getTrackByIndex( 0 );
      while ( track ) {
        protectCagedTerminals( track );
        track = track->getNextTrack();
      }
    }

  //DebugSession::close();
    Session::revalidate ();

    set<TrackElement*> faileds;
    for ( size_t i=0 ; i<_routingPlanes.size() ; ++i ) {
      RoutingPlane* plane = _routingPlanes[i];

      Track* track = plane->getTrackByIndex( 0 );
      while ( track ) {
        for ( size_t i=0 ; i<track->getSize() ; ++i ) {
          TrackElement* segment = track->getSegment( i );
          if (not segment->isFixedSpanRp()) {
            if (segment->isFixedSpan()) continue;
            if (  not segment->isFixed()
               or not segment->isFixedAxis()) continue;
          }
          propagateCagedConstraints( this, segment, i, faileds );
        }
        track = track->getNextTrack();
      }
    }

    Session::revalidate ();
  }


} // Katana namespace.
