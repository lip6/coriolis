// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2009-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |         C R L C o r e   -  ASCII/GDS Driver                     |
// |                                                                 |
// |  Author      :                       Damien DUPUIS              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./crlcore/GdsDriver.h"                    |
// +-----------------------------------------------------------------+


#ifndef CRLCORE_GDS_DRIVER_H
#define CRLCORE_GDS_DRIVER_H

#include <string>

namespace Hurricane {
  class Cell;
}


namespace CRL {


  class GdsDriver {
    public:
                         GdsDriver ( Hurricane::Cell* );
             bool        save      ( const std::string& filePath );
      inline std::string getName   ();
      inline std::string getLib    ();
      inline double      getUUnits ();
      inline double      getPUnits ();
    private:
      Hurricane::Cell* _cell;
      std::string      _name;
      std::string      _lib;
      double           _uUnits;
      double           _pUnits;
  };


  inline std::string GdsDriver::getName()   { return _name;   };
  inline std::string GdsDriver::getLib()    { return _lib;    };
  inline double      GdsDriver::getUUnits() { return _uUnits; };
  inline double      GdsDriver::getPUnits() { return _pUnits; };


} // CRL namespace.

#endif  // CRLCORE_GDS_DRIVER_H

