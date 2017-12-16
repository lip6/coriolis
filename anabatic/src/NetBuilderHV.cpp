// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |         A n a b a t i c  -  Routing Toolbox                     |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./LoadGlobalRouting.cpp"                  |
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
    cdebug_log(145,1) << "NetBuilderHV::doRp_AutoContacts()" << endl;
    cdebug_log(145,0)   << rp << endl;

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
    cdebug_log(145,1) << "NetBuilderHV::doRp_Access() - flags:" << flags << endl;

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


  void  NetBuilderHV::_do_1G_1M1 ()
  {
    cdebug_log(145,1) << "NetBuilderHV::_do_1G_1M1() [Managed Configuration - Optimized] " << getTopology() << endl;

    uint64_t  flags = NoFlags;
    if      (east() ) { flags |= HAccess; }
    else if (west() ) { flags |= HAccess; }
    else if (north()) { flags |= VSmall; }
    else if (south()) { flags |= VSmall; }

    setBothCornerContacts( doRp_Access(getGCell(),getRoutingPads()[0],flags) );

    cdebug_tabw(145,-1);
  }


  void  NetBuilderHV::_do_1G_xM1 ()
  {
    cdebug_log(145,1) << "NetBuilderHV::_do_1G_" << (int)getConnexity().fields.M1 << "M1() [Managed Configuration]" << endl;

    sort( getRoutingPads().begin(), getRoutingPads().end(), SortRpByX(NoFlags) ); // increasing X.
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
  }


}  // Anabatic namespace.
