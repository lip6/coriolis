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
// |  C++ Header  :  "./hurricane/analog/PyStepParameter.h"          |
// +-----------------------------------------------------------------+


#pragma once
#include "hurricane/analog/PyParameter.h"
#include "hurricane/analog/StepParameter.h"


namespace  Isobar {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyStepParameter".

  typedef struct {
      PyParameter  _baseObject;
  } PyStepParameter;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeStepParameter;
  extern PyMethodDef   PyStepParameter_Methods[];

  extern PyObject* PyStepParameter_Link       ( Analog::StepParameter* object );
  extern void      PyStepParameter_LinkPyType ();


#define IsPyStepParameter(v)    ( (v)->ob_type == &PyTypeStepParameter )
#define PYSTEPPARAMETER(v)      ( (PyStepParameter*)(v) )
#define PYSTEPPARAMETER_O(v)    ( PYSTEPPARAMETER(v)->_baseObject->_baseObject._object )


}  // extern "C".

}  // Isobar namespace.
