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
// |  C++ Header  :  "./hurricane/analog/PyCascode.h"                |
// +-----------------------------------------------------------------+


#ifndef  ANALOG_PY_CASCODE_H
#define  ANALOG_PY_CASCODE_H

#include "hurricane/analog/PyTransistorPair.h"
#include "hurricane/analog/Cascode.h"


namespace  Isobar {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyCascode".

  typedef struct {
      PyTransistorPair  _baseObject;
  } PyCascode;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeCascode;
  extern PyMethodDef   PyCascode_Methods[];

  extern PyObject* PyCascode_Link       ( Analog::Cascode* object );
  extern void      PyCascode_LinkPyType ();


#define IsPyCascode(v)    ( (v)->ob_type == &PyTypeCascode )
#define PYCASCODE(v)      ( (PyCascode*)(v) )
#define PYCASCODE_O(v)    ( PYCASCODE(v)->_baseObject->_baseObject->_baseObject._object )


}  // extern "C".

}  // Isobar namespace.

#endif  // ANALOG_PY_CASCODE_H
