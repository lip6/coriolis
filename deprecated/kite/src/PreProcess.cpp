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
#include "katabatic/AutoContactTerminal.h"
#include "kite/DataNegociate.h"
#include "kite/TrackElement.h"
#include "kite/Track.h"
#include "kite/RoutingPlane.h"
#include "kite/NegociateWindow.h"
#include "kite/Session.h"
#include "kite/KiteEngine.h"


namespace {

  using namespace std;
  using namespace Hurricane;
  using namespace CRL;
  using namespace Kite;
  using Katabatic::perpandicularTo;
  using Katabatic::AutoContactTerminal;


  void  getPerpandiculars ( TrackElement*           segment
                          , Katabatic::AutoContact* from
                          , unsigned int            direction
                          , vector<TrackElement*>&  perpandiculars
                          )
  {
    TrackElement* perpandicular;
    for( Segment* osegment : segment->base()->getAutoSource()->getSlaveComponents().getSubSet<Segment*>() ) {
      perpandicular = Session::lookup ( osegment );
      cdebug_log(159,0) << "S " << perpandicular << endl;

      if ( not perpandicular or (perpandicular->getDirection() == direction) ) continue;

      perpandiculars.push_back ( perpandicular );
    }

    for( Segment* osegment : segment->base()->getAutoTarget()->getSlaveComponents().getSubSet<Segment*>() ) {
      perpandicular = Session::lookup ( osegment );
      cdebug_log(159,0) << "T " << perpandicular << endl;

      if ( not perpandicular or (perpandicular->getDirection() == direction) ) continue;

      perpandiculars.push_back ( perpandicular );
    }
  }


  void  findFailedPerpandiculars ( RoutingPad* rp, unsigned int direction, set<TrackElement*>& faileds )
  {
    cdebug_log(159,0) << "Find failed caging: " << rp << endl;

    TrackElement* parallel;
    for( Segment* osegment : rp->getSlaveComponents().getSubSet<Segment*>() ) {
      parallel = Session::lookup ( osegment );
      cdebug_log(159,0) << "* " << parallel << endl;

      if ( parallel->isFixed () ) continue;
      if ( parallel->getDirection() != direction ) continue;

      Katabatic::AutoContact* contact = parallel->base()->getAutoSource();
      if ( contact->base()->getAnchor() != rp ) contact = NULL;

      if ( contact == NULL ) contact = parallel->base()->getAutoTarget();
      if ( contact->base()->getAnchor() != rp ) continue;

    //parallel->makeDogLeg ( contact->getGCell() );
      faileds.insert ( parallel );
    }
  }


  void  propagateCagedConstraints ( TrackElement* segment, set<TrackElement*>& faileds )
  {
    if (not segment->isFixed()) return;

    cdebug_log(159,0) << "Propagate caging: " << segment << endl;

    Track*                  track         = segment->getTrack();
  //unsigned int            direction     = Session::getRoutingGauge()->getLayerDirection(segment->getLayer());
    unsigned int            direction     = segment->getDirection();
    Katabatic::AutoContact* source        = segment->base()->getAutoSource();
    RoutingPad*             rp            = NULL;
    Interval                uside         = source->getGCell()->getSide(direction);
    DbU::Unit               minConstraint = DbU::Min;
    DbU::Unit               maxConstraint = DbU::Max;
    vector<TrackElement*>   perpandiculars;

    if ( not track ) {
      cerr << Bug( "%s is not inserted in a <Track>", getString(segment).c_str() ) << endl;
      return;
    }

  // Computing constraints from fixed only TrackElements (caging).
    TrackElement* parallel;
    size_t i = segment->getIndex();
    while ( i > 0 ) {
      parallel = track->getSegment( --i );
      if (not parallel) continue;
      if (parallel->getTargetU() < uside.getVMin()) break;
      if (parallel->getNet() == segment->getNet()) continue;
      if (not parallel->isFixed()) continue;

      cdebug_log(159,0) << "Min Constraint from: " << parallel << endl;
      minConstraint = max( minConstraint, parallel->getTargetU() );
    }

    i = segment->getIndex();
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
      return;
    }
    if ( (minConstraint <= uside.getVMin()) and (maxConstraint >= uside.getVMax()) ) {
      cdebug_log(159,0) << "No constraints [" << DbU::getValueString(minConstraint)
                  << ":"                << DbU::getValueString(maxConstraint)
                  << " vs. " << uside << endl;
      return;
    }

