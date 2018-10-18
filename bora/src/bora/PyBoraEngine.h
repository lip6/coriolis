// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |  B o r a  -  A n a l o g   S l i c i n g   T r e e              |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./bora/PyBoraEngine.h"                         |
// +-----------------------------------------------------------------+


#ifndef PY_BORA_ENGINE_H
#define PY_BORA_ENGINE_H

#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/PyToolEngine.h"
#include "bora/BoraEngine.h"


namespace  Bora {

  using Bora::BoraEngine;


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyBoraEngine".

  typedef struct {
      CRL::PyToolEngine  _baseObject;
  } PyBoraEngine;


// -------------------------------------------------------------------
// Functions & Types exported to "PyBora.ccp".

  extern  PyTypeObject  PyTypeBoraEngine;
  extern  PyMethodDef   PyBoraEngine_Methods[];

  extern  PyObject* PyBoraEngine_Link           ( BoraEngine* );
  extern  void      PyBoraEngine_LinkPyType     ();
  extern  void      PyBoraEngine_postModuleInit ();


#define IsPyBoraEngine(v)  ( (v)->ob_type == &PyTypeBoraEngine )
#define PYBORAENGINE(v)    ( (PyBoraEngine*)(v) )
#define PYBORAENGINE_O(v)  ( PYBORAENGINE(v)->_baseObject._object )


}  // extern "C".

}  // Bora namespace.

#endif  // PY_BORA_ENGINE_H
