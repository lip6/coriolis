// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2017-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |        T o o l E n g i n e   T u t o r i a l                    |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./tutorial/PyGraphicTutorialEngine.h"          |
// +-----------------------------------------------------------------+


#ifndef TUTORIAL_PY_GRAPHIC_TUTORIAL_ENGINE_H
#define TUTORIAL_PY_GRAPHIC_TUTORIAL_ENGINE_H

#include "crlcore/PyGraphicToolEngine.h"
#include "tutorial/GraphicTutorialEngine.h"


namespace  Tutorial {

extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyGraphicTutorialEngine".

  typedef struct {
      CRL::PyGraphicTool  _baseObject;
  } PyGraphicTutorialEngine;


// -------------------------------------------------------------------
// Functions & Types exported to "PyTutorial.ccp".

  extern  PyTypeObject  PyTypeGraphicTutorialEngine;
  extern  PyMethodDef   PyGraphicTutorialEngine_Methods[];

  extern  void  PyGraphicTutorialEngine_LinkPyType ();


#define IsPyGraphicTutorialEngine(v)    ( (v)->ob_type == &PyTypeGraphicTutorialEngine )
#define PY_GRAPHIC_TUTORIAL_ENGINE(v)   ( (PyGraphicTutorialEngine*)(v) )
#define PY_GRAPHIC_TUTORIAL_ENGINE_O(v) ( PY_GRAPHIC_TUTORIAL_ENGINE(v)->_baseObject._object )


}  // extern "C".

}  // Tutorial namespace.

#endif  // TUTORIAL_PY_GRAPHIC_TUTORIAL_ENGINE_H
