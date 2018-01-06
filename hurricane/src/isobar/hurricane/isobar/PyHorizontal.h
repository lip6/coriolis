// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2007-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                     Sophie BELLOEIL              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/isobar/PyCell.h"                   |
// +-----------------------------------------------------------------+


#ifndef PY_HORIZONTAL_H
#define PY_HORIZONTAL_H

#include "hurricane/isobar/PySegment.h"
#include "hurricane/Horizontal.h"


namespace  Isobar {

  extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyHorizontal".

    typedef struct {
        PySegment  _baseObject;
    } PyHorizontal;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

    extern  PyTypeObject  PyTypeHorizontal;
    extern  PyMethodDef   PyHorizontal_Methods[];

    extern  PyObject* PyHorizontal_Link       ( Hurricane::Horizontal* object );
    extern  void      PyHorizontal_LinkPyType ();



#define IsPyHorizontal(v)    ( (v)->ob_type == &PyTypeHorizontal )
#define PYHORIZONTAL(v)      ( (PyHorizontal*)(v) )
#define PYHORIZONTAL_O(v)    ( PYHORIZONTAL(v)->_baseObject._baseObject._baseObject._object )


  }  // extern "C".

}  // Isobar namespace.

#endif  // PY_HORIZONTAL_H
