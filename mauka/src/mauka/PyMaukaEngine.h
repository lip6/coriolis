
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2010-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |              M a u k a  -  P l a c e r                          |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./PyMaukaEngine.cpp"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef __PY_MAUKA_ENGINE__
#define __PY_MAUKA_ENGINE__

#include "hurricane/isobar/PyHurricane.h"
#include "mauka/MaukaEngine.h"


namespace  Mauka {


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyMaukaEngine".

  typedef struct {
      PyObject_HEAD
      Mauka::MaukaEngine* _object;
  } PyMaukaEngine;


// -------------------------------------------------------------------
// Functions & Types exported to "PyMauka.ccp".

  extern  PyTypeObject  PyTypeMaukaEngine;
  extern  PyMethodDef   PyMaukaEngine_Methods[];

  extern  PyObject* PyMaukaEngine_get        ( PyObject* module, PyObject* args );
  extern  PyObject* PyMaukaEngine_create     ( PyObject* module, PyObject* args );
  extern  PyObject* PyMaukaEngine_Link       ( Mauka::MaukaEngine* );
  extern  void      PyMaukaEngine_LinkPyType ();


#define IsPyMaukaEngine(v)  ( (v)->ob_type == &PyTypeMaukaEngine )
#define PYMAUKAENGINE(v)    ( (PyMaukaEngine*)(v) )
#define PYMAUKAENGINE_O(v)  ( PYALLIANCEFRAMEWORK(v)->_object )


}  // End of extern "C".


}  // End of Mauka namespace.


#endif  // __PY_MAUKA_ENGINE__
