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
// |  C++ Header  : "./hurricane/analog/PyTransistorMultiArguments.h"|
// +-----------------------------------------------------------------+

#ifndef ANALOG_PY_TRANSISTOR_MULTI_ARGUMENTS_H
#define ANALOG_PY_TRANSISTOR_MULTI_ARGUMENTS_H

#include "Python.h"
#include "hurricane/analog/TransistorMultiArguments.h"

namespace  Isobar {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyTransistorMultiArguments".

  typedef struct {
      PyObject_HEAD
      Analog::TransistorMultiArguments* _object;
  } PyTransistorMultiArguments;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeTransistorMultiArguments;
  extern PyMethodDef   PyTransistorMultiArguments_Methods[];

  extern void PyTransistorMultiArguments_LinkPyType();
  extern void PyTransistorMultiArguments_Constructor();


#define IsPyTransistorMultiArguments(v)    ( (v)->ob_type == &PyTypeTransistorMultiArguments )
#define PYTRANSISTORMULTIARGUMENTS(v)      ( (PyTransistorMultiArguments*)(v) )
#define PYTRANSISTORMULTIARGUMENTS_O(v)    ( PYTRANSISTORMULTIARGUMENTS(v)->_object )


}  // extern "C".

}  // Isobar namespace.

#endif
