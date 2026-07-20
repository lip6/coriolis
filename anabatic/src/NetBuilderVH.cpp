// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2018-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |         A n a b a t i c  -  Routing Toolbox                     |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./NetBuilderVH.cpp"                       |
// +-----------------------------------------------------------------+


#include <cstdlib>
#include <sstream>
#include "hurricane/Bug.h"
#include "hurricane/Breakpoint.h"
#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/DebugSession.h"
#include "hurricane/Layer.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/RegularLayer.h"
#include "hurricane/Technology.h"
#include "hurricane/DataBase.h"
#include "hurricane/Net.h"
#include "hurricane/NetExternalComponents.h"
#include "hurricane/NetRoutingProperty.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/RoutingPads.h"
#include "hurricane/Pad.h"
#include "hurricane/Pin.h"
#include "hurricane/Plug.h"
#include "hurricane/Cell.h"
#include "hurricane/Instance.h"
#include "hurricane/Vertical.h"
#include "hurricane/Horizontal.h"
#include "crlcore/AllianceFramework.h"
#include "crlcore/RoutingGauge.h"
#include "crlcore/Measures.h"
#include "anabatic/AutoContactTerminal.h"
#include "anabatic/AutoContactTurn.h"
#include "anabatic/AutoContactHTee.h"
#include "anabatic/AutoContactVTee.h"
#include "anabatic/AutoSegment.h"
#include "anabatic/NetBuilderVH.h"
#include "anabatic/AnabaticEngine.h"


namespace Anabatic {

  using std::swap;
  using Hurricane::Transformation;
  using Hurricane::Warning;
  using Hurricane::Error;
  using Hurricane::Pin;
  using Constant::LayerGaugeType;

  
  NetBuilderVH::NetBuilderVH ()
    : NetBuilder()
  { }


  NetBuilderVH::~NetBuilderVH () { }


  string  NetBuilderVH::getStyle ()
  { return "VH,3RL+"; }

  
  void  NetBuilderVH::doRp_AutoContacts ( GCell*        gcell
                                        , RoutingPad*   rp
                                        , AutoContact*& source
                                        , AutoContact*& target
                                        , uint64_t      flags
                                        )
  {
    cdebug_log(145,1) << getTypeName() << "::doRp_AutoContacts()" << endl;
    cdebug_log(145,0)   << rp << endl;

    source = target = NULL;

    Point        sourcePosition;
    Point        targetPosition;
    const Layer* rpLayer        = rp->getLayer();
    size_t       rpDepth        = Session::getLayerDepth( rp->getLayer() );
    Flags        direction      = Session::getDirection ( rpDepth );
    DbU::Unit    viaSide        = Session::getViaWidth  ( rpDepth );

    Session::getPositions( rp, sourcePosition, targetPosition );

    if (sourcePosition.getX() > targetPosition.getX()) swap( sourcePosition, targetPosition );
    if (sourcePosition.getY() > targetPosition.getY()) swap( sourcePosition, targetPosition );

    GCell* sourceGCell = Session::getAnabatic()->getGCellUnder( sourcePosition );
    GCell* targetGCell = Session::getAnabatic()->getGCellUnder( targetPosition );

    if (rpDepth == 0) {
      rpLayer   = Session::getContactLayer(0);
      direction = Flags::Vertical;
      viaSide   = Session::getViaWidth( rpDepth );

      RoutingPad* rrp = dynamic_cast<RoutingPad*>( rp );
      if (rrp) {
        // if (not getAnabatic()->getConfiguration()->selectRpComponent(rrp)) {
        //   cerr << Warning( "%s::doRp_AutoContacts(): %s has no components on grid."
        //                  , getTypeName().c_str()
        //                  , getString(rp).c_str() ) << endl;
        // }
      } else {
        cerr << Warning( "%s::doRp_AutoContacts(): %s is *not* a RoutingPad."
                       , getTypeName().c_str()
                       , getString(rp).c_str() ) << endl;
      }
    }

  // Non-M1 terminal or punctual M1 protections.
    if ( (rpDepth != 0) or (sourcePosition == targetPosition) ) {
      map<Component*,AutoSegment*>::iterator irp = getRpLookup().find( rp );
      if (irp == getRpLookup().end()) {
        AutoContact* sourceProtect = AutoContactTerminal::create( sourceGCell
                                                                , rp
                                                                , rpLayer
                                                                , sourcePosition
                                                                , viaSide, viaSide
                                                                );
        AutoContact* targetProtect = AutoContactTerminal::create( targetGCell
                                                                , rp
                                                                , rpLayer
                                                                , targetPosition
                                                                , viaSide, viaSide
                                                                );
        sourceProtect->setFlags( CntFixed );
        targetProtect->setFlags( CntFixed );

        AutoSegment* segment = AutoSegment::create( sourceProtect, targetProtect, direction );
        segment->setFlags( AutoSegment::SegFixed );

        getRpLookup().insert( make_pair(rp,segment) );
      }
    }

    if (sourcePosition != targetPosition) {
      if (flags & DoSourceContact)
        source = AutoContactTerminal::create( sourceGCell
                                            , rp
                                            , rpLayer
                                            , sourcePosition
                                            , viaSide, viaSide
                                            );
      if (flags & DoTargetContact)
        target = AutoContactTerminal::create( targetGCell
                                            , rp
                                            , rpLayer
                                            , targetPosition
                                            , viaSide, viaSide
                                            );
    }

    if ( (rpDepth > 0) and not Session::getRoutingGauge()->isSuperPitched() ) {
      rpLayer = Session::getContactLayer( rpDepth );
    }

    if (not source and not target) {
      source = target = AutoContactTerminal::create( gcell
                                                   , rp
                                                   , rpLayer
                                                   , rp->getCenter()
                                                   , viaSide, viaSide
                                                   );
    }

    cdebug_tabw(145,-1);
    return;
  }


