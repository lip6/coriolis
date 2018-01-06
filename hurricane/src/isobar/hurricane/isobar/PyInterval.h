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
// |  C++ Header  :  "./hurricane/isobar/PyInterval.h"               |
// +-----------------------------------------------------------------+


#ifndef  ISOBAR_PY_INTERVAL_H
#define  ISOBAR_PY_INTERVAL_H

#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/Interval.h"


namespace  Isobar {

  extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyInterval".

    typedef struct {
        PyObject_HEAD
        Hurricane::Interval* _object;
    } PyInterval;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

    extern PyTypeObject  PyTypeInterval;
    extern PyMethodDef   PyInterval_Methods[];

    extern PyObject*     PyInterval_Link       ( Hurricane::Interval* object );
    extern void          PyInterval_LinkPyType ();


#define IsPyInterval(v)    ( (v)->ob_type == &PyTypeInterval )
#define PYINTERVAL(v)      ( (PyInterval*)(v) )
#define PYINTERVAL_O(v)    ( PYINTERVAL(v)->_object )


  }  // extern "C".

}  // Isobar namespace.

#endif  // PY_INTERVAL_H
