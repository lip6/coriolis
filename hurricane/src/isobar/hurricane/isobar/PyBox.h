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
// |  C++ Header  :  "./hurricane/isobar/PyBox.h"                    |
// +-----------------------------------------------------------------+


#ifndef  PY_BOX_H
#define  PY_BOX_H

#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/Box.h"


namespace  Isobar {

  extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyBox".

    typedef struct {
        PyObject_HEAD
        Hurricane::Box* _object;
    } PyBox;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

    extern PyTypeObject  PyTypeBox;
    extern PyMethodDef   PyBox_Methods[];

    extern void      PyBox_LinkPyType ();


#define IsPyBox(v)    ( (v)->ob_type == &PyTypeBox )
#define PYBOX(v)      ( (PyBox*)(v) )
#define PYBOX_O(v)    ( PYBOX(v)->_object )


  }  // extern "C".

}  // Isobar namespace.

#endif  // PY_BOX_H
