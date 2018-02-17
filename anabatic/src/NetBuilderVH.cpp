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

  
  NetBuilderVH::NetBuilderVH ()
    : NetBuilder()
  { }


  NetBuilderVH::~NetBuilderVH () { }

  
  void  NetBuilderVH::doRp_AutoContacts ( GCell*        gcell
                                        , Component*    rp
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
    Net*         net            = rp->getNet();
    const Layer* rpLayer        = rp->getLayer();
    size_t       rpDepth        = Session::getLayerDepth( rp->getLayer() );
    Flags        direction      = Session::getDirection ( rpDepth );
    DbU::Unit    viaSide        = Session::getViaWidth  ( rpDepth );
    DbU::Unit    gridPosition   = 0;

    getPositions( rp, sourcePosition, targetPosition );

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

#if 0
  // Quasi-punctual M1 terminal.
    if (flags & VSmall) {
      Box                ab             = rp->getCell()->getBoundingBox();
      RoutingLayerGauge* gaugeMetal3    = Session::getLayerGauge( 2 );
      DbU::Unit          metal3axis     = gaugeMetal3->getTrackPosition( ab.getYMin()
                                                                       , ab.getYMax()
                                                                       , sourcePosition.getY()
                                                                       , Constant::Nearest );
      DbU::Unit          viaSideProtect = Session::getViaWidth((size_t)0);

      AutoContact* sourceVia12 = AutoContactTerminal::create( sourceGCell
                                                            , rp
                                                            , Session::getContactLayer(0)
                                                            , sourcePosition
                                                            , viaSideProtect, viaSideProtect
                                                            );
      AutoContact* targetVia12 = AutoContactTerminal::create( targetGCell
                                                            , rp
                                                            , Session::getContactLayer(0)
                                                            , targetPosition
                                                            , viaSideProtect, viaSideProtect
                                                            );
      AutoContact* sourceVia23 = AutoContactTurn::create( sourceGCell, net, Session::getContactLayer(1) );
      AutoContact* targetVia23 = AutoContactTurn::create( targetGCell, net, Session::getContactLayer(1) );
      sourceVia23->setY( metal3axis );
      targetVia23->setY( metal3axis );
      sourceVia23->setX( sourcePosition.getX() );
      targetVia23->setX( targetPosition.getX() );

      AutoSegment* segmentS = AutoSegment::create( sourceVia12, sourceVia23, Flags::Vertical );
      AutoSegment* segmentT = AutoSegment::create( targetVia12, targetVia23, Flags::Vertical );
      AutoSegment* segmentM = AutoSegment::create( sourceVia23, targetVia23, Flags::Horizontal );

      sourceVia12->setFlags( CntFixed );
      sourceVia23->setFlags( CntFixed );
      targetVia12->setFlags( CntFixed );
      targetVia23->setFlags( CntFixed );
      segmentS->setFlags( AutoSegment::SegFixed );
      segmentT->setFlags( AutoSegment::SegFixed );
      segmentM->setFlags( AutoSegment::SegFixed );

      cdebug_log(145,0) << "Hard protect: " << rp << endl;
      cdebug_log(145,0) << "X:" << DbU::getValueString(sourcePosition.getX())
                        << " Metal3 Track Y:" << DbU::getValueString(metal3axis) << endl;
    }
#endif

  // Non-M1 terminal or punctual M1 protections.
    if ( (rpDepth != 0) or ((sourcePosition == targetPosition) and (gridPosition == 0)) ) {
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


  AutoContact* NetBuilderVH::doRp_Access ( GCell* gcell, Component* rp, uint64_t flags )
  {
    cdebug_log(145,1) << getTypeName() << "::doRp_Access() - flags:" << flags << endl;

    AutoContact* rpContactSource;
    AutoContact* rpContactTarget;

    flags |= checkRoutingPadSize( rp );

    doRp_AutoContacts( gcell, rp, rpContactSource, rpContactTarget, flags );

    if (not (flags & (HAccess|HAccessEW))) {
      AutoContact* subContact1 = AutoContactTurn::create( gcell, rp->getNet(), Session::getContactLayer(1) );
      AutoContact* subContact2 = AutoContactTurn::create( gcell, rp->getNet(), Session::getContactLayer(1) );
      AutoSegment::create( rpContactSource, subContact1, Flags::Vertical  );
      AutoSegment::create( subContact1,     subContact2, Flags::Horizontal );
      rpContactSource = subContact2;
    } else {
      if (flags & VSmall) {
        AutoContact* subContact1 = NULL;
        if (flags & HAccessEW)
          subContact1 = AutoContactHTee::create( gcell, rp->getNet(), Session::getContactLayer(1) );
        else
          subContact1 = AutoContactTurn::create( gcell, rp->getNet(), Session::getContactLayer(1) );

        AutoSegment::create( rpContactSource, subContact1, Flags::Vertical );
        rpContactSource = subContact1;
      }
    }

    cdebug_tabw(145,-1);

    return rpContactSource;
  }


  bool  NetBuilderVH::_do_1G_1M1 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_1G_1M1() [Managed Configuration - Optimized] " << getTopology() << endl;

    uint64_t  flags = NoFlags;
    if      (east() ) { flags |= HAccess|VSmall; }
    else if (west() ) { flags |= HAccess|VSmall; }

    setBothCornerContacts( doRp_Access(getGCell(),getRoutingPads()[0],flags) );

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

    AutoContact* tee  = NULL;

    if (east() and west()) {
      tee = doRp_Access( getGCell(), getRoutingPads()[0], HAccessEW|VSmall );
    } else {
      AutoContact* turn = doRp_Access( getGCell(), getRoutingPads()[0], HAccess|VSmall );

      if (east() or west())
        tee = AutoContactHTee::create( getGCell(), getNet(), Session::getDContactLayer() );
      else
        tee = AutoContactVTee::create( getGCell(), getNet(), Session::getDContactLayer() );

      AutoSegment::create( turn, tee, Flags::Horizontal );
    }
    setBothCornerContacts( tee );

    cdebug_tabw(145,-1);
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
    AutoContact* leftContact  = NULL;
    AutoContact* rightContact = NULL;

    if (south() or west()) {
      leftContact = doRp_Access( getGCell(), getRoutingPads()[0], HAccessEW|VSmall );
      if (south() and west()) {
        setSouthWestContact( AutoContactHTee::create( getGCell(), getNet(), Session::getDContactLayer() ) );
        AutoSegment::create( getSouthWestContact(), leftContact, Flags::Horizontal );
      } else {
        if (west())
          setSouthWestContact( leftContact );
        else {
          setSouthWestContact( AutoContactTurn::create( getGCell(), getNet(), Session::getDContactLayer() ) );
          AutoSegment::create( leftContact, getSouthWestContact(), Flags::Horizontal );
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
        AutoSegment::create( rightContact, getNorthEastContact(), Flags::Horizontal );
      } else {
        if (east())
          setNorthEastContact( rightContact );
        else {
          setNorthEastContact( AutoContactTurn::create( getGCell(), getNet(), Session::getDContactLayer() ) );
          AutoSegment::create( rightContact, getNorthEastContact(), Flags::Horizontal );
        }
      }
    } else {
      rightContact = doRp_Access( getGCell(), getRoutingPads()[iLast], HAccess|VSmall );
      AutoSegment::create( leftContact, rightContact, Flags::Horizontal );
    }

    cdebug_tabw(145,-1);
    return true;
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


  bool  NetBuilderVH::_do_globalSegment ()
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


  void  NetBuilderVH::singleGCell ( AnabaticEngine* anbt, Net* net )
  {
    cdebug_log(145,1) << getTypeName() << "::singleGCell() " << net << endl;

    vector<RoutingPad*>  rps;

    for ( RoutingPad* rp : net->getRoutingPads() ) {
      if (Session::getRoutingGauge()->getLayerDepth(rp->getLayer()) == 0) {
        rps.push_back( rp );
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

    if (rps.size() < 2) {
      cerr << Error( "%s::singleGCell(): For %s, less than two Plugs/Pins (%d)."
                   , getTypeName().c_str()
                   , getString(net).c_str()
                   , rps.size() ) << endl;
      cdebug_tabw(145,-1);
      return;
    }

    if (rps.empty()) {
      cerr << Error( "%s::singleGCell(): No terminals for Net \"%s\"."
                   , getTypeName().c_str()
                   , getString(net->getName()).c_str() ) << endl;
      cdebug_tabw(145,-1);
      return;
    }

    sortRpByX( rps, NetBuilder::NoFlags ); // increasing X.

    GCell* gcell1 = anbt->getGCellUnder( (*rps.begin ())->getCenter() );
    GCell* gcell2 = anbt->getGCellUnder( (*rps.rbegin())->getCenter() );

    if (not gcell1) {
      cerr << Error( "%s::singleGCell(): No GCell under %s."
                   , getTypeName().c_str()
                   , getString(*(rps.begin())).c_str() ) << endl;
      cdebug_tabw(145,-1);
      return;
    }
    if (gcell1 != gcell2) {
      cerr << Error( "%s::singleGCell(): Not under a single GCell %s."
                   , getTypeName().c_str()
                   , getString(*(rps.rbegin())).c_str() ) << endl;
      cdebug_tabw(145,-1);
      return;
    }

    cdebug_log(145,0) << "singleGCell " << gcell1 << endl;

    AutoContact* source = NULL;
    AutoContact* target = NULL;

    for ( size_t irp=1 ; irp<rps.size() ; ++irp ) {
      source = doRp_Access( gcell1, rps[irp-1], HAccess|VSmall );
      target = doRp_Access( gcell1, rps[irp  ], HAccess|VSmall );
      AutoSegment::create( source, target, Flags::Horizontal );
    }

    cdebug_tabw(145,-1);
  }


  string  NetBuilderVH::getTypeName () const
  { return "NetBuilderVH"; }


}  // Anabatic namespace.
