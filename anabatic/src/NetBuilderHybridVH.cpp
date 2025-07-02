// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2022-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |         A n a b a t i c  -  Routing Toolbox                     |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./NetBuilderHybridVH.cpp"                 |
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
#include "hurricane/Pin.h"
#include "hurricane/Pad.h"
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
#include "anabatic/NetBuilderHybridVH.h"
#include "anabatic/AnabaticEngine.h"


namespace Anabatic {

  using std::swap;
  using Hurricane::Error;
  using Hurricane::Pin;

  
  NetBuilderHybridVH::NetBuilderHybridVH ()
    : NetBuilder()
  { }


  NetBuilderHybridVH::~NetBuilderHybridVH () { }


  string  NetBuilderHybridVH::getStyle ()
  { return "VH,2RL"; }

  
  void  NetBuilderHybridVH::doRp_AutoContacts ( GCell*        gcell
                                              , RoutingPad*   rp
                                              , AutoContact*& source
                                              , AutoContact*& target
                                              , uint64_t      flags
                                              )
  {
    throw Error ( "%s::dpRp_AutoContacts() method must never be called.", getTypeName().c_str() );
  }


  AutoContact* NetBuilderHybridVH::doRp_Access ( GCell* gcell, RoutingPad* rp, uint64_t flags )
  {
    cdebug_log(145,1) << getTypeName() << "::doRp_Access()" << endl;
    cdebug_log(145,0) << rp << endl;

    const Layer* rpLayer   = rp->getLayer();
  //const Layer* viaLayer  = Session::getDContactLayer();
    DbU::Unit    viaSide   = Session::getDContactWidth();
    Point        position  = rp->getCenter();
    AutoContact* rpContact = AutoContactTerminal::create( gcell, rp, rpLayer, position, viaSide, viaSide );

    cdebug_tabw(145,-1);
    return rpContact;
  }


  AutoContact* NetBuilderHybridVH::doRp_AccessNorthSouthPin ( GCell* gcell, RoutingPad* rp )
  {
    cdebug_log(145,1) << getTypeName() << "::doRp_AccessNorthSouthPin() " << rp << endl;

    AutoContact* rpContact = doRp_Access( gcell, rp, NoFlags );
    AutoContact* turn      = NULL;

    Net* net = rp->getNet();
    Pin* pin = dynamic_cast<Pin*>( rp->getOccurrence().getEntity() );
    Pin::AccessDirection pinDir = pin->getAccessDirection();
    if (pinDir == Pin::AccessDirection::NORTH) {
      turn = AutoContactTurn::create( gcell, net, Session::getBuildRoutingLayer(0) );
      AutoSegment* segment = AutoSegment::create( rpContact, turn, Flags::Vertical );
      segment->setAxis( rp->getX(), Flags::Force );
      segment->setFlags( AutoSegment::SegFixed|AutoSegment::SegFixedAxis );
      rpContact = turn;

      turn    = AutoContactTurn::create( gcell, net, Session::getBuildContactLayer(0) );
      segment = AutoSegment::create( rpContact, turn, Flags::Horizontal );
      rpContact = turn;

      DbU::Unit axis = gcell->getYMax() - Session::getDHorizontalPitch();
      cdebug_log(145,0) << "axis:" << DbU::getValueString(axis) << endl;
      
      segment->setAxis( axis, Flags::Force );
    //segment->setFlags( AutoSegment::SegFixed|AutoSegment::SegFixedAxis );
      cdebug_log(145,0) << segment << endl;
    } else {
      turn = rpContact;
    }

    cdebug_tabw(145,-1);
    return turn;
  }


