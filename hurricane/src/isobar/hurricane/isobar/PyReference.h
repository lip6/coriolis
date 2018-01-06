// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./hurricane/isobar/PyReference.cpp"       |
// +-----------------------------------------------------------------+


#ifndef PY_REFERENCE_H
#define PY_REFERENCE_H

#include "hurricane/isobar/PyEntity.h"
#include "hurricane/Reference.h"


namespace Isobar {

  extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyReference".

    typedef struct {
        PyEntity  _baseObject;
    } PyReference;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

    extern PyTypeObject PyTypeReference;
    extern PyMethodDef  PyReference_Methods[];
  
    extern PyObject* PyReference_Link       ( Hurricane::Reference* object );
    extern void      PyReference_LinkPyType ();



#define IsPyReference(v) ((v)->ob_type == &PyTypeReference )
#define PYREFERENCE(v)   ((PyReference*)(v) )
#define PYREFERENCE_O(v) (PYREFERENCE(v)->_baseObject._object)


  }  // extern "C".

}  // Isobar namespace.

#endif
