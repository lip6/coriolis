// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2014-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/isobar/PyOrientation.h"            |
// +-----------------------------------------------------------------+


#ifndef PY_ORIENTATION_H
#define PY_ORIENTATION_H

#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/Transformation.h"


namespace  Isobar {

  extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyOrientation".

    typedef struct {
        PyObject_HEAD
        Hurricane::Transformation::Orientation* _object;
    } PyOrientation;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

    extern  PyTypeObject  PyTypeOrientation;
    extern  PyMethodDef   PyOrientation_Methods[];

    extern  PyObject* PyOrientation_Link           ( Hurricane::Transformation::Orientation* );
    extern  void      PyOrientation_LinkPyType     ();
    extern  void      PyOrientation_postModuleInit ();


#define IsPyOrientation(v)  ( (v)->ob_type == &PyTypeOrientation )
#define PYORIENTATION(v)    ( (PyOrientation*)(v) )
#define PYORIENTATION_O(v)  ( PYORIENTATION(v)->_object )


  }  // End of extern "C".

}  // End of Isobar namespace.

#endif  // PY_ORIENTATION_H
