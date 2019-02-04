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
// |  C++ Header  :  "./hurricane/analog/PyCrossCoupledPair.h"       |
// +-----------------------------------------------------------------+


#ifndef  ANALOG_PY_CROSS_COUPLED_PAIR_H
#define  ANALOG_PY_CROSS_COUPLED_PAIR_H

#include "hurricane/analog/PyTransistorPair.h"
#include "hurricane/analog/CrossCoupledPair.h"


namespace  Isobar {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyCrossCoupledPair".

  typedef struct {
      PyTransistorPair  _baseObject;
  } PyCrossCoupledPair;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeCrossCoupledPair;
  extern PyMethodDef   PyCrossCoupledPair_Methods[];

  extern PyObject* PyCrossCoupledPair_Link       ( Analog::CrossCoupledPair* object );
  extern void      PyCrossCoupledPair_LinkPyType ();


#define IsPyCrossCoupledPair(v)    ( (v)->ob_type == &PyTypeCrossCoupledPair )
#define PYCROSSCOUPLEDPAIR(v)      ( (PyCrossCoupledPair*)(v) )
#define PYCROSSCOUPLEDPAIR_O(v)    ( PYCROSSCOUPLEDPAIR(v)->_baseObject->_baseObject->_baseObject._object )


}  // extern "C".

}  // Isobar namespace.

#endif  // ANALOG_PY_CROSS_COUPLED_PAIR_H
