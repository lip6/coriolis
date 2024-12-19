// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2008-2022, All Rights Reserved
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
#include "anabatic/NetBuilderHV.h"
#include "anabatic/AnabaticEngine.h"


namespace Anabatic {

  using std::swap;
  using Hurricane::Transformation;
  using Hurricane::Warning;
  using Hurricane::Error;
  using Hurricane::Pin;

  
  NetBuilderHV::NetBuilderHV ()
    : NetBuilder()
  { }


  NetBuilderHV::~NetBuilderHV () { }


  string  NetBuilderHV::getStyle ()
  { return "HV,3RL+"; }

  
  void  NetBuilderHV::doRp_AutoContacts ( GCell*        gcell
                                        , RoutingPad*   rp
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

    Session::getPositions( rp, sourcePosition, targetPosition );

    GCell* sourceGCell = Session::getAnabatic()->getGCellUnder( sourcePosition );
    GCell* targetGCell = Session::getAnabatic()->getGCellUnder( targetPosition );

    if (rpDepth == 0) {
      rpLayer   = Session::getBuildContactLayer(0);
      direction = Flags::Horizontal;
      viaSide   = Session::getViaWidth( rpDepth );
    }

    if (sourcePosition != targetPosition) {
      if (flags & DoSourceContact)
        source = AutoContactTerminal::create( gcell
                                            , rp
                                            , rpLayer
                                            , sourcePosition
                                            , viaSide, viaSide
                                            );
      if (flags & DoTargetContact)
        target = AutoContactTerminal::create( gcell
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


  AutoContact* NetBuilderHV::doRp_Access ( GCell* gcell, RoutingPad* rp, uint64_t flags )
  {
    cdebug_log(145,1) << getTypeName() << "::doRp_Access() - flags:" << flags << endl;
    if (rp->isM1Offgrid()) {
      cdebug_tabw(145,-1);
      return doRp_AccessOffgrid( gcell, rp, flags );
    }

    size_t       rpDepth         = Session::getLayerDepth( rp->getLayer() );
    AutoContact* rpSourceContact = NULL;
    AutoContact* rpContactTarget = NULL;

    Flags useNonPref = Flags::NoFlags;
    if (flags & UseNonPref) useNonPref |= Flags::UseNonPref;
    
    flags |= checkRoutingPadSize( rp );

    doRp_AutoContacts( gcell, rp, rpSourceContact, rpContactTarget, flags );

    cdebug_log(145,0) << "flags: " << flags << endl;
    if ((rpDepth == 0) or (rpDepth == 2)) {
      const Layer* contactLayer = Session::getBuildContactLayer( 1 );
      size_t       wireDepth    = RoutingGauge::nlayerdepth;
      cdebug_log(145,0) << "case: METAL1 or METAL3 RoutingPad." << endl;
      if (rpDepth == 2) {
        cdebug_log(145,0) << "METAL3 override." << endl;
        contactLayer = Session::getRoutingLayer( 2 );
        wireDepth    = 2;
        flags       &= ~(Punctual|VSmall);
      }

      if (flags & HAccess) {
        cdebug_log(145,0) << "case: HAccess" << endl;
#ifdef NEW_IMPL
        if ((flags & Punctual) or ((flags & VSmall) and not (flags & UseNonPref)) ) {
          cdebug_log(145,0) << "case: Punctual or (VSmall and not UseNonPref)" << endl;
          AutoContact* subContact1 = AutoContactTurn::create( gcell, rp->getNet(), contactLayer );
          AutoSegment::create( rpSourceContact, subContact1, Flags::Horizontal, wireDepth );
          rpSourceContact = subContact1;
          subContact1 = AutoContactTurn::create( gcell, rp->getNet(), Session::getBuildContactLayer(rpDepth+1) );
          AutoSegment::create( rpSourceContact, subContact1, Flags::Vertical );
          rpSourceContact = subContact1;
        } else {
          cdebug_log(145,0) << "case: VSmall or UseNonPref" << endl;
          if ((flags & VSmall) or (flags & UseNonPref)) {
            AutoContact* subContact1 = AutoContactTurn::create( gcell, rp->getNet(), Session::getBuildContactLayer(rpDepth+1) );
            AutoSegment::create( rpSourceContact, subContact1, Flags::Vertical|Flags::UseNonPref );
            rpSourceContact = subContact1;
          }
        }
#endif

        if ( ((flags & VSmall) and not ((flags & UseNonPref))) or (flags & Punctual) ) {
          cdebug_log(145,0) << "case: VSmall and *not* UseNonPref" << endl;
  
          AutoContact* subContact1 = AutoContactTurn::create( gcell, rp->getNet(), contactLayer );
          AutoSegment::create( rpSourceContact, subContact1, Flags::Horizontal, wireDepth );
          rpSourceContact = subContact1;

          flags &= ~UseNonPref;
          useNonPref.reset( Flags::UseNonPref );
        }

        if (flags & (VSmall|UseNonPref)) {
          cdebug_log(145,0) << "case: UseNonPref" << endl;

          if (flags & VSmall) {
            useNonPref.reset( Flags::UseNonPref );
          }
          AutoContact* subContact1 = AutoContactTurn::create( gcell, rp->getNet(), Session::getBuildContactLayer(rpDepth+1) );
          AutoSegment::create( rpSourceContact, subContact1, Flags::Vertical|useNonPref );
          rpSourceContact = subContact1;
        }
      } else {
        if (flags & HSmall) {
          cdebug_log(145,0) << "case: HSmall" << endl;
  
          AutoContact* subContact1 = rpSourceContact;
          AutoContact* subContact2 = AutoContactTurn::create( gcell, rp->getNet(), Session::getBuildContactLayer(rpDepth+1) );
          AutoSegment::create( subContact1, subContact2, Flags::Horizontal );
          rpSourceContact = subContact2;
  
          if (flags & Punctual) {
            cdebug_log(145,0) << "case: HSmall + Punctual" << endl;
            subContact1 = subContact2;
            subContact2 = AutoContactTurn::create( gcell, rp->getNet(), Session::getBuildContactLayer(rpDepth+1) );
            AutoSegment::create( subContact1, subContact2, Flags::Vertical, rpDepth+2 );
  
            subContact1 = subContact2;
            subContact2 = AutoContactTurn::create( gcell, rp->getNet(), Session::getBuildContactLayer(rpDepth+1) );
            AutoSegment::create( subContact1, subContact2, Flags::Horizontal, rpDepth+1 );
  
            rpSourceContact = subContact2;
          }
        }
      }
    } else {
      cdebug_log(145,0) << "case: METAL2." << endl;
      AutoContact* subContact1 = NULL;

      if (flags & HAccess) {
        cdebug_log(145,0) << "HAccess" << endl;
        if (flags & HAccessEW) {
          cdebug_log(145,0) << "HAccessEW" << endl;
          subContact1 = AutoContactHTee::create( gcell, rp->getNet(), Session::getBuildContactLayer(rpDepth+1) );
        } else
          subContact1 = AutoContactTurn::create( gcell, rp->getNet(), Session::getBuildContactLayer(rpDepth+1) );

        AutoSegment* segment = AutoSegment::create( rpSourceContact, subContact1, Flags::Vertical, rpDepth+1 );
        if (rp->isPunctual())
          segment->setFlags( AutoSegment::SegFixed );
      } else {
#if OFFGRID_M2_DISABLED
        Box                cellAb    = getAnabatic()->getCell()->getAbutmentBox();
        RoutingLayerGauge* lgM2      = Session::getLayerGauge( 1 );
        DbU::Unit          trackAxis = lgM2->getTrackPosition( cellAb.getYMin()
                                                             , cellAb.getYMax()
                                                             , rp->getY()
                                                             , Constant::Nearest );
        bool offGrid = (trackAxis != rp->getY());
        if (offGrid) {
          cdebug_log(145,0) << "Off grid, Misaligned M2 RoutingPad, add vertical strap" << endl;
          subContact1 = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
          AutoSegment::create( rpSourceContact, subContact1, Flags::Vertical );
          rpSourceContact = subContact1;
        }
#endif
        if (Session::getRoutingGauge()->isSuperPitched()) {
          cdebug_log(145,0) << "Vertical access & super-pitched" << endl;
          subContact1 = AutoContactTurn::create( gcell, rp->getNet(), Session::getBuildContactLayer(1) );
          AutoSegment::create( rpSourceContact, subContact1, Flags::Horizontal );
        } else {
          cdebug_log(145,0) << "Vertical access" << endl;
          subContact1 = rpSourceContact;
        }
      }
      rpSourceContact = subContact1;
    }

    cdebug_tabw(145,-1);
    return rpSourceContact;
  }


  AutoContact* NetBuilderHV::doRp_AccessOffgrid ( GCell* gcell, RoutingPad* rp, uint64_t flags )
  {
    cdebug_log(145,1) << getTypeName() << "::doRp_AccessOffgrid() - flags:" << flags << endl;

    size_t       rpDepth         = Session::getLayerDepth( rp->getLayer() );
    AutoContact* rpSourceContact = NULL;
    AutoContact* rpContactTarget = NULL;
    doRp_AutoContacts( gcell, rp, rpSourceContact, rpContactTarget, flags );

    AutoContact* subContact1 = AutoContactTurn::create( gcell, rp->getNet(), Session::getBuildRoutingLayer(rpDepth+1) );
    AutoSegment::create( rpSourceContact, subContact1, Flags::Vertical|Flags::UseNonPref );
    rpSourceContact = subContact1;

    subContact1 = AutoContactTurn::create( gcell, rp->getNet(), Session::getBuildContactLayer(rpDepth+1) );
    AutoSegment::create( rpSourceContact, subContact1, Flags::Horizontal );
    rpSourceContact = subContact1;

    subContact1 = AutoContactTurn::create( gcell, rp->getNet(), Session::getBuildContactLayer(rpDepth+1) );
    AutoSegment::create( rpSourceContact, subContact1, Flags::Vertical );
    rpSourceContact = subContact1;

    if (not (flags & HAccess)) {
      subContact1 = AutoContactTurn::create( gcell, rp->getNet(), Session::getBuildContactLayer(rpDepth+1) );
      AutoSegment::create( rpSourceContact, subContact1, Flags::Horizontal );
      rpSourceContact = subContact1;
    }

    cdebug_tabw(145,-1);

    return rpSourceContact;
  }


  AutoContact* NetBuilderHV::doRp_AccessNorthSouthPin ( GCell* gcell, RoutingPad* rp )
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


  AutoContact* NetBuilderHV::doRp_AccessEastWestPin ( GCell* gcell, RoutingPad* rp )
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


  bool  NetBuilderHV::_do_1G_1M1 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_1G_1M1() [Managed Configuration - Optimized] " << getTopology() << endl;

#if THIS_IS_DISABLED
    uint64_t  flags = NoFlags;
    if      (east() ) { flags |= HAccess; }
    else if (west() ) { flags |= HAccess; }
    else if (north()) { flags |= VSmall; }
    else if (south()) { flags |= VSmall; }

    setBothCornerContacts( doRp_Access(getGCell(),getRoutingPads()[0],flags) );
#endif

    if (north() or south()) {
      setBothCornerContacts( doRp_Access(getGCell(),getRoutingPads()[0],VSmall) );
    } else {
      setBothCornerContacts( doRp_Access( getGCell(), getRoutingPads()[0], UseNonPref|HAccess ) );
    }

    cdebug_tabw(145,-1);

    return true;
  }


  bool  NetBuilderHV::_do_1G_xM1 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_1G_" << (int)getConnexity().fields.M1 << "M1() [Managed Configuration]" << endl;

    sortRpByX( getRoutingPads(), NoFlags ); // increasing X.
    for ( size_t i=1 ; i<getRoutingPads().size() ; ++i ) {
      AutoContact* leftContact  = doRp_Access( getGCell(), getRoutingPads()[i-1], HAccess|UseNonPref );
      AutoContact* rightContact = doRp_Access( getGCell(), getRoutingPads()[i  ], HAccess|UseNonPref );
      AutoSegment::create( leftContact, rightContact, Flags::Horizontal );
    }

    RoutingPad* globalRp = NULL;
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
      globalContact = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
      AutoSegment::create( globalContact, turn, Flags::Horizontal );
    }

    setBothCornerContacts( globalContact );

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHV::_do_xG ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_xG()" << endl;

    size_t hDepth = Session::getDHorizontalDepth();
    size_t vDepth = Session::getDVerticalDepth();
    size_t cDepth = Session::getDContactDepth();
    if (getFlags() & ToUpperRouting) {
      hDepth += 2;
      vDepth += 2;
      cDepth += 2;
    }
    const Layer* viaLayer = Session::getBuildContactLayer( cDepth );

    if (getConnexity().fields.globals == 2) {
      setBothCornerContacts( AutoContactTurn::create( getGCell(), getNet(), viaLayer ) );
    } else if (getConnexity().fields.globals == 3) {
      if (east() and west()) {
        setSouthWestContact( AutoContactTurn::create( getGCell(), getNet(), viaLayer ) );
        setNorthEastContact( AutoContactVTee::create( getGCell(), getNet(), viaLayer ) );
        if (south()) swapCornerContacts();

        AutoSegment::create( getSouthWestContact(), getNorthEastContact(), Flags::Vertical, vDepth );
      } else {
        setSouthWestContact( AutoContactTurn::create( getGCell(), getNet(), viaLayer ) );
        setNorthEastContact( AutoContactHTee::create( getGCell(), getNet(), viaLayer ) );
        if (west()) swapCornerContacts();

        AutoSegment::create( getSouthWestContact(), getNorthEastContact(), Flags::Horizontal, hDepth );
      }
    } else { // fields.globals == 4.
      AutoContact* turn = AutoContactTurn::create( getGCell(), getNet(), viaLayer );
      setSouthWestContact( AutoContactHTee::create( getGCell(), getNet(), viaLayer ) );
      setNorthEastContact( AutoContactVTee::create( getGCell(), getNet(), viaLayer ) );
      AutoSegment::create( getSouthWestContact(), turn, Flags::Horizontal, hDepth );
      AutoSegment::create( turn, getNorthEastContact(), Flags::Vertical  , vDepth );
    } 
    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHV::_do_2G ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_2G()" << endl;

    size_t hDepth = Session::getDHorizontalDepth();
    size_t vDepth = Session::getDVerticalDepth();
    size_t cDepth = Session::getDContactDepth();
    if (getFlags() & ToUpperRouting) {
      hDepth += 2;
      vDepth += 2;
      cDepth += 2;
    }
    const Layer* viaLayer = Session::getBuildContactLayer( cDepth );

    if (east() and west()) {
      setSouthWestContact( AutoContactTurn::create( getGCell(), getNet(), viaLayer ) );
      setNorthEastContact( AutoContactTurn::create( getGCell(), getNet(), viaLayer ) );
      AutoSegment::create( getSouthWestContact(), getNorthEastContact(), Flags::Vertical, vDepth );
    } else if (south() and north()) {
      setSouthWestContact( AutoContactTurn::create( getGCell(), getNet(), viaLayer ) );
      setNorthEastContact( AutoContactTurn::create( getGCell(), getNet(), viaLayer ) );
      AutoSegment::create( getSouthWestContact(), getNorthEastContact(), Flags::Horizontal, hDepth );
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
    //                                      , Session::getBuildContactLayer(3)
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
        AutoContact* turn = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
        setBothCornerContacts( AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) ) );
        AutoSegment::create( source, turn, Flags::Horizontal );
        AutoSegment::create( turn, getNorthEastContact(), Flags::Vertical );
        cdebug_tabw(145,-1);
        return true;
      } else if (  (southPad and (north() != NULL))
                or (northPad and (south() != NULL)) ) {
        AutoContact* turn = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
        setBothCornerContacts( AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) ) );
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

    AutoContact* rpSourceContact = NULL;
    AutoContact* rpContactTarget = NULL;

    doRp_AutoContacts( getGCell(), getRoutingPads()[0], rpSourceContact, rpContactTarget, NoFlags );

    AutoContact* turn1   = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
    AutoSegment::create( rpSourceContact, turn1, Flags::Horizontal );

    if (east() or west()) {
      AutoContact* turn2 = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
      AutoSegment::create( turn1, turn2, Flags::Vertical );
      turn1 = turn2;
    }
    setBothCornerContacts( turn1 );

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHV::_do_1G_1PinM1 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_1G_1PinM1() [Managed Configuration - Optimized] " << getTopology() << endl;

    AutoContact* rpSourceContact = NULL;
    AutoContact* rpContactTarget = NULL;
    AutoContact* turn            = NULL;

    doRp_AutoContacts( getGCell(), getRoutingPads()[0], rpSourceContact, rpContactTarget, NoProtect );

    Pin* pin = dynamic_cast<Pin*>( getRoutingPads()[0]->getOccurrence().getEntity() );
    Pin::AccessDirection pinDir = pin->getAccessDirection();
    if (  (pinDir == Pin::AccessDirection::NORTH)
       or (pinDir == Pin::AccessDirection::SOUTH) ) {
      turn = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildRoutingLayer(1) );
      AutoSegment::create( rpSourceContact, turn, Flags::Vertical|Flags::UseNonPref );
      rpSourceContact = turn;

      turn = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
      AutoSegment* horizontal = AutoSegment::create( rpSourceContact, turn, Flags::Horizontal );
      rpSourceContact = turn;

      DbU::Unit axis = getGCell()->getYMax() - Session::getDHorizontalPitch();
      if (pinDir == Pin::AccessDirection::SOUTH)
        axis = getGCell()->getYMin() + Session::getDHorizontalPitch();
      cdebug_log(145,0) << "axis:" << DbU::getValueString(axis) << endl;
      
      horizontal->setAxis( axis, Flags::Force );
      horizontal->setFlags( AutoSegment::SegFixed|AutoSegment::SegFixedAxis );
      cdebug_log(145,0) << horizontal << endl;

      turn = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildRoutingLayer(1) );
      AutoSegment* vertical = AutoSegment::create( rpSourceContact, turn, Flags::Vertical );
      rpSourceContact = turn;
      vertical->setAxis( pin->getX(), Flags::Force );
      vertical->setFlags( AutoSegment::SegFixed|AutoSegment::SegFixedAxis );
      cdebug_log(145,0) << vertical << endl;

      turn = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
      horizontal = AutoSegment::create( rpSourceContact, turn, Flags::Horizontal );
      horizontal->setAxis( axis, Flags::Force );
      rpSourceContact = turn;
    } else {
      turn = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
      AutoSegment::create( rpSourceContact, turn, Flags::Horizontal );
      rpSourceContact = turn;
    }

    if (east() or west()) {
      rpSourceContact = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
      AutoSegment* vertical = AutoSegment::create( turn, rpSourceContact, Flags::Vertical );

      DbU::Unit axis = getGCell()->getXMax() - Session::getDVerticalPitch();
      if (pinDir == Pin::AccessDirection::WEST)
        axis = getGCell()->getXMin() + Session::getDVerticalPitch();
      cdebug_log(145,0) << "axis:" << DbU::getValueString(axis) << endl;
      
      vertical->setAxis( axis, Flags::Force );
      vertical->setFlags( AutoSegment::SegFixed|AutoSegment::SegFixedAxis );

      turn = rpSourceContact;
    }
    setBothCornerContacts( turn );

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHV::_do_2G_1PinM1 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_2G_1PinM1() [Managed Configuration - Optimized] " << getTopology() << endl;

    AutoContact* rpSourceContact = NULL;
    AutoContact* rpContactTarget = NULL;
    AutoContact* tee             = NULL;
    AutoContact* turn            = NULL;

    doRp_AutoContacts( getGCell(), getRoutingPads()[0], rpSourceContact, rpContactTarget, NoProtect );

    Pin* pin = dynamic_cast<Pin*>( getRoutingPads()[0]->getOccurrence().getEntity() );
    Pin::AccessDirection pinDir = pin->getAccessDirection();
    if (  (pinDir == Pin::AccessDirection::NORTH)
       or (pinDir == Pin::AccessDirection::SOUTH) ) {
      turn = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildRoutingLayer(1) );
      AutoSegment::create( rpSourceContact, turn, Flags::Vertical|Flags::UseNonPref );
      rpSourceContact = turn;

      turn = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
      AutoSegment* horizontal = AutoSegment::create( rpSourceContact, turn, Flags::Horizontal );
      rpSourceContact = turn;

      DbU::Unit axis = getGCell()->getYMax() - Session::getDHorizontalPitch();
      if (pinDir == Pin::AccessDirection::SOUTH)
        axis = getGCell()->getYMin() + Session::getDHorizontalPitch();
      horizontal->setAxis( axis, Flags::Force );
      horizontal->setFlags( AutoSegment::SegFixed );

      turn = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildRoutingLayer(1) );
      AutoSegment* vertical = AutoSegment::create( rpSourceContact, turn, Flags::Vertical );
      rpSourceContact = turn;
      vertical->setFlags( AutoSegment::SegFixed );

      turn = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
      horizontal = AutoSegment::create( rpSourceContact, turn, Flags::Horizontal );
      horizontal->setAxis( axis, Flags::Force );
      rpSourceContact = turn;
      turn            = NULL;
    }

    if (east() and west()) {
    // Pin must be North or South.
      tee = AutoContactHTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
      AutoSegment::create( rpSourceContact, tee, Flags::Vertical );
    } else if (north() and south()) {
    // Pin must be East or West.
      tee = AutoContactVTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
      AutoSegment::create( rpSourceContact, tee, Flags::Horizontal );
    } else {
      if (  (pinDir == Pin::AccessDirection::EAST)
         or (pinDir == Pin::AccessDirection::WEST) ) {
        turn = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
        AutoSegment::create( rpSourceContact, turn, Flags::Horizontal );
        rpSourceContact = turn;
      }

      tee = AutoContactVTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
      AutoSegment* vertical = AutoSegment::create( rpSourceContact, tee, Flags::Vertical );

      if (  (pinDir == Pin::AccessDirection::EAST)
         or (pinDir == Pin::AccessDirection::WEST) ) {
        DbU::Unit axis = getGCell()->getXMax() - Session::getDVerticalPitch();
        if (pinDir == Pin::AccessDirection::WEST)
          axis = getGCell()->getXMin() + Session::getDVerticalPitch();
        cdebug_log(145,0) << "axis:" << DbU::getValueString(axis) << endl;
      
        vertical->setAxis( axis, Flags::Force );
        vertical->setFlags( AutoSegment::SegFixed|AutoSegment::SegFixedAxis );
      }
    }

    setBothCornerContacts( tee );

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHV::_do_xG_1PinM2 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_xG_1PinM2() [Managed Configuration - Optimized] " << getTopology() << endl;

    AutoContact* rpSourceContact = NULL;
    AutoContact* rpContactTarget = NULL;
    AutoContact* turn            = NULL;

    doRp_AutoContacts( getGCell(), getRoutingPads()[0], rpSourceContact, rpContactTarget, NoFlags );

    Pin* pin = dynamic_cast<Pin*>( getRoutingPads()[0]->getOccurrence().getEntity() );
    Pin::AccessDirection pinDir = pin->getAccessDirection();
    if (  (pinDir == Pin::AccessDirection::EAST)
       or (pinDir == Pin::AccessDirection::WEST) ) {
      turn = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
      AutoSegment::create( rpSourceContact, turn, Flags::Horizontal );
      rpSourceContact = turn;

      turn = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
      AutoSegment* vertical = AutoSegment::create( rpSourceContact, turn, Flags::Vertical );
      rpSourceContact = turn;

      Box                cellAb = getAnabatic()->getCell()->getAbutmentBox();
      RoutingLayerGauge* lgM3   = Session::getLayerGauge( 2 );
      DbU::Unit axis = lgM3->getTrackPosition( cellAb.getXMin()
                                             , cellAb.getXMax()
                                             , getGCell()->getXMax() - Session::getDVerticalPitch()
                                             , Constant::Superior );
      if (pinDir == Pin::AccessDirection::WEST)
        axis = lgM3->getTrackPosition( cellAb.getXMin()
                                     , cellAb.getXMax()
                                     , getGCell()->getXMin() + Session::getDVerticalPitch()
                                     , Constant::Inferior );
      cdebug_log(145,0) << "axis:" << DbU::getValueString(axis) << endl;
      
      vertical->setAxis( axis, Flags::Force );
      vertical->setFlags( AutoSegment::SegFixedAxis );
    }

    if (getConnexity().fields.globals == 2) {
      if (west() and south()) {
        AutoContact* turn1 = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
        AutoSegment::create( rpSourceContact, turn1, Flags::Horizontal );

        AutoContact* vtee1 = AutoContactVTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
        AutoSegment::create( turn1, vtee1, Flags::Vertical );

        setBothCornerContacts( vtee1 );
      } else if (west() and north()) {
        AutoContact* vtee1 = AutoContactVTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
        AutoSegment::create( rpSourceContact, vtee1, Flags::Horizontal );

        AutoContact* turn1 = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
        AutoSegment::create( vtee1, turn1, Flags::Vertical );

        setSouthWestContact( turn1 );
        setNorthEastContact( vtee1 );
      } else if (south() and north()) {
        AutoContact* vtee1 = AutoContactVTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
        AutoSegment::create( rpSourceContact, vtee1, Flags::Horizontal );

        setBothCornerContacts( vtee1 );
      } else if (east() and north()) {
        AutoContact* htee1 = AutoContactHTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
        AutoSegment::create( rpSourceContact, htee1, Flags::Horizontal );

        setBothCornerContacts( htee1 );
      } else if (east() and south()) {
        AutoContact* htee1 = AutoContactHTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
        AutoSegment::create( rpSourceContact, htee1, Flags::Horizontal );

        setBothCornerContacts( htee1 );
      } else if (east() and west()) {
        AutoContact* turn1 = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
        AutoSegment::create( rpSourceContact, turn1, Flags::Horizontal );

        AutoContact* htee1 = AutoContactHTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
        AutoSegment::create( turn1, htee1, Flags::Vertical );

        setBothCornerContacts( htee1 );
      }
    } else {
      AutoContact* htee1 = AutoContactHTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
      AutoSegment::create( rpSourceContact, htee1, Flags::Horizontal );

      AutoContact* htee2 = AutoContactHTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
      AutoSegment::create( htee1, htee2, Flags::Horizontal );
      
      if (pinDir == Pin::AccessDirection::EAST)
        std::swap( htee1, htee2 );

      setSouthWestContact( htee1 );
      setNorthEastContact( htee2 );
    }

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHV::_do_1G_1PinM3 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_1G_1PinM3() [Managed Configuration - Optimized] " << getTopology() << endl;

    AutoContact* rpSourceContact = NULL;

    rpSourceContact = doRp_AccessNorthSouthPin( getGCell(), getRoutingPads()[0] );

    AutoContact* turn1 = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(0) );
    AutoSegment::create( rpSourceContact, turn1, Flags::Vertical );

    if (north() or south()) {
      AutoContact* turn2 = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
      AutoSegment::create( turn1, turn2, Flags::Horizontal );
      turn1 = turn2;
    }
    setBothCornerContacts( turn1 );

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHV::_do_xG_1PinM3 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_xG_1PinM3() [Managed Configuration - Optimized] " << getTopology() << endl;

    AutoContact* rpSourceContact = NULL;
    AutoContact* rpContactTarget = NULL;

    doRp_AutoContacts( getGCell(), getRoutingPads()[0], rpSourceContact, rpContactTarget, NoFlags );

    if (getConnexity().fields.globals == 2) {
      if (west() and south()) {
        AutoContact* turn1 = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
        AutoSegment::create( rpSourceContact, turn1, Flags::Vertical );

        AutoContact* htee1 = AutoContactHTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
        AutoSegment::create( turn1, htee1, Flags::Horizontal );

        setBothCornerContacts( htee1 );
      } else if (west() and north()) {
        AutoContact* htee1 = AutoContactHTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
        AutoSegment::create( rpSourceContact, htee1, Flags::Vertical );

        AutoContact* turn1 = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
        AutoSegment::create( turn1, htee1, Flags::Horizontal );

        setSouthWestContact( htee1 );
        setNorthEastContact( turn1 );
      } else if (east() and north()) {
        AutoContact* vtee1 = AutoContactVTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
        AutoSegment::create( rpSourceContact, vtee1, Flags::Vertical );

        setBothCornerContacts( vtee1 );
      } else if (east() and south()) {
        AutoContact* htee1 = AutoContactHTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
        AutoSegment::create( rpSourceContact, htee1, Flags::Vertical );

        AutoContact* turn1 = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
        AutoSegment::create( htee1, turn1, Flags::Horizontal );

        setSouthWestContact( turn1 );
        setNorthEastContact( htee1 );
      } else if (north() and south()) {
        AutoContact* turn1 = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
        AutoSegment::create( rpSourceContact, turn1, Flags::Vertical );

        AutoContact* vtee1 = AutoContactVTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
        AutoSegment::create( turn1, vtee1, Flags::Horizontal );

        setBothCornerContacts( vtee1 );
      } else {  // Remaining case is East & West.
        AutoContact* htee1 = AutoContactHTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
        AutoSegment::create( rpSourceContact, htee1, Flags::Vertical );

        setBothCornerContacts( htee1 );
      }
    } else {
      AutoContact* turn1 = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
      AutoContact* turn2 = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
      AutoSegment::create( rpSourceContact, turn1, Flags::Vertical );
      AutoSegment::create( turn1, turn2, Flags::Horizontal );

      AutoContact* vtee1 = AutoContactVTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
      AutoSegment::create( turn2, vtee1, Flags::Vertical );

      AutoContact* vtee2 = AutoContactVTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
      AutoSegment::create( vtee1, vtee2, Flags::Vertical );
      
      setSouthWestContact( vtee1 );
      setNorthEastContact( vtee2 );
    }

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHV::_do_2G_xM1_1PinM3 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_2G_xM1_1PinM3() [Managed Configuration - Optimized] " << getTopology() << endl;

    RoutingPad*  pinM3      = NULL;
    AutoContact* pinContact = NULL;
    AutoContact* dummy      = NULL;

    sortRpByX( getRoutingPads(), NoFlags ); // increasing X.

    vector<RoutingPad*> rpsM1;
    for ( RoutingPad* rp : getRoutingPads() ) {
      if (dynamic_cast<Pin*>(rp->getOccurrence().getEntity())) pinM3 = rp;
      else rpsM1.push_back( rp );
    }
    
    cdebug_log(145,0) << "Linking M1 terminals together" << endl;
    for ( size_t i=1 ; i<rpsM1.size() ; ++i ) {
      AutoContact* leftContact  = doRp_Access( getGCell(), rpsM1[i-1], HAccess );
      AutoContact* rightContact = doRp_Access( getGCell(), rpsM1[i  ], HAccess );
      AutoSegment::create( leftContact, rightContact, Flags::Horizontal );
    }
    cdebug_log(145,0) << "Linking M1 terminals together - Done" << endl;

    doRp_AutoContacts( getGCell(), pinM3, pinContact, dummy, NoFlags );

    AutoContact* m1contact  = doRp_Access( getGCell(), rpsM1.back(), HAccess );

    AutoContact* vtee1 = AutoContactVTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
    AutoSegment::create( m1contact , vtee1, Flags::Horizontal );
    AutoSegment::create( pinContact, vtee1, Flags::Vertical );

    AutoContact* gtee = nullptr;
    if (getStateG() == 1) {
      if (north() or south()) {
        gtee = vtee1;
      } else {
        gtee = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
        AutoSegment::create( vtee1, gtee, Flags::Vertical );
      }
    } else if (getStateG() == 2) {
      if (east() and west())
        gtee = AutoContactHTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
      else
        gtee = AutoContactVTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
      AutoSegment::create( vtee1, gtee, Flags::Vertical );
    }

    setBothCornerContacts( gtee );

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHV::_do_3G_xM1_1PinM3 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_3G_1M1_1PinM3() [Managed Configuration - Optimized] " << getTopology() << endl;

    RoutingPad*  pinM3      = NULL;
    AutoContact* pinContact = NULL;
    AutoContact* dummy      = NULL;

    sortRpByX( getRoutingPads(), NoFlags ); // increasing X.

    vector<RoutingPad*> rpsM1;
    for ( RoutingPad* rp : getRoutingPads() ) {
      if (dynamic_cast<Pin*>(rp->getOccurrence().getEntity())) pinM3 = rp;
      else rpsM1.push_back( rp );
    }
    
    for ( size_t i=1 ; i<rpsM1.size() ; ++i ) {
      AutoContact* leftContact  = doRp_Access( getGCell(), rpsM1[i-1], HAccess );
      AutoContact* rightContact = doRp_Access( getGCell(), rpsM1[i  ], HAccess );
      AutoSegment::create( leftContact, rightContact, Flags::Horizontal );
    }

    doRp_AutoContacts( getGCell(), pinM3, pinContact, dummy, NoFlags );

    AutoContact* m1contact  = doRp_Access( getGCell(), rpsM1.back(), HAccess );

    AutoContact* vtee1 = AutoContactVTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
    AutoSegment::create( m1contact , vtee1, Flags::Horizontal );

    AutoContact* vtee2 = AutoContactVTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
    AutoSegment::create( vtee1, vtee2, Flags::Vertical );

    AutoContact* vtee3 = AutoContactVTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
    AutoSegment::create( vtee2, vtee3, Flags::Vertical );
      
    if (not south() or not north()) {
      AutoSegment::create( pinContact, vtee1, Flags::Vertical );
    }
      
    if (not east() or not west()) {
      AutoSegment::create( pinContact, vtee2, Flags::Horizontal );
    }

    if (not south()) {
      setSouthWestContact( vtee2 );
      setNorthEastContact( vtee3 );
    } else if (not north()) {
      setSouthWestContact( vtee3 );
      setNorthEastContact( vtee2 );
    } else if (not east()) {
      setSouthWestContact( vtee3 );
      setNorthEastContact( vtee1 );
    } else if (not west()) {
      setSouthWestContact( vtee1 );
      setNorthEastContact( vtee3 );
    }

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHV::_do_1G_xM1_1PinM2 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_1G_xM1_1PinM2() [Managed Configuration - Optimized] " << getTopology() << endl;

    sortRpByX( getRoutingPads(), NoFlags ); // increasing X.

    vector<RoutingPad*> rpsM1;
    RoutingPad*         pinM2 = NULL;
    for ( RoutingPad* rp : getRoutingPads() ) {
      if (dynamic_cast<Pin*>(rp->getOccurrence().getEntity())) pinM2 = rp;
      else rpsM1.push_back( rp );
    }
    
    for ( size_t i=1 ; i<rpsM1.size() ; ++i ) {
      AutoContact* leftContact  = doRp_Access( getGCell(), rpsM1[i-1], HAccess );
      AutoContact* rightContact = doRp_Access( getGCell(), rpsM1[i  ], HAccess );
      AutoSegment::create( leftContact, rightContact, Flags::Horizontal );
    }

    AutoContact* turn         = NULL;
    AutoContact* tee          = NULL;
    AutoContact* pinM2Contact = NULL;
    AutoContact* rpM1Contact  = NULL;
    doRp_AutoContacts( getGCell(), pinM2, pinM2Contact, rpM1Contact, NoProtect );
    rpM1Contact = doRp_Access( getGCell(), rpsM1[0], (north() or south()) ? HAccess : NoFlags );

    if (north() or south()) {
      turn = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
      tee  = AutoContactVTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );

      AutoSegment::create( rpM1Contact , tee , Flags::Horizontal );
      AutoSegment::create( pinM2Contact, turn, Flags::Horizontal );
      AutoSegment::create( tee         , turn, Flags::Vertical );

      setBothCornerContacts( tee );
    } else {
      turn = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
      tee  = AutoContactVTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );

      AutoSegment::create( rpM1Contact , tee , Flags::Vertical );
      AutoSegment::create( pinM2Contact, tee , Flags::Horizontal );
      AutoSegment::create( tee         , turn, Flags::Vertical );

      setBothCornerContacts( turn );
    }

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHV::_do_2G_xM1_1PinM2 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_2G_xM1_1PinM2() [Managed Configuration - Optimized] " << getTopology() << endl;

    sortRpByX( getRoutingPads(), NoFlags ); // increasing X.

    vector<RoutingPad*> rpsM1;
    RoutingPad*         pinM2 = NULL;
    for ( RoutingPad* rp : getRoutingPads() ) {
      if (dynamic_cast<Pin*>(rp->getOccurrence().getEntity())) pinM2 = rp;
      else rpsM1.push_back( rp );
    }

    AutoContact* pinM2Contact = NULL;
    AutoContact* rpM1Contact  = NULL;
    doRp_AutoContacts( getGCell(), pinM2, pinM2Contact, rpM1Contact, NoFlags );
    rpM1Contact = doRp_Access( getGCell(), rpsM1[0], NoFlags );

    if (north() and south()) {
      cdebug_log(145,0) << "Case north & south." << endl;
      AutoContact* htee = AutoContactHTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
      AutoContact* vtee = AutoContactVTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );

      AutoSegment::create( pinM2Contact, htee, Flags::Horizontal );
      AutoSegment::create( rpM1Contact , htee, Flags::Vertical   );
      AutoSegment::create( htee        , vtee, Flags::Horizontal );
      setBothCornerContacts( vtee );
    } else if (east() and west()) {
      cdebug_log(145,0) << "Case east & west." << endl;
      AutoContact* htee1 = AutoContactHTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
      AutoContact* htee2 = AutoContactHTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
      AutoContact* turn  = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );

      AutoSegment::create( pinM2Contact, htee1, Flags::Horizontal );
      AutoSegment::create( rpM1Contact , htee1, Flags::Vertical   );
      AutoSegment::create( htee1       , turn , Flags::Horizontal );
      AutoSegment::create( htee2       , turn , Flags::Vertical   );
      setBothCornerContacts( htee2 );
    } else {
      cdebug_log(145,0) << "Case bend." << endl;
      AutoContact* vtee1 = AutoContactVTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
      AutoContact* htee2 = AutoContactHTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
      AutoContact* turn1 = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );

      AutoSegment::create( pinM2Contact, vtee1, Flags::Horizontal );
      AutoSegment::create( rpM1Contact , vtee1, Flags::Vertical   );
      AutoSegment::create( vtee1       , turn1, Flags::Vertical   );
      AutoSegment::create( turn1       , htee2, Flags::Horizontal );
      setBothCornerContacts( htee2 );
    }

    cdebug_log(145,0) << "Wiring all M1 together." << endl;
    for ( size_t i=1 ; i<rpsM1.size() ; ++i ) {
      AutoContact* leftContact  = doRp_Access( getGCell(), rpsM1[i-1], HAccess );
      AutoContact* rightContact = doRp_Access( getGCell(), rpsM1[i  ], HAccess );
      AutoSegment::create( leftContact, rightContact, Flags::Horizontal );
    }

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHV::_do_1G_1M1_1PinM3 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_1G_1M1_1PinM3() [Managed Configuration - Optimized] " << getTopology() << endl;

    AutoContact* rpSourceContact = NULL;

    RoutingPad* pinM3 = getRoutingPads()[0];
    RoutingPad* rpM1  = getRoutingPads()[1];
    if (dynamic_cast<Pin*>(rpM1->getOccurrence().getEntity())) std::swap( rpM1, pinM3 );

    AutoContact* contact1 = doRp_Access( getGCell(), rpM1, HAccess );
    AutoContact* htee     = AutoContactHTee::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
    AutoSegment::create( contact1, htee, Flags::Horizontal );

    rpSourceContact = doRp_AccessNorthSouthPin( getGCell(), pinM3 );

    if (north() or south()) {
      AutoContact* turn = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer(1) );
      AutoSegment::create( rpSourceContact, turn, Flags::Vertical   );
      AutoSegment::create( turn           , htee, Flags::Horizontal );
    } else {
      AutoSegment::create( rpSourceContact, htee, Flags::Vertical );
    }
    setBothCornerContacts( htee );

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHV::_do_2G_1M1 ()
  {
    return _do_xG_1M1();
  }


  bool  NetBuilderHV::_do_xG_1M1 ()
  {
    if ((int)getConnexity().fields.M1 != 1) return false;
    
    cdebug_log(145,1) << getTypeName()
                      << "::_do_xG_"  << (int)getConnexity().fields.M1
                      << "M1() [G:"   << (int)getConnexity().fields.globals << " Managed Configuration]" << endl;
    cdebug_log(145,0) << "getConnexity(): " << getConnexity().connexity << endl;
    cdebug_log(145,0) << "north:     " << north() << endl;
    cdebug_log(145,0) << "south:     " << south() << endl;
    cdebug_log(145,0) << "east:      " << east () << endl;
    cdebug_log(145,0) << "west:      " << west () << endl;

    const Layer* viaLayer1 = Session::getBuildContactLayer(1);

    if (getConnexity().fields.globals == 2) {
      if (north() and south()) {
        AutoContact* contact1 = doRp_Access( getGCell(), getRoutingPads()[0], HAccess );
        AutoContact* contact2 = AutoContactHTee::create( getGCell(), getNet(), viaLayer1 );
        AutoSegment::create( contact1, contact2, Flags::Horizontal );
        contact1 = AutoContactTurn::create( getGCell(), getNet(), viaLayer1 );
        AutoSegment::create( contact1, contact2, Flags::Horizontal );
        setNorthEastContact( contact1 );
        setSouthWestContact( contact2 );
      } else if (east() and west()) {
        // AutoContact* contact1 = doRp_Access( getGCell(), getRoutingPads()[0], NoFlags );
        // AutoContact* contact2 = AutoContactHTee::create( getGCell(), getNet(), viaLayer1 );
        // AutoSegment::create( contact1, contact2, Flags::Vertical );
        // setBothCornerContacts( contact2 );
        AutoContact* contact1  = NULL;
        AutoContact* contact2  = NULL;
        uint64_t     flags     = checkRoutingPadSize( getRoutingPads()[0] );

        if (flags & VSmall) {
        //doRp_AutoContacts( getGCell(), getRoutingPads()[0], contact1, contact2, NoFlags );
          contact1 = doRp_Access( getGCell(), getRoutingPads()[0], NoFlags );
          contact2 = AutoContactHTee::create( getGCell(), getNet(), viaLayer1 );
          AutoSegment::create( contact1, contact2, Flags::Vertical );

          setBothCornerContacts( contact2 );
        } else {
        //doRp_AutoContacts( getGCell(), getRoutingPads()[0], contact1, contact2, NoFlags );
          contact1 = doRp_Access( getGCell(), getRoutingPads()[0], HAccess );
          setSouthWestContact( contact1 );

        //doRp_AutoContacts( getGCell(), getRoutingPads()[0], contact1, contact2, NoFlags );
          contact1 = doRp_Access( getGCell(), getRoutingPads()[0], HAccess );
          setNorthEastContact( contact1 );
        }
      } else {
        AutoContact* contact1 = doRp_Access( getGCell(), getRoutingPads()[0], HAccess );
        AutoContact* contact2 = AutoContactTurn::create( getGCell(), getNet(), viaLayer1 );
        AutoSegment::create( contact1, contact2, Flags::Horizontal );

        contact1 = AutoContactVTee::create( getGCell(), getNet(), viaLayer1 );
        AutoSegment::create( contact1, contact2, Flags::Vertical );
        setBothCornerContacts( contact1 );
      }
    } else if (getConnexity().fields.globals == 3) {
      if (not west() or not east()) {
        AutoContact* contact1 = doRp_Access( getGCell(), getRoutingPads()[0], HAccess );
        AutoContact* contact2 = AutoContactVTee::create( getGCell(), getNet(), viaLayer1 );
        AutoSegment::create( contact1, contact2, Flags::Horizontal );
        AutoContact* contact3 = AutoContactVTee::create( getGCell(), getNet(), viaLayer1 );
        AutoSegment::create( contact2, contact3, Flags::Vertical );

        setSouthWestContact( (east()) ? contact2 : contact3 );
        setNorthEastContact( (east()) ? contact3 : contact2 );
      } else {
        AutoContact* contact1 = doRp_Access( getGCell(), getRoutingPads()[0], HAccess );
        AutoContact* contact2 = AutoContactTurn::create( getGCell(), getNet(), viaLayer1 );
        AutoSegment::create( contact1, contact2, Flags::Horizontal );
        contact1 = contact2;
        contact2 = AutoContactVTee::create( getGCell(), getNet(), viaLayer1 );
        AutoSegment::create( contact1, contact2, Flags::Vertical );
        AutoContact* contact3 = AutoContactVTee::create( getGCell(), getNet(), viaLayer1 );
        AutoSegment::create( contact2, contact3, Flags::Vertical );

        setSouthWestContact( (north()) ? contact2 : contact3 );
        setNorthEastContact( (north()) ? contact3 : contact2 );
      }
    } else {
      AutoContact* contact1 = doRp_Access( getGCell(), getRoutingPads()[0], HAccess );
      AutoContact* contact2 = AutoContactVTee::create( getGCell(), getNet(), viaLayer1 );
      AutoSegment::create( contact1, contact2, Flags::Horizontal );
      AutoContact* contact3 = AutoContactVTee::create( getGCell(), getNet(), viaLayer1 );
      AutoSegment::create( contact2, contact3, Flags::Vertical );
      AutoContact* contact4 = AutoContactVTee::create( getGCell(), getNet(), viaLayer1 );
      AutoSegment::create( contact2, contact4, Flags::Vertical );

      setSouthWestContact( contact3 );
      setNorthEastContact( contact4 );
    }

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHV::_do_xG_1M1_1M2 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_xG_1M1_1M2() [Managed Configuration]" << endl;

    RoutingPad* rpL1;
    RoutingPad* rpL2;
    if (getRoutingPads()[0]->getLayer() == Session::getBuildRoutingLayer(0)) {
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

    const Layer* viaLayer1 = Session::getBuildContactLayer(1);
    const Layer* viaLayer2 = Session::getBuildContactLayer(2);

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

    RoutingPad* rpM3 = NULL;
    if (getRoutingPads()[0]->getLayer() == Session::getBuildRoutingLayer(2))
      rpM3 = getRoutingPads()[0];

    sortRpByX( getRoutingPads(), NoFlags ); // increasing X.
    for ( size_t i=1 ; i<getRoutingPads().size() ; ++i ) {
      AutoContact* leftContact  = doRp_Access( getGCell(), getRoutingPads()[i-1], HAccess );
      AutoContact* rightContact = doRp_Access( getGCell(), getRoutingPads()[i  ], HAccess );
      AutoSegment::create( leftContact, rightContact, Flags::Horizontal );

      if (not rpM3 and (getRoutingPads()[i]->getLayer() == Session::getBuildRoutingLayer(2)))
        rpM3 = getRoutingPads()[i];
    }

    const Layer* viaLayer1   = Session::getBuildContactLayer(1);
    AutoContact* subContact1 = NULL;

    if (rpM3) {
    // At least one M3 RoutingPad is present: use it.
      if (west() and not south()) {
        setSouthWestContact( doRp_Access( getGCell(), getRoutingPads()[0], HAccess ) );
      } else if (not west() and south()) {
        doRp_AutoContacts( getGCell(), rpM3, getSouthWestContact(), subContact1, DoSourceContact );
      } else if (west() and south()) {
        AutoContact* rpContact = NULL;
        doRp_AutoContacts( getGCell(), rpM3, rpContact, subContact1, DoSourceContact );
        setSouthWestContact( AutoContactVTee::create( getGCell(), getNet(), viaLayer1 ) );
        AutoSegment::create( rpContact, getSouthWestContact(), Flags::Vertical );
      }

      if (east() and not north()) {
        setNorthEastContact( doRp_Access( getGCell(), getRoutingPads()[getRoutingPads().size()-1], HAccess ) );
      } else if (not east() and north()) {
        doRp_AutoContacts( getGCell(), rpM3, subContact1, getNorthEastContact(), DoTargetContact );
      } else if (east() and north()) {
        AutoContact* rpContact = NULL;
        doRp_AutoContacts( getGCell(), rpM3, subContact1, rpContact, DoTargetContact );
        setNorthEastContact( AutoContactVTee::create( getGCell(), getNet(), viaLayer1 ) );
        AutoSegment::create( rpContact, getNorthEastContact(), Flags::Vertical );
      }
    } else {
      cdebug_log(145,0) << "getRoutingPads().size():" << getRoutingPads().size()<< endl;

      if (getRoutingPads().size() == 1) {
        if (   ((east () != NULL) xor (west () != NULL))
           and ((north() != NULL) xor (south() != NULL)) ) {
          cdebug_log(145,0) << "case: One M1, with global turn." << endl;
          
          AutoContact* rpContact = doRp_Access( getGCell(), getRoutingPads()[0], HAccess );
          subContact1 = AutoContactTurn::create( getGCell(), getNet(), viaLayer1 );
          AutoSegment::create( rpContact, subContact1, Flags::Horizontal );

          rpContact   = subContact1;
          subContact1 = AutoContactVTee::create( getGCell(), getNet(), viaLayer1 );
          AutoSegment::create( rpContact, subContact1, Flags::Vertical );

          setBothCornerContacts( subContact1 );
        
          cdebug_tabw(145,-1);
          return true;
        }
      }
      
    // All RoutingPad are M1.
      RoutingPad* southWestRp = getRoutingPads()[0];
      cdebug_log(145,0) << "| Initial S-W Global RP: " << southWestRp << endl;
      // for ( size_t i=1 ; i<getRoutingPads().size() ; ++i ) {
      //   if (southWestRp->getBoundingBox().getHeight() >= 4*Session::getPitch(1)) break;
      //   if (getRoutingPads()[i]->getBoundingBox().getHeight() > southWestRp->getBoundingBox().getHeight()) {
      //     southWestRp = getRoutingPads()[i];
      //     cdebug_log(145,0) << "| Better RP: " << southWestRp << endl;
      //   }
      // }

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

      RoutingPad* northEastRp = getRoutingPads()[getRoutingPads().size()-1];
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

    RoutingPad* rpL2 = getRoutingPads()[0];
    cdebug_log(145,0) << "rpL2 := " << rpL2 << endl;

    AutoContact* rpL2ContactSource = NULL;
    AutoContact* rpL2ContactTarget = NULL;

    doRp_AutoContacts( getGCell(), rpL2, rpL2ContactSource, rpL2ContactTarget, DoSourceContact|DoTargetContact );

    const Layer* viaLayer2 = Session::getBuildContactLayer(2);

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

    RoutingPad* biggestRp = getRoutingPads()[0];
    for ( size_t i=1 ; i<getRoutingPads().size() ; ++i ) {
      doRp_StairCaseH( getGCell(), getRoutingPads()[i-1], getRoutingPads()[i] );
      if (getRoutingPads()[i]->getBoundingBox().getWidth() > biggestRp->getBoundingBox().getWidth())
        biggestRp = getRoutingPads()[i];
    }

    const Layer* viaLayer1 = Session::getBuildContactLayer(1);

    if (east() and west() and not south() and not north()) {
      AutoContact* rpContact = doRp_Access( getGCell(), biggestRp, HBothAccess );
      setBothCornerContacts( rpContact );
      cdebug_tabw(145,-1);
      return true;
    }

    if (west() and not south()) {
      setSouthWestContact( doRp_Access( getGCell(), getRoutingPads()[0], HAccess ) );
    } else if (not west() and south()) {
      cdebug_log(145,0) << "case: not west and south" << endl;
      setSouthWestContact( doRp_Access( getGCell(), biggestRp, NoFlags ) );
    } else if (west() and south()) {
      AutoContact* rpContact = doRp_Access( getGCell(), biggestRp, NoFlags );
      setSouthWestContact( AutoContactVTee::create( getGCell(), getNet(), viaLayer1 ) );
      AutoSegment::create( rpContact, getSouthWestContact(), Flags::Vertical );
    }

    if (east() and not north()) {
      setNorthEastContact( doRp_Access( getGCell(), getRoutingPads()[getRoutingPads().size()-1], HAccess ) );
    } else if (not east() and north()) {
      cdebug_log(145,0) << "case: not east and north" << endl;
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
    size_t rpDepth = Session::getLayerDepth( getRoutingPads()[0]->getLayer() );

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

    if (not (east() or west())) {
      AutoContact* subContact = AutoContactTurn::create( getGCell(), getNet(), Session::getBuildContactLayer((rpDepth)) );
      AutoSegment::create( getSouthWestContact(), subContact, Flags::Horizontal, rpDepth+1 );
      setBothCornerContacts( subContact );
    }

#if THIS_IS_DISABLED
    const Layer* viaLayer1 = Session::getBuildContactLayer(1);

    Box                cellAb    = getAnabatic()->getCell()->getAbutmentBox();
    RoutingLayerGauge* lgM3      = Session::getLayerGauge( 2 );
    DbU::Unit          trackAxis = lgM3->getTrackPosition( cellAb.getXMin()
                                                         , cellAb.getXMax()
                                                         , getRoutingPads()[0]->getX()
                                                         , Constant::Nearest );
    bool offGrid = (trackAxis != getRoutingPads()[0]->getX());
    if (offGrid) {
      cdebug_log(145,0) << "Off grid, Misaligned M3, add horizontal strap" << endl;
      AutoContactTurn* turn1 = AutoContactTurn::create( getGCell(), getNet(), viaLayer1 );
      AutoSegment::create( getSouthWestContact(), turn1, Flags::Horizontal );
      setBothCornerContacts( turn1 );
    }

    if (flags & HAccess) {
    // HARDCODED VALUE.
      if (getRoutingPads()[0]->getBoundingBox().getHeight() < 3*Session::getPitch(1)) {
        AutoContact* subContact = AutoContactTurn::create( getGCell(), getNet(), viaLayer1 );
        AutoSegment::create( getSouthWestContact(), subContact, Flags::Vertical );

        setBothCornerContacts( subContact );
      }
    } else {
      if (getSourceContact() and (getSourceContact()->getX() != getSouthWestContact()->getX())) {
        cdebug_log(145,0) << "On grid, Misaligned M3, add dogleg" << endl;
        AutoContactTurn* turn1 = AutoContactTurn::create( getGCell(), getNet(), viaLayer1 );
        AutoContactTurn* turn2 = AutoContactTurn::create( getGCell(), getNet(), viaLayer1 );
        AutoSegment*     v1    = AutoSegment::create( getSouthWestContact(), turn1, Flags::Vertical   );
        AutoSegment::create( turn1, turn2, Flags::Horizontal );
        v1->setAxis( getSouthWestContact()->getX(), Flags::Force );
        v1->setFlags( AutoSegment::SegFixed|AutoSegment::SegFixedAxis );
        setBothCornerContacts( turn2 );
      }
    }
#endif
    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHV::_do_xG_xM3 ()
  {
    size_t rpDepth  = Session::getLayerDepth( getRoutingPads()[0]->getLayer() );
    if (getGCell()->getDensity(rpDepth) > 0.5)
      return  _do_xG_xM3_upperRouting();
    return _do_xG_xM3_baseRouting();
  }


  bool  NetBuilderHV::_do_xG_xM3_baseRouting ()
  {
    size_t       rpDepth  = Session::getLayerDepth( getRoutingPads()[0]->getLayer() );
    size_t       vDepth   = rpDepth + 2;
    size_t       hDepth   = rpDepth + 1;
    const Layer* viaLayer = Session::getBuildContactLayer( rpDepth );
    if (Session::getRoutingGauge()->isSuperPitched()) {
      viaLayer = Session::getBuildContactLayer( 1 );
      vDepth   = 2;
      hDepth   = 1;
    }

    cdebug_log(145,1) << getTypeName()
                      << "::_do_xG_" << (int)getConnexity().fields.M3
                      << "M3_baseRouting() [Managed Configuration]" << endl;
    cdebug_log(145,0) << "west:"  << west()  << endl;
    cdebug_log(145,0) << "east:"  << east()  << endl;
    cdebug_log(145,0) << "south:" << south() << endl;
    cdebug_log(145,0) << "north:" << north() << endl;

    sortRpByY( getRoutingPads(), NoFlags ); // increasing Y.
    for ( size_t i=1 ; i<getRoutingPads().size() ; i++ ) {
      doRp_StairCaseV( getGCell(), getRoutingPads()[i-1], getRoutingPads()[i] );
    }

    AutoContact* unusedContact = NULL;
    RoutingPad*  rp            = getRoutingPads()[0];

    if (west() and not south()) {
      setSouthWestContact( doRp_Access( getGCell(), rp, HAccess ) );
    } else if (not west() and south()) {
      doRp_AutoContacts( getGCell(), rp, getSouthWestContact(), unusedContact, DoSourceContact );
      if (getSourceContact()) {
        if (getSourceContact()->getX() != getSouthWestContact()->getX()) {
          cdebug_log(149,0) << "Misaligned South: _source:" << DbU::getValueString(getSourceContact()->getX())
                            << "_southWest:"                << DbU::getValueString(getSouthWestContact()->getX()) << endl;

          AutoContactTurn* turn1 = AutoContactTurn::create( getGCell(), getNet(), viaLayer );
          AutoContactTurn* turn2 = AutoContactTurn::create( getGCell(), getNet(), viaLayer );
          AutoSegment::create( getSouthWestContact(), turn1, Flags::Vertical  , vDepth );
          AutoSegment::create( turn1                , turn2, Flags::Horizontal, hDepth );
          setSouthWestContact( turn2 );
        }
      }
    } else if (west() and south()) {
      AutoContact* rpContact = NULL;
      doRp_AutoContacts( getGCell(), rp, rpContact, unusedContact, DoSourceContact );
      setSouthWestContact( AutoContactVTee::create( getGCell(), getNet(), viaLayer ) );
      AutoSegment::create( rpContact, getSouthWestContact(), Flags::Vertical, vDepth );
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

          AutoContactTurn* turn1 = AutoContactTurn::create( getGCell(), getNet(), viaLayer );
          AutoContactTurn* turn2 = AutoContactTurn::create( getGCell(), getNet(), viaLayer );
          AutoSegment::create( getNorthEastContact(), turn1, Flags::Vertical  , vDepth );
          AutoSegment::create( turn1                , turn2, Flags::Horizontal, hDepth );
          setNorthEastContact( turn2 );
        }
      }
    } else if (east() and north()) {
      AutoContact* rpContact = NULL;
      doRp_AutoContacts( getGCell(), rp, unusedContact, rpContact, DoTargetContact );
      setNorthEastContact( AutoContactVTee::create( getGCell(), getNet(), viaLayer ) );
      AutoSegment::create( rpContact, getNorthEastContact(), Flags::Vertical, vDepth );
    }

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHV::_do_xG_xM3_upperRouting ()
  {
    cdebug_log(145,1) << getTypeName()
                      << "::_do_xG_"  << (int)getConnexity().fields.M3
                      << "M3_upperRouting() [G:"   << (int)getConnexity().fields.globals << " Managed Configuration]" << endl;
    cdebug_log(145,0) << "getConnexity(): " << getConnexity().connexity << endl;
    cdebug_log(145,0) << "north:     " << north() << endl;
    cdebug_log(145,0) << "south:     " << south() << endl;
    cdebug_log(145,0) << "east:      " << east () << endl;
    cdebug_log(145,0) << "west:      " << west () << endl;
    size_t rpDepth = Session::getLayerDepth( getRoutingPads()[0]->getLayer() );

    sortRpByY( getRoutingPads(), NoFlags ); // increasing Y.
    for ( size_t i=1 ; i<getRoutingPads().size() ; i++ ) {
      doRp_StairCaseV( getGCell(), getRoutingPads()[i-1], getRoutingPads()[i] );
    }

    if ((int)getConnexity().fields.M3 != 1) return false;

    const Layer* viaLayer2 = Session::getBuildContactLayer(rpDepth);

    if (getConnexity().fields.globals == 2) {
      if (north() and south()) {
        AutoContact* contact1 = doRp_Access( getGCell(), getRoutingPads()[0], HAccess );
        AutoContact* contact2 = AutoContactHTee::create( getGCell(), getNet(), viaLayer2 );
        AutoSegment::create( contact1, contact2, Flags::Horizontal, rpDepth+1 );
        contact1 = AutoContactTurn::create( getGCell(), getNet(), viaLayer2 );
        AutoSegment::create( contact1, contact2, Flags::Horizontal, rpDepth+1 );
        setNorthEastContact( contact1 );
        setSouthWestContact( contact2 );
      } else if (east() and west()) {
        AutoContact* contact1  = NULL;
        AutoContact* contact2  = NULL;
        doRp_AutoContacts( getGCell(), getRoutingPads()[0], contact1, contact2, NoFlags );
        setSouthWestContact( contact1 );
        doRp_AutoContacts( getGCell(), getRoutingPads()[0], contact1, contact2, NoFlags );
        setNorthEastContact( contact1 );
      } else {
        AutoContact* contact1 = doRp_Access( getGCell(), getRoutingPads()[0], HAccess );
        AutoContact* contact2 = AutoContactTurn::create( getGCell(), getNet(), viaLayer2 );
        AutoSegment::create( contact1, contact2, Flags::Horizontal, rpDepth+1 );

        contact1 = AutoContactVTee::create( getGCell(), getNet(), viaLayer2 );
        AutoSegment::create( contact1, contact2, Flags::Vertical, rpDepth+2 );
        setBothCornerContacts( contact1 );
      }
    } else if (getConnexity().fields.globals == 3) {
      if (not west() or not east()) {
        AutoContact* contact1 = doRp_Access( getGCell(), getRoutingPads()[0], HAccess );
        AutoContact* contact2 = AutoContactVTee::create( getGCell(), getNet(), viaLayer2 );
        AutoSegment::create( contact1, contact2, Flags::Horizontal, rpDepth+1 );
        AutoContact* contact3 = AutoContactVTee::create( getGCell(), getNet(), viaLayer2 );
        AutoSegment::create( contact2, contact3, Flags::Vertical, rpDepth+2 );

        setSouthWestContact( (east()) ? contact2 : contact3 );
        setNorthEastContact( (east()) ? contact3 : contact2 );
      } else {
        AutoContact* contact1 = doRp_Access( getGCell(), getRoutingPads()[0], NoFlags );
        AutoContact* contact2 = AutoContactVTee::create( getGCell(), getNet(), viaLayer2 );
        AutoSegment::create( contact1, contact2, Flags::Vertical, rpDepth+2 );
        AutoContact* contact3 = AutoContactVTee::create( getGCell(), getNet(), viaLayer2 );
        AutoSegment::create( contact2, contact3, Flags::Vertical, rpDepth+2 );

        setSouthWestContact( (north()) ? contact2 : contact3 );
        setNorthEastContact( (north()) ? contact3 : contact2 );
      }
    } else {
      AutoContact* contact1 = doRp_Access( getGCell(), getRoutingPads()[0], HAccess );
      AutoContact* contact2 = AutoContactVTee::create( getGCell(), getNet(), viaLayer2 );
      AutoSegment::create( contact1, contact2, Flags::Horizontal, rpDepth+1 );
      AutoContact* contact3 = AutoContactVTee::create( getGCell(), getNet(), viaLayer2 );
      AutoSegment::create( contact2, contact3, Flags::Vertical, rpDepth+2 );
      AutoContact* contact4 = AutoContactVTee::create( getGCell(), getNet(), viaLayer2 );
      AutoSegment::create( contact2, contact4, Flags::Vertical, rpDepth+2 );

      setSouthWestContact( contact3 );
      setNorthEastContact( contact4 );
    }

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderHV::_do_globalSegment ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_globalSegment()" << endl;
    cdebug_log(145,0) << "getSourceFlags():" << getSourceFlags()
                      << " getFlags():" << getFlags() << endl;

    if (getSourceContact()) {
      AutoContact* targetContact
        = ( getSegmentHookType(getFromHook()) & (NorthBound|EastBound) )
        ? getNorthEastContact() : getSouthWestContact() ;
      if (not getFromHook()) cerr << "getFromHook() is NULL !" << endl;

      Segment* baseSegment = static_cast<Segment*>( getFromHook()->getComponent() );
      if ( (getSourceFlags() | getFlags()) & ToUpperRouting) {
        cdebug_log(145,0) << "Moving up global" << endl;
        vector<GCell*> gcells;
        size_t gdepth = Session::getGHorizontalDepth();
        if (dynamic_cast<Vertical*>(baseSegment)) {
          gdepth = Session::getGVerticalDepth();
          GCell::getVGCellsUnder( gcells
                                , getSourceContact()->getGCell()
                                , targetContact->getGCell()
                                , baseSegment->getX() );
        } else {
          GCell::getHGCellsUnder( gcells
                                , getSourceContact()->getGCell()
                                , targetContact->getGCell()
                                , baseSegment->getY() );
        }
        bool canMoveUp = true;
        for ( GCell* gcell : gcells ) {
          if (not gcell->hasFreeTrack(gdepth+2,2.0)) {
            canMoveUp = false;
            break;
          }
        }
        if (canMoveUp) {
          baseSegment->setLayer( Session::getBuildRoutingLayer( gdepth+2 ));
          baseSegment->setWidth( Session::getWireWidth( gdepth+2 ));
        } else {
          cdebug_log(145,0) << "-> Cancel move up, not enough up free track" << endl;
        }
      }

      AutoSegment* globalSegment = AutoSegment::create( getSourceContact()
                                                      , targetContact
                                                      , baseSegment
                                                      );
      globalSegment->setFlags( (getDegree() == 2) ? AutoSegment::SegBipoint : 0 );
      if (getNetData() and getNetData()->isNoMoveUp(baseSegment)) {
        globalSegment->setFlags( AutoSegment::SegNoMoveUp );
      }
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
    
    push( east (), getNorthEastContact() );
    push( west (), getSouthWestContact() );
    push( north(), getNorthEastContact() );
    push( south(), getSouthWestContact() );

    cdebug_tabw(145,-1);
    return true;
  }


  void  NetBuilderHV::singleGCell ( AnabaticEngine* anbt, Net* net )
  {
    cdebug_log(145,1) << "NetBuilderHV::singleGCell() " << net << endl;

    vector<RoutingPad*>  rpM1s;
    RoutingPad*          rpM2  = NULL;
    RoutingPad*          rpPin = NULL;

    for ( RoutingPad* rp : net->getRoutingPads() ) {
      if (dynamic_cast<Pin*>(rp->getOccurrence().getEntity())) {
        rpPin = rp;
        continue;
      }
      
      if (      Session::getRoutingGauge()->getLayerDepth(rp->getLayer())
         == 1 + Session::getRoutingGauge()->getFirstRoutingLayer())
        rpM2 = rp;
      else
        rpM1s.push_back( rp );
    }

    if ( (rpM1s.size() < 2) and not (rpM2 or rpPin) ) {
      cerr << Error( "For %s, less than two Plugs/Pins (%d)."
                   , getString(net).c_str()
                   , rpM1s.size() ) << endl;
      cdebug_tabw(145,-1);
      return;
    }

    if (rpM1s.empty()) {
      cerr << Error( "No METAL1 in Single GCell for Net \"%s\".", getString(net->getName()).c_str() ) << endl;
      cdebug_tabw(145,-1);
      return;
    }

    sortRpByX( rpM1s, NetBuilder::NoFlags ); // increasing X.

    GCell* gcell1 = anbt->getGCellUnder( (*rpM1s.begin ())->getCenter() );
    GCell* gcell2 = anbt->getGCellUnder( (*rpM1s.rbegin())->getCenter() );

    if (not gcell1) {
      cerr << Error( "No GCell under %s.", getString(*(rpM1s.begin())).c_str() ) << endl;
      cdebug_tabw(145,-1);
      return;
    }
    if (gcell1 != gcell2) {
      cerr << Error( "Not under a single GCell %s.", getString(*(rpM1s.rbegin())).c_str() ) << endl;
      cdebug_tabw(145,-1);
      return;
    }

    cdebug_log(145,0) << "singleGCell " << gcell1 << endl;

    AutoContact* turn1  = NULL;
    AutoContact* turn2  = NULL;
    AutoContact* source = NULL;
    AutoContact* target = NULL;

    for ( size_t irp=1 ; irp<rpM1s.size() ; ++irp ) {
      source = doRp_Access( gcell1, rpM1s[irp-1], HAccess );
      target = doRp_Access( gcell1, rpM1s[irp  ], HAccess );

      if (source->getUConstraints(Flags::Vertical).intersect(target->getUConstraints(Flags::Vertical))) {
        uint64_t flags = checkRoutingPadSize( rpM1s[irp-1] );
        if (flags & VSmall) {
          turn1  = AutoContactTurn::create( gcell1, rpM1s[irp]->getNet(), Session::getDContactLayer() );
          AutoSegment::create( source, turn1, Flags::Horizontal   );
          source = turn1;
          turn1  = AutoContactTurn::create( gcell1, rpM1s[irp]->getNet(), Session::getDContactLayer() );
          AutoSegment::create( source, turn1 , Flags::Vertical   );
          source = turn1;
        }
        flags = checkRoutingPadSize( rpM1s[irp] );
        if (flags & VSmall) {
          turn1  = AutoContactTurn::create( gcell1, rpM1s[irp]->getNet(), Session::getDContactLayer() );
          AutoSegment::create( target, turn1, Flags::Horizontal   );
          target = turn1;
          turn1  = AutoContactTurn::create( gcell1, rpM1s[irp]->getNet(), Session::getDContactLayer() );
          AutoSegment::create( target, turn1 , Flags::Vertical   );
          target = turn1;
        }
        AutoSegment::create( source, target, Flags::Horizontal );
      } else {
        turn1 = AutoContactTurn::create( gcell1, rpM1s[irp]->getNet(), Session::getDContactLayer() );
        turn2 = AutoContactTurn::create( gcell1, rpM1s[irp]->getNet(), Session::getDContactLayer() );
        AutoSegment::create( source, turn1 , Flags::Horizontal );
        AutoSegment::create( turn1 , turn2 , Flags::Vertical   );
        AutoSegment::create( turn2 , target, Flags::Horizontal );
      }
    }

    if (rpM2) {
      source = doRp_Access( gcell1, rpM1s[0], HAccess );
      target = doRp_Access( gcell1, rpM2    , HAccess );
      turn1 = AutoContactTurn::create( gcell1, rpM2->getNet(), Session::getBuildContactLayer(1) );
      AutoSegment::create( source, turn1 , Flags::Horizontal );
      AutoSegment::create( turn1 , target, Flags::Vertical );
    }

    if (rpPin) {
      Pin*                 pin    = dynamic_cast<Pin*>( rpPin->getOccurrence().getEntity() );
      Pin::AccessDirection pinDir = pin->getAccessDirection();

      if (  (pinDir == Pin::AccessDirection::NORTH)
         or (pinDir == Pin::AccessDirection::SOUTH) ) {
        source = doRp_Access( gcell1, rpM1s[0], HAccess );
        target = doRp_AccessNorthSouthPin( gcell1, rpPin );
        turn1  = AutoContactTurn::create( gcell1, rpPin->getNet(), Session::getBuildContactLayer(1) );
        AutoSegment::create( source, turn1 , Flags::Horizontal );
        AutoSegment::create( turn1 , target, Flags::Vertical );
      } else {
        RoutingPad* closest = NULL;
        
        if (pinDir == Pin::AccessDirection::EAST) closest = rpM1s.back();
        else                                      closest = rpM1s.front();

        source = doRp_Access( gcell1, closest, HAccess );
        target = doRp_AccessEastWestPin( gcell1, rpPin );
        AutoSegment::create( source, target , Flags::Horizontal );
      }
    }

    cdebug_tabw(145,-1);
  }


  string  NetBuilderHV::getTypeName () const
  { return "NetBuilderHV"; }


}  // Anabatic namespace.
