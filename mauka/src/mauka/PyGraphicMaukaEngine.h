
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                       Damien DUPUIS              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./mauka/PyGraphicMaukaEngine.h"                |
// +-----------------------------------------------------------------+


#ifndef __MAUKA_PY_GRAPHIC_MAUKA_ENGINE__
#define __MAUKA_PY_GRAPHIC_MAUKA_ENGINE__

#include "crlcore/PyGraphicToolEngine.h"
#include "mauka/GraphicMaukaEngine.h"


namespace  Mauka {

extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyGraphicMaukaEngine".

  typedef struct {
      CRL::PyGraphicTool  _baseObject;
  } PyGraphicMaukaEngine;


// -------------------------------------------------------------------
// Functions & Types exported to "PyMauka.ccp".

  extern  PyTypeObject  PyTypeGraphicMaukaEngine;
  extern  PyMethodDef   PyGraphicMaukaEngine_Methods[];

  extern  void  PyGraphicMaukaEngine_LinkPyType ();


#define IsPyGraphicMaukaEngine(v)     ( (v)->ob_type == &PyTypeGraphicMaukaEngine )
#define PY_GRAPHIC_MAUKA_ENGINE(v)    ( (PyGraphicMaukaEngine*)(v) )
#define PY_GRAPHIC_MAUKA_ENGINE_O(v)  ( PY_GRAPHIC_MAUKA_ENGINE(v)->_baseObject.object )


}  // End of extern "C".

}  // Mauka namespace.

#endif // __MAUKA_PY_GRAPHIC_MAUKA_ENGINE__
