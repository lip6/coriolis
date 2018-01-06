// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2010-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./PyNetExternalComponents.h"              |
// +-----------------------------------------------------------------+


#ifndef  PY_NET_EXTERNAL_COMPONENTS_H
#define  PY_NET_EXTERNAL_COMPONENTS_H

#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/NetExternalComponents.h"


namespace  Isobar {


extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyNetExternalComponents".

  typedef struct {
      PyObject_HEAD
  } PyNetExternalComponents;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeNetExternalComponents;
  extern PyMethodDef   PyNetExternalComponents_Methods[];

  extern void  PyNetExternalComponents_LinkPyType  ();


#define IsPyNetExternalComponents(v)   ( (v)->ob_type == &PyTypeNetExternalComponents )
#define PYNETEXTERNALCOMPONENTS(v)     ( (PyNetExternalComponents*)(v) )
#define PYNETEXTERNALCOMPONENTS_O(v)   ( PY_NET_EXTERNAL_COMPONENTS(v)->_object )


}  // extern "C".


}  // Isobar namespace.

#endif  // PY_NET_EXTERNAL_COMPONENTS_H
