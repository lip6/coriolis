// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  : hurricane/isobar/PyTwoLayersPhysicalRule.h       |
// +-----------------------------------------------------------------+


#ifndef HURRICANE_PY_TWO_LAYERS_PHYSICAL_RULE_H
#define HURRICANE_PY_TWO_LAYERS_PHYSICAL_RULE_H

#include "hurricane/isobar/PyPhysicalRule.h"
#include "hurricane/TwoLayersPhysicalRule.h"


namespace  Isobar {

  extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyTwoLayersPhysicalRule".

    typedef struct {
        PyPhysicalRule  _baseObject;
    } PyTwoLayersPhysicalRule;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

    extern PyTypeObject  PyTypeTwoLayersPhysicalRule;
    extern PyMethodDef   PyTwoLayersPhysicalRule_Methods[];
    extern PyObject*     PyTwoLayersPhysicalRule_Link       ( TwoLayersPhysicalRule* );
    extern void          PyTwoLayersPhysicalRule_LinkPyType ();
    extern void          PyTwoLayersPhysicalRule_Constructor();


#define IsPyTwoLayersPhysicalRule(v)    ( (v)->ob_type == &PyTypeTwoLayersPhysicalRule )
#define PYTWOLAYERSPHYSICALRULE(v)      ( (PyTwoLayersPhysicalRule*)(v) )
#define PYTWOLAYERSPHYSICALRULE_O(v)    ( dynamic_cast<TwoLayersPhysicalRule*>(PYTWOLAYERSPHYSICALRULE(v)->_baseObject._baseObject._object) )


  }  // extern "C".

}  // Isobar namespace.

#endif  // HURRICANE_PY_TWO_LAYERS_PHYSICAL_RULE_H
