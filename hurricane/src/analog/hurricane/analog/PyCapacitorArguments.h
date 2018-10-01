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
// |  C++ Header  :  "./hurricane/analog/PyCapacitorArguments.h"     |
// +-----------------------------------------------------------------+


#ifndef ANALOG_PY_CAPACITOR_ARGUMENTS_H
#define ANALOG_PY_CAPACITOR_ARGUMENTS_H

#include "Python.h"
#include "hurricane/analog/CapacitorArguments.h"


namespace  Isobar {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyCapacitorArguments".

  typedef struct {
      PyObject_HEAD
      Analog::CapacitorArguments* _object;
  } PyCapacitorArguments;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeCapacitorArguments;
  extern PyMethodDef   PyCapacitorArguments_Methods[];

  extern void PyCapacitorArguments_LinkPyType();
  extern void PyCapacitorArguments_Constructor();


#define IsPyCapacitorArguments(v)    ( (v)->ob_type == &PyTypeCapacitorArguments )
#define PYCAPACITORARGUMENTS(v)      ( (PyCapacitorArguments*)(v) )
#define PYCAPACITORARGUMENTS_O(v)    ( PYCAPACITORARGUMENTS(v)->_object )


}  // extern "C".

}  // Isobar namespace.

#endif  // ANALOG_PY_CAPACITOR_ARGUMENTS_H
