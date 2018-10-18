// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |  B o r a  -  A n a l o g   S l i c i n g   T r e e              |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./bora/PyGraphicBoraEngine.h"                  |
// +-----------------------------------------------------------------+


#ifndef BORA_PY_GRAPHIC_BORA_ENGINE_H
#define BORA_PY_GRAPHIC_BORA_ENGINE_H

#include "crlcore/PyGraphicToolEngine.h"
#include "bora/GraphicBoraEngine.h"


namespace  Bora {

extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyGraphicBoraEngine".

  typedef struct {
      CRL::PyGraphicTool  _baseObject;
  } PyGraphicBoraEngine;


// -------------------------------------------------------------------
// Functions & Types exported to "PyBora.ccp".

  extern  PyTypeObject  PyTypeGraphicBoraEngine;
  extern  PyMethodDef   PyGraphicBoraEngine_Methods[];

  extern  void  PyGraphicBoraEngine_LinkPyType ();


#define IsPyGraphicBoraEngine(v)    ( (v)->ob_type == &PyTypeGraphicBoraEngine )
#define PY_GRAPHIC_BORA_ENGINE(v)   ( (PyGraphicBoraEngine*)(v) )
#define PY_GRAPHIC_BORA_ENGINE_O(v) ( PY_GRAPHIC_BORA_ENGINE(v)->_baseObject._object )


}  // extern "C".

}  // Bora namespace.

#endif  // BORA_PY_GRAPHIC_BORA_ENGINE_H