  AutoContact* NetBuilderVH::doRp_Access ( GCell* gcell, RoutingPad* rp, uint64_t flags )
  {
    cdebug_log(145,1) << getTypeName() << "::doRp_Access() - flags:" << flags << endl;

    AutoSegment*       segment         = nullptr;
    AutoContact*       rpContactSource = nullptr;
    AutoContact*       rpContactTarget = nullptr;
    AutoContact*       subContact1     = nullptr;
    const Layer*       rpLayer         = rp->getLayer();
    size_t             rpDepth         = Session::getLayerDepth( rpLayer );
    RoutingLayerGauge* rlg             = Session::getLayerGauge( rpLayer );
    uint64_t           offgridFlag     = 0;

    flags |= checkRoutingPadSize( rp );
    doRp_AutoContacts( gcell, rp, rpContactSource, rpContactTarget, flags );
    const Layer* viaLayer1 = Session::getBuildContactLayer( 1 );

    if (rp->isM1Offgrid()) {
      cerr << Warning( "NetNuilderVH::doRpAccess(): Offgrid %s.", getString(rp).c_str() ) << endl;
    }

    if (flags & (HAccess|HAccessEW)) {
      subContact1 = AutoContactTurn::create( gcell, rp->getNet(), viaLayer1 );
      AutoSegment::create( rpContactSource, subContact1, Flags::Vertical );
      rpContactSource = subContact1;

      subContact1 = AutoContactTurn::create( gcell, rp->getNet(), viaLayer1 );
      AutoSegment::create( rpContactSource, subContact1, Flags::Horizontal|Flags::UseNonPref );
      rpContactSource = subContact1;

      if (flags & HAccessEW) {
        cdebug_log(145,0) << "case HAccessEW" << endl;
        subContact1 = AutoContactHTee::create( gcell, rp->getNet(), viaLayer1 );
      } else {
        cdebug_log(145,0) << "case HAccess" << endl;
        subContact1 = AutoContactTurn::create( gcell, rp->getNet(), viaLayer1 );
      }
      AutoSegment::create( rpContactSource, subContact1, Flags::Vertical );
      rpContactSource = subContact1;
    } else {
      if (rp->isPunctual()) {
        cdebug_log(145,0) << "case rp->isPunctual()" << endl;
      
        subContact1 = AutoContactTurn::create( gcell, rp->getNet(), viaLayer1 );
        AutoSegment::create( rpContactSource, subContact1, Flags::Vertical );
        rpContactSource = subContact1;
        if (flags & VAccessNS)
          subContact1 = AutoContactVTee::create( gcell, rp->getNet(), viaLayer1 );
        else
          subContact1 = AutoContactTurn::create( gcell, rp->getNet(), viaLayer1 );
        AutoSegment::create( rpContactSource, subContact1, Flags::Horizontal );
      } else if (rp->isM1Offgrid() or rp->isHSmall()) {
        cdebug_log(145,0) << "case rp->isM1Offgrid() of rp->isHSmall()" << endl;
      
        subContact1 = AutoContactTurn::create( gcell, rp->getNet(), viaLayer1 );
        AutoSegment::create( rpContactSource, subContact1, Flags::Vertical );
        rpContactSource = subContact1;

        subContact1 = AutoContactTurn::create( gcell, rp->getNet(), viaLayer1 );
        AutoSegment::create( rpContactSource, subContact1, Flags::Horizontal|Flags::UseNonPref );

        rpContactSource = subContact1;
        subContact1 = AutoContactTurn::create( gcell, rp->getNet(), viaLayer1 );
        AutoSegment::create( rpContactSource, subContact1, Flags::Vertical );

        rpContactSource = subContact1;
        subContact1 = AutoContactTurn::create( gcell, rp->getNet(), viaLayer1 );
        AutoSegment::create( rpContactSource, subContact1, Flags::Horizontal );

        if (flags & VAccessNS) {
          rpContactSource = subContact1;
          subContact1 = AutoContactTurn::create( gcell, rp->getNet(), viaLayer1 );
          AutoSegment::create( rpContactSource, subContact1, Flags::Vertical );

          rpContactSource = subContact1;
          subContact1 = AutoContactVTee::create( gcell, rp->getNet(), viaLayer1 );
          AutoSegment::create( rpContactSource, subContact1, Flags::Horizontal );
        }
      } else {
        if (rp->isVSmall()) {
          cdebug_log(145,0) << "case default/VSmall" << endl;
      
          subContact1 = AutoContactTurn::create( gcell, rp->getNet(), viaLayer1 );
          AutoSegment::create( rpContactSource, subContact1, Flags::Vertical );
          rpContactSource = subContact1;
      
          if (flags & VAccessNS)
            subContact1 = AutoContactVTee::create( gcell, rp->getNet(), viaLayer1 );
          else
            subContact1 = AutoContactTurn::create( gcell, rp->getNet(), viaLayer1 );
          AutoSegment::create( rpContactSource, subContact1, Flags::Horizontal|Flags::UseNonPref );
        } else {
          cdebug_log(145,0) << "case default" << endl;
          subContact1 = rpContactSource;
        }
      }
    }

    cdebug_tabw(145,-1);

    return subContact1;
  }


