// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2006-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/isobar/PyTransformation.h"         |
// +-----------------------------------------------------------------+


#ifndef PY_TRANSFORMATION_H
#define PY_TRANSFORMATION_H


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/Transformation.h"


namespace  Isobar {

  extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyTransformation".

    typedef struct {
        PyObject_HEAD
        Hurricane::Transformation* _object;
    } PyTransformation;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

    extern PyTypeObject  PyTypeTransformation;
    extern PyMethodDef   PyTransformation_Methods[];

    extern void      PyTransformation_LinkPyType ();
    extern void      PyTransformation_postModuleInit ();


# define IsPyTransformation(v)    ( (v)->ob_type == &PyTypeTransformation )
# define PYTRANSFORMATION(v)      ( (PyTransformation*)(v) )
# define PYTRANSFORMATION_O(v)    ( PYTRANSFORMATION(v)->_object )


  }  // extern "C".

}  // Isobar namespace.
 
#endif  // PY_TRANSFORMATION_H
