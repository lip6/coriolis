
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PreProcess.cpp"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#include  <sstream>

#include  "hurricane/Bug.h"
#include  "hurricane/Warning.h"
#include  "hurricane/Net.h"
#include  "hurricane/Name.h"
#include  "hurricane/RoutingPad.h"
#include  "katabatic/AutoContact.h"
#include  "kite/GCell.h"
#include  "kite/DataNegociate.h"
#include  "kite/TrackElement.h"
#include  "kite/Track.h"
#include  "kite/RoutingPlane.h"
#include  "kite/Session.h"
#include  "kite/KiteEngine.h"



namespace {


  using namespace std;
  using namespace Hurricane;
  using namespace CRL;
  using namespace Kite;


  void  getPerpandiculars ( TrackElement*          segment
                          , AutoContact*           from
                          , unsigned int           direction
                          , vector<TrackElement*>& perpandiculars
                          )
  {
  //AutoContact* to = segment->base()->getAutoSource();
  //to = (to != from) ? to : segment->base()->getAutoTarget();

    TrackElement* perpandicular;
    forEach ( Segment*, isegment, segment->base()->getAutoSource()->getSlaveComponents().getSubSet<Segment*>() ) {
      perpandicular = Session::lookup ( *isegment );
      ltrace(200) << "S " << perpandicular << endl;

      if ( not perpandicular or (perpandicular->getDirection() == direction) ) continue;

      perpandiculars.push_back ( perpandicular );
    }

    forEach ( Segment*, isegment, segment->base()->getAutoTarget()->getSlaveComponents().getSubSet<Segment*>() ) {
      perpandicular = Session::lookup ( *isegment );
      ltrace(200) << "T " << perpandicular << endl;

      if ( not perpandicular or (perpandicular->getDirection() == direction) ) continue;

      perpandiculars.push_back ( perpandicular );
    }
  }


  void  findFailedPerpandiculars ( RoutingPad* rp, unsigned int direction, set<TrackElement*>& faileds )
  {
    ltrace(200) << "Find failed caging: " << rp << endl;

    TrackElement* parallel;
    forEach ( Segment*, isegment, rp->getSlaveComponents().getSubSet<Segment*>() ) {
      parallel = Session::lookup ( *isegment );
      ltrace(200) << "* " << parallel << endl;

      if ( parallel->isFixed () ) continue;
      if ( parallel->getDirection() != direction ) continue;

      AutoContact* contact = parallel->base()->getAutoSource();
      if ( contact->base()->getAnchor() != rp ) contact = NULL;

      if ( contact == NULL ) contact = parallel->base()->getAutoTarget();
      if ( contact->base()->getAnchor() != rp ) continue;

    //parallel->makeDogLeg ( contact->getGCell() );
      faileds.insert ( parallel );
    }
  }


  void  propagateCagedConstraints ( TrackElement* segment, set<TrackElement*>& faileds )
  {
    if ( not segment->isFixed() ) return;

    ltrace(200) << "Propagate caging: " << segment << endl;

    Track*                 track         = segment->getTrack();
    unsigned int           direction     = Session::getRoutingGauge()->getLayerDirection(segment->getLayer());
    AutoContact*           source        = segment->base()->getAutoSource();
    RoutingPad*            rp            = NULL;
    Interval               uside         = source->getGCell()->getUSide(direction);
    DbU::Unit              minConstraint = DbU::Min;
    DbU::Unit              maxConstraint = DbU::Max;
    vector<TrackElement*>  perpandiculars;

    if ( not track ) {
      cerr << Bug("%s is not inserted in a <Track>",getString(segment).c_str()) << endl;
      return;
    }

  // Computing constraints from fixed only TrackElements (caging).
    TrackElement* parallel;
    size_t i = segment->getIndex();
    while ( i > 0 ) {
      parallel = track->getSegment(--i);
      if ( not parallel ) continue;
      if ( parallel->getTargetU() < uside.getVMin() ) break;
      if ( parallel->getNet() == segment->getNet() ) continue;
      if ( not parallel->isFixed() ) continue;

      ltrace(200) << "Min Constraint from: " << parallel << endl;
      minConstraint = max ( minConstraint, parallel->getTargetU() );
    }

    i = segment->getIndex();
    while ( i < track->getSize()-1 ) {
      parallel = track->getSegment(++i);
      if ( not parallel ) continue;
      if ( parallel->getSourceU() > uside.getVMax() ) break;
      if ( parallel->getNet() == segment->getNet() ) continue;
      if ( not parallel->isFixed() ) continue;

      ltrace(200) << "Max Constraint from: " << parallel << endl;
      maxConstraint = min ( maxConstraint, parallel->getSourceU() );
    }

    if ( minConstraint > maxConstraint ) {
      cerr << Bug("%s have too tight caging constraints.",getString(segment).c_str()) << endl;
      return;
    }
    if ( (minConstraint <= uside.getVMin()) and (maxConstraint >= uside.getVMax()) ) {
      ltrace(200) << "No constraints [" << DbU::getValueString(minConstraint)
                  << ":"                << DbU::getValueString(maxConstraint)
                  << " vs. " << uside << endl;
      return;
    }

  // Finding perpandiculars, by way of the source & target RoutingPad.
    if ( source->getAnchor() ) {
      rp = dynamic_cast<RoutingPad*>(source->getAnchor());
      if ( rp ) {
        TrackElement* parallel;
        forEach ( Segment*, isegment, rp->getSlaveComponents().getSubSet<Segment*>() ) {
          parallel = Session::lookup ( *isegment );
          ltrace(200) << "* " << parallel << endl;

          if ( parallel->isFixed () ) continue;
          if ( parallel->isGlobal() ) continue;
          getPerpandiculars ( parallel, source, direction, perpandiculars );
          getPerpandiculars ( parallel, segment->base()->getAutoTarget(), direction, perpandiculars );
        }
      } else {
        cerr << Bug("%s is not anchored on a <RoutingPad>\n       (%s)"
                   ,getString(source).c_str()
                   ,getString(source->getAnchor()).c_str()) << endl;
      }
    }

  // Apply caging constraints to perpandiculars.
    ltracein(200);
    if ( perpandiculars.size() == 0 ) {
      ltrace(200) << "No perpandiculars to " << segment << endl;
      ltraceout(200);
      return;
    }

    Interval constraints ( minConstraint, maxConstraint );
    for ( size_t iperpand=0 ; iperpand<perpandiculars.size() ; iperpand++ ) {
      ltrace(200) << "Caged: " << constraints << " " << perpandiculars[iperpand] << endl;
      perpandiculars[iperpand]->base()->mergeUserConstraints ( constraints );
      if ( perpandiculars[iperpand]->base()->getUserConstraints().isEmpty() ) {
        ltrace(200) << "Cumulative caged constraints are too tight on " << perpandiculars[iperpand] << endl;
        findFailedPerpandiculars ( rp, direction, faileds );
      }
    }

    ltraceout(200);
  }