  AutoContact* NetBuilderVH::doRp_AccessNorthSouthPin ( GCell* gcell, RoutingPad* rp )
  {
    cdebug_log(145,1) << getTypeName() << "::doRp_AccessNorthSouthPin() " << rp << endl;

    AutoContact* rpSourceContact = NULL;
    AutoContact* rpContactTarget = NULL;
    AutoContact* turn            = NULL;

    doRp_AutoContacts( gcell, rp, rpSourceContact, rpContactTarget, NoProtect );

    Net* net = rp->getNet();
    Pin* pin = dynamic_cast<Pin*>( rp->getOccurrence().getEntity() );
    Pin::AccessDirection pinDir = pin->getAccessDirection();
    if (pinDir == Pin::AccessDirection::NORTH) {
      turn = AutoContactTurn::create( gcell, net, Session::getBuildRoutingLayer(1) );
      AutoSegment* segment = AutoSegment::create( rpSourceContact, turn, Flags::Vertical );
      segment->setAxis( rp->getX(), Flags::Force );
      segment->setFlags( AutoSegment::SegFixed|AutoSegment::SegFixedAxis );
      rpSourceContact = turn;

      turn    = AutoContactTurn::create( gcell, net, Session::getBuildContactLayer(1) );
      segment = AutoSegment::create( rpSourceContact, turn, Flags::Horizontal );
      rpSourceContact = turn;

      DbU::Unit axis = gcell->getYMax() - Session::getDHorizontalPitch();
      cdebug_log(145,0) << "axis:" << DbU::getValueString(axis) << endl;
      
      segment->setAxis( axis, Flags::Force );
    //segment->setFlags( AutoSegment::SegFixed|AutoSegment::SegFixedAxis );
      cdebug_log(145,0) << segment << endl;
    } else {
      turn = rpSourceContact;
    }

    cdebug_tabw(145,-1);
    return turn;
  }


  AutoContact* NetBuilderVH::doRp_AccessEastWestPin ( GCell* gcell, RoutingPad* rp )
  {
    cdebug_log(145,1) << getTypeName() << "::doRp_AccessEastWestPin() " << rp << endl;

    Net*         net             = rp->getNet();
    Pin*         pin             = dynamic_cast<Pin*>( rp->getOccurrence().getEntity() );
    Pin::AccessDirection pinDir  = pin->getAccessDirection();
    AutoContact* rpSourceContact = NULL;
    AutoContact* rpContactTarget = NULL;
    AutoContact* turn            = NULL;
    AutoSegment* segment         = NULL;

    doRp_AutoContacts( gcell, rp, rpSourceContact, rpContactTarget, NoProtect );

    turn    = AutoContactTurn::create( gcell, net, Session::getBuildContactLayer(1) );
    segment = AutoSegment::create( rpSourceContact, turn, Flags::Horizontal );
    segment->setAxis( pin->getCenter().getY(), Flags::Force );

    rpSourceContact = turn;
    turn    = AutoContactTurn::create( gcell, net, Session::getBuildContactLayer(1) );
    segment = AutoSegment::create( rpSourceContact, turn, Flags::Vertical );

    DbU::Unit axis = 0;
    if (pinDir == Pin::AccessDirection::EAST)
      axis = gcell->getXMax() - Session::getDVerticalPitch();
    else
      axis = gcell->getXMin() + Session::getDVerticalPitch();
    segment->setAxis( axis );

    cdebug_tabw(145,-1);
    return turn;
  }


  bool  NetBuilderVH::_do_1G_1PinM2 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_1G_1PinM2() [Managed Configuration - Optimized] " << getTopology() << endl;

    Pin* pin = dynamic_cast<Pin*>( getRoutingPads()[0]->getOccurrence().getEntity() );
    if (   (pin->getAccessDirection() != Pin::AccessDirection::SOUTH)
       and (pin->getAccessDirection() != Pin::AccessDirection::NORTH) ) {
      cerr << Error( "%s::do_1G_1PinM2(): %s *must* be north or south."
                   , getTypeName().c_str()
                   , getString(pin).c_str() ) << endl;
    }

    uint64_t  flags = NoFlags;
    if      (east ()) { setEastContact ( doRp_Access(getGCell(),getRoutingPads()[0], HAccess|VSmall )); }
    else if (west ()) { setWestContact ( doRp_Access(getGCell(),getRoutingPads()[0], HAccess|VSmall )); }
    else if (north()) { setNorthContact( doRp_Access(getGCell(),getRoutingPads()[0], NoFlags )); }
    else if (south()) { setSouthContact( doRp_Access(getGCell(),getRoutingPads()[0], NoFlags )); }

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderVH::_do_xG_1PinM2 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_xG_1PinM2() [Managed Configuration] " << getTopology() << endl;

    Pin* pin = dynamic_cast<Pin*>( getRoutingPads()[0]->getOccurrence().getEntity() );
    if (   (pin->getAccessDirection() != Pin::AccessDirection::SOUTH)
       and (pin->getAccessDirection() != Pin::AccessDirection::NORTH) ) {
      cerr << Error( "%s::do_xG_1PinM2(): %s *must* be north or south."
                   , getTypeName().c_str()
                   , getString(pin).c_str() ) << endl;
    }

    RoutingPad*  rp         = getRoutingPads()[0];
    AutoContact* pinContact = NULL;
    doRp_AutoContacts( getGCell(), rp, pinContact, pinContact, HSmall|VSmall );

