// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |     A n a b a t i c  -  Global Routing Toolbox                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./anabatic/PyAnabaticEngine.h"                 |
// +-----------------------------------------------------------------+


#ifndef PY_ANABATIC_ENGINE_H
#define PY_ANABATIC_ENGINE_H

#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/PyToolEngine.h"
#include "anabatic/AnabaticEngine.h"


namespace  Anabatic {

  extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyAnabaticEngine".

    typedef struct {
        CRL::PyToolEngine  _baseObject;
    } PyAnabaticEngine;


// -------------------------------------------------------------------
// Functions & Types exported to "PyAnabatic.ccp".

    extern  PyTypeObject  PyTypeAnabaticEngine;
    extern  PyMethodDef   PyAnabaticEngine_Methods[];
    extern  PyObject*     PyAnabaticEngine_Link       ( Anabatic::AnabaticEngine* );
    extern  void          PyAnabaticEngine_LinkPyType ();


#define IsPyAnabaticEngine(v)  ( (v)->ob_type == &PyTypeAnabaticEngine )
#define PYANABATICENGINE(v)    ( (PyAnabaticEngine*)(v) )
#define PYANABATICENGINE_O(v)  ( PYANABATICENGINE(v)->_baseObject._object )


  }  // extern "C".

}  // Anabatic namespace.

#endif  // PY_ANABATIC_ENGINE_H
