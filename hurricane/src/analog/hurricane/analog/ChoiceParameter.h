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
// |  C++ Header  :  "./hurricane/analog/ChoiceParameter.h"          |
// +-----------------------------------------------------------------+


#ifndef ANALOG_CHOICE_PARAMETER_H
#define ANALOG_CHOICE_PARAMETER_H

#include "hurricane/analog/Parameter.h"
#include "hurricane/analog/Choices.h"


namespace Analog {


  class ChoiceParameter : public Parameter {
    public:
      typedef Parameter  Super;
    public:
      inline                   ChoiceParameter  ( std::string id, const Choices& );
              Choices::Values  getChoicesValues () const;
              std::string      getValue         () const;
              void             setValue         ( const std::string& choice );
      virtual std::string      _getTypeName     () const;
      virtual std::string      _getString       () const;
      virtual Record*          _getRecord       () const;
    private:
      Choices   _choices;
      unsigned  _value;
  };


  inline  ChoiceParameter::ChoiceParameter ( std::string id, const Choices& choices )
    : Parameter(id), _choices(choices), _value(0)
  { }

  inline  Choices::Values  ChoiceParameter::getChoicesValues () const { return _choices.getValues(); }


}  // Analog namespace.


INSPECTOR_PR_SUPPORT(Analog::ChoiceParameter);

#endif  // ANALOG_CHOICE_PARAMETER_H
