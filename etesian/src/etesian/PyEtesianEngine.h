
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU/LIP6 2014-2020, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |   E t e s i a n  -  A n a l y t i c   P l a c e r               |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./etesian/PyEtesianEngine.cpp"                 |
// +-----------------------------------------------------------------+


#pragma once
#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/PyToolEngine.h"
#include "etesian/EtesianEngine.h"


namespace  Etesian {

extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyEtesianEngine".

  typedef struct {
      CRL::PyToolEngine  _baseObject;
  } PyEtesianEngine;


// -------------------------------------------------------------------
// Functions & Types exported to "PyEtesian.ccp".

  extern  PyTypeObject  PyTypeEtesianEngine;
  extern  PyMethodDef   PyEtesianEngine_Methods[];

  extern  PyObject* PyEtesianEngine_Link           ( Etesian::EtesianEngine* );
  extern  void      PyEtesianEngine_LinkPyType     ();
  extern  void      PyEtesianEngine_postModuleInit ();


#define IsPyEtesianEngine(v)  ( (v)->ob_type == &PyTypeEtesianEngine )
#define PYETESIANENGINE(v)    ( (PyEtesianEngine*)(v) )
#define PYETESIANENGINE_O(v)  ( PYETESIANENGINE(v)->_baseObject._object )


}  // extern "C".

}  // Etesian namespace.
