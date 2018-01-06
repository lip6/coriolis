// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |         A n a b a t i c  -  Routing Toolbox                     |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./NetBuilderHV.cpp"                       |
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
#include "anabatic/NetBuilderHV.h"
#include "anabatic/AnabaticEngine.h"


namespace Anabatic {

  using std::swap;
  using Hurricane::Transformation;
  using Hurricane::Warning;

  
  NetBuilderHV::NetBuilderHV ()
    : NetBuilder()
  { }


  NetBuilderHV::~NetBuilderHV () { }

  
  void  NetBuilderHV::doRp_AutoContacts ( GCell*        gcell
                                        , Component*    rp
                                        , AutoContact*& source
                                        , AutoContact*& target
                                        , uint64_t      flags
                                        )
  {
    cdebug_log(145,1) << getTypeName() << "::doRp_AutoContacts()" << endl;
    cdebug_log(145,0)   << rp << endl;

    RoutingPad* rrp = dynamic_cast<RoutingPad*>( rp );
    if (not rrp) {
      cerr << Warning( "%s::doRp_AutoContacts(): %s is *not* a RoutingPad."
                     , getTypeName().c_str()
                     , getString(rp).c_str() ) << endl;
    }

    source = target = NULL;

    Point        sourcePosition;
    Point        targetPosition;
    const Layer* rpLayer        = rp->getLayer();
    size_t       rpDepth        = Session::getLayerDepth( rp->getLayer() );
    Flags        direction      = Session::getDirection ( rpDepth );
    DbU::Unit    viaSide        = Session::getViaWidth  ( rpDepth );

    getPositions( rp, sourcePosition, targetPosition );

    if (sourcePosition.getX() > targetPosition.getX()) swap( sourcePosition, targetPosition );
    if (sourcePosition.getY() > targetPosition.getY()) swap( sourcePosition, targetPosition );

    GCell* sourceGCell = Session::getAnabatic()->getGCellUnder( sourcePosition );
    GCell* targetGCell = Session::getAnabatic()->getGCellUnder( targetPosition );

    if (rpDepth == 0) {
      rpLayer   = Session::getContactLayer(0);
      direction = Flags::Horizontal;
      viaSide   = Session::getViaWidth( rpDepth );
    }

  // Non-M1 terminal or punctual M1 protections.
    if ((rpDepth != 0) or (sourcePosition == targetPosition)) {
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


  AutoContact* NetBuilderHV::doRp_Access ( GCell* gcell, Component* rp, uint64_t flags )
  {
    cdebug_log(145,1) << getTypeName() << "::doRp_Access() - flags:" << flags << endl;

    AutoContact* rpContactSource;
    AutoContact* rpContactTarget;

    flags |= checkRoutingPadSize( rp );

    doRp_AutoContacts( gcell, rp, rpContactSource, rpContactTarget, flags );

    if (flags & HAccess) {
      if (flags & VSmall) {
        AutoContact* subContact1 = AutoContactTurn::create( gcell, rp->getNet(), Session::getContactLayer(1) );
        AutoContact* subContact2 = AutoContactTurn::create( gcell, rp->getNet(), Session::getContactLayer(1) );
        AutoSegment::create( rpContactSource, subContact1, Flags::Horizontal );
        AutoSegment::create( subContact1,     subContact2, Flags::Vertical );
        rpContactSource = subContact2;
      }
    } else {
      if (flags & HSmall) {
        AutoContact* subContact1 = AutoContactTurn::create( gcell, rp->getNet(), Session::getContactLayer(1) );
        AutoSegment::create( rpContactSource, subContact1, Flags::Horizontal );
        rpContactSource = subContact1;
      }
    }

    cdebug_tabw(145,-1);

    return rpContactSource;
  }


  bool  NetBuilderHV::_do_1G_1M1 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_1G_1M1() [Managed Configuration - Optimized] " << getTopology() << endl;

    uint64_t  flags = NoFlags;
    if      (east() ) { flags |= HAccess; }
    else if (west() ) { flags |= HAccess; }
    else if (north()) { flags |= VSmall; }
    else if (south()) { flags |= VSmall; }

    setBothCornerContacts( doRp_Access(getGCell(),getRoutingPads()[0],flags) );

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHV::_do_1G_xM1 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_1G_" << (int)getConnexity().fields.M1 << "M1() [Managed Configuration]" << endl;

    sortRpByX( getRoutingPads(), NoFlags ); // increasing X.
    for ( size_t i=1 ; i<getRoutingPads().size() ; ++i ) {
      AutoContact* leftContact  = doRp_Access( getGCell(), getRoutingPads()[i-1], HAccess );
      AutoContact* rightContact = doRp_Access( getGCell(), getRoutingPads()[i  ], HAccess );
      AutoSegment::create( leftContact, rightContact, Flags::Horizontal );
    }

    Component* globalRp = NULL;
    if      (east()) globalRp = getRoutingPads()[getRoutingPads().size()-1];
    else if (west()) globalRp = getRoutingPads()[0];
    else {
      globalRp = getRoutingPads()[0];

      cdebug_log(145,0) << "| Initial N/S Global RP: " << globalRp << endl;
      for ( size_t i=1 ; i<getRoutingPads().size() ; ++i ) {
        if (getRoutingPads()[i]->getBoundingBox().getHeight() > globalRp->getBoundingBox().getHeight()) {
          cdebug_log(145,0) << "| Better RP: " << globalRp << endl;
          globalRp = getRoutingPads()[i];
        }
      }
    }
    
    AutoContact* globalContact = doRp_Access( getGCell(), globalRp, HAccess );

    if (north() or south()) {
      AutoContact* turn = globalContact;
      globalContact = AutoContactTurn::create( getGCell(), getNet(), Session::getContactLayer(1) );
      AutoSegment::create( globalContact, turn, Flags::Horizontal );
    }

    setBothCornerContacts( globalContact );

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHV::_do_xG ()
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

        AutoSegment::create( getSouthWestContact(), getNorthEastContact(), Flags::Vertical );
      } else {
        setSouthWestContact( AutoContactTurn::create( getGCell(), getNet(), viaLayer ) );
        setNorthEastContact( AutoContactHTee::create( getGCell(), getNet(), viaLayer ) );
        if (west()) swapCornerContacts();

        AutoSegment::create( getSouthWestContact(), getNorthEastContact(), Flags::Horizontal );
      }
    } else { // fields.globals == 4.
      AutoContact* turn = AutoContactTurn::create( getGCell(), getNet(), viaLayer );
      setSouthWestContact( AutoContactHTee::create( getGCell(), getNet(), viaLayer ) );
      setNorthEastContact( AutoContactVTee::create( getGCell(), getNet(), viaLayer ) );
      AutoSegment::create( getSouthWestContact(), turn, Flags::Horizontal );
      AutoSegment::create( turn, getNorthEastContact(), Flags::Vertical   );
    } 
    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHV::_do_2G ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_2G()" << endl;

