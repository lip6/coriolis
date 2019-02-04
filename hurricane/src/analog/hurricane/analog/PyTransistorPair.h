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
// |  C++ Header  :  "./hurricane/analog/PyTransistorPair.h"         |
// +-----------------------------------------------------------------+


#ifndef  ANALOG_PY_TRANSISTOR_PAIR_H
#define  ANALOG_PY_TRANSISTOR_PAIR_H

#include "hurricane/analog/PyTransistorFamily.h"
#include "hurricane/analog/TransistorPair.h"


namespace  Isobar {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyTransistorPair".

  typedef struct {
      PyTransistorFamily  _baseObject;
  } PyTransistorPair;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeTransistorPair;
  extern PyMethodDef   PyTransistorPair_Methods[];

  extern PyObject* PyTransistorPair_Link       ( Analog::TransistorPair* object );
  extern void      PyTransistorPair_LinkPyType ();


#define IsPyTransistorPair(v)    ( (v)->ob_type == &PyTypeTransistorPair )
#define PYTRANSISTORPAIR(v)      ( (PyTransistorPair*)(v) )
#define PYTRANSISTORPAIR_O(v)    ( PYTRANSISTORPAIR(v)->_baseObject->_baseObject._object )


}  // extern "C".

}  // Isobar namespace.

#endif  // ANALOG_PY_TRANSISTOR_PAIR_H
