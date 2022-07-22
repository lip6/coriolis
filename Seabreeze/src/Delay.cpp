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
// |  C++ Header  :  "./seabreeze/Delay.h"                            |
// +-----------------------------------------------------------------+


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
        cerr << "Driver " << driver << " already exists." << endl
             << "If you want to add value, please use the function addValue( RoutingPad*, RoutingPad*, double)" << endl
             << "Example : addValue(driver, sink, delay)" << endl;
      }
      else {
        map<RoutingPad*, double> val { {sink, delay}, };
        _values.insert({driver, val});
      }
    }

    void Delay::addValue ( RoutingPad* driver, RoutingPad* sink, double delay )
    {
      if ( _values[driver].count(sink) > 0 ) {
        cerr << "Delay from " << driver << " to " << sink << " is already computed" << endl;
      }
      else {
        _values[driver].insert({sink, delay});
      }
    }

    void Delay::printDelays ()
    {
      for ( auto dmap : _values ) {
        cerr << "Delay from : " << dmap.first << " to  :" << endl;
        map<RoutingPad*, double>& val = dmap.second;
        for ( auto smap : val ) {
          cerr << smap.first << " : " << smap.second << endl;
        }
      }
    }
}
