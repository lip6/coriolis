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
// |  C++ Header  :  "./hurricane/analog/PyLevelShifter.h"           |
// +-----------------------------------------------------------------+


#ifndef  ANALOG_PY_LEVEL_SHIFTER_H
#define  ANALOG_PY_LEVEL_SHIFTER_H

#include "hurricane/analog/PyTransistorPair.h"
#include "hurricane/analog/LevelShifter.h"


namespace  Isobar {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyLevelShifter".

  typedef struct {
      PyTransistorPair  _baseObject;
  } PyLevelShifter;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeLevelShifter;
  extern PyMethodDef   PyLevelShifter_Methods[];

  extern PyObject* PyLevelShifter_Link       ( Analog::LevelShifter* object );
  extern void      PyLevelShifter_LinkPyType ();


#define IsPyLevelShifter(v)    ( (v)->ob_type == &PyTypeLevelShifter )
#define PYLEVELSHIFTER(v)      ( (PyLevelShifter*)(v) )
#define PYLEVELSHIFTER_O(v)    ( PYLEVELSHIFTER(v)->_baseObject->_baseObject->_baseObject._object )


}  // extern "C".

}  // Isobar namespace.

#endif  // ANALOG_PY_LEVEL_SHIFTER_H