    const Layer* viaLayer = Session::getDContactLayer();

    if (east() and west()) {
      setSouthWestContact( AutoContactTurn::create( getGCell(), getNet(), viaLayer ) );
      setNorthEastContact( AutoContactTurn::create( getGCell(), getNet(), viaLayer ) );
      AutoSegment::create( getSouthWestContact(), getNorthEastContact(), Flags::Vertical );
    } else if (south() and north()) {
      setSouthWestContact( AutoContactTurn::create( getGCell(), getNet(), viaLayer ) );
      setNorthEastContact( AutoContactTurn::create( getGCell(), getNet(), viaLayer ) );
      AutoSegment::create( getSouthWestContact(), getNorthEastContact(), Flags::Horizontal );
    } else {
      setBothCornerContacts( AutoContactTurn::create( getGCell(), getNet(), viaLayer ) );
    }

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHV::_do_xG_1Pad ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_xG_1Pad() [Managed Configuration - Optimized] " << getTopology() << endl;
    cdebug_log(145,0) << "getConnexity().globals:" << (int)getConnexity().fields.globals << endl;

    uint64_t  flags       = NoFlags;
    bool      eastPad     = false;
    bool      westPad     = false;
    bool      northPad    = false;
    bool      southPad    = false;
    Instance* padInstance = getRoutingPads()[0]->getOccurrence().getPath().getHeadInstance();

