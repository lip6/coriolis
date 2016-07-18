
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
    cdebug_log(145,1) << "propagateConstraintFromRp() - " << rp << endl;

    forEach ( Component*, icomponent, rp->getSlaveComponents() ) {
      cdebug_log(145,0) << "slave component: " << *icomponent << endl;
      AutoContact* sourceContact = Session::lookup( dynamic_cast<Contact*>(*icomponent) );
      if (sourceContact) {
        cdebug_log(145,0) << "Start slave: " << sourceContact << endl;

        set<AutoSegment*>  verticalSegments;
        set<AutoSegment*>  horizontalSegments;

        forEach ( AutoSegment*, isegment, sourceContact->getAutoSegments() ) {
          cdebug_log(145,0) << "Examining: " << (*isegment) << endl;
          AutoContact* targetContact = isegment->getOppositeAnchor(sourceContact);

          if (targetContact) {
            if (isegment->isHorizontal()) {
              cdebug_log(145,0) << "On horizontal stack " << (*isegment) << endl;
              horizontalSegments.insert( (*isegment) );
            } else {
              cdebug_log(145,0) << "On vertical stack " << (*isegment) << endl;
              verticalSegments.insert( (*isegment) );
            }
          }
        }

        Box  constraintBox = sourceContact->getConstraintBox();

        // Propagate constraint through horizontally aligned segments.
        cdebug_log(145,0) << "Propagate constraint on horizontal segments" << endl;

        set<AutoSegment*>::iterator ihorizontal = horizontalSegments.begin();
        for ( ; ihorizontal != horizontalSegments.end() ; ++ihorizontal ) {
          AutoContact* contact = NULL;
          forEach ( AutoSegment*, ialigned, (*ihorizontal)->getAligneds() ) {
            contact = ialigned->getAutoTarget();
            cdebug_log(145,0) << "contact: " << contact << endl;
            if (contact) {
              cdebug_log(145,0) << "Apply to (target): " << contact << endl;
              contact->restrictConstraintBox( constraintBox.getYMin()
                                            , constraintBox.getYMax()
                                            , Flags::Horizontal|Flags::WarnOnError );
            }
            contact = ialigned->getAutoSource();
            cdebug_log(145,0) << "contact: " << contact << endl;
            if (contact) {
              cdebug_log(145,0) << "Apply to (source): " << contact << endl;
              contact->restrictConstraintBox( constraintBox.getYMin()
                                            , constraintBox.getYMax()
                                            , Flags::Horizontal|Flags::WarnOnError );
            }
          }
        } 

        // Propagate constraint through vertically aligned segments.
        cdebug_log(145,0) << "Propagate constraint on vertical segments" << endl;

        set<AutoSegment*>::iterator ivertical = verticalSegments.begin();
        for ( ; ivertical != verticalSegments.end() ; ++ivertical ) {
          AutoContact* contact = NULL;
          forEach ( AutoSegment*, ialigned, (*ivertical)->getAligneds() ) {
            contact = ialigned->getAutoTarget();
            if (contact) {
              cdebug_log(145,0) << "Apply to (target): " << contact << endl;
              contact->restrictConstraintBox( constraintBox.getXMin()
                                            , constraintBox.getXMax()
                                            , Flags::Vertical|Flags::WarnOnError );
            }
            contact = ialigned->getAutoSource();
            if (contact) {
              cdebug_log(145,0) << "Apply to (source): " << contact << endl;
              contact->restrictConstraintBox( constraintBox.getXMin()
                                            , constraintBox.getXMax()
                                            , Flags::Vertical|Flags::WarnOnError );
            }
          } 
        }
      }
    }

    cdebug_log(145,0) << "propagateConstraintFromRp() - Exit" << endl;
    cdebug_tabw(145,-1);
  }


}  // Anonymous namespace.



namespace Anabatic {


  using Hurricane::Cell;


  void  AnabaticEngine::computeNetConstraints ( Net* net )
  {
    DebugSession::open( net, 140, 150);

    cdebug_log(149,0) << "Anabatic::computeNetConstraints( " << net << " )" << endl;
    cdebug_tabw(145,1);

    vector<RoutingPad*> routingPads;
    forEach ( Component*, icomponent, net->getComponents() ) {
      Contact* contact = dynamic_cast<Contact*>( *icomponent );
      if (contact) {
        AutoContact* autoContact = Session::lookup( contact );
        if (autoContact) 
          autoContact->restoreNativeConstraintBox();
      } else {
        RoutingPad* routingPad = dynamic_cast<RoutingPad*>( *icomponent );
        if (routingPad) routingPads.push_back( routingPad );
      }
    }

    for ( size_t i=0 ; i<routingPads.size() ; i++ )
      propagateConstraintFromRp( routingPads[i] );

    // forEach ( Segment*, isegment, net->getSegments() ) {
    //   AutoSegment* autoSegment = Session::lookup( *isegment );
    //   if (autoSegment) autoSegment->toConstraintAxis();
    // }

    cdebug_tabw(145,-1);
    DebugSession::close();
  }


}  // Anabatic namespace.
