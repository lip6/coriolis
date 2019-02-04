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
// |  C++ Header  :  ./hurricane/analog/PySimpleCurrentMirror.h      |
// +-----------------------------------------------------------------+


#ifndef  ANALOG_PY_SIMPLE_CURRENT_MIRROR_H
#define  ANALOG_PY_SIMPLE_CURRENT_MIRROR_H

#include "hurricane/analog/PyTransistorPair.h"
#include "hurricane/analog/SimpleCurrentMirror.h"


namespace  Isobar {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PySimpleCurrentMirror".

  typedef struct {
      PyTransistorPair  _baseObject;
  } PySimpleCurrentMirror;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeSimpleCurrentMirror;
  extern PyMethodDef   PySimpleCurrentMirror_Methods[];

  extern PyObject* PySimpleCurrentMirror_Link       ( Analog::SimpleCurrentMirror* object );
  extern void      PySimpleCurrentMirror_LinkPyType ();


#define IsPySimpleCurrentMirror(v)    ( (v)->ob_type == &PyTypeSimpleCurrentMirror )
#define PYSIMPLECURRENTMIRROR(v)      ( (PySimpleCurrentMirror*)(v) )
#define PYSIMPLECURRENTMIRROR_O(v)    ( PYSIMPLECURRENTMIRROR(v)->_baseObject->_baseObject->_baseObject._object )


}  // extern "C".

}  // Isobar namespace.

#endif  // ANALOG_PY_SIMPLE_CURRENT_MIRROR_H
