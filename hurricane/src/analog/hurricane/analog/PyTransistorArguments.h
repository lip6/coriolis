// -*- C++ -*-
//
// This file is part of the Coriols Software.
// Copyright (c) UPMC 2009-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/analog/PyTransistorArguments.h"    |
// +-----------------------------------------------------------------+

#ifndef ANALOG_PY_TRANSISTOR_ARGUMENTS_H
#define ANALOG_PY_TRANSISTOR_ARGUMENTS_H

#include "Python.h"
#include "hurricane/analog/TransistorArguments.h"


namespace  Isobar {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyTransistorArguments".

  typedef struct {
      PyObject_HEAD
      Analog::TransistorArguments* _object;
  } PyTransistorArguments;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeTransistorArguments;
  extern PyMethodDef   PyTransistorArguments_Methods[];

  extern void PyTransistorArguments_LinkPyType();
  extern void PyTransistorArguments_Constructor();


#define IsPyTransistorArguments(v)    ( (v)->ob_type == &PyTypeTransistorArguments )
#define PYTRANSISTORARGUMENTS(v)      ( (PyTransistorArguments*)(v) )
#define PYTRANSISTORARGUMENTS_O(v)    ( PYTRANSISTORARGUMENTS(v)->_object )


}  // extern "C".

}  // Isobar namespace.

#endif
