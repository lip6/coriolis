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
// |  C++ Module  :       "./NetBuilderM2.cpp"                       |
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
#include "anabatic/NetBuilderM2.h"
#include "anabatic/AnabaticEngine.h"


namespace Anabatic {

  using std::swap;

  
  NetBuilderM2::NetBuilderM2 ()
    : NetBuilder()
  { }


  NetBuilderM2::~NetBuilderM2 () { }


  string  NetBuilderM2::getStyle ()
  { return "2RL-"; }

  
  void  NetBuilderM2::doRp_AutoContacts ( GCell*        gcell
                                        , Component*    rp
                                        , AutoContact*& source
                                        , AutoContact*& target
                                        , uint64_t      flags
                                        )
  {
    cdebug_log(145,1) << "NetBuilderM2::doRp_AutoContacts()" << endl;
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


  AutoContact* NetBuilderM2::doRp_Access ( GCell* gcell, Component* rp, uint64_t flags )
  {
    cdebug_log(145,1) << getTypeName() << "::doRp_Access()" << endl;
    cdebug_log(145,0) << rp << endl;

    Point        sourcePosition;
    Point        targetPosition;
    const Layer* rpLayer        = rp->getLayer();
    const Layer* viaLayer       = Session::getDContactLayer();
    DbU::Unit    viaSide        = Session::getDContactWidth();
    DbU::Unit    ypitch         = Session::getDVerticalPitch();

    getPositions( rp, sourcePosition, targetPosition );

    if (sourcePosition.getX() > targetPosition.getX()) swap( sourcePosition, targetPosition );
    if (sourcePosition.getY() > targetPosition.getY()) swap( sourcePosition, targetPosition );

    Point position = rp->getCenter();
    if (not (flags & Middle)) {
      if (flags & NorthBound) position = targetPosition;
      if (flags & SouthBound) position = sourcePosition;
    }

    DbU::Unit ycontact = (flags & SouthBound) ? gcell->getYMin() : gcell->getYMax()-ypitch;

    AutoContact* rpContact = AutoContactTerminal::create( gcell, rp, rpLayer, position, viaSide, viaSide );
    AutoContact* contact1  = AutoContactTurn::create( gcell, getNet(), viaLayer );
    AutoContact* contact2  = AutoContactTurn::create( gcell, getNet(), viaLayer );
    contact1->setPosition( position.getX(), ycontact );
    contact2->setPosition( position.getX(), ycontact );
    rpContact->setFlags( CntFixed );
    contact1 ->setFlags( CntFixed );
    contact2 ->setFlags( CntFixed );

    AutoSegment* fixed  = AutoSegment::create( rpContact, contact1, Flags::Vertical   );
    AutoSegment* dogleg = AutoSegment::create( contact1 , contact2, Flags::Horizontal|Flags::UseNonPref );
    fixed ->setFlags( AutoSegment::SegFixed );
    dogleg->setFlags( AutoSegment::SegFixed );

    cdebug_tabw(145,-1);
    return contact2;
  }


  bool  NetBuilderM2::_do_1G_1M1 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_1G_1M1()" << endl;

    uint64_t  flags = NoFlags;
    if      (north()) flags |= NorthBound;
    else if (south()) flags |= SouthBound;

    AutoContact* contact = NULL;
    contact = doRp_Access( getGCell(), getRoutingPads()[0], flags );
    setNorthEastContact( contact );
    push( north(), contact, SouthWest );
    push( south(), contact, SouthWest );

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderM2::_do_2G_1M1 ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_2G_1M1()" << endl;

    AutoContact* contact = NULL;
    contact = doRp_Access( getGCell(), getRoutingPads()[0], SouthBound|NorthBound );
    push( north(), contact, SouthWest|Middle );

    contact = doRp_Access( getGCell(), getRoutingPads()[0], SouthBound|NorthBound );
    push( south(), contact, SouthWest|Middle );
    
    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderM2::_do_xG ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_xG()" << endl;

    vector<Hook*> hooksNS = getNorths();
    hooksNS.insert( hooksNS.end(), getSouths().begin(), getSouths().end() );
    sortHookByX( hooksNS, NoFlags );

    const Layer* viaLayer = Session::getDContactLayer();
    AutoContact* contactW = NULL;
    AutoContact* contactE = NULL;

  // Simple turn.
    if (  (west() and not east() and (hooksNS.size() == 1))
       or (east() and not west() and (hooksNS.size() == 1)) ) {
      contactW = AutoContactTurn::create( getGCell(), getNet(), viaLayer );
      push( west()    , contactW, SouthWest );
      push( east()    , contactW, SouthWest );
      push( hooksNS[0], contactW, SouthWest );
      cdebug_tabw(145,-1);
      return true;
    }

  // Simple HTee.
    if (west() and east() and (hooksNS.size() == 1)) {
      contactW = AutoContactHTee::create( getGCell(), getNet(), viaLayer );
      push( west()    , contactW, SouthWest );
      push( east()    , contactW, SouthWest );
      push( hooksNS[0], contactW, SouthWest );
      cdebug_tabw(145,-1);
      return true;
    }
    
    cdebug_log(145,0) << "West side processing." << endl;
  // West side processing.
    if (west()) {
      contactW =  AutoContactHTee::create( getGCell(), getNet(), viaLayer );
      push( west()    , contactW, SouthWest );
      push( hooksNS[0], contactW, SouthWest );
    } else {
      contactW =  AutoContactTurn::create( getGCell(), getNet(), viaLayer );
      push( hooksNS[0], contactW, SouthWest );
    }

    cdebug_log(145,0) << "Middle processing." << endl;
  // Middle (North & South) processing.
    if (hooksNS.size() > 2) {
      for ( size_t i=1 ; i<hooksNS.size()-1 ; ++i ) {
        AutoContact* current = AutoContactHTee::create( getGCell(), getNet(), viaLayer );
        AutoSegment::create( contactW, current, Flags::Horizontal );
        push( hooksNS[i], current, SouthWest );

        contactW = current;
      }
    }

    cdebug_log(145,0) << "East side processing." << endl;
  // East side processing.
    if (east()) {
      contactE =  AutoContactHTee::create( getGCell(), getNet(), viaLayer );
      push( east(), contactE, SouthWest );
      if (hooksNS.size() > 1)
        push( hooksNS[hooksNS.size()-1], contactE, SouthWest );
    } else {
      contactE =  AutoContactTurn::create( getGCell(), getNet(), viaLayer );
      push( hooksNS[hooksNS.size()-1], contactE, SouthWest );
    }

    AutoSegment::create( contactW, contactE, Flags::Horizontal );

    cdebug_tabw(145,-1);
    return true;
  }