    switch ( padInstance->getTransformation().getOrientation() ) {
      case Transformation::Orientation::ID: northPad = true; break;
      case Transformation::Orientation::MY: southPad = true; break;
      case Transformation::Orientation::YR:
      case Transformation::Orientation::R3: eastPad  = true; flags |= HAccess; break;
      case Transformation::Orientation::R1: westPad  = true; flags |= HAccess; break;
      default:
        cerr << Warning( "Unmanaged orientation %s for pad <%s>."
                       , getString(padInstance->getTransformation().getOrientation()).c_str()
                       , getString(padInstance).c_str() ) << endl;
        break;
    }
    cdebug_log(145,0) << "eastPad:"  << eastPad  << ", "
                      << "westPad:"  << westPad  << ", "
                      << "northPad:" << northPad << ", "
                      << "southPad:" << southPad
                      << endl;

    AutoContact* source = doRp_AccessPad( getRoutingPads()[0], flags );
    // Point        position     = getRoutingPads()[0]->getCenter();
    // AutoContact* source       = NULL;
    // GCell*       gcell        = Session::getAnabatic()->getGCellGrid()->getGCell(position);

    // source = AutoContactTerminal::create ( gcell
    //                                      , getRoutingPads()[0]
    //                                      , Session::getContactLayer(3)
    //                                      , position
    //                                      , Session::getViaWidth(3), Session::getViaWidth(3)
    //                                      );
    // source->setFlags( CntFixed );

    // if (northPad or eastPad) {
    //   getSouthWestContact() = getNorthEastContact() = source;
    //   cdebug_tabw(145,-1);
    //   return;
    // }

  // Check for straight lines, which are not managed by _do_xG().
    if (getConnexity().fields.globals == 1) {
      if (  (westPad and (east() != NULL))
         or (eastPad and (west() != NULL)) ) {
        AutoContact* turn = AutoContactTurn::create( getGCell(), getNet(), Session::getContactLayer(1) );
        setBothCornerContacts( AutoContactTurn::create( getGCell(), getNet(), Session::getContactLayer(1) ) );
        AutoSegment::create( source, turn, Flags::Horizontal );
        AutoSegment::create( turn, getNorthEastContact(), Flags::Vertical );
        cdebug_tabw(145,-1);
        return true;
      } else if (  (southPad and (north() != NULL))
                or (northPad and (south() != NULL)) ) {
        AutoContact* turn = AutoContactTurn::create( getGCell(), getNet(), Session::getContactLayer(1) );
        setBothCornerContacts( AutoContactTurn::create( getGCell(), getNet(), Session::getContactLayer(1) ) );
        AutoSegment::create( source, turn, Flags::Vertical );
        AutoSegment::create( turn, getNorthEastContact(), Flags::Horizontal );
        cdebug_tabw(145,-1);
        return true;
      }
    }

    ++getConnexity().fields.globals;
    --getConnexity().fields.Pad;

    if (westPad ) addToWests ( source->getBodyHook() );
    if (eastPad ) addToEasts ( source->getBodyHook() );
    if (southPad) addToSouths( source->getBodyHook() );
    if (northPad) addToNorths( source->getBodyHook() );

    _do_xG();

    if (westPad) {
      AutoSegment::create( source, getSouthWestContact(), Flags::Horizontal );
      clearWests();
    }
    if (eastPad) {
      AutoSegment::create( source, getNorthEastContact(), Flags::Horizontal );
      clearEasts();
    }
    if (southPad) {
      AutoSegment::create( source, getSouthWestContact(), Flags::Vertical );
      clearSouths();
    }
    if (northPad) {
      AutoSegment::create( source, getNorthEastContact(), Flags::Vertical );
      clearNorths();
    }
    --(getConnexity().fields.globals);

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHV::_do_1G_1PinM2 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_1G_1PinM2() [Managed Configuration - Optimized] " << getTopology() << endl;

