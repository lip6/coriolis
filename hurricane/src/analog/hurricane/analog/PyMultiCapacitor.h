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
// |  C++ Header  :  "./hurricane/analog/PyMultiCapacitor.h"         |
// +-----------------------------------------------------------------+


#ifndef  ANALOG_PY_MULTI_CAPACITOR_H
#define  ANALOG_PY_MULTI_CAPACITOR_H

#include "hurricane/analog/PyCapacitorFamily.h"
#include "hurricane/analog/MultiCapacitor.h"


namespace  Isobar {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyMultiCapacitor".

  typedef struct {
      PyCapacitorFamily  _baseObject;
  } PyMultiCapacitor;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeMultiCapacitor;
  extern PyMethodDef   PyMultiCapacitor_Methods[];

  extern PyObject* PyMultiCapacitor_Link       ( Analog::MultiCapacitor* object );
  extern void      PyMultiCapacitor_LinkPyType ();


#define IsPyMultiCapacitor(v)    ( (v)->ob_type == &PyTypeMultiCapacitor )
#define PYMULTICAPACITOR(v)      ( (PyMultiCapacitor*)(v) )
#define PYMULTICAPACITOR_O(v)    ( PYMULTICAPACITOR(v)->_baseObject->_baseObject._object )


}  // extern "C".

}  // Isobar namespace.

#endif  // ANALOG_PY_MULTI_CAPACITOR_H
