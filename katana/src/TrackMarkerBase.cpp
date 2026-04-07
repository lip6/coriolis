// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2025-2025.
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./TrackMarkerBase.cpp"                    |
// +-----------------------------------------------------------------+


#include <iomanip>
#include <sstream>
#include "hurricane/Bug.h"
#include "hurricane/Warning.h"
#include "hurricane/DebugSession.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Net.h"
#include "hurricane/Name.h"
#include "crlcore/RoutingGauge.h"
#include "anabatic/GCell.h"
#include "katana/TrackMarkerBase.h"
#include "katana/Track.h"
#include "katana/RoutingPlane.h"
#include "katana/Session.h"
#include "katana/RoutingEvent.h"
#include "katana/KatanaEngine.h"


namespace Katana {

  using std::cerr;
  using std::endl;
  using std::ostringstream;
  using std::setprecision;
  using Hurricane::Bug;
  using Hurricane::DebugSession;
  using CRL::RoutingGauge;


  void  TrackMarkerBase::destroy ()
  {
    if (--_refCount == 0) delete this;
  }


  TrackMarkerBase::TrackMarkerBase ()
    : _sourcePosition(0)
    , _targetPosition(0)
    , _track         (nullptr)
    , _refCount      (0)
  { }


  string  TrackMarkerBase::_getTypeName () const
  { return "TrackMarkerBase"; }


  string  TrackMarkerBase::_getString () const
  {
    ostringstream s;
    s << "<"   << _getTypeName()
      << " "   << getNet()->getName()
      << " ["  << DbU::getValueString(_sourcePosition)
      << ":"   << DbU::getValueString(_targetPosition)
      << "]>";
    return s.str();
  }


  Record* TrackMarkerBase::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    record->add( getSlot ( "_sourcePosition",  _sourcePosition ));
    record->add( getSlot ( "_targetPosition",  _targetPosition ));
    record->add( getSlot ( "_track"         ,  _track          ));
    record->add( getSlot ( "_refCount"      ,  _refCount       ));
    return record;
  }


} // Katana namespace.
