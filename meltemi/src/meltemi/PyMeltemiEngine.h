
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
// |  C++ Header  :  "./meltemi/PyMeltemiEngine.cpp"                 |
// +-----------------------------------------------------------------+

#pragma once
#include "crlcore/PyToolEngine.h"
#include "hurricane/isobar/PyHurricane.h"
#include "etesian/PyEtesianEngine.h"
#include "meltemi/MeltemiEngine.h"

namespace Meltemi {

  extern "C" {

  // -------------------------------------------------------------------
  // Python Object  :  "PyMeltemiEngine".

  typedef struct {
      CRL::PyToolEngine _baseObject;
  } PyMeltemiEngine;

  // -------------------------------------------------------------------
  // Functions & Types exported to "PyMeltemi.ccp".

  extern PyTypeObject PyTypeMeltemiEngine;
  extern PyMethodDef  PyMeltemiEngine_Methods[];

  extern PyObject* PyMeltemiEngine_Link (Meltemi::MeltemiEngine*);
  extern void      PyMeltemiEngine_LinkPyType ();
  extern void      PyMeltemiEngine_postModuleInit ();

#define IsPyMeltemiEngine(v) ((v)->ob_type == &PyTypeMeltemiEngine)
#define PYMELTEMIENGINE(v)   ((PyMeltemiEngine*)(v))
#define PYMELTEMIENGINE_O(v) (PYMELTEMIENGINE(v)->_baseObject._object)
  }  // extern "C".

}  // namespace Meltemi
