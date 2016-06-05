
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2014-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |   E t e s i a n  -  A n a l y t i c   P l a c e r               |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyGraphicEtesianEngine.cpp"                  |
// +-----------------------------------------------------------------+


#include "etesian/PyGraphicEtesianEngine.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/Cell.h"


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define METHOD_HEAD(function)    GENERIC_METHOD_HEAD(GraphicEtesianEngine,gtool,function)


namespace  Etesian {

using namespace Hurricane;
using namespace Isobar;

extern "C" {


// +=================================================================+
// |        "PyGraphicEtesianEngine" Python Module Code Part         |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  // +-------------------------------------------------------------+
  // |         "PyGraphicEtesianEngine" Attribute Methods          |
  // +-------------------------------------------------------------+


  static PyObject* PyGraphicEtesianEngine_grab ( PyObject* )
  {
    cdebug.log(34) << "PyGraphicEtesianEngine_grab()" << endl;
    PyGraphicEtesianEngine* pyGraphicEtesianEngine = NULL;

    HTRY
    pyGraphicEtesianEngine = PyObject_NEW ( PyGraphicEtesianEngine, &PyTypeGraphicEtesianEngine );
    if ( pyGraphicEtesianEngine == NULL ) return NULL;

    pyGraphicEtesianEngine->ACCESS_OBJECT = GraphicEtesianEngine::grab();
    HCATCH

    return (PyObject*)pyGraphicEtesianEngine;
  }


  static PyObject* PyGraphicEtesianEngine_getCell ( PyGraphicEtesianEngine* self )
  {
    cdebug.log(34) << "PyGraphicEtesianEngine_getCell ()" << endl;

    Cell* cell = NULL;
    
    HTRY
    METHOD_HEAD("GraphicEtesianEngine.getCell()")
    cell = gtool->getCell ();
    HCATCH

    if (cell == NULL) Py_RETURN_NONE;
    return PyCell_Link(cell);
  }


  GetNameMethod(GraphicEtesianEngine, gtool)

  // Standart destroy (Attribute).


  PyMethodDef PyGraphicEtesianEngine_Methods[] =
    { { "grab"                , (PyCFunction)PyGraphicEtesianEngine_grab                , METH_NOARGS|METH_STATIC
                              , "Returns the GraphicEtesianEngine singleton." }
    , { "getName"             , (PyCFunction)PyGraphicEtesianEngine_getName             , METH_NOARGS
                              , "Returns the name of the GraphicEtesianEngine (class attribute)." }
    , { "getCell"             , (PyCFunction)PyGraphicEtesianEngine_getCell             , METH_NOARGS
                              , "Returns the Cell on which this GraphicEtesianEngine is attached." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  // ---------------------------------------------------------------
  // PyGraphicEtesianEngine Type Methods.


  PythonOnlyDeleteMethod(GraphicEtesianEngine)
  PyTypeObjectLinkPyType(GraphicEtesianEngine)


#else  // End of Python Module Code Part.


// +=================================================================+
// |       "PyGraphicEtesianEngine" Shared Library Code Part         |
// +=================================================================+

  // Link/Creation Method.
  LinkCreateMethod(GraphicEtesianEngine)

  PyTypeInheritedObjectDefinitions(GraphicEtesianEngine,GraphicTool)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // CRL namespace. 
