
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2010-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |              M a u k a  -  P l a c e r                          |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./PyMaukaEngine.cpp"                      |
// +-----------------------------------------------------------------+


#ifndef __PY_MAUKA_ENGINE__
#define __PY_MAUKA_ENGINE__

#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/PyToolEngine.h"
#include "mauka/MaukaEngine.h"


namespace  Mauka {


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyMaukaEngine".

  typedef struct {
      CRL::PyToolEngine  _baseObject;
  } PyMaukaEngine;


// -------------------------------------------------------------------
// Functions & Types exported to "PyMauka.ccp".

  extern  PyTypeObject  PyTypeMaukaEngine;
  extern  PyMethodDef   PyMaukaEngine_Methods[];

  extern  PyObject*     PyMaukaEngine_Link       ( Mauka::MaukaEngine* );
  extern  void          PyMaukaEngine_LinkPyType ();


#define IsPyMaukaEngine(v)  ( (v)->ob_type == &PyTypeMaukaEngine )
#define PYMAUKAENGINE(v)    ( (PyMaukaEngine*)(v) )
#define PYMAUKAENGINE_O(v)  ( PYMAUKAENGINE(v)->_baseObject._object )


}  // End of extern "C".


}  // End of Mauka namespace.


#endif  // __PY_MAUKA_ENGINE__
