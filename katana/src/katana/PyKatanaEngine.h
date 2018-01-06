// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2010-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katana/PyKatanaEngine.cpp"                   |
// +-----------------------------------------------------------------+


#ifndef PY_KATANA_ENGINE_H
#define PY_KATANA_ENGINE_H

#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/PyToolEngine.h"
#include "katana/KatanaEngine.h"


namespace  Katana {

extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyKatanaEngine".

  typedef struct {
      CRL::PyToolEngine  _baseObject;
  } PyKatanaEngine;


// -------------------------------------------------------------------
// Functions & Types exported to "PyKatana.ccp".

  extern  PyTypeObject  PyTypeKatanaEngine;
  extern  PyMethodDef   PyKatanaEngine_Methods[];

  extern  PyObject* PyKatanaEngine_Link           ( Katana::KatanaEngine* );
  extern  void      PyKatanaEngine_LinkPyType     ();
  extern  void      PyKatanaEngine_postModuleInit ();


#define IsPyKatanaEngine(v)  ( (v)->ob_type == &PyTypeKatanaEngine )
#define PYKATANAENGINE(v)    ( (PyKatanaEngine*)(v) )
#define PYKATANAENGINE_O(v)  ( PYKATANAENGINE(v)->_baseObject._object )


}  // extern "C".

}  // Katana namespace.

#endif  // PY_KATANA_ENGINE_H
