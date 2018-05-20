// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2018-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/isobar/PyRectilinear.h"            |
// +-----------------------------------------------------------------+


#ifndef PY_RECTILINEAR_H
#define PY_RECTILINEAR_H

#include "hurricane/isobar/PyComponent.h"
#include "hurricane/Rectilinear.h"


namespace  Isobar {

  extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyRectilinear".

    typedef struct {
        PyComponent  _baseObject;
    } PyRectilinear;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

    extern  PyTypeObject  PyTypeRectilinear;
    extern  PyMethodDef   PyRectilinear_Methods[];

    extern  PyObject* PyRectilinear_Link       ( Hurricane::Rectilinear* object );
    extern  void      PyRectilinear_LinkPyType ();


#define IsPyRectilinear(v)    ( (v)->ob_type == &PyTypeRectilinear )
#define PYRECTILINEAR(v)      ( (PyRectilinear*)(v) )
#define PYRECTILINEAR_O(v)    ( PYRECTILINEAR(v)->_baseObject._baseObject._object )


  }  // extern "C".

}  // Isobar namespace.

#endif // PY_RECTILINEAR_H
