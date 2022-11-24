
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2010-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |      N i m b u s  -  Global Routing Framework                   |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./nimbus/PyNimbusEngine.cpp"                   |
// +-----------------------------------------------------------------+


#ifndef __PY_NIMBUS_ENGINE__
#define __PY_NIMBUS_ENGINE__

#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/PyToolEngine.h"
#include "nimbus/NimbusEngine.h"


namespace  Nimbus {

extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyNimbusEngine".

  typedef struct {
      CRL::PyToolEngine  _baseObject;
  } PyNimbusEngine;


// -------------------------------------------------------------------
// Functions & Types exported to "PyNimbus.ccp".

  extern  PyTypeObject  PyTypeNimbusEngine;
  extern  PyMethodDef   PyNimbusEngine_Methods[];

  extern  PyObject* PyNimbusEngine_Link       ( Nimbus::NimbusEngine* );
  extern  void      PyNimbusEngine_LinkPyType ();


#define IsPyNimbusEngine(v)    ( (v)->ob_type == &PyTypeNimbusEngine )
#define PY_NIMBUS_ENGINE(v)    ( (PyNimbusEngine*)(v) )
#define PY_NIMBUS_ENGINE_O(v)  ( PY_NIMBUS_ENGINE(v)->_baseObject._object )


}  // extern "C".

}  // Nimbus namespace.

#endif  // __PY_NIMBUS_ENGINE__