  // Finding perpandiculars, by way of the source & target RoutingPad.
    if (source->getAnchor()) {
      rp = dynamic_cast<RoutingPad*>(source->getAnchor());
      if (rp) {
        TrackElement* parallel;
        for( Segment* osegment : rp->getSlaveComponents().getSubSet<Segment*>() ) {
          parallel = Session::lookup( osegment );
          cdebug_log(159,0) << "* " << parallel << endl;

          if (parallel->isFixed ()) continue;
          if (parallel->isGlobal()) continue;
          getPerpandiculars( parallel, source, direction, perpandiculars );
          getPerpandiculars( parallel, segment->base()->getAutoTarget(), direction, perpandiculars );
        }
      } else {
        cerr << Bug( "%s is not anchored on a <RoutingPad>\n       (%s)"
                   , getString(source).c_str()
                   , getString(source->getAnchor()).c_str() ) << endl;
      }
    }

  // Apply caging constraints to perpandiculars.
    cdebug_tabw(159,1);
    if (perpandiculars.size() == 0) {
      cdebug_log(159,0) << "No perpandiculars to " << segment << endl;
      cdebug_tabw(159,-1);
      return;
    }

    Interval constraints ( minConstraint, maxConstraint );
    for ( size_t iperpand=0 ; iperpand<perpandiculars.size() ; iperpand++ ) {
      cdebug_log(159,0) << "Caged: " << constraints << " " << perpandiculars[iperpand] << endl;
      perpandiculars[iperpand]->base()->mergeUserConstraints( constraints );
      if (perpandiculars[iperpand]->base()->getUserConstraints().isEmpty()) {
        cdebug_log(159,0) << "Cumulative caged constraints are too tight on " << perpandiculars[iperpand] << endl;
        findFailedPerpandiculars( rp, direction, faileds );
      }
    }

    cdebug_tabw(159,-1);
  }


