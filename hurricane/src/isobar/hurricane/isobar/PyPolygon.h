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
// |  C++ Header  :  "./hurricane/isobar/PyPolygon.h"                |
// +-----------------------------------------------------------------+


#ifndef PY_POLYGON_H
#define PY_POLYGON_H

#include "hurricane/isobar/PyComponent.h"
#include "hurricane/Polygon.h"


namespace  Isobar {

  extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyPolygon".

    typedef struct {
        PyComponent  _baseObject;
    } PyPolygon;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

    extern  PyTypeObject  PyTypePolygon;
    extern  PyMethodDef   PyPolygon_Methods[];

    extern  PyObject* PyPolygon_Link       ( Hurricane::Polygon* object );
    extern  void      PyPolygon_LinkPyType ();


#define IsPyPolygon(v)    ( (v)->ob_type == &PyTypePolygon )
#define PYPOLYGON(v)      ( (PyPolygon*)(v) )
#define PYPOLYGON_O(v)    ( PYPOLYGON(v)->_baseObject._baseObject._object )


  }  // extern "C".

}  // Isobar namespace.

#endif // PY_POLYGON_H
