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
// |  C++ Header  : "./hurricane/analog/PyFormFactorParameter.h"     |
// +-----------------------------------------------------------------+


#ifndef  ANALOG_PY_FORM_FACTOR_PARAMETER_H
#define  ANALOG_PY_FORM_FACTOR_PARAMETER_H

#include "hurricane/analog/PyParameter.h"
#include "hurricane/analog/FormFactorParameter.h"


namespace  Isobar {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyFormFactorParameter".

  typedef struct {
      PyParameter  _baseObject;
  } PyFormFactorParameter;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeFormFactorParameter;
  extern PyMethodDef   PyFormFactorParameter_Methods[];

  extern PyObject* PyFormFactorParameter_Link       ( Analog::FormFactorParameter* object );
  extern void      PyFormFactorParameter_LinkPyType ();


#define IsPyFormFactorParameter(v)    ( (v)->ob_type == &PyTypeFormFactorParameter )
#define PYFORMFACTORPARAMETER(v)      ( (PyFormFactorParameter*)(v) )
#define PYFORMFACTORPARAMETER_O(v)    ( PYFORMFACTORPARAMETER(v)->_baseObject->_baseObject._object )


}  // extern "C".

}  // Isobar namespace.

#endif  // ANALOG_PY_FORM_FACTOR_PARAMETER_H
