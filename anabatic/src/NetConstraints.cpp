
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2013, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |         A n a b a t i c  -  Routing Toolbox                     |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./NetConstraints.cpp"                     |
// +-----------------------------------------------------------------+


#include <cstdlib>
#include <sstream>
#include "hurricane/DebugSession.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Net.h"
#include "hurricane/NetExternalComponents.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Pad.h"
#include "hurricane/Plug.h"
#include "hurricane/Instance.h"
#include "hurricane/Vertical.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Cell.h"
#include "anabatic/AutoContactTurn.h"
#include "anabatic/AutoSegment.h"
#include "anabatic/Session.h"
#include "anabatic/AnabaticEngine.h"


namespace {


 //  \addtogroup   NetConstraints
 //  \{

 /*  \function     void  propagateConstraintFromRp ( RoutingPad* rp )
  *  \param        rp The \c RoutingPad starting point.
  *
  *                Do a full constraint propagation starting from this \c RoutingPad.
  */

 //  \}


  using namespace std;
  using namespace CRL;
  using namespace Hurricane;
  using namespace Anabatic;


// -----------------------------------------------------------------
// Local Functions.


  void  propagateConstraintFromRp ( RoutingPad* rp )
  {
    cdebug_log(146,1) << "propagateConstraintFromRp() - " << rp << endl;
    RoutingLayerGauge* rlg          = Session::getLayerGauge( rp->getLayer() );
    bool               allowOffgrid = (rlg->getType() == Constant::LocalOnly);
    bool               isSymbolic   = Session::getRoutingGauge()->isSymbolic();

    for ( Component* component : rp->getSlaveComponents() ) {
      cdebug_log(146,0) << "slave component: " << component << endl;
      AutoContact* sourceContact = Session::lookup( dynamic_cast<Contact*>(component) );
      if (sourceContact) {
        Box  constraintBox = sourceContact->getConstraintBox();

        cdebug_log(146,0) << "Start slave: " << sourceContact << endl;
        cdebug_log(146,0) << "Constraint: " << constraintBox << endl;

        set<AutoSegment*>  verticalSegments;
        set<AutoSegment*>  horizontalSegments;
        set<AutoSegment*>  offgridSegments;
        set<AutoSegment*>  nonprefSegments;

        for ( AutoSegment* segment : sourceContact->getAutoSegments() ) {
          cdebug_log(146,0) << "Examining: " << segment << endl;
          AutoContact* targetContact = segment->getOppositeAnchor( sourceContact );

          if (targetContact) {
            if (allowOffgrid and (segment->getLayer() == rp->getLayer()) and segment->isNonPref()) {
              cdebug_log(146,0) << "On offgrid stack " << segment << endl;
              offgridSegments.insert( segment );
              continue;
            }
            if (segment->isNonPref()) {
              nonprefSegments.insert( segment );
              continue;
            }
            if (segment->isHorizontal()) {
              cdebug_log(146,0) << "On horizontal stack " << segment << endl;
              horizontalSegments.insert( segment );
              continue;
            }
            cdebug_log(146,0) << "On vertical stack " << segment << endl;
            verticalSegments.insert( segment );
          }
        }

        // Propagate constraint through horizontally aligned segments.
        cdebug_log(146,0) << "Propagate constraint on horizontal segments" << endl;

        for ( AutoSegment* horizontal : horizontalSegments ) {
          AutoContact* contact = nullptr;
          for ( AutoSegment* aligned : horizontal->getAligneds(Flags::WithSelf) ) {
            cdebug_log(146,0) << "aligned horizontal: " << aligned << endl;

            contact = aligned->getAutoTarget();
            cdebug_log(146,0) << "contact: " << contact << endl;
            if (contact) {
              cdebug_log(146,0) << "Apply to (target): " << contact << endl;
              contact->restrictConstraintBox( constraintBox.getYMin()
                                            , constraintBox.getYMax()
                                            , Flags::Horizontal|Flags::WarnOnError );
            }
            contact = aligned->getAutoSource();
            cdebug_log(146,0) << "contact: " << contact << endl;
            if (contact) {
              cdebug_log(146,0) << "Apply to (source): " << contact << endl;
              contact->restrictConstraintBox( constraintBox.getYMin()
                                            , constraintBox.getYMax()
                                            , Flags::Horizontal|Flags::WarnOnError );
            }
          }
        } 

        // Propagate constraint on non-pref segments (directly on the RP).
        cdebug_log(146,0) << "Propagate constraint on non-pref segments" << endl;
        for ( AutoSegment* segment : nonprefSegments ) {
          RoutingLayerGauge* segRlg             = Session::getLayerGauge( segment->getLayer() );
          DbU::Unit          pitch              = segRlg->getPitch();
          AutoContact*       targetContact      = segment->getOppositeAnchor( sourceContact );
          AutoContactTurn*   turn               = dynamic_cast<AutoContactTurn*>( targetContact );
          AutoSegment*       perpandicular      = (turn) ? turn->getPerpandicular( segment ) : nullptr;
          Box                nonPrefConstraint  = constraintBox;
          Box                parallelConstraint = constraintBox;

          if (not segRlg or not targetContact) continue;
          if (not isSymbolic and perpandicular and perpandicular->isGlobal()) continue;

          if (segment->isVertical()) {
            nonPrefConstraint .inflate( 0      , pitch, 0      , pitch );
            parallelConstraint.inflate( 2*pitch, pitch, 2*pitch, pitch );
          } else {
            nonPrefConstraint .inflate( pitch, 0      , pitch, 0       );
            parallelConstraint.inflate( pitch, 2*pitch, pitch, 2*pitch );
          }
          targetContact->setConstraintBox( nonPrefConstraint );
          cdebug_log(146,0) << "targetContact " << targetContact << endl;
          cdebug_log(146,0) << "-> non-pref constraint " << nonPrefConstraint << endl;
          cdebug_log(146,0) << "-> parallel constraint " << parallelConstraint << endl;

          if (rp->isVSmall()) {
            if (turn) {
              turn = dynamic_cast<AutoContactTurn*>( perpandicular->getOppositeAnchor( turn ));
              cdebug_log(149,0) << "turn:          " << turn << endl;
              cdebug_log(149,0) << "perpandicular: " << perpandicular << endl;
              if (turn) {
                AutoSegment* parallel = turn->getPerpandicular( perpandicular );
                cdebug_log(149,0) << "parallel: " << parallel << endl;
                if (   not parallel->isNonPref()
                   and not parallel->isGlobal()) {
                  AutoContact* oppositeTurn   = parallel->getOppositeAnchor( turn );
                  Box          segConstraints = oppositeTurn->getConstraintBox();
                  segConstraints = segConstraints.getIntersection( parallelConstraint );
                  cdebug_log(149,0) << "oppositeTurn:   " << oppositeTurn << endl;
                  cdebug_log(149,0) << "segConstraints: " << segConstraints << endl;
                  if (  (parallel->isVertical  () and (segConstraints.getWidth () > pitch))
                     or (parallel->isHorizontal() and (segConstraints.getHeight() > pitch))) {
                    cdebug_log(146,0) << "Applies on perpandicular turn " << turn << endl;
                    turn->setConstraintBox( parallelConstraint );

                    if (parallel->getBreakLevel() > 0) {
                      cdebug_log(146,0) << "Applies on parallel turn " << turn << endl;
                      oppositeTurn->setConstraintBox( parallelConstraint );
                    }
                  }
                }
              }
            }
          }
        }

        // Propagate constraint through vertically aligned segments.
        cdebug_log(146,0) << "Propagate constraint on vertical segments" << endl;

        for ( AutoSegment* vertical : verticalSegments ) {
          AutoContact* contact = nullptr;
          for ( AutoSegment* aligned : vertical->getAligneds(Flags::WithSelf) ) {
            cdebug_log(146,0) << "aligned vertical: " << aligned << endl;

            contact = aligned->getAutoTarget();
            if (contact) {
              cdebug_log(146,0) << "Apply to (target): " << contact << endl;
              contact->restrictConstraintBox( constraintBox.getXMin()
                                            , constraintBox.getXMax()
                                            , Flags::Vertical|Flags::WarnOnError );
            }
            contact = aligned->getAutoSource();
            if (contact) {
              cdebug_log(146,0) << "Apply to (source): " << contact << endl;
              contact->restrictConstraintBox( constraintBox.getXMin()
                                            , constraintBox.getXMax()
                                            , Flags::Vertical|Flags::WarnOnError );
            }
          } 
        }

        // Special case of offgrid segments (usually local M1).
        cdebug_log(146,0) << "Propagate constraint on offgrid segments" << endl;

        for ( AutoSegment* perpandicular : offgridSegments ) {
          AutoContact* targetContact     = perpandicular->getOppositeAnchor( sourceContact );
          Box          nonPrefConstraint = constraintBox;
          Box          ongridConstraint  = constraintBox;
          AutoSegment* parallel          = targetContact->getPerpandicular( perpandicular );
          if (rlg->isHorizontal()) {
            nonPrefConstraint.inflate( 0, rlg->getPitch(), 0, rlg->getPitch() );
          } else {
            nonPrefConstraint.inflate( rlg->getPitch(), 0, rlg->getPitch(), 0 );
          }
          ongridConstraint.inflate( rlg->getPitch() );

          if (targetContact->isTurn()) {
            AutoSegment* nextPerpand = targetContact->getPerpandicular( parallel );
            if (nextPerpand->isNonPref()) {
              cdebug_log(146,0) << "Next perpand is non-pref -> not applying constraints" << endl;
              cdebug_log(146,0) << "  " << nextPerpand << endl;
              continue;
            }
          }

          cdebug_log(146,0) << "perpandicular offgrid: " << perpandicular << endl;
          targetContact->setConstraintBox( nonPrefConstraint );
          cdebug_log(146,0) << "Apply to first turn: " << targetContact << endl;
          if (not targetContact->isTurn()) continue;

          cdebug_log(146,0) << "parallel offgrid: " << parallel << endl;
          targetContact = parallel->getOppositeAnchor( targetContact );
          if (not targetContact) continue;
          targetContact->setConstraintBox( ongridConstraint );
          cdebug_log(146,0) << "Apply to second turn: " << targetContact << endl;
        }
      }
    }

    cdebug_log(146,0) << "propagateConstraintFromRp() - Exit" << endl;
    cdebug_tabw(146,-1);
  }


}  // Anonymous namespace.



