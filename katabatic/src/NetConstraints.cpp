
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2008, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./NetConstraints.cpp"                     |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <cstdlib>
#include  <sstream>

#include  "hurricane/DebugSession.h"
#include  "hurricane/BasicLayer.h"
#include  "hurricane/Net.h"
#include  "hurricane/NetExternalComponents.h"
#include  "hurricane/RoutingPad.h"
#include  "hurricane/Pad.h"
#include  "hurricane/Plug.h"
#include  "hurricane/Instance.h"
#include  "hurricane/Vertical.h"
#include  "hurricane/Horizontal.h"
#include  "hurricane/Cell.h"

#include  "katabatic/AutoContact.h"
#include  "katabatic/AutoSegment.h"
#include  "katabatic/Session.h"
#include  "katabatic/KatabaticEngine.h"


namespace {


 //! \addtogroup   NetConstraints
 //! \{

 /*! \function     void  propagateConstraintFromRp ( RoutingPad* rp )
  *  \param        rp The \c RoutingPad starting point.
  *
  *                Do a full constraint propagation starting from this \c RoutingPad.
  */

 /*! \function     void  propagateConstraint ( AutoContactStack& segmentStack, DbU::Unit constraintMin, DbU::Unit constraintMax, unsigned int direction )
  *  \param        segmentStack   A vector of \c AutoSegment.
  *  \param        constraintMin  The interval lower bound.
  *  \param        constraintMax  The interval upper bound.
  *  \param        direction      The propagation direction.
  *
  *                Propagate the constraint in the appropriate direction through the vector
  *                of \c AutoSegments and any other linked through collapse.
  */

 //! \}


  using namespace std;
  using namespace CRL;
  using namespace Hurricane;
  using namespace Katabatic;


// -----------------------------------------------------------------
// Local Functions.


  void  propagateConstraint ( AutoContactStack&  segmentStack
                            , DbU::Unit          constraintMin
                            , DbU::Unit          constraintMax
                            , unsigned int       direction
                            , set<AutoSegment*>& faileds
                            )
  {
    ltracein(99);

    while ( !segmentStack.isEmpty() ) {
      AutoContact* sourceContact     = segmentStack.getAutoContact ();
      Segment*     sourceSegment     = segmentStack.getSegment ();
      AutoSegment* sourceAutoSegment = Session::lookup ( sourceSegment );

      segmentStack.pop ();

      if ( sourceContact->isAlignate(direction) ) {
        ltrace(99) << "Apply to (source): " << (void*)sourceContact->base() << ":" << sourceContact << endl;
        if ( not sourceContact->restrictConstraintBox(constraintMin,constraintMax,direction,false) )
          faileds.insert ( sourceAutoSegment );
      }


      forEach ( Component*, icomponent, sourceContact->getSlaveComponents() ) {
        if ( *icomponent == sourceSegment ) continue;

        Segment* targetSegment = dynamic_cast<Segment*>(*icomponent);
        if ( !targetSegment ) continue;

        AutoSegment* targetAutoSegment = Session::lookup ( targetSegment );
        if ( !targetAutoSegment ) {
          const BasicLayer* basicLayer = dynamic_cast<const BasicLayer*>(targetSegment->getLayer());
          if ( basicLayer && basicLayer->getMaterial() != BasicLayer::Material::metal )
            continue;

          cerr << Error("Can't lookup <AutoSegment> for ",getString(targetSegment).c_str()) << endl;
          continue;
        }

        AutoContact* targetContact = Session::lookup 
          ( dynamic_cast<Contact*>(targetAutoSegment->getOppositeAnchor(sourceContact->base())) );

        if ( sourceAutoSegment and targetAutoSegment ) {
          unsigned int  state = AutoSegment::getPerpandicularState
            ( sourceContact
            , sourceAutoSegment
            , targetAutoSegment
            , (direction & Constant::Horizontal)?true:false
            );

          if ( !( state & (AutoSegment::PerpandicularIndirect
                          |AutoSegment::ParallelOrExpanded
                          |AutoSegment::ParallelAndLayerChange )) ) {
            segmentStack.push ( targetContact, targetSegment );

            if (   targetAutoSegment
               && (targetAutoSegment->getDirection() == direction)
               &&  targetContact->isAlignate(direction) ) {
              ltrace(99) << "Apply to (target): " << (void*)targetContact->base() << ":" << targetContact << endl;
              if ( not targetContact->restrictConstraintBox(constraintMin,constraintMax,direction,false) )
                faileds.insert ( targetAutoSegment );
            }
            continue;
          }
        }
        
      }
    }

    ltraceout(99);
    ltrace(99) << "Finished propagating." << endl;
  }