    if (not north() and not south()) {
      AutoContact* subContact1 = AutoContactHTee::create( getGCell(), rp->getNet(), Session::getContactLayer(1) );
      AutoSegment::create( pinContact, subContact1, Flags::Vertical );
      setBothCornerContacts( subContact1 );
    } else {
      AutoContact* subContact1 = AutoContactVTee::create( getGCell(), rp->getNet(), Session::getContactLayer(1) );
      AutoSegment::create( pinContact, subContact1, Flags::Vertical );
      
      if (east() and west()) {
        AutoContact* subContact2 = AutoContactVTee::create( getGCell(), rp->getNet(), Session::getContactLayer(1) );
        AutoSegment::create( subContact1, subContact2, Flags::Vertical );
      
        setSouthWestContact( (south()) ? subContact1 : subContact2 );
        setNorthEastContact( (south()) ? subContact2 : subContact1 );
      } else
        setBothCornerContacts( subContact1 );
    }

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderVH::_do_1G_xM1_1PinM2 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_1G_xM1_1PinM2() [Managed Configuration - Optimized] " << getTopology() << endl;

    sortRpByX( getRoutingPads(), NoFlags ); // increasing X.

    vector<RoutingPad*> rpsM1;
    RoutingPad*         pinM2 = nullptr;
    for ( RoutingPad* rp : getRoutingPads() ) {
      if (dynamic_cast<Pin*>(rp->getOccurrence().getEntity())) pinM2 = rp;
      else rpsM1.push_back( rp );
    }

    Pin* pin = dynamic_cast<Pin*>( pinM2->getOccurrence().getEntity() );
    if (   (pin->getAccessDirection() != Pin::AccessDirection::SOUTH)
       and (pin->getAccessDirection() != Pin::AccessDirection::NORTH) ) {
      cerr << Error( "%s::do_1G_xM1_1PinM2(): %s *must* be north or south."
                   , getTypeName().c_str()
                   , getString(pin).c_str() ) << endl;
    }

    
    for ( size_t i=1 ; i<rpsM1.size() ; ++i ) {
      AutoContact* leftContact  = doRp_Access( getGCell(), rpsM1[i-1], HAccess );
      AutoContact* rightContact = doRp_Access( getGCell(), rpsM1[i  ], HAccess );
      AutoSegment::create( leftContact, rightContact, Flags::Horizontal );
    }

    AutoContact* pinM2Contact = nullptr;
    AutoContact* rpM1Contact  = nullptr;
    doRp_AutoContacts( getGCell(), pinM2, pinM2Contact, rpM1Contact, NoProtect );
    rpM1Contact = doRp_Access( getGCell(), rpsM1[0], HAccess );

    AutoContact* htee  = AutoContactHTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
    AutoSegment::create( pinM2Contact, htee, Flags::Vertical   );
    AutoSegment::create( rpM1Contact , htee, Flags::Horizontal );

    if (north() or south()) {
      AutoContact* turn1 = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
      AutoSegment::create( htee, turn1, Flags::Horizontal );

      if (north()) setNorthContact( turn1 );
      else         setSouthContact( turn1 );
    } else {
      if (west()) setWestContact( htee );
      else        setEastContact( htee );
    }

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderVH::_do_1G_1PinM3 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_1G_1PinM3() [Managed Configuration - Optimized] " << getTopology() << endl;

    Pin* pin = dynamic_cast<Pin*>( getRoutingPads()[0]->getOccurrence().getEntity() );
    if (   (pin->getAccessDirection() != Pin::AccessDirection::EAST)
       and (pin->getAccessDirection() != Pin::AccessDirection::WEST) ) {
      cerr << Error( "%s::do_1G_1PinM2(): %s *must* be east or west."
                   , getTypeName().c_str()
                   , getString(pin).c_str() ) << endl;
    }

    AutoContact* pinContact = NULL;
    doRp_AutoContacts( getGCell(), getRoutingPads()[0], pinContact, pinContact, HSmall|VSmall );

    if (east() or west()) {
      AutoContact* subContact1 = AutoContactTurn::create( getGCell(), getRoutingPads()[0]->getNet(), Session::getContactLayer(1) );
      AutoContact* subContact2 = AutoContactTurn::create( getGCell(), getRoutingPads()[0]->getNet(), Session::getContactLayer(1) );

      AutoSegment::create( pinContact , subContact1, Flags::Horizontal  );
      AutoSegment::create( subContact1, subContact2, Flags::Vertical );
      pinContact = subContact2;
    } else {
      AutoContact* subContact1 = NULL;
      subContact1 = AutoContactTurn::create( getGCell(), getRoutingPads()[0]->getNet(), Session::getContactLayer(1) );

      AutoSegment::create( pinContact, subContact1, Flags::Horizontal );
      pinContact = subContact1;
    }

    if      (west ()) setWestContact ( pinContact );
    else if (east ()) setEastContact ( pinContact );
    else if (north()) setNorthContact( pinContact );
    else if (south()) setSouthContact( pinContact );

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderVH::_do_xG_1PinM3 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_xG_1PinM3() [Managed Configuration] " << getTopology() << endl;

    RoutingPad* rp  = getRoutingPads()[0];
    Pin*        pin = dynamic_cast<Pin*>( rp->getOccurrence().getEntity() );
    if (   (pin->getAccessDirection() != Pin::AccessDirection::EAST)
       and (pin->getAccessDirection() != Pin::AccessDirection::WEST) ) {
      cerr << Error( "%s::do_xG_1PinM3(): %s *must* be east or west."
                   , getTypeName().c_str()
                   , getString(pin).c_str() ) << endl;
    }

    AutoContact* pinContact = NULL;
    doRp_AutoContacts( getGCell(), rp, pinContact, pinContact, HSmall|VSmall );

