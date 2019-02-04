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
// |  C++ Header  :  "./hurricane/isobar/PyUnitRule.h"               |
// +-----------------------------------------------------------------+


#ifndef HURRICANE_PY_UNIT_RULE_H
#define HURRICANE_PY_UNIT_RULE_H

#include "hurricane/isobar/PyRule.h"
#include "hurricane/UnitRule.h"


namespace  Isobar {

  extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyUnitRule".

    typedef struct {
        PyRule  _baseObject;
    } PyUnitRule;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

    extern PyTypeObject  PyTypeUnitRule;
    extern PyMethodDef   PyUnitRule_Methods[];
    extern PyObject*     PyUnitRule_Link       ( UnitRule* );
    extern void          PyUnitRule_LinkPyType ();
    extern void          PyUnitRule_Constructor();


#define IsPyUnitRule(v)    ( (v)->ob_type == &PyTypeUnitRule )
#define PYUNITRULE(v)      ( (PyUnitRule*)(v) )
#define PYUNITRULE_O(v)    ( dynamic_cast<UnitRule*>(PYUNITRULE(v)->_baseObject._object) )


  }  // extern "C".

}  // Isobar namespace.

#endif  // HURRICANE_PY_UNIT_RULE_H
