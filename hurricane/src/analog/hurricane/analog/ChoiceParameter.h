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
      inline                   ChoiceParameter  ( std::string id, const Choices& );
      virtual                 ~ChoiceParameter  () { }
              Choices::Values  getChoicesValues () const;
              std::string      getValue         () const;
              void             setValue         ( const std::string& choice );
    private:
      Choices   _choices;
      unsigned  _value;
  };


  inline  ChoiceParameter::ChoiceParameter ( std::string id, const Choices& choices )
    : Parameter(id), _choices(choices), _value(0)
  { }

  inline  Choices::Values  ChoiceParameter::getChoicesValues () const { return _choices.getValues(); }


}  // Analog namespace.

#endif  // ANALOG_CHOICE_PARAMETER_H
