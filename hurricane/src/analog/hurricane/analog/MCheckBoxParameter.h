// File:   MCheckBoxParameter.h
//
// Date:   2010/10/05
// Author: Damien Dupuis
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2010-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Authors     :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/analog/MCheckBoxParameter.h"       |
// +-----------------------------------------------------------------+


#ifndef ANALOG_MCHECKBOX_PARAMETER_H
#define ANALOG_MCHECKBOX_PARAMETER_H

#include "hurricane/analog/Parameter.h"
#include "hurricane/analog/Choices.h"


namespace Analog {
  
  class MCheckBoxParameter : public Parameter {
    public:
      inline                  MCheckBoxParameter ( std::string id, const Choices& );
      inline Choices::Values  getChoicesValues   () const;
      inline unsigned int     getValue           () const;
      inline void             setValue           ( unsigned int );
    private:
      Choices   _choices;
      unsigned  _value;
  };


  inline                  MCheckBoxParameter::MCheckBoxParameter ( std::string id, const Choices& choices ): Parameter(id), _choices(choices), _value(0) { } 
  inline Choices::Values  MCheckBoxParameter::getChoicesValues   () const { return _choices.getValues(); }
  inline unsigned int     MCheckBoxParameter::getValue           () const { return _value; }
  inline void             MCheckBoxParameter::setValue           ( unsigned int value ) { _value = value; }


}  // Analog namespace.

#endif  // ANALOG_MCHECKBOX_PARAMETER_H
