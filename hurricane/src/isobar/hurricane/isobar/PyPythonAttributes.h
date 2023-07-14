// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2022-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/isobar//PyPythonAttributes.h"      |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/isobar/PyHurricane.h"


namespace  Isobar {


extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyPythonAttributes".

  typedef struct {
      PyObject_HEAD
  } PyPythonAttributes;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypePythonAttributes;
  extern PyMethodDef   PyPythonAttributes_Methods[];

  extern void  PyPythonAttributes_LinkPyType ();


#define IsPyPythonAttributes(v)   ( (v)->ob_type == &PyTypePythonAttributes )


}  // extern "C".


}  // Isobar namespace.
