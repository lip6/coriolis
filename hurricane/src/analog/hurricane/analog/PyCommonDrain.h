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
// |  C++ Header  :  "./hurricane/analog/PyCommonDrain.h"            |
// +-----------------------------------------------------------------+


#ifndef  ANALOG_PY_COMMON_DRAIN_H
#define  ANALOG_PY_COMMON_DRAIN_H

#include "hurricane/analog/PyTransistorPair.h"
#include "hurricane/analog/CommonDrain.h"


namespace  Isobar {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyCommonDrain".

  typedef struct {
      PyTransistorPair  _baseObject;
  } PyCommonDrain;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeCommonDrain;
  extern PyMethodDef   PyCommonDrain_Methods[];

  extern PyObject* PyCommonDrain_Link       ( Analog::CommonDrain* object );
  extern void      PyCommonDrain_LinkPyType ();


#define IsPyCommonDrain(v)    ( (v)->ob_type == &PyTypeCommonDrain )
#define PYCOMMONDRAIN(v)      ( (PyCommonDrain*)(v) )
#define PYCOMMONDRAIN_O(v)    ( PYCOMMONDRAIN(v)->_baseObject->_baseObject->_baseObject._object )


}  // extern "C".

}  // Isobar namespace.

#endif  // ANALOG_PY_COMMON_DRAIN_H
