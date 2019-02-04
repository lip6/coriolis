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
// |  C++ Header  :  "./hurricane/analog/PySpinBoxParameter.h"       |
// +-----------------------------------------------------------------+


#ifndef  ANALOG_PY_SPINBOX_PARAMETER_H
#define  ANALOG_PY_SPINBOX_PARAMETER_H

#include "hurricane/analog/PyParameter.h"
#include "hurricane/analog/SpinBoxParameter.h"


namespace  Isobar {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PySpinBoxParameter".

  typedef struct {
      PyParameter  _baseObject;
  } PySpinBoxParameter;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeSpinBoxParameter;
  extern PyMethodDef   PySpinBoxParameter_Methods[];

  extern PyObject* PySpinBoxParameter_Link       ( Analog::SpinBoxParameter* object );
  extern void      PySpinBoxParameter_LinkPyType ();


#define IsPySpinBoxParameter(v)    ( (v)->ob_type == &PyTypeSpinBoxParameter )
#define PYSPINBOXPARAMETER(v)      ( (PySpinBoxParameter*)(v) )
#define PYSPINBOXPARAMETER_O(v)    ( PYSPINBOXPARAMETER(v)->_baseObject->_baseObject._object )


}  // extern "C".

}  // Isobar namespace.

#endif  // ANALOG_PY_SPINBOX_PARAMETER_H
