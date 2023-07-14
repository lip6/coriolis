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
// |  C++ Header  :  "./hurricane/analog/PyCapacitiesParameter.h"    |
// +-----------------------------------------------------------------+


#ifndef  ANALOG_PY_CAPACITIES_PARAMETER_H
#define  ANALOG_PY_CAPACITIES_PARAMETER_H

#include "hurricane/analog/PyParameter.h"
#include "hurricane/analog/CapacitiesParameter.h"


namespace  Isobar {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyCapacitiesParameter".

  typedef struct {
      PyParameter  _baseObject;
  } PyCapacitiesParameter;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeCapacitiesParameter;
  extern PyMethodDef   PyCapacitiesParameter_Methods[];

  extern PyObject* PyCapacitiesParameter_Link       ( Analog::CapacitiesParameter* object );
  extern void      PyCapacitiesParameter_LinkPyType ();


#define IsPyCapacitiesParameter(v)    ( (v)->ob_type == &PyTypeCapacitiesParameter )
#define PYCAPACITIESPARAMETER(v)      ( (PyCapacitiesParameter*)(v) )
#define PYCAPACITIESPARAMETER_O(v)    ( PYCAPACITIESPARAMETER(v)->_baseObject->_baseObject._object )


}  // extern "C".

}  // Isobar namespace.

#endif  // ANALOG_PY_CAPACITIES_PARAMETER_H