    if (not east() and not west()) {
      AutoContact* subContact1 = AutoContactVTee::create( getGCell(), rp->getNet(), Session::getContactLayer(1) );
      AutoSegment::create( pinContact, subContact1, Flags::Horizontal );

      setNorthContact( subContact1 );
      setSouthContact( subContact1 );
    } else {
      AutoContact* subContact1 = AutoContactHTee::create( getGCell(), rp->getNet(), Session::getContactLayer(1) );
      AutoSegment::create( pinContact, subContact1, Flags::Horizontal );
      
      if (north() and south()) {
        AutoContact* subContact2 = AutoContactHTee::create( getGCell(), rp->getNet(), Session::getContactLayer(1) );
        AutoSegment::create( subContact1, subContact2, Flags::Horizontal );
      
        setSouthContact( subContact1 );
        setNorthContact( subContact2 );
        if (west())
          setWestContact( subContact2 );
        else
          setEastContact( subContact2 );
      } else
        setBothCornerContacts( subContact1 );
    }

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderVH::_do_1G_1M1 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_1G_1M1() [Managed Configuration - Optimized] " << getTopology() << endl;

    uint64_t  flags = AddHNonPref;
    if      (east()) { flags |= HAccess|VSmall; }
    else if (west()) { flags |= HAccess|VSmall; }

    AutoContact* term = doRp_Access( getGCell(), getRoutingPads()[0], flags );
  
    if      (west ()) setWestContact ( term );
    else if (east ()) setEastContact ( term );
    else if (north()) setNorthContact( term );
    else if (south()) setSouthContact( term );

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderVH::_do_1G_xM1 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_1G_" << (int)getConnexity().fields.M1 << "M1() [Defered Configuration]" << endl;

    _do_xG_xM1_xM3();

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderVH::_do_2G_1M1 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_2G_1M1 [Managed Configuration]" << endl;
    cdebug_log(145,0) << "north:     " << north() << endl;
    cdebug_log(145,0) << "south:     " << south() << endl;
    cdebug_log(145,0) << "east:      " << east() << endl;
    cdebug_log(145,0) << "west:      " << west() << endl;