  AutoContact* NetBuilderHybridVH::doRp_AccessEastWestPin ( GCell* gcell, RoutingPad* rp )
  {
    cdebug_log(145,1) << getTypeName() << "::doRp_AccessEastWestPin() " << rp << endl;

    Net*         net            = rp->getNet();
    Pin*         pin            = dynamic_cast<Pin*>( rp->getOccurrence().getEntity() );
    Pin::AccessDirection pinDir = pin->getAccessDirection();
    AutoContact* rpContact      = doRp_Access( gcell, rp, NoFlags );
    AutoContact* turn           = NULL;
    AutoSegment* segment        = NULL;

    turn    = AutoContactTurn::create( gcell, net, Session::getBuildContactLayer(0) );
    segment = AutoSegment::create( rpContact, turn, Flags::Horizontal );
    segment->setAxis( pin->getCenter().getY(), Flags::Force );

    rpContact = turn;
    turn    = AutoContactTurn::create( gcell, net, Session::getBuildContactLayer(0) );
    segment = AutoSegment::create( rpContact, turn, Flags::Vertical );

    DbU::Unit axis = 0;
    if (pinDir == Pin::AccessDirection::EAST)
      axis = gcell->getXMax() - Session::getDVerticalPitch();
    else
      axis = gcell->getXMin() + Session::getDVerticalPitch();
    segment->setAxis( axis );

    cdebug_tabw(145,-1);
    return turn;
  }


  bool  NetBuilderHybridVH::doRp_xG_1M1 ( RoutingPad* rp )
  {
    cdebug_log(145,1) << getTypeName() << "::doRp_xG_1M1()" << endl;

    AutoContact* swContact = nullptr;
    if (west() or south()) {
      AutoContact* termContact = doRp_Access( getGCell(), rp, NoFlags );
      if (west() and south()) {
        swContact = AutoContactVTee::create( getGCell(), getNet(), Session::getBuildContactLayer(0) );
        AutoSegment::create( termContact, swContact, Flags::Vertical );
      } else {
        if (west()) {
          swContact = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(0) );
          AutoSegment::create( termContact, swContact, Flags::Vertical );
        } else
          swContact = termContact;
      }
      setSouthWestContact( swContact );
    }

