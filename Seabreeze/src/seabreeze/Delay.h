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


#pragma once
#include <map>
#include <iostream>
#include "hurricane/Contact.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Component.h"

using namespace std;

namespace Seabreeze {

  using Hurricane::RoutingPad; 

//---------------------------------------------------------
// Class : Seabreeze::Delay

  class Delay {
    public:
                        Delay       ();
                       ~Delay       ();
      inline const void getValue    ( RoutingPad* );
      inline       void addPair     ( RoutingPad*, RoutingPad*, double );
      inline       void addValue    ( RoutingPad*, RoutingPad*, double );
      inline       void printDelays ();
      
    private:
      map<RoutingPad*, map<RoutingPad*, double>> _values;
  };
}
