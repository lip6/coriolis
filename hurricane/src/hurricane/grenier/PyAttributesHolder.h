// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2022-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/isobar/PyAttributesHolder.h"       |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/Box.h"


namespace  Isobar {

  extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyAttributesHolder".

    typedef struct {
        PyObject  _base;
    } PyAttributesHolder;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

    extern PyTypeObject  PyTypeAttributesHolder;
    extern PyMethodDef   PyAttributesHolder_Methods[];


#define IsPyAttributesHolder(v)    ( (v)->ob_type == &PyTypeAttributesHolder )
#define PYATTRIBUTESHOLDER(v)      ( (PyAttributesHolder*)(v) )
#define PYATTRIBUTESHOLDER_O(v)    ( PYATTRIBUTESHOLDER(v)->_object )


  }  // extern "C".

}  // Isobar namespace.
