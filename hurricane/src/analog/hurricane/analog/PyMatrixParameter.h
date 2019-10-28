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
// |  C++ Header  :  "./hurricane/analog/PyMatrixParameter.h"        |
// +-----------------------------------------------------------------+


#ifndef  ANALOG_PY_MATRIX_PARAMETER_H
#define  ANALOG_PY_MATRIX_PARAMETER_H

#include "hurricane/analog/PyParameter.h"
#include "hurricane/analog/MatrixParameter.h"


namespace  Isobar {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyMatrixParameter".

  typedef struct {
      PyParameter  _baseObject;
  } PyMatrixParameter;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeMatrixParameter;
  extern PyMethodDef   PyMatrixParameter_Methods[];

  extern PyObject* PyMatrixParameter_Link       ( Analog::MatrixParameter* object );
  extern void      PyMatrixParameter_LinkPyType ();


#define IsPyMatrixParameter(v)    ( (v)->ob_type == &PyTypeMatrixParameter )
#define PYMATRIXPARAMETER(v)      ( (PyMatrixParameter*)(v) )
#define PYMATRIXPARAMETER_O(v)    ( PYMATRIXPARAMETER(v)->_baseObject->_baseObject._object )


}  // extern "C".

}  // Isobar namespace.

#endif  // ANALOG_PY_MATRIX_PARAMETER_H
