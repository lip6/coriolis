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
// |  C++ Header  :  "./hurricane/analog/PyBJTArguments.h"           |
// +-----------------------------------------------------------------+


#ifndef ANALOG_PY_BJT_ARGUMENTS_H
#define ANALOG_PY_BJT_ARGUMENTS_H

#include "Python.h"
#include "hurricane/analog/BJTArguments.h"


namespace  Isobar {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyBJTArguments".

  typedef struct {
      PyObject_HEAD
      Analog::BJTArguments* _object;
  } PyBJTArguments;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeBJTArguments;
  extern PyMethodDef   PyBJTArguments_Methods[];

  extern void PyBJTArguments_LinkPyType();
  extern void PyBJTArguments_Constructor();

 
#define IsPyBJTArguments(v)    ( (v)->ob_type == &PyTypeBJTArguments )
#define PYBJTARGUMENTS(v)      ( (PyBJTArguments*)(v) )
#define PYBJTARGUMENTS_O(v)    ( PYBJTARGUMENTS(v)->_object )


}  // extern "C".

}  // Isobar namespace.

#endif  // ANALOG_PY_BJT_ARGUMENTS_H