    AutoContact* neContact = nullptr;
    if (east() or north()) {
      AutoContact* termContact = doRp_Access( getGCell(), rp, NoFlags );
      if (east() and north()) {
        neContact = AutoContactVTee::create( getGCell(), getNet(), Session::getBuildContactLayer(0) );
        AutoSegment::create( termContact, neContact, Flags::Vertical );
      } else {
        if (east()) {
          neContact = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(0) );
          AutoSegment::create( termContact, neContact, Flags::Vertical );
        } else
          neContact = termContact;
      }
      setNorthEastContact( neContact );
    }

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHybridVH::_do_xG_1M1 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_xG_1M1()" << endl;

    doRp_xG_1M1( getRoutingPads()[0] );

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHybridVH::_do_2G_1M1 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_2G_1M1()" << endl;

    doRp_xG_1M1( getRoutingPads()[0] );

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHybridVH::doRp_xG_xM1_xM3 ( const vector<RoutingPad*>& rps )
  {
    cdebug_log(145,1) << getTypeName() << "::doRp_xG_xM1()" << endl;

    AutoContact* prevContact = nullptr;
    AutoContact* currContact = nullptr;

    for ( size_t i=0 ; i<rps.size() ; ++i ) {
      prevContact = currContact;
      AutoContact* currTerm = doRp_Access( getGCell(), rps[i], NoFlags );

      if (i == 0) {
        if (west() or south()) {
          AutoContact* swContact = nullptr;
          currContact = AutoContactVTee::create( getGCell(), getNet(), Session::getBuildContactLayer(0) );
          if (west() and south()) {
            swContact = AutoContactVTee::create( getGCell(), getNet(), Session::getBuildContactLayer(0) );
          } else {
            if (west())
              swContact = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(0) );
            else
              swContact = currContact;
          }
          if (swContact != currContact)
            AutoSegment::create( currContact, swContact, Flags::Vertical );
          setSouthWestContact( swContact );
        } else {
          currContact = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(0) );
        }
        AutoSegment::create( currTerm, currContact, Flags::Vertical );
        continue;
      }

      if (i+1 == rps.size()) {
        if (east() or north()) {
          AutoContact* neContact = nullptr;
          currContact = AutoContactVTee::create( getGCell(), getNet(), Session::getBuildContactLayer(0) );
          if (east() and north()) {
            neContact = AutoContactVTee::create( getGCell(), getNet(), Session::getBuildContactLayer(0) );
          } else {
            if (east())
              neContact = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(0) );
            else
              neContact = currContact;
          }
          if (neContact != currContact)
            AutoSegment::create( currContact, neContact, Flags::Vertical );
          setNorthEastContact( neContact );
        } else {
          currContact = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(0) );
        }
      } else {
        currContact = AutoContactHTee::create( getGCell(), getNet(), Session::getBuildContactLayer(0) );
      }
      AutoSegment::create( currTerm   , currContact, Flags::Vertical );
      AutoSegment::create( prevContact, currContact, Flags::Horizontal );
    }

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHybridVH::_do_xG_xM1_xM3 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_xG_xM1()" << endl;

    sortRpByX( getRoutingPads(), NoFlags ); // increasing X.
    doRp_xG_xM1_xM3( getRoutingPads() );

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHybridVH::_do_1G_xM1 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_1G_xM1()" << endl;

    if (getConnexity().fields.M1 == 1)
      _do_xG_1M1();
    else
      _do_xG_xM1_xM3();

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHybridVH::_do_xG ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_xG()" << endl;

    const Layer* viaLayer = Session::getBuildContactLayer( 0 );
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
      AutoSegment::create( turn, getNorthContact(), Flags::Vertical );
    } 
    
    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHybridVH::_do_1G_1PinM1 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_1G_1PinM1() [Managed Configuration - Optimized] " << getTopology() << endl;

    AutoContact* rpContact = doRp_AccessNorthSouthPin( getGCell(), getRoutingPads()[0] );

    AutoContact* turn1 = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(0) );
    AutoSegment::create( rpContact, turn1, Flags::Vertical );

    if (north() or south()) {
      AutoContact* turn2 = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(0) );
      AutoSegment::create( turn1, turn2, Flags::Horizontal );
      turn1 = turn2;
    }
    setBothCornerContacts( turn1 );

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHybridVH::_do_2G_1PinM1 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_2G_1PinM1() [Managed Configuration - Optimized] " << getTopology() << endl;

    AutoContact* rpContact = doRp_AccessNorthSouthPin( getGCell(), getRoutingPads()[0] );
    AutoContact* tee       = nullptr;

    if (east() and west()) {
      tee = AutoContactHTee::create( getGCell(), getNet(), Session::getBuildContactLayer(0) );
    } else {
      tee = AutoContactVTee::create( getGCell(), getNet(), Session::getBuildContactLayer(0) );
    }
    AutoSegment::create( rpContact, tee, Flags::Vertical );
    setBothCornerContacts( tee );

    cdebug_tabw(145,-1);
    return true;
  }
  

  bool  NetBuilderHybridVH::_do_1G_xM1_1PinM1 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_1G_xM1_1PinM1() [Managed Configuration - Optimized] " << getTopology() << endl;

    sortRpByX( getRoutingPads(), NoFlags ); // increasing X.

    vector<RoutingPad*> rpsM1;
    RoutingPad*         rpM2  = nullptr;
    for ( RoutingPad* rp : getRoutingPads() ) {
      if (dynamic_cast<Pin*>(rp->getOccurrence().getEntity())) rpM2 = rp;
      else rpsM1.push_back( rp );
    }
    if (rpsM1.size() > 1)
      doRp_xG_xM1_xM3( rpsM1 );
    else
      doRp_xG_1M1( rpsM1[0] );

    AutoContact* rpContact      = doRp_AccessNorthSouthPin( getGCell(), rpM2 );
    AutoContact* rpM1Contact    = doRp_Access( getGCell(), rpsM1.front(), NoFlags );
    AutoContact* turn1 = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(0) );
    AutoContact* turn2 = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(0) );
    AutoSegment::create( rpM1Contact, turn1, Flags::Vertical );
    AutoSegment::create( rpContact  , turn2, Flags::Vertical );
    AutoSegment::create( turn1      , turn2, Flags::Horizontal );

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHybridVH::doRp_1G_1PinM2 ( RoutingPad* rp )
  {
    cdebug_log(145,1) << getTypeName() << "::doRp_1G_1PinM2() [Managed Configuration - Optimized] " << getTopology() << endl;

    AutoContact* rpContact = doRp_AccessEastWestPin( getGCell(), rp );
    AutoContact* turn1     = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(0) );
    AutoSegment::create( rpContact, turn1, Flags::Horizontal );

    if (east() or west()) {
      AutoContact* turn2 = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(0) );
      AutoSegment::create( turn1, turn2, Flags::Vertical );
      turn1 = turn2;
    }
    setBothCornerContacts( turn1 );

    cdebug_tabw(145,-1);
    return true;
  }
  

  bool  NetBuilderHybridVH::_do_1G_1PinM2 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_1G_1PinM2() [Managed Configuration - Optimized] " << getTopology() << endl;

    doRp_1G_1PinM2( getRoutingPads()[0] );

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHybridVH::_do_xG_1PinM2 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_xG_1PinM2()" << endl;

    AutoContact* rpContact = doRp_AccessEastWestPin( getGCell(), getRoutingPads()[0] );
    AutoContact* neContact = nullptr;
    AutoContact* swContact = nullptr;

    const Layer* viaLayer = Session::getBuildContactLayer( 0 );
    if (getConnexity().fields.globals == 2) {
      if (north() and south())
        neContact = AutoContactVTee::create( getGCell(), getNet(), viaLayer );
      else
        neContact = AutoContactHTee::create( getGCell(), getNet(), viaLayer );
      AutoSegment::create( rpContact, neContact, Flags::Horizontal );
      setBothCornerContacts( neContact );
    } else if (getConnexity().fields.globals == 3) {
      swContact = AutoContactVTee::create( getGCell(), getNet(), viaLayer );
      neContact = AutoContactVTee::create( getGCell(), getNet(), viaLayer );
      if (west())
        AutoSegment::create( rpContact, neContact, Flags::Horizontal );
      else
        AutoSegment::create( rpContact, swContact, Flags::Horizontal );
      AutoSegment::create( swContact, neContact, Flags::Vertical );
      setNorthEastContact( neContact );
      setSouthWestContact( swContact );
    } 
    
    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHybridVH::_do_1G_xM1_1PinM2 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_1G_xM1_1PinM2() [Managed Configuration - Optimized] " << getTopology() << endl;

    sortRpByX( getRoutingPads(), NoFlags ); // increasing X.

    vector<RoutingPad*> rpsM1;
    RoutingPad*         rpM2  = nullptr;
    for ( RoutingPad* rp : getRoutingPads() ) {
      if (dynamic_cast<Pin*>(rp->getOccurrence().getEntity())) rpM2 = rp;
      else rpsM1.push_back( rp );
    }
    if (rpsM1.size() > 1)
      doRp_xG_xM1_xM3( rpsM1 );
    else
      doRp_xG_1M1( rpsM1[0] );


    Pin*         pin            = dynamic_cast<Pin*>( rpM2->getOccurrence().getEntity() );
    Pin::AccessDirection pinDir = pin->getAccessDirection();
    AutoContact* rpContact      = doRp_AccessEastWestPin( getGCell(), rpM2 );
    AutoContact* rpM1Contact    = nullptr;
    if (pinDir == Pin::AccessDirection::EAST)
      rpM1Contact = doRp_Access( getGCell(), rpsM1.back(), NoFlags );
    else
      rpM1Contact = doRp_Access( getGCell(), rpsM1.front(), NoFlags );
    AutoContact* turn = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(0) );
    AutoSegment::create( rpM1Contact, turn, Flags::Vertical );
    AutoSegment::create( rpContact  , turn, Flags::Horizontal );

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHybridVH::_do_1G_1M1     () { return false; }
  // bool  NetBuilderHybridVH::_do_xG_1Pad    () { return false; }
  // bool  NetBuilderHybridVH::_do_xG_xM2     () { return false; }
  // bool  NetBuilderHybridVH::_do_1G_1M3     () { return false; }
  // bool  NetBuilderHybridVH::_do_xG_xM3     () { return false; }
  // bool  NetBuilderHybridVH::_do_xG_1M1_1M2 () { return false; }
  // bool  NetBuilderHybridVH::_do_4G_1M2     () { return false; }
  bool  NetBuilderHybridVH::_do_2G         () { return false; }


  bool  NetBuilderHybridVH::_do_globalSegment ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_globalSegment()" << endl;
    cdebug_log(145,0) << "getSourceFlags():" << getSourceFlags()
                      << " getFlags():" << getFlags() << endl;

    if (getSourceContact()) {
      uint64_t     segmentBound  = getSegmentHookType( getFromHook() );
      AutoContact* targetContact = nullptr;
      switch ( segmentBound ) {
        case EastBound:  targetContact = getEastContact (); break;
        case WestBound:  targetContact = getWestContact (); break;
        case NorthBound: targetContact = getNorthContact(); break;
        case SouthBound: targetContact = getSouthContact(); break;
      }
      if (not getFromHook()) cerr << "getFromHook() is NULL !" << endl;

      AutoContact* sourceContact = getSourceContact();
      if (segmentBound & (NorthBound|SouthBound)) {
        AutoContact* turn = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(0) );
        AutoSegment::create( sourceContact, turn, Flags::Vertical );
        sourceContact = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(0) );
        AutoSegment::create( sourceContact, turn, Flags::Horizontal );
      }
      Segment*     baseSegment   = static_cast<Segment*>( getFromHook()->getComponent() );
      AutoSegment* globalSegment = AutoSegment::create( sourceContact
                                                      , targetContact
                                                      , baseSegment
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
    
    push( east (), getEastContact() );
    push( west (), getWestContact() );
    push( north(), getNorthContact() );
    push( south(), getSouthContact() );

    cdebug_tabw(145,-1);
    return true;
  }


  void  NetBuilderHybridVH::singleGCell ( AnabaticEngine* anbt, Net* net )
  {
    cdebug_log(145,1) << getTypeName() << "::singleGCell() " << net << endl;

    vector<RoutingPad*> rps;
    for ( RoutingPad* irp : net->getRoutingPads() )
      rps.push_back( irp );

    if (rps.size() < 2) {
      cerr << Error( "%s::singleGCell(): For %s, less than two Plugs/Pins (%d)."
                   , getTypeName().c_str()
                   , getString(net).c_str()
                   , rps.size() ) << endl;
      cdebug_tabw(145,-1);
      return;
    }

    sortRpByX( rps, NetBuilder::NoFlags ); // increasing X.

    GCell* gcell1 = anbt->getGCellUnder( rps.front()->getCenter() );
    GCell* gcell2 = anbt->getGCellUnder( rps.back ()->getCenter() );

    if (not gcell1) {
      cerr << Error( "No GCell under %s.", getString(rps.front()).c_str() ) << endl;
      cdebug_tabw(145,-1);
      return;
    }
    if (gcell1 != gcell2) {
      cerr << Error( "Not under a single GCell %s.", getString(rps.back()).c_str() ) << endl;
      cdebug_tabw(145,-1);
      return;
    }

    AutoContact* prevContact = nullptr;
    AutoContact* currContact = nullptr;

    for ( size_t i=0 ; i<rps.size() ; ++i ) {
      prevContact = currContact;
      AutoContact* currTerm = doRp_Access( gcell1, rps[i], NoFlags );

      if (i == 0) {
        currContact = AutoContactTurn::create( gcell1, net, Session::getBuildContactLayer(0) );
        AutoSegment::create( currTerm, currContact, Flags::Vertical );
        continue;
      }

      if (i+1 == rps.size()) {
        currContact = AutoContactTurn::create( gcell1, net, Session::getBuildContactLayer(0) );
      } else {
        currContact = AutoContactHTee::create( gcell1, net, Session::getBuildContactLayer(0) );
      }
      AutoSegment::create( currTerm   , currContact, Flags::Vertical );
      AutoSegment::create( prevContact, currContact, Flags::Horizontal );
    }
    
    cdebug_tabw(145,-1);
  }


  string  NetBuilderHybridVH::getTypeName () const
  { return "NetBuilderHybridVH"; }


}  // Anabatic namespace.
