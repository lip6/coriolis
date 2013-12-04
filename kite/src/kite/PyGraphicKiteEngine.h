
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2012-2013, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./kite/PyGraphicKiteEngine.h"                  |
// +-----------------------------------------------------------------+


#ifndef KITE_PY_GRAPHIC_KITE_ENGINE_H
#define KITE_PY_GRAPHIC_KITE_ENGINE_H

#include "crlcore/PyGraphicToolEngine.h"
#include "kite/GraphicKiteEngine.h"


namespace  Kite {

extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyGraphicKiteEngine".

  typedef struct {
      CRL::PyGraphicTool  _baseObject;
  } PyGraphicKiteEngine;


// -------------------------------------------------------------------
// Functions & Types exported to "PyKite.ccp".

  extern  PyTypeObject  PyTypeGraphicKiteEngine;
  extern  PyMethodDef   PyGraphicKiteEngine_Methods[];

  extern  void  PyGraphicKiteEngine_LinkPyType ();


#define IsPyGraphicKiteEngine(v)    ( (v)->ob_type == &PyTypeGraphicKiteEngine )
#define PY_GRAPHIC_KITE_ENGINE(v)   ( (PyGraphicKiteEngine*)(v) )
#define PY_GRAPHIC_KITE_ENGINE_O(v) ( PY_GRAPHIC_KITE_ENGINE(v)->_baseObject._object )


}  // extern "C".

}  // Kite namespace.

#endif  // KITE_PY_GRAPHIC_KITE_ENGINE_H
