// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2019-2019, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/analog/PyFloatParameter.h"         |
// +-----------------------------------------------------------------+


#ifndef  ANALOG_PY_FLOAT_PARAMETER_H
#define  ANALOG_PY_FLOAT_PARAMETER_H

#include "hurricane/analog/PyParameter.h"
#include "hurricane/analog/FloatParameter.h"


namespace  Isobar {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyFloatParameter".

  typedef struct {
      PyParameter  _baseObject;
  } PyFloatParameter;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeFloatParameter;
  extern PyMethodDef   PyFloatParameter_Methods[];

  extern PyObject* PyFloatParameter_Link       ( Analog::FloatParameter* object );
  extern void      PyFloatParameter_LinkPyType ();


#define IsPyFloatParameter(v)    ( (v)->ob_type == &PyTypeFloatParameter )
#define PYFLOATPARAMETER(v)      ( (PyFloatParameter*)(v) )
#define PYFLOATPARAMETER_O(v)    ( PYFLOATPARAMETER(v)->_baseObject->_baseObject._object )


}  // extern "C".

}  // Isobar namespace.

#endif  // ANALOG_PY_FLOAT_PARAMETER_H
