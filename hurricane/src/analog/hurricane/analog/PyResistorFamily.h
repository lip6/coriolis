// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2019-2019, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/analog/PyResistorFamily.h"         |
// +-----------------------------------------------------------------+


#ifndef  ANALOG_PY_RESISTOR_FAMILY_H
#define  ANALOG_PY_RESISTOR_FAMILY_H

#include "hurricane/analog/PyDevice.h"
#include "hurricane/analog/ResistorFamily.h"


namespace  Isobar {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyResistorFamily".

  typedef struct {
      PyDevice  _baseObject;
  } PyResistorFamily;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeResistorFamily;
  extern PyMethodDef   PyResistorFamily_Methods[];

  extern PyObject* PyResistorFamily_Link           ( Analog::ResistorFamily* object );
  extern void      PyResistorFamily_LinkPyType     ();
  extern void      PyResistorFamily_postModuleInit ();


#define IsPyResistorFamily(v)    ( (v)->ob_type == &PyTypeResistorFamily )
#define PYRESISTORFAMILY(v)      ( (PyResistorFamily*)(v) )
#define PYRESISTORFAMILY_O(v)    ( PYRESISTORFAMILY(v)->_baseObject->_baseObject._object )


}  // extern "C".

}  // Isobar namespace.

#endif  // ANALOG_PY_RESISTOR_FAMILY_H
