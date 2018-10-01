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
// |  C++ Header  :  "./hurricane/analog/PyCommonGatePair.h"         |
// +-----------------------------------------------------------------+


#ifndef  ANALOG_PY_COMMON_GATE_PAIR_H
#define  ANALOG_PY_COMMON_GATE_PAIR_H

#include "hurricane/analog/PyTransistorPair.h"
#include "hurricane/analog/CommonGatePair.h"


namespace  Isobar {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyCommonGatePair".

  typedef struct {
      PyTransistorPair  _baseObject;
  } PyCommonGatePair;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeCommonGatePair;
  extern PyMethodDef   PyCommonGatePair_Methods[];

  extern PyObject* PyCommonGatePair_Link       ( Analog::CommonGatePair* object );
  extern void      PyCommonGatePair_LinkPyType ();


#define IsPyCommonGatePair(v)    ( (v)->ob_type == &PyTypeCommonGatePair )
#define PYCOMMONGATEPAIR(v)      ( (PyCommonGatePair*)(v) )
#define PYCOMMONGATEPAIR_O(v)    ( PYCOMMONGATEPAIR(v)->_baseObject->_baseObject->_baseObject._object )


}  // extern "C".

}  // Isobar namespace.

#endif  // ANALOG_PY_COMMON_GATE_PAIR_H