namespace Anabatic {


  using Hurricane::Cell;


  void  propagateDistanceFromRp ( RoutingPad* rp )
  {
    cdebug_log(146,1) << "propagateDistanceFromRp() - " << rp << endl;

    unsigned int                              distance = 0;
    vector< pair<AutoContact*,AutoSegment*> > currents;
    vector< pair<AutoContact*,AutoSegment*> > successors;

    for ( Component* component : rp->getSlaveComponents() ) {
      cdebug_log(146,0) << "slave component: " << component << endl;

      AutoContact* sourceContact = Session::lookup( dynamic_cast<Contact*>(component) );
      if (sourceContact) {
        cdebug_log(146,0) << "Start slave: " << sourceContact << endl;

        for ( AutoSegment* segment : sourceContact->getAutoSegments() ) {
          cdebug_log(146,0) << "Pushing: " << segment << endl;
          currents.push_back( make_pair(sourceContact,segment) );
        }
      }
    }

    while ( not currents.empty() ) {
      for ( size_t i = 0 ; i<currents.size() ; ++i ) {
        AutoContact* source  = currents[i].first;
        AutoSegment* segment = currents[i].second;

        if ( (distance == 1) and (segment->getRpDistance() == 1) ) {
          vector<GCell*> gcells;
          segment->getGCells( gcells );

          if (gcells.size() < 3)
            segment->setFlags( AutoSegment::SegUnbreakable );
        }

        if (distance >= segment->getRpDistance()) continue;
        segment->setRpDistance( distance );
        cdebug_log(146,0) << "Popped: " << segment << endl;
        
        AutoContact* target = segment->getOppositeAnchor( source );
        if (target) {
          for ( AutoSegment* successor : target->getAutoSegments() ) {
            if (successor == segment) continue;
            
            // if (successor->isNonPref()) {
            //   cdebug_log(146,0) << "Pushing (non-pref): " << successor << endl;
            //   currents.push_back( make_pair(target,successor) );
            // } else {
              cdebug_log(146,0) << "Pushing: " << successor << endl;
              successors.push_back( make_pair(target,successor) );
            // }
          }
        }
      }

      if (++distance > 15) break;

      currents.clear();
      currents.swap( successors );
    }

    cdebug_log(146,0) << "propagateDistanceFromRp() - Exit" << endl;
    cdebug_tabw(146,-1);
  }


