// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2010-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/analog/PyDevice.h"                 |
// +-----------------------------------------------------------------+


#ifndef  ANALOG_PY_DEVICE_H
#define  ANALOG_PY_DEVICE_H

#include "hurricane/isobar/PyCell.h"
#include "hurricane/analog/Device.h"


namespace  Isobar {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyDevice".

  typedef struct {
      PyCell  _baseObject;
  } PyDevice;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeDevice;
  extern PyMethodDef   PyDevice_Methods[];

  extern PyObject* PyDevice_Link           ( Analog::Device* object );
  extern void      PyDevice_LinkPyType     ();
  extern void      PyDevice_postModuleInit ();


#define IsPyDevice(v)    ( (v)->ob_type == &PyTypeDevice )
#define PYDEVICE(v)      ( (PyDevice*)(v) )
#define PYDEVICE_O(v)    ( dynamic_cast<Analog::Device*>(PYDEVICE(v)->_baseObject._baseObject._object) )


}  // extern "C".

}  // Isobar namespace.

#endif  // ANALOG_PY_DEVICE_H
