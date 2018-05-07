// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2006-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/isobar/PyDiagonal.h"               |
// +-----------------------------------------------------------------+


#ifndef PY_DIAGONAL_H
#define PY_DIAGONAL_H

#include "hurricane/isobar/PyComponent.h"
#include "hurricane/Diagonal.h"


namespace  Isobar {

  extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyDiagonal".

    typedef struct {
        PyComponent  _baseObject;
    } PyDiagonal;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

    extern  PyTypeObject  PyTypeDiagonal;
    extern  PyMethodDef   PyDiagonal_Methods[];

    extern  PyObject* PyDiagonal_Link       ( Hurricane::Diagonal* object );
    extern  void      PyDiagonal_LinkPyType ();


#define IsPyDiagonal(v)    ( (v)->ob_type == &PyTypeDiagonal )
#define PYDIAGONAL(v)      ( (PyDiagonal*)(v) )
#define PYDIAGONAL_O(v)    ( PYDIAGONAL(v)->_baseObject._baseObject._object )


  }  // extern "C".

}  // Isobar namespace.

#endif // PY_DIAGONAL_H
