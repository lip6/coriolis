// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU 2022-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        S e a b r e e z e  -  Timing Analysis                    |
// |                                                                 |
// |  Author      :                   Vu Hoang Anh PHAM              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./seabreeze/Delay.h"                           |
// +-----------------------------------------------------------------+


#pragma once
#include <map>
#include <iostream>
#include "hurricane/RoutingPad.h"
#include "seabreeze/Delay.h"

using namespace std;

namespace Seabreeze {

  using Hurricane::RoutingPad; 

//---------------------------------------------------------
// Class : Seabreeze::Delay

    Delay::Delay()
      : _values()
    {}

    Delay::~Delay()
    {}
    
    void Delay::addPair ( RoutingPad* driver, RoutingPad* sink, double delay)
    { 
      if ( _values.count(driver) > 0 ) {
        cerr << "Driver already exist." << endl
             << "If you want to add value, please use the function addValue( RoutingPad*, RoutingPad*, double)" << endl
             << "Example : addValue(driver, sink, delay)" << endl;
      }
      else {
        map<RoutingPad*, double> val { {sink, delay}, };
        _values.insert(driver, val);
      }
    }
}
