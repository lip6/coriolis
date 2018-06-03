// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2018-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/isobar/PyPointCollection.h"        |
// +-----------------------------------------------------------------+


#ifndef PY_POINT_COLLECTION_H
#define PY_POINT_COLLECTION_H

#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/Point.h"
#include "hurricane/Points.h"


namespace Isobar {


  extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyPointCollection".

    typedef struct {
        PyObject_HEAD
        Hurricane::Points* _object;
    } PyPointCollection;

    typedef struct {
        PyObject_HEAD
        Hurricane::Locator<Hurricane::Point>* _object;
        PyPointCollection* _collection;
    } PyPointCollectionLocator;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

    extern PyTypeObject PyTypePointCollection;
    extern PyTypeObject PyTypePointCollectionLocator;

    extern void PyPointCollection_LinkPyType();
    extern void PyPointCollectionLocator_LinkPyType();

  }  // extern "C".

}  // Isobar namespace.

#endif  // PY_POINT_COLLECTION_H
