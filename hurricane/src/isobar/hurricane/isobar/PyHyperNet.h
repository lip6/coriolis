// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2007-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                       Damien DUPUIS              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/isobar/PyHyperNet.h"               |
// +-----------------------------------------------------------------+


#ifndef PY_HYPERNET_H
#define PY_HYPERNET_H

#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/HyperNet.h"


namespace  Isobar {

  extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyHyperNet".

    typedef struct {
        PyObject_HEAD
        Hurricane::HyperNet* _object;
    } PyHyperNet;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

    extern PyTypeObject PyTypeHyperNet;
    extern PyMethodDef  PyHyperNet_Methods[];

    extern void      PyHyperNet_LinkPyType();


#define IsPyHyperNet(v)    ( (v)->ob_type == &PyTypeHyperNet )
#define PYHYPERNET(v)      ( (PyHyperNet*)(v) )
#define PYHYPERNET_O(v)    ( PYHYPERNET(v)->_object )


  }  // extern "C".

}  // Isobar namespace.

#endif  // PY_HYPERNET_H
