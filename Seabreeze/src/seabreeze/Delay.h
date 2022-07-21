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
#include "hurricane/RoutingPad.h"


namespace Seabreeze {

  using Hurricane::DBo; 
  using Hurricane::RoutingPad; 


//---------------------------------------------------------
// Class : Seabreeze::Delay

  class Delay {
    public:
      typedef std::map< RoutingPad*
                      , std::map< RoutingPad*, double, DBo::CompareById >
                      , DBo::CompareById>  DelayMap;
    public:
                                Delay        ();
                               ~Delay        ();
      inline const DelayMap&    getValues    () const ;
                   void         addPair      ( RoutingPad*, RoutingPad*, double );
                   void         addValue     ( RoutingPad*, RoutingPad*, double );
                   void         printDelays  ();
                   Record*      _getRecord   () const;
                   std::string  _getString   () const;
                   std::string  _getTypeName () const;
      
    private:
      DelayMap  _values;
  };
  

  inline const Delay::DelayMap& Delay::getValues () const { return _values; }

  
}  // Seabreeze namespace.


INSPECTOR_P_SUPPORT(Seabreeze::Delay);
