// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/isobar/PyRule.h"                   |
// +-----------------------------------------------------------------+


#ifndef HURRICANE_PY_RULE_H
#define HURRICANE_PY_RULE_H

#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/Rule.h"

namespace  Isobar {

  extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyRule".

    typedef struct {
        PyObject_HEAD
        Rule* _object;
    } PyRule;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

    extern PyTypeObject  PyTypeRule;
    extern PyMethodDef   PyRule_Methods[];
    extern PyObject*     PyRule_Link        ( Rule* );
    extern void          PyRule_LinkPyType  ();
    extern void          PyRule_Constructor ();
    extern PyObject*     PyRule_LinkDerived ( Rule* object );


#define IsPyRule(v)    ( (v)->ob_type == &PyTypeRule )
#define PYRULE(v)      ( (PyRule*)(v) )
#define PYRULE_O(v)    ( PYRULE(v)->_object )


  }  // extern "C".

}  // Isobar namespace.

#endif  // HURRICANE_PY_RULE_H
