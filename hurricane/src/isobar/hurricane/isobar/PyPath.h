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
// |  C++ Header  :  "./hurricane/isobar/PyPath.h"                   |
// +-----------------------------------------------------------------+


#ifndef PY_PATH_H
#define PY_PATH_H

#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/Path.h"


namespace Isobar {

  extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyPath".

    typedef struct {
        PyObject_HEAD
        Hurricane::Path* _object;
    } PyPath;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

    extern  PyTypeObject  PyTypePath;
    extern  PyMethodDef   PyPath_Methods[];

    extern  void      PyPath_LinkPyType ();


#define IsPyPath(v)    ( (v)->ob_type == &PyTypePath )
#define PYPATH(v)      ( (PyPath*)(v) )
#define PYPATH_O(v)    ( PYPATH(v)->_object )


  }  // extern "C".

}  // Isobar namespace.

#endif  // PY_PATH_H
