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
// |  C++ Header  :  "./foehn/PyDag.cpp"                             |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/isobar/PyHurricane.h"
#include "foehn/Dag.h"


namespace Foehn {

  extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyDag".

    typedef struct {
        PyObject_HEAD
        Dag* _object;
    } PyDag;


// -------------------------------------------------------------------
// Functions & Types exported to "PyFoehn.ccp".

    extern  PyTypeObject  PyTypeDag;
    extern  PyMethodDef   PyDag_Methods[];

    extern  PyObject* PyDag_Link           ( Foehn::Dag* );
    extern  void      PyDag_LinkPyType     ();


#define IsPyDag(v)  ( (v)->ob_type == &PyTypeDag )
#define PYDAG(v)    ( (PyDag*)(v) )
#define PYDAG_O(v)  ( PYDAG(v)->_object )


}  // extern "C".

}  // Foehn namespace.