  void  protectCagedTerminals ( Track* track )
  {
    Configuration* configuration = Session::getConfiguration ();
    const Layer*   metal2        = configuration->getRoutingLayer ( 1 );
    const Layer*   metal3        = configuration->getRoutingLayer ( 2 );

    for ( size_t i=0 ; i<track->getSize() ; i++ ) {
      TrackElement* segment = track->getSegment ( i );
      if ( segment and segment->isFixed() and segment->isTerminal() ) {
        Interval freeInterval = track->getFreeInterval ( segment->getSourceU(), segment->getNet() );

        if ( freeInterval.getSize() < DbU::lambda(30.0) ) {
          cinfo << "Caged terminal: " << segment << endl;
          if ( segment->getLayer() != metal2 ) continue;
          if ( segment->getLength() >= DbU::lambda(5.0) ) continue;

          AutoContact* support = segment->base()->getAutoSource();
          RoutingPad*  rp      = dynamic_cast<RoutingPad*>(support->getAnchor());
          GCell*       gcell   = Session::lookup ( support->getGCell() );

          AutoContact* source = AutoContact::fromRp ( gcell->base()
                                                    , rp
                                                    , metal3
                                                    , rp->getSourcePosition()
                                                    , DbU::lambda(1.0), DbU::lambda(1.0)
                                                    , true
                                                    );

          AutoContact* target = AutoContact::fromRp ( gcell->base()
                                                    , rp
                                                    , metal3
                                                    , rp->getSourcePosition()
                                                    , DbU::lambda(1.0), DbU::lambda(1.0)
                                                    , true
                                                    );

          AutoSegment* segment = AutoSegment::create ( source
                                                     , target
                                                     , Constant::Vertical
                                                     , AutoSegment::Local
                                                     , true
                                                     , false
                                                     );
          segment->setFixed ( true );
          GCell::addTrackSegment ( gcell, segment, true );

#if DISABLED
        // Force slackening.
          bool breakFlag = false;
          forEach ( Contact*, icontact, rp->getSlaveComponents().getSubSet<Contact*>() ) {
            forEach ( Segment*, isegment, icontact->getSlaveComponents().getSubSet<Segment*>() ) {
              TrackElement* trackSegment = Session::lookup(*isegment);
              if ( not trackSegment or trackSegment->isFixed() ) continue;

              if ( trackSegment->isHorizontal() ) {
                ltrace(200) << "M2 to slacken for " << rp << endl;
                breakFlag = true;

                const vector<AutoSegment*>& dogLegs = Session::getDogLegs();

                trackSegment->base()->makeDogLeg ( gcell->base(), true );
                GCell::addTrackSegment ( gcell, dogLegs[1], true );
                GCell::addTrackSegment ( gcell, dogLegs[2], true );
                Session::revalidate ();
              } 

              if ( breakFlag ) break;
            }
            if ( breakFlag ) break;
          }
#endif
        }
      }
    }
  }


} // End of local namespace.


namespace Kite {


  using Hurricane::Bug;
  using Hurricane::Net;
  using Hurricane::Name;


  void  KiteEngine::preProcess ()
  {
    for ( size_t i=0 ; i<_routingPlanes.size() ; i++ ) {
      RoutingPlane* plane = _routingPlanes[i];

      Track* track = plane->getTrackByIndex ( 0 );
      while ( track ) {
        protectCagedTerminals ( track );
        track = track->getNext ();
      }
    }
    Session::revalidate ();
  }


  void  KiteEngine::_computeCagedConstraints ()
  {
    set<TrackElement*> faileds;

    TrackElementLut::iterator isegment = _trackSegmentLut.begin();
    for ( ; isegment != _trackSegmentLut.end() ; isegment++ ) {
      if ( not isegment->second->isFixed() ) continue;
      propagateCagedConstraints ( isegment->second, faileds );
    }
  }


  void  KiteEngine::_computeCagedConstraints ( Net* net, set<TrackElement*>& faileds )
  {
    TrackElement* segment = NULL;

    forEach ( Segment*, isegment, net->getComponents().getSubSet<Segment*>() ) {
      segment = Session::lookup ( *isegment );
      if ( not segment ) continue;

      segment->base()->resetUserConstraints();
    }

    forEach ( Segment*, isegment, net->getComponents().getSubSet<Segment*>() ) {
      segment = Session::lookup ( *isegment );
      if ( not segment ) continue;

      propagateCagedConstraints ( segment, faileds );
    }
  }


} // End of Kite namespace.
