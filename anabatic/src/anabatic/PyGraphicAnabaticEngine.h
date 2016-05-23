// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |     A n a b a t i c  -  Global Routing Toolbox                  |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./anabatic/PyGraphicAnabaticEngine.h"          |
// +-----------------------------------------------------------------+


#ifndef ANABATIC_PY_GRAPHIC_ANABATIC_ENGINE_H
#define ANABATIC_PY_GRAPHIC_ANABATIC_ENGINE_H

#include "crlcore/PyGraphicToolEngine.h"
#include "anabatic/GraphicAnabaticEngine.h"


namespace  Anabatic {

  extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyGraphicAnabaticEngine".

    typedef struct {
        CRL::PyGraphicTool  _baseObject;
    } PyGraphicAnabaticEngine;


// -------------------------------------------------------------------
// Functions & Types exported to "PyAnabatic.ccp".

    extern  PyTypeObject  PyTypeGraphicAnabaticEngine;
    extern  PyMethodDef   PyGraphicAnabaticEngine_Methods[];
    extern  void          PyGraphicAnabaticEngine_LinkPyType ();


#define IsPyGraphicAnabaticEngine(v)    ( (v)->ob_type == &PyTypeGraphicAnabaticEngine )
#define PY_GRAPHIC_ANABATIC_ENGINE(v)   ( (PyGraphicAnabaticEngine*)(v) )
#define PY_GRAPHIC_ANABATIC_ENGINE_O(v) ( PY_GRAPHIC_ANABATIC_ENGINE(v)->_baseObject._object )


  }  // extern "C".

}  // Anabatic namespace.

#endif  // ANABATIC_PY_GRAPHIC_ANABATIC_ENGINE_H
