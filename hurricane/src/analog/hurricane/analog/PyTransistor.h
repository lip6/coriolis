// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/analog/PyTransistor.h"             |
// +-----------------------------------------------------------------+


#ifndef  ANALOG_PY_TRANSISTOR_H
#define  ANALOG_PY_TRANSISTOR_H

#include "hurricane/analog/PyTransistorFamily.h"
#include "hurricane/analog/Transistor.h"


namespace  Isobar {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyTransistor".

  typedef struct {
      PyTransistorFamily  _baseObject;
  } PyTransistor;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeTransistor;
  extern PyMethodDef   PyTransistor_Methods[];

  extern PyObject* PyTransistor_Link       ( Analog::Transistor* object );
  extern void      PyTransistor_LinkPyType ();


#define IsPyTransistor(v)    ( (v)->ob_type == &PyTypeTransistor )
#define PYTRANSISTOR(v)      ( (PyTransistor*)(v) )
#define PYTRANSISTOR_O(v)    ( PYTRANSISTOR(v)->_baseObject->_baseObject._object )


}  // extern "C".

}  // Isobar namespace.

#endif  // ANALOG_PY_TRANSISTOR_H