#if THIS_IS_DISABLED
  void  moveUpCaged ( TrackElement* segment )
  {
    DebugSession::open( segment->getNet(), 150, 160 );
    cdebug_log(159,1) << "::moveUpCaged() " << segment << endl;

  //Configuration* configuration = Session::getConfiguration();
  //const Layer*   metal2        = configuration->getRoutingLayer( 1 );
  //const Layer*   metal3        = configuration->getRoutingLayer( 2 );

    Katabatic::AutoContact* support = segment->base()->getAutoSource();
    RoutingPad*             rp      = dynamic_cast<RoutingPad*>(support->getAnchor());

    for( Component* component : rp->getSlaveComponents() ) {
      Horizontal*   baseSegment   = dynamic_cast<Horizontal*>( component );
      TrackElement* accessSegment = Session::lookup( baseSegment );

      if (accessSegment and not accessSegment->isFixed()) {
        accessSegment->moveUp( Katabatic::KbNoFlags );
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
    unsigned int   moveUpCount   = 0;

    Configuration* configuration = Session::getConfiguration();
    const Layer*   metal2        = configuration->getRoutingLayer( 1 );
    const Layer*   metal3        = configuration->getRoutingLayer( 2 );
    Net*           neighborNet   = NULL;
    RoutingPlane*  metal3plane   = track->getRoutingPlane()->getTop();

    if (track->getLayer() != metal2) {
      cdebug_tabw(159,-1);
      return;
    }

    for ( size_t i=0 ; i<track->getSize() ; ++i ) {
      TrackElement* segment = track->getSegment(i);
      if (not segment or segment->isRouted()) continue;
      if (segment and segment->isFixed() and segment->isTerminal()) {
        Interval   freeInterval = track->getFreeInterval( segment->getSourceU(), segment->getNet() );
        DbU::Unit  ppitch       = segment->getPPitch();

      //if (freeInterval.getSize() < ppitch*6) {
        if (  (segment->getSourceU() - freeInterval.getVMin() < ppitch*3)
           or (freeInterval.getVMax() - segment->getTargetU() < ppitch*3) ) {
          cparanoid << "[INFO] Caged terminal: " << segment << endl;
          if (  (segment->getLayer () != metal2)
             or (segment->getLength() >= ppitch)
             or (segment->getNet   () == neighborNet) ) {
            neighborNet = segment->getNet();
            continue;
          }

          Katabatic::AutoContact* support     = NULL;
          Katabatic::AutoContact* turn        = NULL;
          if (segment->base()->isSourceTerminal()) {
            support = segment->base()->getAutoSource();
            turn    = segment->base()->getAutoTarget();
          } else {
            support = segment->base()->getAutoTarget();
            turn    = segment->base()->getAutoSource();
          }

          RoutingPad* rp          = dynamic_cast<RoutingPad*>(support->getAnchor());
          Track*      metal3track = metal3plane->getTrackByPosition( rp->getSourcePosition().getX() );

          turn->restrictConstraintBox( freeInterval.getVMin()
                                     , freeInterval.getVMax()
                                     , KbVertical );

          if (metal3track->getFreeInterval(segment->getAxis(),segment->getNet()).isEmpty()) {
            cparanoid << "[INFO]   Cannot protect caged terminal because top layer (metal3) is obstructed." << endl;
            continue;
          }

          if (segment->getSourceU() - lastMovedUp < ppitch*4) {
            ++moveUpCount;
            if (moveUpCount % 2 == 0) {
            //moveUpCaged( segment );
            }
          } else {
            moveUpCount = 0;
          }
          lastMovedUp = segment->getSourceU();
          
          Katabatic::AutoContact* source
            = Katabatic::AutoContactTerminal::create( support->getGCell()
                                                    , rp
                                                    , metal3
                                                    , rp->getSourcePosition()
                                                    , DbU::fromLambda(1.0), DbU::fromLambda(1.0)
                                                    );
          source->setFlags( Katabatic::CntIgnoreAnchor );
    
          Katabatic::AutoContact* target =
            Katabatic::AutoContactTerminal::create( support->getGCell()
                                                  , rp
                                                  , metal3
                                                  , rp->getSourcePosition()
                                                  , DbU::fromLambda(1.0), DbU::fromLambda(1.0)
                                                  );
          target->setFlags( Katabatic::CntIgnoreAnchor );
          
          AutoSegment* fixedSegment = AutoSegment::create( source, target, Katabatic::KbVertical );
          fixedSegment->setFlags( Katabatic::SegFixed );
          Session::getNegociateWindow()->createTrackSegment( fixedSegment, KtLoadingStage );
        }

        neighborNet = segment->getNet();
      }
    }

    cdebug_tabw(159,-1);
  }


} // End of local namespace.


namespace Kite {


  using Hurricane::Bug;
  using Hurricane::Net;
  using Hurricane::Name;
  using Katabatic::AutoSegmentLut;


  void  KiteEngine::preProcess ()
  {
    for ( size_t i=0 ; i<_routingPlanes.size() ; ++i ) {
      RoutingPlane* plane = _routingPlanes[i];

      Track* track = plane->getTrackByIndex( 0 );
      while ( track ) {
        protectCagedTerminals( track );
        track = track->getNextTrack();
      }
    }
    Session::revalidate ();
  }


  void  KiteEngine::_computeCagedConstraints ()
  {
    set<TrackElement*> faileds;

    TrackElement*                  segment  = NULL;
    AutoSegmentLut::const_iterator isegment = _getAutoSegmentLut().begin();
    for ( ; isegment != _getAutoSegmentLut().end() ; isegment++ ) {
      segment = _lookup( isegment->second );
      if (not segment or not segment->isFixed()) continue;

      DebugSession::open( segment->getNet(), 150, 160 );
      propagateCagedConstraints( segment, faileds );
      DebugSession::close();
    }
  }


} // Kite namespace.
