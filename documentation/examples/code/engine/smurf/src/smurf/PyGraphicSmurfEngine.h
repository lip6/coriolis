
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2014-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |   S m u r f  -  A   D e m o   T o o l E n g i n e               |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./smurf/PyGraphicSmurfEngine.h"                |
// +-----------------------------------------------------------------+


#ifndef SMURF_PY_GRAPHIC_SMURF_ENGINE_H
#define SMURF_PY_GRAPHIC_SMURF_ENGINE_H

#include "crlcore/PyGraphicToolEngine.h"
#include "smurf/GraphicSmurfEngine.h"


namespace  Smurf {

extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyGraphicSmurfEngine".

  typedef struct {
      CRL::PyGraphicTool  _baseObject;
  } PyGraphicSmurfEngine;


// -------------------------------------------------------------------
// Functions & Types exported to "PySmurf.ccp".

  extern  PyTypeObject  PyTypeGraphicSmurfEngine;
  extern  PyMethodDef   PyGraphicSmurfEngine_Methods[];

  extern  void  PyGraphicSmurfEngine_LinkPyType ();


#define IsPyGraphicSmurfEngine(v)    ( (v)->ob_type == &PyTypeGraphicSmurfEngine )
#define PY_GRAPHIC_SMURF_ENGINE(v)   ( (PyGraphicSmurfEngine*)(v) )
#define PY_GRAPHIC_SMURF_ENGINE_O(v) ( PY_GRAPHIC_SMURF_ENGINE(v)->_baseObject._object )


}  // extern "C".

}  // Smurf namespace.

#endif  // SMURF_PY_GRAPHIC_SMURF_ENGINE_H
