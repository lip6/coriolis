
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
    cdebug.log(145,1) << "propagateConstraintFromRp() - " << rp << endl;

    forEach ( Component*, icomponent, rp->getSlaveComponents() ) {
      cdebug.log(145) << "slave component: " << *icomponent << endl;
      AutoContact* sourceContact = Session::lookup( dynamic_cast<Contact*>(*icomponent) );
      if (sourceContact) {
        cdebug.log(145) << "Start slave: " << sourceContact << endl;

        set<AutoSegment*>  verticalSegments;
        set<AutoSegment*>  horizontalSegments;

        forEach ( AutoSegment*, isegment, sourceContact->getAutoSegments() ) {
          cdebug.log(145) << "Examining: " << (*isegment) << endl;
          AutoContact* targetContact = isegment->getOppositeAnchor(sourceContact);

          if (targetContact) {
            if (isegment->isHorizontal()) {
              cdebug.log(145) << "On horizontal stack " << (*isegment) << endl;
              horizontalSegments.insert( (*isegment) );
            } else {
              cdebug.log(145) << "On vertical stack " << (*isegment) << endl;
              verticalSegments.insert( (*isegment) );
            }
          }
        }

        Box  constraintBox = sourceContact->getConstraintBox();

        // Propagate constraint through horizontally aligned segments.
        cdebug.log(145) << "Propagate constraint on horizontal segments" << endl;

        set<AutoSegment*>::iterator ihorizontal = horizontalSegments.begin();
        for ( ; ihorizontal != horizontalSegments.end() ; ++ihorizontal ) {
          AutoContact* contact = NULL;
          forEach ( AutoSegment*, ialigned, (*ihorizontal)->getAligneds() ) {
            contact = ialigned->getAutoTarget();
            cdebug.log(145) << "contact: " << contact << endl;
            if (contact) {
              cdebug.log(145) << "Apply to (target): " << contact << endl;
              contact->restrictConstraintBox( constraintBox.getYMin()
                                            , constraintBox.getYMax()
                                            , KbHorizontal|KbWarnOnError );
            }
            contact = ialigned->getAutoSource();
            cdebug.log(145) << "contact: " << contact << endl;
            if (contact) {
              cdebug.log(145) << "Apply to (source): " << contact << endl;
              contact->restrictConstraintBox( constraintBox.getYMin()
                                            , constraintBox.getYMax()
                                            , KbHorizontal|KbWarnOnError );
            }
          }
        } 

        // Propagate constraint through vertically aligned segments.
        cdebug.log(145) << "Propagate constraint on vertical segments" << endl;

        set<AutoSegment*>::iterator ivertical = verticalSegments.begin();
        for ( ; ivertical != verticalSegments.end() ; ++ivertical ) {
          AutoContact* contact = NULL;
          forEach ( AutoSegment*, ialigned, (*ivertical)->getAligneds() ) {
            contact = ialigned->getAutoTarget();
            if (contact) {
              cdebug.log(145) << "Apply to (target): " << contact << endl;
              contact->restrictConstraintBox( constraintBox.getXMin()
                                            , constraintBox.getXMax()
                                            , KbVertical|KbWarnOnError );
            }
            contact = ialigned->getAutoSource();
            if (contact) {
              cdebug.log(145) << "Apply to (source): " << contact << endl;
              contact->restrictConstraintBox( constraintBox.getXMin()
                                            , constraintBox.getXMax()
                                            , KbVertical|KbWarnOnError );
            }
          } 
        }
      }
    }

    cdebug.log(145) << "propagateConstraintFromRp() - Exit" << endl;
    cdebug.tabw(145,-1);
  }


}  // Anonymous namespace.



namespace Katabatic {


  using Hurricane::Cell;


  void  KatabaticEngine::computeNetConstraints ( Net* net )
  {
    DebugSession::open( net, 140, 150);

    cdebug.log(149) << "Katabatic::computeNetConstraints( " << net << " )" << endl;
    cdebug.tabw(145,1);

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

    cdebug.tabw(145,-1);
    DebugSession::close();
  }


}  // Katabatic namespace.
