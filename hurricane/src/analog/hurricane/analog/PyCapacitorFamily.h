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
// |  C++ Header  :  "./hurricane/analog/PyCapacitorFamily.h"        |
// +-----------------------------------------------------------------+


#ifndef  ANALOG_PY_CAPACITOR_FAMILY_H
#define  ANALOG_PY_CAPACITOR_FAMILY_H

#include "hurricane/analog/PyDevice.h"
#include "hurricane/analog/CapacitorFamily.h"


namespace  Isobar {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyCapacitorFamily".

  typedef struct {
      PyDevice  _baseObject;
  } PyCapacitorFamily;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeCapacitorFamily;
  extern PyMethodDef   PyCapacitorFamily_Methods[];

  extern PyObject* PyCapacitorFamily_Link           ( Analog::CapacitorFamily* object );
  extern void      PyCapacitorFamily_LinkPyType     ();
  extern void      PyCapacitorFamily_postModuleInit ();


#define IsPyCapacitorFamily(v)    ( (v)->ob_type == &PyTypeCapacitorFamily )
#define PYCAPACITORFAMILY(v)      ( (PyCapacitorFamily*)(v) )
#define PYCAPACITORFAMILY_O(v)    ( PYCAPACITORFAMILY(v)->_baseObject->_baseObject._object )


}  // extern "C".

}  // Isobar namespace.

#endif  // ANALOG_PY_CAPACITOR_FAMILY_H
