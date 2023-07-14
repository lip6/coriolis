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
// |  C++ Header   :  "./tramontana/PyGraphicTramontanaEngine.h"     |
// +-----------------------------------------------------------------+


#pragma  once
#include "crlcore/PyGraphicToolEngine.h"
#include "tramontana/GraphicTramontanaEngine.h"


namespace  Tramontana {

extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyGraphicTramontanaEngine".

  typedef struct {
      CRL::PyGraphicTool  _baseObject;
  } PyGraphicTramontanaEngine;


// -------------------------------------------------------------------
// Functions & Types exported to "PyTramontana.ccp".

  extern  PyTypeObject  PyTypeGraphicTramontanaEngine;
  extern  PyMethodDef   PyGraphicTramontanaEngine_Methods[];

  extern  void  PyGraphicTramontanaEngine_LinkPyType ();


#define IsPyGraphicTramontanaEngine(v)    ( (v)->ob_type == &PyTypeGraphicTramontanaEngine )
#define PY_GRAPHIC_TRAMONTANA_ENGINE(v)   ( (PyGraphicTramontanaEngine*)(v) )
#define PY_GRAPHIC_TRAMONTANA_ENGINE_O(v) ( PY_GRAPHIC_TRAMONTANA_ENGINE(v)->_baseObject._object )


}  // extern "C".

}  // Tramontana namespace.
