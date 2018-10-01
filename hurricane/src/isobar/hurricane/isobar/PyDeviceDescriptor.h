// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/isobar/PyDeviceDescriptor.h"       |
// +-----------------------------------------------------------------+


#ifndef HURRICANE_PY_DEVICE_DESCRIPTOR_H
#define HURRICANE_PY_DEVICE_DESCRIPTOR_H

#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/DeviceDescriptor.h"


namespace  Isobar {

  extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyDeviceDescriptor".

    typedef struct {
        PyObject_HEAD
        DeviceDescriptor* _object;
    } PyDeviceDescriptor;


// -------------------------------------------------------------------
// Functions & Types exported to "PyAnalog.ccp".

  extern  PyTypeObject  PyTypeDeviceDescriptor;
  extern  PyMethodDef   PyDeviceDescriptor_Methods[];

  extern  PyObject* PyDeviceDescriptor_Link       ( DeviceDescriptor* );
  extern  void      PyDeviceDescriptor_LinkPyType ();


#define IsPyDeviceDescriptor(v)    ( (v)->ob_type == &PyTypeDeviceDescriptor )
#define PYDEVICEDESCRIPTOR(v)      ( (PyDeviceDescriptor*)(v) )
#define PYDEVICEDESCRIPTOR_O(v)    ( PYDEVICEDESCRIPTOR(v)->_object )


  } // extern "C".

}  // Isobar namespace.

#endif  // HURRICANE_PY_DEVICE_DESCRIPTOR_H