  void  propagateConstraintFromRp ( RoutingPad* rp, set<AutoSegment*>& faileds )
  {
    ltrace(99) << "propagateConstraintFromRp() - " << (void*)rp << " " << rp << endl;

    forEach ( Component*, icomponent, rp->getSlaveComponents() ) {
      ltrace(99) << "slave: " << *icomponent << endl;
      AutoContact* contact = Session::lookup ( dynamic_cast<Contact*>(*icomponent) );
      if ( contact ) {
        ltrace(99) << "Start slave: " << (void*)contact->getContact() << ":" << contact << endl;

        set<AutoContact*>  collapsedContactsSet;
        AutoContactStack   collapsedContactsStack;
        AutoContactStack   verticalSegmentsStack;
        AutoContactStack   horizontalSegmentsStack;

        collapsedContactsStack.push ( contact, NULL );
        collapsedContactsSet.insert ( contact );

        // Find all AutoContacts directly collapeds on the RoutingPad.
        while ( !collapsedContactsStack.isEmpty() ) {
          AutoContact* sourceContact = collapsedContactsStack.getAutoContact ();
          Segment*     sourceSegment = collapsedContactsStack.getSegment ();
          Segment*     segment;

          collapsedContactsStack.pop ();

          forEach ( Component*, icomponent2, sourceContact->getSlaveComponents() ) {
            bool isHorizontal = true;
            segment = dynamic_cast<Horizontal*>(*icomponent2);
            if ( !segment && (segment = dynamic_cast<Vertical*>(*icomponent2)) )
              isHorizontal = false;
            if ( !segment || (segment == sourceSegment) ) continue;

            AutoSegment* autoSegment = Session::lookup ( segment );
            if ( !autoSegment ) {
              const BasicLayer* basicLayer = dynamic_cast<const BasicLayer*>(segment->getLayer());
              if ( basicLayer && basicLayer->getMaterial() != BasicLayer::Material::metal )
                continue;
              cerr << Error("Can't lookup <AutoSegment> for %s.",getString(segment).c_str()) << endl;
              continue;
            }

            ltrace(99) << "Examining: " << autoSegment << endl;

            AutoContact* targetContact = Session::lookup
              ( dynamic_cast<Contact*>(autoSegment->getOppositeAnchor(sourceContact->base())) );

            if ( targetContact ) {
              if ( !autoSegment->isCollapsed() ) {
                if ( isHorizontal ) {
                  ltrace(99) << "On horizontal stack " << (void*)segment << ":" << segment << endl;
                  horizontalSegmentsStack.push ( targetContact, segment );
                } else {
                  ltrace(99) << "On vertical stack " << (void*)segment << ":" << segment << endl;
                  verticalSegmentsStack.push ( targetContact, segment );
                }
              } else {
                ltrace(99) << "On collapsed stack " << (void*)segment << ":" << segment << endl;
                collapsedContactsStack.push ( targetContact, segment );
                collapsedContactsSet.insert ( targetContact );
              }
            }
          }
        }

        // compute constraint on all AutoContacts collapseds,
        // then sets all the constraint Box to the intersection.
        ltrace(99) << "Computing constraint:" << endl;
        ltracein(99);
        set<AutoContact*>::iterator  it  = collapsedContactsSet.begin();
        set<AutoContact*>::iterator  end = collapsedContactsSet.end();
        ltrace(99) << *it << " " << (*it)->getConstraintBox() << endl;
        Box  constraintBox = (*it++)->getConstraintBox ();
        for ( ; it != end ; it++ ) {
          (*it)->intersectConstraintBox ( constraintBox );
          ltrace(99) << *it << " " << constraintBox << endl;
        }

        if ( constraintBox.isEmpty() ) {
          cerr << "[ERROR] incompatible segment collapse for " << rp << endl;
          ltraceout(99);
          break;
        }
        ltraceout(99);

        ltrace(99) << "Applying constraint " << constraintBox << " to:" << endl;
        ltracein(99);
        it  = collapsedContactsSet.begin();
        for ( ; it != end ; it++ ) {
          ltrace(99) << *it << endl;
          (*it)->setConstraintBox ( constraintBox );
        }
        ltraceout(99);

        // Propagate constraint through horizontally bound segments.
        ltrace(99) << "Propagate constraint on horizontal segments" << endl;
        propagateConstraint ( horizontalSegmentsStack
                            , constraintBox.getYMin()
                            , constraintBox.getYMax()
                            , Constant::Horizontal
                            , faileds );

        // Propagate constraint through vertically bound segments.
        ltrace(99) << "Propagate constraint on vertical segments" << endl;
        propagateConstraint ( verticalSegmentsStack
                            , constraintBox.getXMin()
                            , constraintBox.getXMax()
                            , Constant::Vertical
                            , faileds );
      }
    }

    ltrace(99) << "propagateConstraintFromRp() - Exit" << endl;
  }


} // End of local namespace.




