// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2019-2019, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/analog/FloatParameter.h"           |
// +-----------------------------------------------------------------+

#ifndef ANALOG_FLOAT_PARAMETER_H
#define ANALOG_FLOAT_PARAMETER_H

#include "hurricane/analog/Parameter.h"


namespace Analog {

  class FloatParameter : public Parameter {
    public:
      typedef Parameter  Super;
    public:
      inline               FloatParameter ( std::string id, float value=0.0 );
      inline  float        getValue       () const;
      inline  void         setValue       ( float );
      inline               operator long  () const;
      virtual std::string  _getTypeName   () const;
      virtual std::string  _getString     () const;
      virtual Record*      _getRecord     () const;
    private:
      float  _value;
  };

  
  inline        FloatParameter::FloatParameter ( std::string id, float value ): Parameter(id), _value(value) {} 
  inline float  FloatParameter::getValue       () const { return _value; }
  inline void   FloatParameter::setValue       ( float value ) { _value = value; }
  inline        FloatParameter::operator long  () const { return getValue(); }


}  // Analog namespace.


INSPECTOR_PR_SUPPORT(Analog::FloatParameter);

#endif // ANALOG_FLOAT_PARAMETER_H