  bool NetBuilderM2::_do_globalSegment ()
  {
    cdebug_log(145,1) << getTypeName() << "::_do_globalSegment()" << endl;

    if (getSourceContact()) {
      Segment*     segment       = static_cast <Segment*> ( getFromHook()->getComponent() );
      AutoSegment* globalSegment = AutoSegment::create( getSourceContact(), getSouthWestContact(), segment );

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

    cdebug_tabw(145,-1);
    return true;
  }


  bool  NetBuilderM2::_do_1G_xM1     () { return false; }
  bool  NetBuilderM2::_do_xG_1Pad    () { return false; }
  bool  NetBuilderM2::_do_1G_1PinM2  () { return false; }
  bool  NetBuilderM2::_do_xG_xM2     () { return false; }
  bool  NetBuilderM2::_do_1G_1M3     () { return false; }
  bool  NetBuilderM2::_do_xG_xM3     () { return false; }
  bool  NetBuilderM2::_do_xG_1M1_1M2 () { return false; }
  bool  NetBuilderM2::_do_xG_xM1_xM3 () { return false; }
  bool  NetBuilderM2::_do_4G_1M2     () { return false; }
  bool  NetBuilderM2::_do_2G         () { return false; }


  string  NetBuilderM2::getTypeName () const
  { return "NetBuilderM2"; }


}  // Anabatic namespace.
