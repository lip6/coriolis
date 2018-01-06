// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2012-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katana/PyGraphicKatanaEngine.h"              |
// +-----------------------------------------------------------------+


#ifndef KATANA_PY_GRAPHIC_KATANA_ENGINE_H
#define KATANA_PY_GRAPHIC_KATANA_ENGINE_H

#include "crlcore/PyGraphicToolEngine.h"
#include "katana/GraphicKatanaEngine.h"


namespace  Katana {

extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyGraphicKatanaEngine".

  typedef struct {
      CRL::PyGraphicTool  _baseObject;
  } PyGraphicKatanaEngine;


// -------------------------------------------------------------------
// Functions & Types exported to "PyKatana.ccp".

  extern  PyTypeObject  PyTypeGraphicKatanaEngine;
  extern  PyMethodDef   PyGraphicKatanaEngine_Methods[];

  extern  void  PyGraphicKatanaEngine_LinkPyType ();


#define IsPyGraphicKatanaEngine(v)    ( (v)->ob_type == &PyTypeGraphicKatanaEngine )
#define PY_GRAPHIC_KATANA_ENGINE(v)   ( (PyGraphicKatanaEngine*)(v) )
#define PY_GRAPHIC_KATANA_ENGINE_O(v) ( PY_GRAPHIC_KATANA_ENGINE(v)->_baseObject._object )


}  // extern "C".

}  // Katana namespace.

#endif  // KATANA_PY_GRAPHIC_KATANA_ENGINE_H
