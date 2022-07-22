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

  using Hurricane::Record; 
  using Hurricane::DBo; 
  using Hurricane::RoutingPad; 
  class Elmore;


//---------------------------------------------------------
// Class : Seabreeze::Delay

  class Delay {
    public:
                          Delay        ( Elmore*, RoutingPad* );
                         ~Delay        ();
      inline Elmore*      getElmore    () const;
      inline RoutingPad*  getSink      () const;
      inline double       getDelay     () const;
      inline void         setDelay     ( double );
      inline void         incDelay     ( double );
             std::string  _getTypeName () const;
             std::string  _getString   () const;
             Record*      _getRecord   () const;
      
    private:
      Elmore*     _elmore;
      RoutingPad* _sink;
      double      _delay;
  };

  
  inline Elmore*      Delay::getElmore () const { return _elmore; }
  inline RoutingPad*  Delay::getSink   () const { return _sink; }
  inline double       Delay::getDelay  () const { return _delay; }
  inline void         Delay::setDelay  ( double delay ) { _delay  = delay; }
  inline void         Delay::incDelay  ( double delay ) { _delay += delay; }
  

}  // Seabreeze namespace.


INSPECTOR_P_SUPPORT(Seabreeze::Delay);
