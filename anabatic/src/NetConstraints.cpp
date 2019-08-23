
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
#include "anabatic/AutoContact.h"
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

    for ( Component* component : rp->getSlaveComponents() ) {
      cdebug_log(146,0) << "slave component: " << component << endl;
      AutoContact* sourceContact = Session::lookup( dynamic_cast<Contact*>(component) );
      if (sourceContact) {
        Box  constraintBox = sourceContact->getConstraintBox();

        cdebug_log(146,0) << "Start slave: " << sourceContact << endl;
        cdebug_log(146,0) << "Constraint: " << constraintBox << endl;

        set<AutoSegment*>  verticalSegments;
        set<AutoSegment*>  horizontalSegments;

        for ( AutoSegment* segment : sourceContact->getAutoSegments() ) {
          cdebug_log(146,0) << "Examining: " << segment << endl;
          AutoContact* targetContact = segment->getOppositeAnchor(sourceContact);

          if (targetContact) {
            if (segment->isHorizontal()) {
              cdebug_log(146,0) << "On horizontal stack " << segment << endl;
              horizontalSegments.insert( segment );
            } else {
              cdebug_log(146,0) << "On vertical stack " << segment << endl;
              verticalSegments.insert( segment );
            }
          }
        }

        // Propagate constraint through horizontally aligned segments.
        cdebug_log(146,0) << "Propagate constraint on horizontal segments" << endl;

        for ( AutoSegment* horizontal : horizontalSegments ) {
          AutoContact* contact = NULL;
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

        // Propagate constraint through vertically aligned segments.
        cdebug_log(146,0) << "Propagate constraint on vertical segments" << endl;

        for ( AutoSegment* vertical : verticalSegments ) {
          AutoContact* contact = NULL;
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
      if (autoSegment->getRpDistance() >= 2) continue;
      if (autoSegment->getRpDistance() == 1) continue;

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
