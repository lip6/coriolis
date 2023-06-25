// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2007-2023, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |       T r a m o n t a n a  -  Extractor & LVX                   |
// |                                                                 |
// |  Algorithm    :                   Christian MASSON              |
// |  First impl.  :                           Yifei WU              |
// |  Second impl. :                   Jean-Paul CHAPUT              |
// |  E-mail       :           Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header   :  "./tramontana/PyTramontanaEngine.h"            |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/PyToolEngine.h"
#include "tramontana/TramontanaEngine.h"


namespace  Tramontana {

extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyTramontanaEngine".

  typedef struct {
      CRL::PyToolEngine  _baseObject;
  } PyTramontanaEngine;


// -------------------------------------------------------------------
// Functions & Types exported to "PyTramontana.ccp".

  extern  PyTypeObject  PyTypeTramontanaEngine;
  extern  PyMethodDef   PyTramontanaEngine_Methods[];

  extern  PyObject* PyTramontanaEngine_Link           ( Tramontana::TramontanaEngine* );
  extern  void      PyTramontanaEngine_LinkPyType     ();
//extern  void      PyTramontanaEngine_postModuleInit ();


#define IsPyTramontanaEngine(v)  ( (v)->ob_type == &PyTypeTramontanaEngine )
#define PYTRAMONTANAENGINE(v)    ( (PyTramontanaEngine*)(v) )
#define PYTRAMONTANAENGINE_O(v)  ( PYTRAMONTANAENGINE(v)->_baseObject._object )


}  // extern "C".

}  // Tramontana namespace.
