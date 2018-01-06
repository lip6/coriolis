// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2017-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |        T o o l E n g i n e   T u t o r i a l                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./tutorial/PyTutorialEngine.cpp"               |
// +-----------------------------------------------------------------+


#ifndef PY_TUTORIAL_ENGINE_H
#define PY_TUTORIAL_ENGINE_H

#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/PyToolEngine.h"
#include "tutorial/TutorialEngine.h"


namespace  Tutorial {

extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyTutorialEngine".

  typedef struct {
      CRL::PyToolEngine  _baseObject;
  } PyTutorialEngine;


// -------------------------------------------------------------------
// Functions & Types exported to "PyTutorial.ccp".

  extern  PyTypeObject  PyTypeTutorialEngine;
  extern  PyMethodDef   PyTutorialEngine_Methods[];

  extern  PyObject* PyTutorialEngine_Link           ( Tutorial::TutorialEngine* );
  extern  void      PyTutorialEngine_LinkPyType     ();
  extern  void      PyTutorialEngine_postModuleInit ();


#define IsPyTutorialEngine(v)  ( (v)->ob_type == &PyTypeTutorialEngine )
#define PYTUTORIALENGINE(v)    ( (PyTutorialEngine*)(v) )
#define PYTUTORIALENGINE_O(v)  ( PYTUTORIALENGINE(v)->_baseObject._object )


}  // extern "C".

}  // Tutorial namespace.

#endif  // PY_TUTORIAL_ENGINE_H
