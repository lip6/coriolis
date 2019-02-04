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
// |  C++ Header  :  "./hurricane/analog/PyTransistorFamily.h"       |
// +-----------------------------------------------------------------+


#ifndef  ANALOG_PY_TRANSISTOR_FAMILY_H
#define  ANALOG_PY_TRANSISTOR_FAMILY_H

#include "hurricane/analog/PyDevice.h"
#include "hurricane/analog/TransistorFamily.h"


namespace  Isobar {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyTransistorFamily".

  typedef struct {
      PyDevice  _baseObject;
  } PyTransistorFamily;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeTransistorFamily;
  extern PyMethodDef   PyTransistorFamily_Methods[];

  extern PyObject* PyTransistorFamily_Link           ( Analog::TransistorFamily* object );
  extern void      PyTransistorFamily_LinkPyType     ();
  extern void      PyTransistorFamily_postModuleInit ();


#define IsPyTransistorFamily(v)    ( (v)->ob_type == &PyTypeTransistorFamily )
#define PYTRANSISTORFAMILY(v)      ( (PyTransistorFamily*)(v) )
#define PYTRANSISTORFAMILY_O(v)    ( PYTRANSISTORFAMILY(v)->_baseObject->_baseObject._object )


}  // extern "C".

}  // Isobar namespace.

#endif  // ANALOG_PY_TRANSISTOR_FAMILY_H
