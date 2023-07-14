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
// |  C++ Header  :  "./hurricane/analog/PyMatrix.h"                 |
// +-----------------------------------------------------------------+


#ifndef  ANALOG_PY_MATRIX_H
#define  ANALOG_PY_MATRIX_H

#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/analog/Matrix.h"


namespace  Isobar {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyMatrix".

  typedef struct {
      PyObject_HEAD
      Analog::Matrix* _object;
  } PyMatrix;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeMatrix;
  extern PyMethodDef   PyMatrix_Methods[];

  extern void            PyMatrix_LinkPyType   ();


#define IsPyMatrix(v)    ( (v)->ob_type == &PyTypeMatrix )
#define PYMATRIX(v)      ( (PyMatrix*)(v) )
#define PYMATRIX_O(v)    ( PYMATRIX(v)->_object )


}  // extern "C".

  extern Analog::Matrix  Matrix_FromListOfList ( PyObject* );

}  // Isobar namespace.

#endif  // ANALOG_PY_MATRIX_H
