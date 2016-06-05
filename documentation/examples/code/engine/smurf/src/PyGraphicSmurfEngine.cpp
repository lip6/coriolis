
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2014-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |   S m u r f  -  A  D e m o   T o o l E n g i n e                |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyGraphicSmurfEngine.cpp"                    |
// +-----------------------------------------------------------------+


#include "smurf/PyGraphicSmurfEngine.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/Cell.h"


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define METHOD_HEAD(function)    GENERIC_METHOD_HEAD(GraphicSmurfEngine,gtool,function)


namespace  Smurf {

using namespace Hurricane;
using namespace Isobar;

extern "C" {


// +=================================================================+
// |        "PyGraphicSmurfEngine" Python Module Code Part         |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  // +-------------------------------------------------------------+
  // |         "PyGraphicSmurfEngine" Attribute Methods          |
  // +-------------------------------------------------------------+


  static PyObject* PyGraphicSmurfEngine_grab ( PyObject* )
  {
    trace << "PyGraphicSmurfEngine_grab()" << endl;
    PyGraphicSmurfEngine* pyGraphicSmurfEngine = NULL;

    HTRY
    pyGraphicSmurfEngine = PyObject_NEW ( PyGraphicSmurfEngine, &PyTypeGraphicSmurfEngine );
    if ( pyGraphicSmurfEngine == NULL ) return NULL;

    pyGraphicSmurfEngine->ACCESS_OBJECT = GraphicSmurfEngine::grab();
    HCATCH

    return (PyObject*)pyGraphicSmurfEngine;
  }


  static PyObject* PyGraphicSmurfEngine_getCell ( PyGraphicSmurfEngine* self )
  {
    trace << "PyGraphicSmurfEngine_getCell ()" << endl;

    Cell* cell = NULL;
    
    HTRY
    METHOD_HEAD("GraphicSmurfEngine.getCell()")
    cell = gtool->getCell ();
    HCATCH

    if (cell == NULL) Py_RETURN_NONE;
    return PyCell_Link(cell);
  }


  GetNameMethod(GraphicSmurfEngine, gtool)

  // Standart destroy (Attribute).


  PyMethodDef PyGraphicSmurfEngine_Methods[] =
    { { "grab"                , (PyCFunction)PyGraphicSmurfEngine_grab                , METH_NOARGS|METH_STATIC
                              , "Returns the GraphicSmurfEngine singleton." }
    , { "getName"             , (PyCFunction)PyGraphicSmurfEngine_getName             , METH_NOARGS
                              , "Returns the name of the GraphicSmurfEngine (class attribute)." }
    , { "getCell"             , (PyCFunction)PyGraphicSmurfEngine_getCell             , METH_NOARGS
                              , "Returns the Cell on which this GraphicSmurfEngine is attached." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  // ---------------------------------------------------------------
  // PyGraphicSmurfEngine Type Methods.


  PythonOnlyDeleteMethod(GraphicSmurfEngine)
  PyTypeObjectLinkPyType(GraphicSmurfEngine)


#else  // End of Python Module Code Part.


// +=================================================================+
// |       "PyGraphicSmurfEngine" Shared Library Code Part         |
// +=================================================================+

  // Link/Creation Method.
  LinkCreateMethod(GraphicSmurfEngine)

  PyTypeInheritedObjectDefinitions(GraphicSmurfEngine,GraphicTool)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // CRL namespace. 