    if (east() and west()) {
      AutoContact* rpContact = nullptr;
      AutoContact* vtee      = nullptr;
      doRp_AutoContacts( getGCell(), getRoutingPads()[0], rpContact, vtee, NoFlags );
      vtee = AutoContactTurn::create( getGCell(), getNet(), Session::getDContactLayer() );
      AutoSegment::create( rpContact, vtee, Flags::Vertical );
      rpContact = vtee;
      vtee = AutoContactTurn::create( getGCell(), getNet(), Session::getDContactLayer() );
      AutoSegment::create( rpContact, vtee, Flags::Horizontal|Flags::UseNonPref );
      rpContact = vtee;

      vtee = AutoContactVTee::create( getGCell(), getNet(), Session::getDContactLayer() );
      AutoSegment::create( rpContact, vtee, Flags::Vertical );
      setWestContact( vtee );

      rpContact = AutoContactTurn::create( getGCell(), getNet(), Session::getDContactLayer() );
      AutoSegment::create( vtee, rpContact, Flags::Vertical );
      setEastContact( rpContact );
    } else if (north() and south()) {
      AutoContact* rpContact = nullptr;
      AutoContact* htee      = nullptr;
      AutoContact* turn      = nullptr;
      doRp_AutoContacts( getGCell(), getRoutingPads()[0], rpContact, htee, NoFlags );
      turn = AutoContactTurn::create( getGCell(), getNet(), Session::getDContactLayer() );
      AutoSegment::create( rpContact, turn, Flags::Vertical );
      htee = AutoContactTurn::create( getGCell(), getNet(), Session::getDContactLayer() );
      AutoSegment::create( turn, htee, Flags::Horizontal|Flags::UseNonPref );
      rpContact = htee;

      htee = AutoContactHTee::create( getGCell(), getNet(), Session::getDContactLayer() );
      AutoSegment::create( rpContact, htee, Flags::Vertical );

      rpContact = AutoContactTurn::create( getGCell(), getNet(), Session::getDContactLayer() );
      AutoSegment::create( htee, rpContact, Flags::Horizontal );
      setSouthContact( rpContact );

      rpContact = AutoContactTurn::create( getGCell(), getNet(), Session::getDContactLayer() );
      AutoSegment::create( htee, rpContact, Flags::Horizontal );
      setNorthContact( rpContact );
    } else {
      AutoContact* turn = doRp_Access( getGCell(), getRoutingPads()[0], NoFlags );
      AutoContact* vtee = AutoContactVTee::create( getGCell(), getNet(), Session::getDContactLayer() );
      AutoSegment::create( turn, vtee, Flags::Vertical );

      if (north()) setNorthContact( vtee );
      else         setSouthContact( vtee );
      if (east()) setEastContact( vtee );
      else        setWestContact( vtee );
    }

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderVH::_do_xG_1M1 ()
  {
    cdebug_log(145,1) << getTypeName()
                      << "::_do_xG_1M1() [G:" << (int)getConnexity().fields.globals
                      << " Managed Configuration]" << endl;
    cdebug_log(145,0) << "getConnexity(): " << getConnexity().connexity << endl;
    cdebug_log(145,0) << "north:     " << north() << endl;
    cdebug_log(145,0) << "south:     " << south() << endl;
    cdebug_log(145,0) << "east:      " << east() << endl;
    cdebug_log(145,0) << "west:      " << west() << endl;

    AutoContact* rpContact1 = nullptr;
    AutoContact* rpContact2 = nullptr;
    AutoContact* rpContact3 = nullptr;
    uint64_t     flags      = checkRoutingPadSize( getRoutingPads()[0] );

    doRp_AutoContacts( getGCell(), getRoutingPads()[0], rpContact1, rpContact2, flags );
    AutoContact* turn = nullptr;
    turn = AutoContactTurn::create( getGCell(), getNet(), Session::getDContactLayer() );
    AutoSegment::create( rpContact1, turn, Flags::Vertical );
    rpContact1 = turn;
    
    if (getConnexity().fields.globals == 1) {
      cdebug_log(145,0) << "case 1G, should never be here, but in 1G_1M1." << endl;
      return false;
    }

    if (getConnexity().fields.globals == 2) {
      cdebug_log(145,0) << "case 2G, should never be here, but in 2G_1M1." << endl;
      return false;
    }

    if (getConnexity().fields.globals == 3) {
      if (not east() or not west()) {
        cdebug_log(145,0) << "case 3G no East or no West" << endl;
        doRp_AutoContacts( getGCell(), getRoutingPads()[0], rpContact2, rpContact3, flags );
        AutoContact* turnN = AutoContactTurn::create( getGCell(), getNet(), Session::getDContactLayer() );
        AutoContact* turnS = AutoContactTurn::create( getGCell(), getNet(), Session::getDContactLayer() );
        AutoSegment::create( rpContact1, turnN, Flags::Horizontal|Flags::UseNonPref );
        AutoSegment::create( rpContact2, turnS, Flags::Horizontal|Flags::UseNonPref );
        AutoContact* vteeNorth = AutoContactVTee::create( getGCell(), getNet(), Session::getDContactLayer() );
        AutoSegment::create( turnN, vteeNorth, Flags::Vertical );

        if (east()) setEastContact( vteeNorth );
        if (west()) setWestContact( vteeNorth );
        setNorthContact( vteeNorth );
        setSouthContact( turnS );
        return true;
      }

      cdebug_log(145,0) << "case 3G no North or no South" << endl;
      doRp_AutoContacts( getGCell(), getRoutingPads()[0], rpContact2, rpContact3, flags );
      AutoContact* turnNS = AutoContactTurn::create( getGCell(), getNet(), Session::getDContactLayer() );
      AutoContact* turnEW = AutoContactTurn::create( getGCell(), getNet(), Session::getDContactLayer() );
      AutoSegment::create( rpContact1, turnNS, Flags::Horizontal|Flags::UseNonPref );
      AutoSegment::create( rpContact2, turnEW, Flags::Horizontal|Flags::UseNonPref );
      AutoContact* hteeEW = AutoContactHTee::create( getGCell(), getNet(), Session::getDContactLayer() );
      AutoSegment::create( turnEW, hteeEW, Flags::Vertical );

      setEastContact( hteeEW );
      setWestContact( hteeEW );
      if (north()) setNorthContact( turnNS );
      if (south()) setSouthContact( turnNS );
      return true;
    }

    cdebug_log(145,0) << "case 4G" << endl;
    doRp_AutoContacts( getGCell(), getRoutingPads()[0], rpContact2, rpContact3, flags );
    AutoContact* turnNE = AutoContactTurn::create( getGCell(), getNet(), Session::getDContactLayer() );
    AutoContact* turnSW = AutoContactTurn::create( getGCell(), getNet(), Session::getDContactLayer() );
    AutoSegment::create( rpContact1, turnNE, Flags::Horizontal|Flags::UseNonPref );
    AutoSegment::create( rpContact2, turnSW, Flags::Horizontal|Flags::UseNonPref );
    AutoContact* vteeNE = AutoContactVTee::create( getGCell(), getNet(), Session::getDContactLayer() );
    AutoSegment::create( turnNE, vteeNE, Flags::Vertical );
    AutoContact* vteeSW = AutoContactVTee::create( getGCell(), getNet(), Session::getDContactLayer() );
    AutoSegment::create( turnSW, vteeSW, Flags::Vertical );

    setSouthContact( vteeSW );
    setWestContact ( vteeSW );
    setNorthContact( vteeNE );
    setEastContact ( vteeNE );
    return true;
  }
  