    AutoContact* rpContact = doRp_Access( getGCell(), getRoutingPads()[0], NoFlags );
    AutoContact* turn1     = AutoContactTurn::create( getGCell(), getNet(), Session::getContactLayer(1) );
    AutoSegment::create( rpContact, turn1, Flags::Vertical );

    if (north() or south()) {
      AutoContact* turn2 = AutoContactTurn::create( getGCell(), getNet(), Session::getContactLayer(1) );
      AutoSegment::create( turn1, turn2, Flags::Horizontal );
      turn1 = turn2;
    }
    setBothCornerContacts( turn1 );

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHV::_do_xG_1M1_1M2 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_xG_1M1_1M2() [Managed Configuration]" << endl;

    Component* rpL1;
    Component* rpL2;
    if (getRoutingPads()[0]->getLayer() == Session::getRoutingLayer(0)) {
      rpL1 = getRoutingPads()[0];
      rpL2 = getRoutingPads()[1];
    } else {
      rpL1 = getRoutingPads()[1];
      rpL2 = getRoutingPads()[0];
    }
    cdebug_log(145,0) << "rpL1 := " << rpL1 << endl;
    cdebug_log(145,0) << "rpL2 := " << rpL2 << endl;

    AutoContact* rpL1ContactSource = NULL;
    AutoContact* rpL1ContactTarget = NULL;
    AutoContact* rpL2ContactSource = NULL;
    AutoContact* rpL2ContactTarget = NULL;

    doRp_AutoContacts( getGCell(), rpL1, rpL1ContactSource, rpL1ContactTarget, NoFlags );
    doRp_AutoContacts( getGCell(), rpL2, rpL2ContactSource, rpL2ContactTarget, NoFlags );

    const Layer* viaLayer1 = Session::getContactLayer(1);
    const Layer* viaLayer2 = Session::getContactLayer(2);

    AutoContact* subContact = AutoContactTurn::create( getGCell(), getNet(), viaLayer1 );
    AutoSegment::create( rpL1ContactSource, subContact, Flags::Horizontal );
    AutoSegment::create( rpL2ContactSource, subContact, Flags::Vertical );

    if (south() or west()) {
      doRp_AutoContacts( getGCell(), rpL2, rpL2ContactSource, rpL2ContactTarget, DoSourceContact );
      if (south() and west()) {
        setSouthWestContact( AutoContactHTee::create( getGCell(), getNet(), viaLayer2 ) );
        AutoSegment::create( rpL2ContactSource, getSouthWestContact(), Flags::Horizontal );
      } else {
        if (south()) {
          setSouthWestContact( AutoContactTurn::create( getGCell(), getNet(), viaLayer2 ) );
          AutoSegment::create( rpL2ContactSource, getSouthWestContact(), Flags::Horizontal );
        } else {
          setSouthWestContact( rpL2ContactSource );
        }
      }
    }

