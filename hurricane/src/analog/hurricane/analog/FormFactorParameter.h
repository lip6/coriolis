// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2009-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                Christophe Alexandre              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/analog/FormFactorParameter.h"      |
// +-----------------------------------------------------------------+


#ifndef ANALOG_FORMFACTOR_PARAMETER_H
#define ANALOG_FORMFACTOR_PARAMETER_H

#include "hurricane/analog/Parameter.h"


namespace Analog {

  class FormFactorParameter : public Parameter {
    public:
      typedef Parameter  Super;
    public:
      inline               FormFactorParameter ( std::string id, long min, long max);
      inline  long         getMin              () const;
      inline  long         getMax              () const;
      inline  long         getValue            () const;
      inline               operator long       () const ;
      inline  void         setValue            ( long );
      inline  void         setMax              ( long );
      virtual std::string  _getTypeName        () const;
      virtual std::string  _getString          () const;
      virtual Record*      _getRecord          () const;
    private:
      long _min;
      long _max;
      long _value;
  };


  inline FormFactorParameter::FormFactorParameter ( std::string id, long min, long max )
    : Parameter(id)
    , _min     (min)
    , _max     (max)
    , _value   (min)
  { } 

  inline long     FormFactorParameter::getMin        () const { return _min; }
  inline long     FormFactorParameter::getMax        () const { return _max; }
  inline long     FormFactorParameter::getValue      () const { return _value; }
  inline          FormFactorParameter::operator long () const { return getValue(); }
  inline void     FormFactorParameter::setValue      ( long value ) { _value = value; }
  inline void     FormFactorParameter::setMax        ( long max ) { _max = max; }
  

}  // Analog namespace.


INSPECTOR_P_SUPPORT(Analog::FormFactorParameter);

#endif // ANALOG_FORMFACTOR_PARAMETER_H
