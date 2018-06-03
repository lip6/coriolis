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
// |  C++ Module  :  "./PyPointCollection.cpp"                       |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyPointCollection.h"
#include "hurricane/isobar/PyPoint.h"

namespace  Isobar {

using namespace Hurricane;

  extern "C" {


// +=================================================================+
// |           "PyPointCollection" Python Module Code Part           |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


    DirectDeleteMethod(PyPointCollection_DeAlloc, PyPointCollection)


    static PyObject* PyPointLocatorNext ( PyPointCollectionLocator* pyLocator )
    { 
      Locator<Point>* locator = pyLocator->_object;

      HTRY
        if (locator->isValid()) {
          Point point = locator->getElement();
          PyPoint* pyPoint = PyObject_NEW(PyPoint, &PyTypePoint);
          if (pyPoint == NULL) return NULL;
          pyPoint->_object = new Point(point);

          locator->progress();
          return (PyObject*)pyPoint;
        }
      HCATCH
      return NULL;
  }


  CollectionMethods(Point)


#else  // Python Module Code Part.


// +=================================================================+
// |          "PyPointCollection" Shared Library Code Part           |
// +=================================================================+


  PyTypeCollectionObjectDefinitions(PointCollection)
  PyTypeCollectionObjectDefinitions(PointCollectionLocator)


#endif  // Shared Library Code Part.


    }  // extern "C".

}  // Isobar namespace.
