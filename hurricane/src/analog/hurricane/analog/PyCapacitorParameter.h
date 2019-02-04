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
// |  C++ Header  : "./hurricane/analog/PyCapacitorParameter.h"      |
// +-----------------------------------------------------------------+


#ifndef  ANALOG_PY_CAPACITOR_PARAMETER_H
#define  ANALOG_PY_CAPACITOR_PARAMETER_H

#include "hurricane/analog/PyParameter.h"
#include "hurricane/analog/CapacitorParameter.h"


namespace  Isobar {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyCapacitorParameter".

  typedef struct {
      PyParameter  _baseObject;
  } PyCapacitorParameter;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeCapacitorParameter;
  extern PyMethodDef   PyCapacitorParameter_Methods[];

  extern PyObject* PyCapacitorParameter_Link       ( Analog::CapacitorParameter* object );
  extern void      PyCapacitorParameter_LinkPyType ();


#define IsPyCapacitorParameter(v)    ( (v)->ob_type == &PyTypeCapacitorParameter )
#define PYCAPACITORPARAMETER(v)      ( (PyCapacitorParameter*)(v) )
#define PYCAPACITORPARAMETER_O(v)    ( PYCAPACITORPARAMETER(v)->_baseObject->_baseObject._object )


}  // extern "C".

}  // Isobar namespace.

#endif  // ANALOG_PY_CAPACITOR_PARAMETER_H
