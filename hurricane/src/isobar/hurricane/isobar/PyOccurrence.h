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
// |  C++ Header  :  "./hurricane/isobar/PyOccurrence.h"             |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/Occurrence.h"


namespace  Isobar {

  extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyOccurrence".

    typedef struct {
        PyObject_HEAD
        Hurricane::Occurrence* _object;
    } PyOccurrence;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

    extern  PyTypeObject  PyTypeOccurrence;
    extern  PyMethodDef   PyOccurrence_Methods[];

    extern  void      PyOccurrence_LinkPyType ();


#define IsPyOccurrence(v)    ( (v)->ob_type == &PyTypeOccurrence )
#define PYOCCURRENCE(v)      ( (PyOccurrence*)(v) )
#define PYOCCURRENCE_O(v)    ( PYOCCURRENCE(v)->_object )


  }  // extern "C".

}  // Isobar namespace.
