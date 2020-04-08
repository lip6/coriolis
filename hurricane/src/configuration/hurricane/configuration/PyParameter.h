// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2020-2020, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/configuration/PyParameter.h"       |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/configuration/PyHurricane2.h"
#include "hurricane/configuration/Parameter.h"


namespace  Cfg2 {


extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyParameter".

  typedef struct {
      PyObject_HEAD
      Parameter* _object;
  } PyParameter;


// -------------------------------------------------------------------
// Functions & Types exported to "PyConfiguration.cpp".

  extern PyTypeObject  PyTypeParameter;
  extern PyMethodDef   PyParameter_Methods[];

//   extern PyObject* PyParameter_create         ( PyObject* self, PyObject* args );
//   extern PyObject* PyParameter_Link           ( Hurricane::Parameter* object );
//   extern void      PyParameter_LinkPyType     ();
//   extern void      PyParameter_postModuleInit ();


// #define  IsPyParameter(v)      ( (v)->ob_type == &PyTypeParameter )
// #define  PYCELLVIEWER(v)        ( (PyParameter*)(v) )
// #define  PYCELLVIEWER_O(v)      ( PYCELLVIEWER(v)->_object )


}  // extern "C".

}  // Cfc namespace.
