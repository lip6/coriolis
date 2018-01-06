// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2007-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                     Sophie BELLOEIL              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/isobar/PyVertical.h"               |
// +-----------------------------------------------------------------+


#ifndef PY_VERTICAL_H
#define PY_VERTICAL_H

#include "hurricane/isobar/PySegment.h"
#include "hurricane/Vertical.h"


namespace  Isobar {

  extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyVertical".

    typedef struct {
        PySegment  _baseObject;
    } PyVertical;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

    extern  PyTypeObject  PyTypeVertical;
    extern  PyMethodDef   PyVertical_Methods[];

    extern  PyObject* PyVertical_Link       ( Hurricane::Vertical* object );
    extern  void      PyVertical_LinkPyType ();


#define IsPyVertical(v) ((v)->ob_type == &PyTypeVertical)
#define PYVERTICAL(v)   ((PyVertical*)(v))
#define PYVERTICAL_O(v) (PYVERTICAL(v)->_baseObject._baseObject._baseObject._object)

    
  }  // extern "C".
  
}  // Isobar namespace.

#endif  // PY_VERTICAL_H
