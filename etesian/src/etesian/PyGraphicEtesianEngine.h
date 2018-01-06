
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2014-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |   E t e s i a n  -  A n a l y t i c   P l a c e r               |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./etesian/PyGraphicEtesianEngine.h"            |
// +-----------------------------------------------------------------+


#ifndef ETESIAN_PY_GRAPHIC_ETESIAN_ENGINE_H
#define ETESIAN_PY_GRAPHIC_ETESIAN_ENGINE_H

#include "crlcore/PyGraphicToolEngine.h"
#include "etesian/GraphicEtesianEngine.h"


namespace  Etesian {

extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyGraphicEtesianEngine".

  typedef struct {
      CRL::PyGraphicTool  _baseObject;
  } PyGraphicEtesianEngine;


// -------------------------------------------------------------------
// Functions & Types exported to "PyEtesian.ccp".

  extern  PyTypeObject  PyTypeGraphicEtesianEngine;
  extern  PyMethodDef   PyGraphicEtesianEngine_Methods[];

  extern  void  PyGraphicEtesianEngine_LinkPyType ();


#define IsPyGraphicEtesianEngine(v)    ( (v)->ob_type == &PyTypeGraphicEtesianEngine )
#define PY_GRAPHIC_ETESIAN_ENGINE(v)   ( (PyGraphicEtesianEngine*)(v) )
#define PY_GRAPHIC_ETESIAN_ENGINE_O(v) ( PY_GRAPHIC_ETESIAN_ENGINE(v)->_baseObject._object )


}  // extern "C".

}  // Etesian namespace.

#endif  // ETESIAN_PY_GRAPHIC_ETESIAN_ENGINE_H
