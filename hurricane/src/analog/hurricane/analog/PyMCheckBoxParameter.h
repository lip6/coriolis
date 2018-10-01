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
// |  C++ Header  :  "./hurricane/analog/PyMCheckBoxParameter.h"     |
// +-----------------------------------------------------------------+


#ifndef  ANALOG_PY_M_CHECKBOX_PARAMETER_H
#define  ANALOG_PY_M_CHECKBOX_PARAMETER_H

#include "hurricane/analog/PyParameter.h"
#include "hurricane/analog/MCheckBoxParameter.h"


namespace  Isobar {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyMCheckBoxParameter".

  typedef struct {
      PyParameter  _baseObject;
  } PyMCheckBoxParameter;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeMCheckBoxParameter;
  extern PyMethodDef   PyMCheckBoxParameter_Methods[];

  extern PyObject* PyMCheckBoxParameter_Link       ( Analog::MCheckBoxParameter* object );
  extern void      PyMCheckBoxParameter_LinkPyType ();


#define IsPyMCheckBoxParameter(v)    ( (v)->ob_type == &PyTypeMCheckBoxParameter )
#define PYMCHECKBOXPARAMETER(v)      ( (PyMCheckBoxParameter*)(v) )
#define PYMCHECKBOXPARAMETER_O(v)    ( PYMCHECKBOXPARAMETER(v)->_baseObject->_baseObject._object )


}  // extern "C".

}  // Isobar namespace.

#endif  // ANALOG_PY_M_CHECKBOX_PARAMETER_H
