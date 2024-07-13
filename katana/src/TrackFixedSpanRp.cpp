// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2024-2024.
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./TrackFixedSpanRp.cpp"                   |
// +-----------------------------------------------------------------+


#include <sstream>
#include "hurricane/Bug.h"
#include "hurricane/Warning.h"
#include "hurricane/Net.h"
#include "hurricane/Name.h"
#include "hurricane/RegularLayer.h"
#include "hurricane/Technology.h"
#include "hurricane/DataBase.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Vertical.h"
#include "hurricane/RoutingPad.h"
#include "anabatic/AutoContact.h"
#include "crlcore/RoutingGauge.h"
#include "katana/DataNegociate.h"
#include "katana/TrackFixedSpanRp.h"
#include "katana/TrackCost.h"
#include "katana/Track.h"
#include "katana/RoutingPlane.h"
#include "katana/Session.h"
#include "katana/RoutingEvent.h"
#include "katana/NegociateWindow.h"
#include "katana/KatanaEngine.h"


namespace Katana {

  using namespace std;
  using Hurricane::Warning;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::Net;
  using Hurricane::Name;
  using Hurricane::RegularLayer;
  using Hurricane::Technology;
  using Hurricane::DataBase;
  using Hurricane::Horizontal;
  using Hurricane::Vertical;
  using Anabatic::GCellsUnder;


// -------------------------------------------------------------------
// Class  :  "TrackFixedSpanRp".


  TrackFixedSpanRp::TrackFixedSpanRp ( RoutingPad* rp, const Box& bb )
    : Super(rp->getNet(),bb)
    , _routingPad(rp)
  { }


  TrackFixedSpanRp::~TrackFixedSpanRp ()
  { }


  void  TrackFixedSpanRp::_preDestroy ()
  {
    cdebug_log(155,0) << "TrackFixedSpanRp::_preDestroy() - " << (void*)this << endl;
    Super::_preDestroy();
  }


  TrackFixedSpanRp* TrackFixedSpanRp::create ( RoutingPad* rp, const Box& bb, Track* track )
  {
    TrackFixedSpanRp* trackTerminal = nullptr;
    if (not track) 
      throw Error( "TrackFixedSpanRp::create(): NULL track argument." );
    
    cdebug_log(159,0) << "TrackFixedSpanRp::create() track:" << track << endl;
    trackTerminal = new TrackFixedSpanRp ( rp, bb );
    trackTerminal->_postCreate( track );
    cdebug_log(159,0) << trackTerminal << endl;
    Session::addInsertEvent( trackTerminal, track, track->getAxis() );
    return trackTerminal;
  }


  bool         TrackFixedSpanRp::isFixedSpanRp () const { return true; }
  RoutingPad*  TrackFixedSpanRp::getRoutingPad () const { return _routingPad; }

  
  void  TrackFixedSpanRp::setNet ( Net* )
  {
    throw Error( "TrackFixedSpanRp::setNet(): Not callable on this type of object.\n"
                 "        on %s."
               , getString(this).c_str() );
  }

  Net* TrackFixedSpanRp::getNet () const
  { return _routingPad->getNet(); }


  string  TrackFixedSpanRp::_getTypeName () const
  { return "TrackFixedSpanRp"; }


  Record* TrackFixedSpanRp::_getRecord () const
  {
    Record* record = Super::_getRecord();
    record->add( getSlot( "_routingPad", _routingPad ));
    return record;
  }


} // Katana namespace.
