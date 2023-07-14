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
// |  C++ Header  :  "./hurricane/analog/PyResistor.h"               |
// +-----------------------------------------------------------------+


#ifndef  ANALOG_PY_RESISTOR_H
#define  ANALOG_PY_RESISTOR_H

#include "hurricane/analog/PyResistorFamily.h"
#include "hurricane/analog/Resistor.h"


namespace  Isobar {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyResistor".

  typedef struct {
      PyResistorFamily  _baseObject;
  } PyResistor;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeResistor;
  extern PyMethodDef   PyResistor_Methods[];

  extern PyObject* PyResistor_Link       ( Analog::Resistor* object );
  extern void      PyResistor_LinkPyType ();


#define IsPyResistor(v)    ( (v)->ob_type == &PyTypeResistor )
#define PYRESISTOR(v)      ( (PyResistor*)(v) )
#define PYRESISTOR_O(v)    ( PYRESISTOR(v)->_baseObject->_baseObject._object )


}  // extern "C".

}  // Isobar namespace.

#endif  // ANALOG_PY_RESISTOR_H
