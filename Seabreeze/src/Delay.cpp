// -*- C++ -*-
// //
// // This file is part of the Coriolis Software.
// // Copyright (c) SU 2022-2022, All Rights Reserved
// //
// // +-----------------------------------------------------------------+
// // |                   C O R I O L I S                               |
// // |        S e a b r e e z e  -  Timing Analysis                    |
// // |                                                                 |
// // |  Author      :                   Vu Hoang Anh PHAM              |
// // |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// // | =============================================================== |
// // |  C++ Header  :  "./seabreeze/Delay.h"                           |
// // +-----------------------------------------------------------------+

#include <map>
#include <iostream>
#include "hurricane/RoutingPad.h"
#include "seabreeze/Delay.h"
#include "seabreeze/Elmore.h"

using namespace std;


namespace Seabreeze {

  using Hurricane::RoutingPad; 


//---------------------------------------------------------
// Class : Seabreeze::Delay

Delay::Delay ( Elmore* elmore, RoutingPad* sink )
    : _elmore(elmore)
    , _sink  (sink)
    , _delay (0.0)
  { }


  Delay::~Delay ()
  { }


  string  Delay::_getTypeName () const
  { return "Seabreeze::Delay"; }


  string  Delay::_getString () const
  {
    string  s = "<Delay ";
    s += getString( _sink );
    s += " d=" + getString( _delay );
    s += ">";
    return s;
  }


  Record* Delay::_getRecord () const
  {
    Record* record = new Record ( _getString() );
    if (record != nullptr) {
      record->add( getSlot("_elmore" ,  _elmore) );
      record->add( getSlot("_sink"   ,  _sink  ) );
      record->add( getSlot("_delay"  ,  _delay ) );
    }
   return record;
  }

  
}  // Seabreeze namespace.

