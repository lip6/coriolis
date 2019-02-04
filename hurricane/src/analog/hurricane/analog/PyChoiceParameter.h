// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/analog/PyChoiceParameter.h"        |
// +-----------------------------------------------------------------+


#ifndef  ANALOG_PY_CHOICE_PARAMETER_H
#define  ANALOG_PY_CHOICE_PARAMETER_H

#include "hurricane/analog/PyParameter.h"
#include "hurricane/analog/ChoiceParameter.h"


namespace  Isobar {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyChoiceParameter".

  typedef struct {
      PyParameter  _baseObject;
  } PyChoiceParameter;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeChoiceParameter;
  extern PyMethodDef   PyChoiceParameter_Methods[];

  extern PyObject* PyChoiceParameter_Link       ( Analog::ChoiceParameter* object );
  extern void      PyChoiceParameter_LinkPyType ();


#define IsPyChoiceParameter(v)    ( (v)->ob_type == &PyTypeChoiceParameter )
#define PYCHOICEPARAMETER(v)      ( (PyChoiceParameter*)(v) )
#define PYCHOICEPARAMETER_O(v)    ( PYCHOICEPARAMETER(v)->_baseObject->_baseObject._object )


}  // extern "C".

}  // Isobar namespace.

#endif  // ANALOG_PY_CHOICE_PARAMETER_H
