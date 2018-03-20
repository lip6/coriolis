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
// |  C++ Header  :  "./hurricane/isobar/PyTriangle.h"                    |
// +-----------------------------------------------------------------+


#ifndef PY_TRIANGLE_H
#define PY_TRIANGLE_H

#include "hurricane/isobar/PyComponent.h"
#include "hurricane/Triangle.h"


namespace  Isobar {

  extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyTriangle".

    typedef struct {
        PyComponent  _baseObject;
    } PyTriangle;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

    extern  PyTypeObject  PyTypeTriangle;
    extern  PyMethodDef   PyTriangle_Methods[];

    extern  PyObject* PyTriangle_Link       ( Hurricane::Triangle* object );
    extern  void      PyTriangle_LinkPyType ();


#define IsPyTriangle(v)    ( (v)->ob_type == &PyTypeTriangle )
#define PYTRIANGLE(v)      ( (PyTriangle*)(v) )
#define PYTRIANGLE_O(v)    ( PYTRIANGLE(v)->_baseObject._baseObject._object )


  }  // extern "C".

}  // Isobar namespace.

#endif // PY_TRIANGLE_H
