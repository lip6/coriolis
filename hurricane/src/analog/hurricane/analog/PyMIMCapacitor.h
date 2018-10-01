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
// |  C++ Header  :  "./hurricane/analog/PyMIMCapacitor.h"           |
// +-----------------------------------------------------------------+


#ifndef  ANALOG_PY_MIM_CAPACITOR_H
#define  ANALOG_PY_MIM_CAPACITOR_H

#include "hurricane/analog/PyCapacitorFamily.h"
#include "hurricane/analog/MIMCapacitor.h"


namespace  Isobar {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyMIMCapacitor".

  typedef struct {
      PyCapacitorFamily  _baseObject;
  } PyMIMCapacitor;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeMIMCapacitor;
  extern PyMethodDef   PyMIMCapacitor_Methods[];

  extern PyObject* PyMIMCapacitor_Link       ( Analog::MIMCapacitor* object );
  extern void      PyMIMCapacitor_LinkPyType ();


#define IsPyMIMCapacitor(v)    ( (v)->ob_type == &PyTypeMIMCapacitor )
#define PYMIMCAPACITOR(v)      ( (PyMIMCapacitor*)(v) )
#define PYMIMCAPACITOR_O(v)    ( PYMIMCAPACITOR(v)->_baseObject->_baseObject._object )


}  // extern "C".

}  // Isobar namespace.

#endif  // ANALOG_PY_MIM_CAPACITOR_H