    if (north() or east()) {
      doRp_AutoContacts( getGCell(), rpL2, rpL2ContactSource, rpL2ContactTarget, DoTargetContact );
      if (north() and east()) {
        setNorthEastContact( AutoContactHTee::create( getGCell(), getNet(), viaLayer2 ) );
        AutoSegment::create( rpL2ContactTarget, getNorthEastContact(), Flags::Horizontal );
      } else {
        if (north()) {
          setNorthEastContact( AutoContactTurn::create( getGCell(), getNet(), viaLayer2 ) );
          AutoSegment::create( rpL2ContactTarget, getNorthEastContact(), Flags::Horizontal );
        } else {
          setNorthEastContact( rpL2ContactTarget );
        }
      }
    }

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHV::_do_xG_xM1_xM3 ()
  {
    cdebug_log(145,1) << getTypeName()
                      << "::_do_xG_"  << (int)getConnexity().fields.M1
                      << "M1_"        << (int)getConnexity().fields.M3
                      << "M3() [G:"   << (int)getConnexity().fields.globals << " Managed Configuration]" << endl;
    cdebug_log(145,0) << "getConnexity(): " << getConnexity().connexity << endl;
    cdebug_log(145,0) << "north:     " << north() << endl;
    cdebug_log(145,0) << "south:     " << south() << endl;
    cdebug_log(145,0) << "east:      " << east() << endl;
    cdebug_log(145,0) << "west:      " << west() << endl;

    Component* rpM3 = NULL;
    if (getRoutingPads()[0]->getLayer() == Session::getRoutingLayer(2))
      rpM3 = getRoutingPads()[0];

    sortRpByX( getRoutingPads(), NoFlags ); // increasing X.
    for ( size_t i=1 ; i<getRoutingPads().size() ; ++i ) {
      AutoContact* leftContact  = doRp_Access( getGCell(), getRoutingPads()[i-1], HAccess );
      AutoContact* rightContact = doRp_Access( getGCell(), getRoutingPads()[i  ], HAccess );
      AutoSegment::create( leftContact, rightContact, Flags::Horizontal );

      if (not rpM3 and (getRoutingPads()[i]->getLayer() == Session::getRoutingLayer(2)))
        rpM3 = getRoutingPads()[i];
    }

    const Layer* viaLayer1     = Session::getContactLayer(1);
    AutoContact* unusedContact = NULL;

    if (rpM3) {
    // At least one M3 RoutingPad is present: use it.
      if (west() and not south()) {
        setSouthWestContact( doRp_Access( getGCell(), getRoutingPads()[0], HAccess ) );
      } else if (not west() and south()) {
        doRp_AutoContacts( getGCell(), rpM3, getSouthWestContact(), unusedContact, DoSourceContact );
      } else if (west() and south()) {
        AutoContact* rpContact = NULL;
        doRp_AutoContacts( getGCell(), rpM3, rpContact, unusedContact, DoSourceContact );
        setSouthWestContact( AutoContactVTee::create( getGCell(), getNet(), viaLayer1 ) );
        AutoSegment::create( rpContact, getSouthWestContact(), Flags::Vertical );
      }

      if (east() and not north()) {
        setNorthEastContact( doRp_Access( getGCell(), getRoutingPads()[getRoutingPads().size()-1], HAccess ) );
      } else if (not east() and north()) {
        doRp_AutoContacts( getGCell(), rpM3, unusedContact, getNorthEastContact(), DoTargetContact );
      } else if (east() and north()) {
        AutoContact* rpContact = NULL;
        doRp_AutoContacts( getGCell(), rpM3, unusedContact, rpContact, DoTargetContact );
        setNorthEastContact( AutoContactVTee::create( getGCell(), getNet(), viaLayer1 ) );
        AutoSegment::create( rpContact, getNorthEastContact(), Flags::Vertical );
      }
    } else {
    // All RoutingPad are M1.
      Component* southWestRp = getRoutingPads()[0];
      cdebug_log(145,0) << "| Initial S-W Global RP: " << southWestRp << endl;
      for ( size_t i=1 ; i<getRoutingPads().size() ; ++i ) {
        if (southWestRp->getBoundingBox().getHeight() >= 4*Session::getPitch(1)) break;
        if (getRoutingPads()[i]->getBoundingBox().getHeight() > southWestRp->getBoundingBox().getHeight()) {
          cdebug_log(145,0) << "| Better RP: " << southWestRp << endl;
          southWestRp = getRoutingPads()[i];
        }
      }

      if (west() and not south()) {
        setSouthWestContact( doRp_Access( getGCell(), southWestRp, HAccess ) );
      } else if (not west() and south()) {
        AutoContact* rpContact = doRp_Access( getGCell(), southWestRp, HAccess );
        setSouthWestContact( AutoContactTurn::create( getGCell(), getNet(), viaLayer1 ) );
        AutoSegment::create( rpContact, getSouthWestContact(), Flags::Horizontal );
      } else if (west() and south()) {
        AutoContact* rpContact = doRp_Access( getGCell(), southWestRp, HAccess );
        setSouthWestContact( AutoContactHTee::create( getGCell(), getNet(), viaLayer1 ) );
        AutoSegment::create( rpContact, getSouthWestContact(), Flags::Horizontal );
      }

      Component* northEastRp = getRoutingPads()[getRoutingPads().size()-1];
      cdebug_log(145,0) << "| Initial N-E Global RP: " << northEastRp << endl;

      if (getRoutingPads().size() > 1) {
        for ( size_t i=getRoutingPads().size()-1; i != 0 ; ) {
          i -= 1;
          if (northEastRp->getBoundingBox().getHeight() >= 4*Session::getPitch(1)) break;
          if (getRoutingPads()[i]->getBoundingBox().getHeight() > northEastRp->getBoundingBox().getHeight()) {
            cdebug_log(145,0) << "| Better RP: " << northEastRp << endl;
            northEastRp = getRoutingPads()[i];
          }
        } 
      }

      if (east() and not north()) {
        setNorthEastContact( doRp_Access( getGCell(), northEastRp, HAccess ) );
      } else if (not east() and north()) {
        AutoContact* rpContact = doRp_Access( getGCell(), northEastRp, HAccess );
        setNorthEastContact( AutoContactTurn::create( getGCell(), getNet(), viaLayer1 ) );
        AutoSegment::create( rpContact, getNorthEastContact(), Flags::Horizontal );
      } else if (east() and north()) {
        AutoContact* rpContact = doRp_Access( getGCell(), northEastRp, HAccess );
        setNorthEastContact( AutoContactHTee::create( getGCell(), getNet(), viaLayer1 ) );
        AutoSegment::create( rpContact, getNorthEastContact(), Flags::Horizontal );
      }
    }

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHV::_do_4G_1M2 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_4G_1M2() [Managed Configuration]" << endl;

    Component* rpL2 = getRoutingPads()[0];
    cdebug_log(145,0) << "rpL2 := " << rpL2 << endl;

    AutoContact* rpL2ContactSource = NULL;
    AutoContact* rpL2ContactTarget = NULL;

    doRp_AutoContacts( getGCell(), rpL2, rpL2ContactSource, rpL2ContactTarget, DoSourceContact|DoTargetContact );

    const Layer* viaLayer2 = Session::getContactLayer(2);

    setSouthWestContact( AutoContactHTee::create( getGCell(), getNet(), viaLayer2 ) );
    setNorthEastContact( AutoContactHTee::create( getGCell(), getNet(), viaLayer2 ) );

    AutoSegment::create( getSouthWestContact(), rpL2ContactSource, Flags::Horizontal );
    AutoSegment::create( rpL2ContactTarget, getNorthEastContact(), Flags::Horizontal );

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHV::_do_xG_xM2 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_"
                      << (int)getConnexity().fields.globals << "G_"
                      << (int)getConnexity().fields.M2 << "M2() [Managed Configuration - x]" << endl;

    Component* biggestRp = getRoutingPads()[0];
    for ( size_t i=1 ; i<getRoutingPads().size() ; ++i ) {
      doRp_StairCaseH( getGCell(), getRoutingPads()[i-1], getRoutingPads()[i] );
      if (getRoutingPads()[i]->getBoundingBox().getWidth() > biggestRp->getBoundingBox().getWidth())
        biggestRp = getRoutingPads()[i];
    }

    const Layer* viaLayer1 = Session::getContactLayer(1);
    AutoContact* unusedContact = NULL;

    if (west() and not south()) {
      doRp_AutoContacts( getGCell(), getRoutingPads()[0], getSouthWestContact(), unusedContact, DoSourceContact );
    } else if (not west() and south()) {
      setSouthWestContact( doRp_Access( getGCell(), biggestRp, NoFlags ) );
    } else if (west() and south()) {
      AutoContact* rpContact = doRp_Access( getGCell(), biggestRp, NoFlags );
      setSouthWestContact( AutoContactVTee::create( getGCell(), getNet(), viaLayer1 ) );
      AutoSegment::create( rpContact, getSouthWestContact(), Flags::Vertical );
    }

    if (east() and not north()) {
      doRp_AutoContacts( getGCell(), getRoutingPads()[getRoutingPads().size()-1], getNorthEastContact(), unusedContact, DoSourceContact );
    } else if (not east() and north()) {
      setNorthEastContact( doRp_Access( getGCell(), biggestRp, NoFlags ) );
    } else if (east() and north()) {
      AutoContact* rpContact = doRp_Access( getGCell(), biggestRp, NoFlags );
      setNorthEastContact( AutoContactVTee::create( getGCell(), getNet(), viaLayer1 ) );
      AutoSegment::create( rpContact, getNorthEastContact(), Flags::Vertical );
    }

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHV::_do_1G_1M3 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_1G_1M3() [Optimised Configuration]" << endl;

    uint64_t flags = (east() or west()) ? HAccess : NoFlags;
    flags |= (north()) ? DoTargetContact : NoFlags;
    flags |= (south()) ? DoSourceContact : NoFlags;

    doRp_AutoContacts( getGCell()
                     , getRoutingPads()[0]
                     , getSouthWestContact()
                     , getNorthEastContact()
                     , flags
                     );
    if (not getSouthWestContact()) setSouthWestContact( getNorthEastContact() );
    if (not getNorthEastContact()) setNorthEastContact( getSouthWestContact() );

    cdebug_log(145,0) << "_southWest: " << getSouthWestContact() << endl;
    cdebug_log(145,0) << "_northEast: " << getNorthEastContact() << endl;

    const Layer* viaLayer1 = Session::getContactLayer(1);

    if (flags & HAccess) {
    // HARDCODED VALUE.
      if (getRoutingPads()[0]->getBoundingBox().getHeight() < 3*Session::getPitch(1)) {
        AutoContact* subContact = AutoContactTurn::create( getGCell(), getNet(), viaLayer1 );
        AutoSegment::create( getSouthWestContact(), subContact, Flags::Vertical );

        setBothCornerContacts( subContact );
      }
    } else {
      if (getSourceContact()) {
        if (getSourceContact()->getX() != getSouthWestContact()->getX()) {
          AutoContactTurn* turn1 = AutoContactTurn::create( getGCell(), getNet(), viaLayer1 );
          AutoContactTurn* turn2 = AutoContactTurn::create( getGCell(), getNet(), viaLayer1 );
          AutoSegment::create( getSouthWestContact(), turn1, Flags::Vertical   );
          AutoSegment::create( turn1                , turn2, Flags::Horizontal );
          setBothCornerContacts( turn2 );
        }
      }
    }
    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHV::_do_xG_xM3 ()
  {
    cdebug_log(145,1) << getTypeName()
                      << "::_do_xG_" << (int)getConnexity().fields.M3
                      << "M3() [Managed Configuration]" << endl;
    cdebug_log(145,0) << "west:"  << west()  << endl;
    cdebug_log(145,0) << "east:"  << east()  << endl;
    cdebug_log(145,0) << "south:" << south() << endl;
    cdebug_log(145,0) << "north:" << north() << endl;

    sortRpByY( getRoutingPads(), NoFlags ); // increasing Y.
    for ( size_t i=1 ; i<getRoutingPads().size() ; i++ ) {
      doRp_StairCaseV( getGCell(), getRoutingPads()[i-1], getRoutingPads()[i] );
    }

    const Layer* viaLayer1     = Session::getContactLayer(1);
    AutoContact* unusedContact = NULL;
    Component*   rp            = getRoutingPads()[0];

    if (west() and not south()) {
      setSouthWestContact( doRp_Access( getGCell(), rp, HAccess ) );
    } else if (not west() and south()) {
      doRp_AutoContacts( getGCell(), rp, getSouthWestContact(), unusedContact, DoSourceContact );
      if (getSourceContact()) {
        if (getSourceContact()->getX() != getSouthWestContact()->getX()) {
          cdebug_log(149,0) << "Misaligned South: _source:" << DbU::getValueString(getSourceContact()->getX())
                            << "_southWest:"                << DbU::getValueString(getSouthWestContact()->getX()) << endl;

          AutoContactTurn* turn1 = AutoContactTurn::create( getGCell(), getNet(), viaLayer1 );
          AutoContactTurn* turn2 = AutoContactTurn::create( getGCell(), getNet(), viaLayer1 );
          AutoSegment::create( getSouthWestContact(), turn1, Flags::Vertical );
          AutoSegment::create( turn1            , turn2, Flags::Horizontal );
          setSouthWestContact( turn2 );
        }
      }
    } else if (west() and south()) {
      AutoContact* rpContact = NULL;
      doRp_AutoContacts( getGCell(), rp, rpContact, unusedContact, DoSourceContact );
      setSouthWestContact( AutoContactVTee::create( getGCell(), getNet(), viaLayer1 ) );
      AutoSegment::create( rpContact, getSouthWestContact(), Flags::Vertical );
    }

    rp = getRoutingPads()[getRoutingPads().size()-1];
    if (east() and not north()) {
      setNorthEastContact( doRp_Access( getGCell(), rp, HAccess ) );
    } else if (not east() and north()) {
      doRp_AutoContacts( getGCell(), rp, unusedContact, getNorthEastContact(), DoTargetContact );
      if (getSourceContact()) {
        if (getSourceContact()->getX() != getNorthEastContact()->getX()) {
          cdebug_log(149,0) << "Misaligned North: _source:" << DbU::getValueString(getSourceContact()->getX())
                            << "_southWest:"                << DbU::getValueString(getNorthEastContact()->getX()) << endl;

          AutoContactTurn* turn1 = AutoContactTurn::create( getGCell(), getNet(), viaLayer1 );
          AutoContactTurn* turn2 = AutoContactTurn::create( getGCell(), getNet(), viaLayer1 );
          AutoSegment::create( getNorthEastContact(), turn1, Flags::Vertical   );
          AutoSegment::create( turn1            , turn2, Flags::Horizontal );
          setNorthEastContact( turn2 );
        }
      }
    } else if (east() and north()) {
      AutoContact* rpContact = NULL;
      doRp_AutoContacts( getGCell(), rp, unusedContact, rpContact, DoTargetContact );
      setNorthEastContact( AutoContactVTee::create( getGCell(), getNet(), viaLayer1 ) );
      AutoSegment::create( rpContact, getNorthEastContact(), Flags::Vertical );
    }

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHV::_do_globalSegment ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_globalSegment()" << endl;

    if (getSourceContact()) {
      AutoContact* targetContact
        = ( getSegmentHookType(getFromHook()) & (NorthBound|EastBound) )
        ? getNorthEastContact() : getSouthWestContact() ;
      AutoSegment* globalSegment = AutoSegment::create( getSourceContact()
                                                      , targetContact
                                                      , static_cast<Segment*>( getFromHook()->getComponent() )
                                                      );
      globalSegment->setFlags( (getDegree() == 2) ? AutoSegment::SegBipoint : 0 );
      cdebug_log(145,0) << "Create global segment: " << globalSegment << endl;
  
    // HARDCODED VALUE.
      if ( (getTopology() & Global_Fixed) and (globalSegment->getLength() < 2*Session::getSliceHeight()) )
        addToFixSegments( globalSegment );
        
      if (getConnexity().fields.globals < 2) {
        cdebug_tabw(145,-1);
        return false;
      }
    } else
      setFromHook( NULL );
    
    push( east (), getNorthEastContact() );
    push( west (), getSouthWestContact() );
    push( north(), getNorthEastContact() );
    push( south(), getSouthWestContact() );

    cdebug_tabw(145,-1);
    return true;
  }


  string  NetBuilderHV::getTypeName () const
  { return "NetBuilderHV"; }


}  // Anabatic namespace.
