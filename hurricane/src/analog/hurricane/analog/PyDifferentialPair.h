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
// |  C++ Header  :  "./hurricane/analog/PyDifferentialPair.h"       |
// +-----------------------------------------------------------------+


#ifndef  ANALOG_PY_DIFFERENTIAL_PAIR_H
#define  ANALOG_PY_DIFFERENTIAL_PAIR_H

#include "hurricane/analog/PyTransistorPair.h"
#include "hurricane/analog/DifferentialPair.h"


namespace  Isobar {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyDifferentialPair".

  typedef struct {
      PyTransistorPair  _baseObject;
  } PyDifferentialPair;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeDifferentialPair;
  extern PyMethodDef   PyDifferentialPair_Methods[];

  extern PyObject* PyDifferentialPair_Link       ( Analog::DifferentialPair* object );
  extern void      PyDifferentialPair_LinkPyType ();


#define IsPyDifferentialPair(v)    ( (v)->ob_type == &PyTypeDifferentialPair )
#define PYDIFFERENTIALPAIR(v)      ( (PyDifferentialPair*)(v) )
#define PYDIFFERENTIALPAIR_O(v)    ( PYDIFFERENTIALPAIR(v)->_baseObject->_baseObject->_baseObject._object )


}  // extern "C".

}  // Isobar namespace.

#endif  // ANALOG_PY_DIFFERENTIAL_PAIR_H
