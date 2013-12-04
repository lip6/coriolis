
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2013, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
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
#include "katabatic/AutoContact.h"
#include "katabatic/AutoSegment.h"
#include "katabatic/Session.h"
#include "katabatic/KatabaticEngine.h"


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
  using namespace Katabatic;


// -----------------------------------------------------------------
// Local Functions.


  void  propagateConstraintFromRp ( RoutingPad* rp )
  {
    ltrace(99) << "propagateConstraintFromRp() - " << rp << endl;
    ltracein(99);

    forEach ( Component*, icomponent, rp->getSlaveComponents() ) {
      ltrace(99) << "slave component: " << *icomponent << endl;
      AutoContact* sourceContact = Session::lookup( dynamic_cast<Contact*>(*icomponent) );
      if (sourceContact) {
        ltrace(99) << "Start slave: " << sourceContact << endl;

        set<AutoSegment*>  verticalSegments;
        set<AutoSegment*>  horizontalSegments;

        forEach ( AutoSegment*, isegment, sourceContact->getAutoSegments() ) {
          ltrace(99) << "Examining: " << (*isegment) << endl;
          AutoContact* targetContact = isegment->getOppositeAnchor(sourceContact);

          if (targetContact) {
            if (isegment->isHorizontal()) {
              ltrace(99) << "On horizontal stack " << (*isegment) << endl;
              horizontalSegments.insert( (*isegment) );
            } else {
              ltrace(99) << "On vertical stack " << (*isegment) << endl;
              verticalSegments.insert( (*isegment) );
            }
          }
        }

        Box  constraintBox = sourceContact->getConstraintBox();

        // Propagate constraint through horizontally aligned segments.
        ltrace(99) << "Propagate constraint on horizontal segments" << endl;

        set<AutoSegment*>::iterator ihorizontal = horizontalSegments.begin();
        for ( ; ihorizontal != horizontalSegments.end() ; ++ihorizontal ) {
          AutoContact* contact = NULL;
          forEach ( AutoSegment*, ialigned, (*ihorizontal)->getAligneds() ) {
            contact = ialigned->getAutoTarget();
            ltrace(99) << "contact: " << contact << endl;
            if (contact) {
              ltrace(99) << "Apply to (target): " << contact << endl;
              contact->restrictConstraintBox( constraintBox.getYMin()
                                            , constraintBox.getYMax()
                                            , KbHorizontal );
            }
            contact = ialigned->getAutoSource();
            ltrace(99) << "contact: " << contact << endl;
            if (contact) {
              ltrace(99) << "Apply to (source): " << contact << endl;
              contact->restrictConstraintBox( constraintBox.getYMin()
                                            , constraintBox.getYMax()
                                            , KbHorizontal );
            }
          }
        } 

        // Propagate constraint through vertically aligned segments.
        ltrace(99) << "Propagate constraint on vertical segments" << endl;

        set<AutoSegment*>::iterator ivertical = verticalSegments.begin();
        for ( ; ivertical != verticalSegments.end() ; ++ivertical ) {
          AutoContact* contact = NULL;
          forEach ( AutoSegment*, ialigned, (*ivertical)->getAligneds() ) {
            contact = ialigned->getAutoTarget();
            if (contact) {
              ltrace(99) << "Apply to (target): " << contact << endl;
              contact->restrictConstraintBox( constraintBox.getXMin()
                                            , constraintBox.getXMax()
                                            , KbVertical );
            }
            contact = ialigned->getAutoSource();
            if (contact) {
              ltrace(99) << "Apply to (source): " << contact << endl;
              contact->restrictConstraintBox( constraintBox.getXMin()
                                            , constraintBox.getXMax()
                                            , KbVertical );
            }
          } 
        }
      }
    }

    ltrace(99) << "propagateConstraintFromRp() - Exit" << endl;
    ltraceout(99);
  }


}  // Anonymous namespace.



namespace Katabatic {


  using Hurricane::Cell;


  void  KatabaticEngine::computeNetConstraints ( Net* net )
  {
    DebugSession::open( net );

    ltrace(100) << "Katabatic::computeNetConstraints( " << net << " )" << endl;
    ltracein(99);

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

    ltraceout(99);
    DebugSession::close();
  }


}  // Katabatic namespace.
