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
// |  C++ Header  :  "./hurricane/isobar/PyPhysicalRule.h"           |
// +-----------------------------------------------------------------+


#ifndef HURRICANE_PY_PHYSICAL_RULE_H
#define HURRICANE_PY_PHYSICAL_RULE_H

#include "hurricane/isobar/PyRule.h"
#include "hurricane/PhysicalRule.h"


namespace  Isobar {

  extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyPhysicalRule".

    typedef struct {
        PyRule  _baseObject;
    } PyPhysicalRule;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

    extern PyTypeObject  PyTypePhysicalRule;
    extern PyMethodDef   PyPhysicalRule_Methods[];
    extern PyObject*     PyPhysicalRule_Link       ( PhysicalRule* );
    extern void          PyPhysicalRule_LinkPyType ();
    extern void          PyPhysicalRule_Constructor();


#define IsPyPhysicalRule(v)    ( (v)->ob_type == &PyTypePhysicalRule )
#define PYPHYSICALRULE(v)      ( (PyPhysicalRule*)(v) )
#define PYPHYSICALRULE_O(v)    ( dynamic_cast<PhysicalRule*>(PYPHYSICALRULE(v)->_baseObject._object) )


  }  // extern "C".

}  // Isobar namespace.

#endif  // HURRICANE_PY_PHYSICAL_RULE_H
