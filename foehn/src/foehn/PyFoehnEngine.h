// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2022-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |              F o e h n  -  DAG Toolbox                          |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./foehn/PyFoehnEngine.cpp"                     |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/PyToolEngine.h"
#include "foehn/FoehnEngine.h"


namespace Foehn {

  extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyFoehnEngine".

    typedef struct {
        CRL::PyToolEngine  _baseObject;
    } PyFoehnEngine;


// -------------------------------------------------------------------
// Functions & Types exported to "PyFoehn.ccp".

    extern  PyTypeObject  PyTypeFoehnEngine;
    extern  PyMethodDef   PyFoehnEngine_Methods[];

    extern  PyObject* PyFoehnEngine_Link           ( Foehn::FoehnEngine* );
    extern  void      PyFoehnEngine_LinkPyType     ();
    extern  void      PyFoehnEngine_postModuleInit ();


#define IsPyFoehnEngine(v)  ( (v)->ob_type == &PyTypeFoehnEngine )
#define PYFOEHNENGINE(v)    ( (PyFoehnEngine*)(v) )
#define PYFOEHNENGINE_O(v)  ( PYFOEHNENGINE(v)->_baseObject._object )


}  // extern "C".

}  // Foehn namespace.
