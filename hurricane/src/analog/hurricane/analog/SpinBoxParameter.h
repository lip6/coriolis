// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2009-2023, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                Christophe Alexandre              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/analog/SpinBoxParameter.h"         |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/analog/Parameter.h"

namespace Analog {
  

  class SpinBoxParameter : public Parameter {
    public:
      typedef Parameter Super;
    public:
      inline               SpinBoxParameter ( std::string id, long min, long max );
      inline  long         getMin           () const;
      inline  long         getMax           () const;
      inline  long         getValue         () const;
      inline  operator     long             () const;
      inline  void         setValue         ( long value );
      virtual std::string  _getTypeName     () const;
      virtual std::string  _getString       () const;
      virtual Record*      _getRecord       () const;
    private:
      long _min;
      long _max;
      long _value;
  };

  
  inline          SpinBoxParameter::SpinBoxParameter ( std::string id, long min, long max ): Parameter(id), _min(min), _max(max), _value(min) { } 
  inline long     SpinBoxParameter::getMin           () const { return _min; }
  inline long     SpinBoxParameter::getMax           () const { return _max; }
  inline long     SpinBoxParameter::getValue         () const { return _value; }
  inline          SpinBoxParameter::operator long    () const { return getValue(); }
  inline void     SpinBoxParameter::setValue         ( long value ) { _value = value; }


}  // Analog namespace.


INSPECTOR_PR_SUPPORT(Analog::SpinBoxParameter);