namespace Katabatic {


  using Hurricane::Cell;


  void  KatabaticEngine::_computeNetConstraints ( Net* net, set<AutoSegment*>& faileds )
  {
    DebugSession::open ( net );

    ltrace(100) << "Katabatic::_computeNetConstraints ( " << net << " )" << endl;
    ltracein(99);

  //cmess2 << "     - " << net << endl;

    vector<RoutingPad*> routingPads;
    forEach ( Component*, icomponent, net->getComponents() ) {
      Contact* contact = dynamic_cast<Contact*>(*icomponent);
      if ( contact ) {
        AutoContact* autoContact = Session::lookup ( contact );
        if ( autoContact ) 
          autoContact->restoreNativeConstraintBox ();
      } else {
        RoutingPad* routingPad = dynamic_cast<RoutingPad*>(*icomponent);
        if ( routingPad ) routingPads.push_back ( routingPad );
      }
    }

    for ( size_t i=0 ; i<routingPads.size() ; i++ )
      propagateConstraintFromRp ( routingPads[i], faileds );

    set<AutoSegment*> processeds;
    forEach ( Segment*, isegment, net->getSegments() ) {
      AutoSegment* autoSegment = Session::lookup ( *isegment );
      if ( !autoSegment ) continue;
      autoSegment->toConstraintAxis ( &processeds );
    }

    ltraceout(99);

    DebugSession::close ();
  }


  void  KatabaticEngine::_collapseNets ( Nets nets, unsigned int depth )
  {
    forEach ( Net*, inet, nets )
      _collapseNet ( *inet, depth );
  }


  void  KatabaticEngine::_collapseNet ( const Name& name, unsigned int depth )
  {
    Net* net = getCell()->getNet ( name );

    if ( !net ) {
      cerr << Error("No net %s in %s"
                   ,getString(name).c_str()
                   ,getString(getCell()).c_str()) << endl;
      return;
    }

    _collapseNet ( net );
  }


  void  KatabaticEngine::_collapseNet ( Net* net, unsigned int depth )
  {
    ltrace(100) << "Katabatic::_collapseNet ( " << net << " )" << endl;
    ltracein(99);

    forEach ( Component*, icomponent, net->getComponents() ) {
      RoutingPad* routingPad = dynamic_cast<RoutingPad*>(*icomponent);
      if ( routingPad ) {
        ltrace(99) << "RoutingPad " << routingPad << endl;
        forEach ( Component*, islaveComponent, routingPad->getSlaveComponents() ) {
          ltrace(99) << "SlaveComponent " << *islaveComponent << endl;
          Segment* segment = dynamic_cast<Segment*>(*islaveComponent);
          if ( segment ) {
            AutoSegment* autoSegment = Session::lookup ( segment );
            if ( !autoSegment ) continue;

            if ( autoSegment->isLocal() ) {
              ltrace(99) << "Collapsing " << autoSegment << endl;
              autoSegment->collapse ();
            }
          }
        } // islaveComponent.
      }
    } // icomponent.

    ltraceout(99);
  }


} // End of Katabatic namespace.
