// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./TrackFixedSpan.cpp"                     |
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
#include "anabatic/AutoContact.h"
#include "crlcore/RoutingGauge.h"
#include "katana/DataNegociate.h"
#include "katana/TrackFixedSpan.h"
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
// Class  :  "TrackFixedSpan".


  TrackFixedSpan::TrackFixedSpan ( Net* net, const Box& bb )
    : Super(net,bb)
    , _net (net)
  { }


  TrackFixedSpan::~TrackFixedSpan ()
  { }


  void  TrackFixedSpan::_preDestroy ()
  {
    cdebug_log(155,0) << "TrackFixedSpan::_preDestroy() - " << (void*)this << endl;
    Super::_preDestroy();
  }


  TrackFixedSpan* TrackFixedSpan::create ( Net* net, const Box& bb, Track* track )
  {
    TrackFixedSpan* trackTerminal = nullptr;
    if (not track) 
      throw Error( "TrackFixedSpan::create(): NULL track argument." );
    
    cdebug_log(159,0) << "TrackFixedSpan::create() track:" << track << endl;
    trackTerminal = new TrackFixedSpan ( net, bb );
    trackTerminal->_postCreate( track );
    cdebug_log(159,0) << trackTerminal << endl;
    Session::addInsertEvent( trackTerminal, track, track->getAxis() );
    return trackTerminal;
  }

  
  void  TrackFixedSpan::setNet ( Net* net )
  {
    if (not net or (net == Session::getBlockageNet())) {
      _net = Session::getBlockageNet();
      setFlags( TElemBlockage );
    } else {
      _net = net;
      unsetFlags( TElemBlockage );
    }
  }

  Net* TrackFixedSpan::getNet () const
  { return _net; }


  string  TrackFixedSpan::_getTypeName () const
  { return "TrackFixedSpan"; }


  Record* TrackFixedSpan::_getRecord () const
  {
    Record* record = Super::_getRecord();
    record->add( getSlot( "_net", _net ));
    return record;
  }


} // Katana namespace.
