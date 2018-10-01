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
// |  C++ Header  :  "./hurricane/analog/PyCommonSourcePair.h"       |
// +-----------------------------------------------------------------+


#ifndef  ANALOG_PY_COMMON_SOURCE_PAIR_H
#define  ANALOG_PY_COMMON_SOURCE_PAIR_H

#include "hurricane/analog/PyTransistorPair.h"
#include "hurricane/analog/CommonSourcePair.h"


namespace  Isobar {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyCommonSourcePair".

  typedef struct {
      PyTransistorPair  _baseObject;
  } PyCommonSourcePair;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeCommonSourcePair;
  extern PyMethodDef   PyCommonSourcePair_Methods[];

  extern PyObject* PyCommonSourcePair_Link       ( Analog::CommonSourcePair* object );
  extern void      PyCommonSourcePair_LinkPyType ();


#define IsPyCommonSourcePair(v)    ( (v)->ob_type == &PyTypeCommonSourcePair )
#define PYCOMMONSOURCEPAIR(v)      ( (PyCommonSourcePair*)(v) )
#define PYCOMMONSOURCEPAIR_O(v)    ( PYCOMMONSOURCEPAIR(v)->_baseObject->_baseObject->_baseObject._object )


}  // extern "C".

}  // Isobar namespace.

#endif  // ANALOG_PY_COMMON_SOURCE_PAIR_H
