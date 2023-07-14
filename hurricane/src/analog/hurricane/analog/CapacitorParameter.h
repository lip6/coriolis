// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2010-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :             Damien Dupuis, Eric Lao              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/analog/CapacitorParameter.h"       |
// +-----------------------------------------------------------------+


#ifndef ANALOG_CAPACITOR_PARAMETER_H
#define ANALOG_CAPACITOR_PARAMETER_H

#include "hurricane/analog/Parameter.h"


namespace Analog {

  class CapacitorParameter : public Parameter {
    public:
      inline               CapacitorParameter ( std::string id, double value );
      inline  double       getValue           () const;
      inline               operator double    () const;
      inline  void         setValue           ( double );
      virtual std::string  _getTypeName       () const { return "CapacitorParameter"; }
    private:
      double  _value;
  };


  inline          CapacitorParameter::CapacitorParameter ( std::string id, double value ) : Parameter(id), _value(value) { } 
  inline double   CapacitorParameter::getValue           () const { return _value; }
  inline          CapacitorParameter::operator double    () const { return getValue(); }
  inline void     CapacitorParameter::setValue           ( double value ) { _value = value; }


}  // Analog namespace.

#endif // ANALOG_CAPACITOR_PARAMETER_H