  void  AnabaticEngine::computeNetConstraints ( Net* net )
  {
    DebugSession::open( net, 146, 150);

    cdebug_log(149,0) << "Anabatic::computeNetConstraints( " << net << " )" << endl;
    cdebug_tabw(146,1);

    vector<RoutingPad*> routingPads;
    for ( Component* component : net->getComponents() ) {
      Contact* contact = dynamic_cast<Contact*>( component );
      if (contact) {
        AutoContact* autoContact = Session::lookup( contact );
        if (autoContact) 
          autoContact->restoreNativeConstraintBox();
      } else {
        Segment* segment = dynamic_cast<Segment*>( component );
        if (segment) {
          AutoSegment* autoSegment = Session::lookup( segment );
          if (autoSegment) 
            autoSegment->setRpDistance( 15 );
        } else {
          RoutingPad* routingPad = dynamic_cast<RoutingPad*>( component );
          if (routingPad) routingPads.push_back( routingPad );
        }
      }
    }

    for ( size_t i=0 ; i<routingPads.size() ; i++ ) {
      propagateConstraintFromRp( routingPads[i] );
      propagateDistanceFromRp  ( routingPads[i] );
    }

    for ( Segment* segment : net->getSegments() ) {
      AutoSegment* autoSegment = Session::lookup( segment );
      if (not autoSegment) continue;
      if (autoSegment->isUnbreakable()) continue;
      if (autoSegment->getRpDistance() >= 1) continue;

      vector<GCell*> gcells;
      autoSegment->getGCells( gcells );

      if     (gcells.size() >  2) continue;
    //if (   (gcells.size() == 2)
    //   and (  not autoSegment->getAutoSource()->isTerminal()
    //       or not autoSegment->getAutoTarget()->isTerminal()) ) continue;

      autoSegment->setFlags( AutoSegment::SegUnbreakable );
    }

    // forEach ( Segment*, isegment, net->getSegments() ) {
    //   AutoSegment* autoSegment = Session::lookup( *isegment );
    //   if (autoSegment) autoSegment->toConstraintAxis();
    // }

    cdebug_tabw(146,-1);
    DebugSession::close();
  }


}  // Anabatic namespace.
