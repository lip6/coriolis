// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |     A n a b a t i c  -  Global Routing Toolbox                  |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyGraphicAnabaticEngine.cpp"                 |
// +-----------------------------------------------------------------+


#include "anabatic/PyGraphicAnabaticEngine.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/Cell.h"


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define METHOD_HEAD(function)    GENERIC_METHOD_HEAD(GraphicAnabaticEngine,gengine,function)


namespace  Anabatic {

using namespace Hurricane;
using namespace Isobar;

extern "C" {


// +=================================================================+
// |        "PyGraphicAnabaticEngine" Python Module Code Part        |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  // +-------------------------------------------------------------+
  // |         "PyGraphicAnabaticEngine" Attribute Methods         |
  // +-------------------------------------------------------------+


  static PyObject* PyGraphicAnabaticEngine_grab ( PyObject* )
  {
    cdebug.log(32) << "PyGraphicAnabaticEngine_grab()" << endl;
    PyGraphicAnabaticEngine* pyGraphicAnabaticEngine = NULL;

    HTRY
      pyGraphicAnabaticEngine = PyObject_NEW ( PyGraphicAnabaticEngine, &PyTypeGraphicAnabaticEngine );
      if ( pyGraphicAnabaticEngine == NULL ) return NULL;
  
      pyGraphicAnabaticEngine->ACCESS_OBJECT = GraphicAnabaticEngine::grab();
    HCATCH

    return (PyObject*)pyGraphicAnabaticEngine;
  }


  static PyObject* PyGraphicAnabaticEngine_getCell ( PyGraphicAnabaticEngine* self )
  {
    cdebug.log(32) << "PyGraphicAnabaticEngine_getCell ()" << endl;

    Cell* cell = NULL;
    
    HTRY
    METHOD_HEAD("GraphicAnabaticEngine.getCell()")
      cell = gengine->getCell ();
    HCATCH

    if (cell == NULL) Py_RETURN_NONE;
    return PyCell_Link(cell);
  }


  GetNameMethod(GraphicAnabaticEngine, gengine)

  // Standart destroy (Attribute).


  PyMethodDef PyGraphicAnabaticEngine_Methods[] =
    { { "grab"                , (PyCFunction)PyGraphicAnabaticEngine_grab                , METH_NOARGS|METH_STATIC
                              , "Returns the GraphicAnabaticEngine singleton." }
    , { "getName"             , (PyCFunction)PyGraphicAnabaticEngine_getName             , METH_NOARGS
                              , "Returns the name of the GraphicAnabaticEngine (class attribute)." }
    , { "getCell"             , (PyCFunction)PyGraphicAnabaticEngine_getCell             , METH_NOARGS
                              , "Returns the Cell on which this GraphicAnabaticEngine is attached." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  // ---------------------------------------------------------------
  // PyGraphicAnabaticEngine Type Methods.


  PythonOnlyDeleteMethod(GraphicAnabaticEngine)
  PyTypeObjectLinkPyType(GraphicAnabaticEngine)


#else  // End of Python Module Code Part.


// +=================================================================+
// |       "PyGraphicAnabaticEngine" Shared Library Code Part        |
// +=================================================================+

  // Link/Creation Method.
  LinkCreateMethod(GraphicAnabaticEngine)

  PyTypeInheritedObjectDefinitions(GraphicAnabaticEngine,GraphicTool)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // CRL namespace. 
