
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |              M a u k a  -  P l a c e r                          |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyGraphicMaukaEngine.cpp"                    |
// +-----------------------------------------------------------------+


#include "mauka/PyGraphicMaukaEngine.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/Cell.h"


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define METHOD_HEAD(function)    GENERIC_METHOD_HEAD(GraphicMaukaEngine,gtool,function)


namespace  Mauka {

using namespace Hurricane;
using namespace Isobar;

extern "C" {


// +=================================================================+
// |         "PyGraphicMaukaEngine" Python Module Code Part          |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  // +-------------------------------------------------------------+
  // |          "PyGraphicMaukaEngine" Attribute Methods           |
  // +-------------------------------------------------------------+


  static PyObject* PyGraphicMaukaEngine_grab ( PyObject* )
  {
    trace << "PyGraphicMaukaEngine_grab()" << endl;
    PyGraphicMaukaEngine* pyGraphicMaukaEngine = NULL;

    HTRY
    pyGraphicMaukaEngine = PyObject_NEW ( PyGraphicMaukaEngine, &PyTypeGraphicMaukaEngine );
    if ( pyGraphicMaukaEngine == NULL ) return NULL;

    pyGraphicMaukaEngine->ACCESS_OBJECT = GraphicMaukaEngine::grab();
    HCATCH

    return (PyObject*)pyGraphicMaukaEngine;
  }


  static PyObject* PyGraphicMaukaEngine_getCell ( PyGraphicMaukaEngine* self )
  {
    trace << "PyGraphicMaukaEngine_getCell ()" << endl;

    Cell* cell = NULL;
    
    HTRY
    METHOD_HEAD("GraphicMaukaEngine.getCell()")
    cell = gtool->getCell ();
    HCATCH

    if (cell == NULL) Py_RETURN_NONE;
    return PyCell_Link(cell);
  }


  GetNameMethod(GraphicMaukaEngine, gtool)

  // Standart destroy (Attribute).


  PyMethodDef PyGraphicMaukaEngine_Methods[] =
    { { "grab"                , (PyCFunction)PyGraphicMaukaEngine_grab                , METH_NOARGS|METH_STATIC
                              , "Returns the GraphicMaukaEngine singleton." }
    , { "getName"             , (PyCFunction)PyGraphicMaukaEngine_getName             , METH_NOARGS
                              , "Returns the name of the GraphicMaukaEngine (class attribute)." }
    , { "getCell"             , (PyCFunction)PyGraphicMaukaEngine_getCell             , METH_NOARGS
                              , "Returns the Cell on which this GraphicMaukaEngine is attached." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  // ---------------------------------------------------------------
  // PyGraphicMaukaEngine Type Methods.


  PythonOnlyDeleteMethod(GraphicMaukaEngine)
  PyTypeObjectLinkPyType(GraphicMaukaEngine)


#else  // End of Python Module Code Part.


// +=================================================================+
// |        "PyGraphicMaukaEngine" Shared Library Code Part          |
// +=================================================================+

  // Link/Creation Method.
  LinkCreateMethod(GraphicMaukaEngine)

  PyTypeInheritedObjectDefinitions(GraphicMaukaEngine,GraphicTool)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // CRL namespace. 
