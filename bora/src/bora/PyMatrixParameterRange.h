// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2019-2019, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |  B o r a  -  A n a l o g   S l i c i n g   T r e e              |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./bora/PyMatrixParameterRange.h"               |
// +-----------------------------------------------------------------+


#ifndef  BORA_PY_MATRIX_PARAMETER_RANGE_H
#define  BORA_PY_MATRIX_PARAMETER_RANGE_H

#include "bora/PyParameterRange.h"


namespace  Bora {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyMatrixParameterRange".

  typedef struct {
      PyParameterRange  _baseObject;
  } PyMatrixParameterRange;


// -------------------------------------------------------------------
// Functions & Types exported to "PyBora.cpp".

  extern PyTypeObject  PyTypeMatrixParameterRange;
  extern PyMethodDef   PyMatrixParameterRange_Methods[];

  extern PyObject* PyMatrixParameterRange_Link       ( MatrixParameterRange* node );
  extern void      PyMatrixParameterRange_LinkPyType ();


#define IsPyMatrixParameterRange(v)    ( (v)->ob_type == &PyTypeMatrixParameterRange )
#define PYMATRIXPARAMETERRANGE(v)      ( (PyMatrixParameterRange*)(v) )
#define PYMATRIXPARAMETERRANGE_O(v)    ( dynamic_cast<MatrixParameterRange*>(PYMATRIXPARAMETERRANGE(v)->_baseObject._object) )


}  // extern "C".

}  // Bora namespace.

#endif  // BORA_PY_MATRIX_PARAMETER_RANGE_H
