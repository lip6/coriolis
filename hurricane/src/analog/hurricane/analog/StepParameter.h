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
// |  C++ Header  :  "./hurricane/analog/StepParameter.h"            |
// +-----------------------------------------------------------------+

#pragma once
#include "hurricane/analog/Parameter.h"


namespace Analog {

  class StepParameter : public Parameter {
    public:
      typedef Parameter  Super;
    public:
      inline               StepParameter ( std::string id, long min, long max, long step );
      inline  long         getMin        () const;
      inline  long         getMax        () const;
      inline  long         getStep       () const;
      inline  long         getValue      () const;
      inline  operator long              () const;
      inline  void         setValue      ( long );
      virtual std::string  _getTypeName  () const;
      virtual std::string  _getString    () const;
      virtual Record*      _getRecord    () const;
    private:
      long  _min;
      long  _max;
      long  _step;
      long  _value;
  };

  
  inline       StepParameter::StepParameter ( std::string id, long min, long max, long step ): Parameter(id), _min(min), _max(max), _step(step), _value(min) {} 
  inline long  StepParameter::getMin        () const { return _min; }
  inline long  StepParameter::getMax        () const { return _max; }
  inline long  StepParameter::getStep       () const { return _step; }
  inline long  StepParameter::getValue      () const { return _value; }
  inline       StepParameter::operator long () const { return getValue(); }
  inline void  StepParameter::setValue      ( long value ) { _value = value; }


}  // Analog namespace.


INSPECTOR_PR_SUPPORT(Analog::StepParameter);