  bool  NetBuilderVH::_do_xG_xM1_xM3 ()
  {
  // Implicit hypothesis : we have at least two globals and at least one terminal.
    
    cdebug_log(145,1) << getTypeName()
                      << "::_do_xG_"  << (int)getConnexity().fields.M1
                      << "M1_"        << (int)getConnexity().fields.M3
                      << "M3() [G:"   << (int)getConnexity().fields.globals << " Managed Configuration]" << endl;
    cdebug_log(145,0) << "getConnexity(): " << getConnexity().connexity << endl;
    cdebug_log(145,0) << "north:     " << north() << endl;
    cdebug_log(145,0) << "south:     " << south() << endl;
    cdebug_log(145,0) << "east:      " << east() << endl;
    cdebug_log(145,0) << "west:      " << west() << endl;

    sortRpByX( getRoutingPads(), NoFlags ); // increasing X.

    size_t       iLast        = getRoutingPads().size()-1;
    AutoContact* leftContact  = nullptr;
    AutoContact* rightContact = nullptr;

    if (south() or west()) {
      leftContact = doRp_Access( getGCell(), getRoutingPads()[0], HAccessEW|VSmall );
      if (south() and west()) {
        setSouthWestContact( AutoContactHTee::create( getGCell(), getNet(), Session::getDContactLayer() ) );
        AutoSegment::create( getWestContact(), leftContact, Flags::Horizontal );
      } else {
        if (west())
          setSouthWestContact( leftContact );
        else {
          setSouthWestContact( AutoContactTurn::create( getGCell(), getNet(), Session::getDContactLayer() ) );
          AutoSegment::create( leftContact, getWestContact(), Flags::Horizontal );
        }
      }
    } else {
      leftContact = doRp_Access( getGCell(), getRoutingPads()[0], HAccess|VSmall );
    }
    
    for ( size_t i=1 ; i<getRoutingPads().size()-1 ; ++i ) {
      rightContact = doRp_Access( getGCell(), getRoutingPads()[i], HAccessEW|VSmall );
      AutoSegment::create( leftContact, rightContact, Flags::Horizontal );

      leftContact = rightContact;
    }

    if (north() or east()) {
      if (getRoutingPads().size() > 1) {
        rightContact = doRp_Access( getGCell(), getRoutingPads()[iLast], HAccessEW|VSmall );
        AutoSegment::create( leftContact, rightContact, Flags::Horizontal );
      } else {
        rightContact = leftContact;
      }

      if (north() and east()) {
        setNorthEastContact( AutoContactHTee::create( getGCell(), getNet(), Session::getDContactLayer() ) );
        AutoSegment::create( rightContact, getEastContact(), Flags::Horizontal );
      } else {
        if (east())
          setNorthEastContact( rightContact );
        else {
          setNorthEastContact( AutoContactTurn::create( getGCell(), getNet(), Session::getDContactLayer() ) );
          AutoSegment::create( rightContact, getEastContact(), Flags::Horizontal );
        }
      }
    } else {
      rightContact = doRp_Access( getGCell(), getRoutingPads()[iLast], HAccess|VSmall );
      AutoSegment::create( leftContact, rightContact, Flags::Horizontal );
    }

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderVH::_do_1G_1M1_1PinM3 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_1G_1M1_1PinM3() [Delegated] " << getTopology() << endl;
    bool rvalue = false;
    rvalue = NetBuilderVH::_do_1G_xM1_1PinM3 ();
    cdebug_tabw(145,-1);
    return rvalue;
  }
  

  bool  NetBuilderVH::_do_1G_xM1_1PinM3 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_1G_xM1_1PinM3() [Managed Configuration - Optimized] " << getTopology() << endl;

    sortRpByX( getRoutingPads(), NoFlags ); // increasing X.

    vector<RoutingPad*> rpsM1;
    RoutingPad*         pinM3 = NULL;
    for ( RoutingPad* rp : getRoutingPads() ) {
      if (dynamic_cast<Pin*>(rp->getOccurrence().getEntity())) pinM3 = rp;
      else rpsM1.push_back( rp );
    }
    
    for ( size_t i=1 ; i<rpsM1.size() ; ++i ) {
      AutoContact* leftContact  = doRp_Access( getGCell(), rpsM1[i-1], HAccess );
      AutoContact* rightContact = doRp_Access( getGCell(), rpsM1[i  ], HAccess );
      AutoSegment::create( leftContact, rightContact, Flags::Horizontal );
    }

    AutoContact* turn         = NULL;
    AutoContact* tee          = NULL;
    AutoContact* pinM3Contact = NULL;
    AutoContact* rpM1Contact  = NULL;
    doRp_AutoContacts( getGCell(), pinM3, pinM3Contact, rpM1Contact, NoProtect );
    rpM1Contact = doRp_Access( getGCell(), rpsM1[0], (north() or south()) ? HAccess : NoFlags );

    if (north() or south()) {
      turn = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
      tee  = AutoContactVTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );

      AutoSegment::create( rpM1Contact , tee , Flags::Horizontal );
      AutoSegment::create( pinM3Contact, turn, Flags::Horizontal );
      AutoSegment::create( tee         , turn, Flags::Vertical );

