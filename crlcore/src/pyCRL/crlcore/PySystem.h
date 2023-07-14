// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2019-2019, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./crlcore/PySystem.h"                          |
// +-----------------------------------------------------------------+


#ifndef CRL_PY_SYSTEM_H
#define CRL_PY_SYSTEM_H

#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/Utilities.h"


namespace  CRL {


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PySystem".

  typedef struct {
      PyObject_HEAD
      CRL::System* _object;
  } PySystem;


// -------------------------------------------------------------------
// Functions & Types exported to "PyCRL.ccp".

  extern  PyTypeObject  PyTypeSystem;
  extern  PyMethodDef   PySystem_Methods[];

  extern  PyObject* PySystem_Link           ( CRL::System* );
  extern  void      PySystem_LinkPyType     ();
  extern  void      PySystem_postModuleInit ();


#define IsPySystem(v)  ( (v)->ob_type == &PyTypeSystem )
#define PYSYSTEM(v)    ( (PySystem*)(v) )
#define PYSYSTEM_O(v)  ( PYSYSTEM(v)->_object )


}  // extern "C".


}  // CRL namespace.


#endif  // CRL_PY_SYSTEM_H
