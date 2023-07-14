// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2009-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/analog/StringParameter.h"          |
// +-----------------------------------------------------------------+

#pragma once
#include "hurricane/analog/Parameter.h"


namespace Analog {

  class StringParameter : public Parameter {
    public:
      typedef Parameter  Super;
    public:
      inline               StringParameter ( std::string id, std::string v );
      inline  std::string  getValue        () const;
      inline  operator std::string         () const;
      inline  void         setValue        ( std::string );
      virtual std::string  _getTypeName    () const;
      virtual std::string  _getString      () const;
      virtual Record*      _getRecord      () const;
    private:
      std::string  _value;
  };

  
  inline              StringParameter::StringParameter ( std::string id, std::string v ): Parameter(id), _value(v) {} 
  inline std::string  StringParameter::getValue        () const { return _value; }
  inline              StringParameter::operator std::string () const { return getValue(); }
  inline void         StringParameter::setValue        ( std::string value ) { _value = value; }


}  // Analog namespace.


INSPECTOR_PR_SUPPORT(Analog::StringParameter);