      setBothCornerContacts( tee );
    } else {
      turn = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
      tee  = AutoContactVTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );

      AutoSegment::create( rpM1Contact , tee , Flags::Vertical );
      AutoSegment::create( pinM3Contact, tee , Flags::Horizontal );
      AutoSegment::create( tee         , turn, Flags::Vertical );

      setBothCornerContacts( turn );
    }

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderVH::_do_2G ()
  {
    cdebug_log(145,0) << getTypeName() << "::_do_2G()" << endl;
    return _do_xG();
  }

  bool  NetBuilderVH::_do_xG ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_xG()" << endl;

    const Layer* viaLayer = Session::getDContactLayer();

    if (getConnexity().fields.globals == 2) {
      setBothCornerContacts( AutoContactTurn::create( getGCell(), getNet(), viaLayer ) );
    } else if (getConnexity().fields.globals == 3) {
      if (east() and west()) {
        setSouthWestContact( AutoContactTurn::create( getGCell(), getNet(), viaLayer ) );
        setNorthEastContact( AutoContactVTee::create( getGCell(), getNet(), viaLayer ) );
        if (south()) swapCornerContacts();

        AutoSegment::create( getSouthContact(), getNorthContact(), Flags::Vertical );
      } else {
        setSouthWestContact( AutoContactTurn::create( getGCell(), getNet(), viaLayer ) );
        setNorthEastContact( AutoContactHTee::create( getGCell(), getNet(), viaLayer ) );
        if (west()) swapCornerContacts();

        AutoSegment::create( getWestContact(), getEastContact(), Flags::Horizontal );
      }
    } else { // fields.globals == 4.
      AutoContact* turn = AutoContactTurn::create( getGCell(), getNet(), viaLayer );
      setSouthWestContact( AutoContactHTee::create( getGCell(), getNet(), viaLayer ) );
      setNorthEastContact( AutoContactVTee::create( getGCell(), getNet(), viaLayer ) );
      AutoSegment::create( getWestContact(), turn, Flags::Horizontal );
      AutoSegment::create( turn, getNorthContact(), Flags::Vertical   );
    } 
    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderVH::_do_globalSegment ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_globalSegment()" << endl;

    if (getSourceContact()) {
      AutoContact* targetContact = nullptr;
      switch ( getSegmentHookType(getFromHook()) ) {
        case EastBound:  targetContact = getEastContact (); break;
        case WestBound:  targetContact = getWestContact (); break;
        case NorthBound: targetContact = getNorthContact(); break;
        case SouthBound: targetContact = getSouthContact(); break;
      }
      AutoSegment* globalSegment = AutoSegment::create( getSourceContact()
                                                      , targetContact
                                                      , static_cast<Segment*>( getFromHook()->getComponent() )
                                                      );
      globalSegment->setFlags( (getDegree() == 2) ? AutoSegment::SegBipoint : 0 );
      cdebug_log(145,0) << "Create global segment: " << globalSegment << endl;
  
    // HARDCODED VALUE.
      if ( (getTopology() & Global_Fixed) and (globalSegment->getAnchoredLength() < 2*Session::getSliceHeight()) )
        addToFixSegments( globalSegment );
        
      if (getConnexity().fields.globals < 2) {
        cdebug_tabw(145,-1);
        return false;
      }
    } else
      setFromHook( NULL );
    
    push( east (), getEastContact () );
    push( west (), getWestContact () );
    push( north(), getNorthContact() );
    push( south(), getSouthContact() );

    cdebug_tabw(145,-1);
    return true;
  }


  void  NetBuilderVH::singleGCell ( AnabaticEngine* anbt, Net* net )
  {
    cdebug_log(145,1) << getTypeName() << "::singleGCell() " << net << endl;

    vector<RoutingPad*>  rpM1s;
    RoutingPad*          rpPin = NULL;

    for ( RoutingPad* rp : net->getRoutingPads() ) {
      if (dynamic_cast<Pin*>(rp->getOccurrence().getEntity())) {
        rpPin = rp;
        continue;
      }
      if (Session::getRoutingGauge()->getLayerDepth(rp->getLayer()) == 0) {
        rpM1s.push_back( rp );
        continue;
      }
      cerr << Error( "%s::singleGCell(): Non metal1 terminals are not managed yet.\n"
                     "        (%s)"
                   , getTypeName().c_str()
                   , getString(rp).c_str()
                   ) << endl;
      cdebug_tabw(145,-1);
      return;
    }

    if (rpM1s.size() + ((rpPin)?1:0) < 2) {
      cerr << Error( "%s::singleGCell(): For %s, less than two Plugs/Pins (%d+%d)."
                   , getTypeName().c_str()
                   , getString(net).c_str()
                   , rpM1s.size()
                   , (rpPin?1:0) ) << endl;
      cdebug_tabw(145,-1);
      return;
    }

    if (rpM1s.empty()) {
      cerr << Error( "%s::singleGCell(): No terminals for Net \"%s\"."
                   , getTypeName().c_str()
                   , getString(net->getName()).c_str() ) << endl;
      cdebug_tabw(145,-1);
      return;
    }

    sortRpByX( rpM1s, NetBuilder::NoFlags ); // increasing X.

    GCell* gcell1 = anbt->getGCellUnder( (*rpM1s.begin ())->getCenter() );
    GCell* gcell2 = anbt->getGCellUnder( (*rpM1s.rbegin())->getCenter() );

    if (not gcell1) {
      cerr << Error( "%s::singleGCell(): No GCell under %s."
                   , getTypeName().c_str()
                   , getString(*(rpM1s.begin())).c_str() ) << endl;
      cdebug_tabw(145,-1);
      return;
    }
    if (gcell1 != gcell2) {
      cerr << Error( "%s::singleGCell(): Not under a single GCell %s."
                   , getTypeName().c_str()
                   , getString(*(rpM1s.rbegin())).c_str() ) << endl;
      cdebug_tabw(145,-1);
      return;
    }

    cdebug_log(145,0) << "singleGCell " << gcell1 << endl;

    AutoContact* source = NULL;
    AutoContact* target = NULL;

    for ( size_t irp=1 ; irp<rpM1s.size() ; ++irp ) {
      source = doRp_Access( gcell1, rpM1s[irp-1], HAccess|VSmall );
      target = doRp_Access( gcell1, rpM1s[irp  ], HAccess|VSmall );
      AutoSegment::create( source, target, Flags::Horizontal );
    }

    if (rpPin) {
      Pin*                 pin    = dynamic_cast<Pin*>( rpPin->getOccurrence().getEntity() );
      Pin::AccessDirection pinDir = pin->getAccessDirection();

      if (  (pinDir == Pin::AccessDirection::NORTH)
         or (pinDir == Pin::AccessDirection::SOUTH) ) {
        source = doRp_Access( gcell1, rpM1s[0], HAccess );
        target = doRp_AccessNorthSouthPin( gcell1, rpPin );
        AutoContact* turn1  = AutoContactTurn::create( gcell1, rpPin->getNet(), Session::getBuildContactLayer(1) );
        AutoSegment::create( source, turn1 , Flags::Horizontal );
        AutoSegment::create( turn1 , target, Flags::Vertical );
      } else {
        RoutingPad* closest = NULL;
        
        if (pinDir == Pin::AccessDirection::EAST) closest = rpM1s.back();
        else                                      closest = rpM1s.front();

        source = doRp_Access( gcell1, closest, HAccess );
        target = doRp_AccessEastWestPin( gcell1, rpPin );
        AutoSegment::create( source, target, Flags::Horizontal );
      }
    }

    cdebug_tabw(145,-1);
  }


  string  NetBuilderVH::getTypeName () const
  { return "NetBuilderVH"; }


}  // Anabatic namespace.
