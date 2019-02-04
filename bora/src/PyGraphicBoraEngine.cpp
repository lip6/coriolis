// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |  B o r a  -  A n a l o g   S l i c i n g   T r e e              |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./bora/PyGraphicBoraEngine.cpp"                |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyCell.h"
#include "bora/PyGraphicBoraEngine.h"


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define METHOD_HEAD(function)    GENERIC_METHOD_HEAD(GraphicBoraEngine,gtool,function)


namespace  Bora {

using namespace Hurricane;
using namespace Isobar;

extern "C" {


// +=================================================================+
// |          "PyGraphicBoraEngine" Python Module Code Part          |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  static PyObject* PyGraphicBoraEngine_grab ( PyObject* )
  {
    cdebug.log(61) << "PyGraphicBoraEngine_grab()" << endl;
    PyGraphicBoraEngine* pyGraphicBoraEngine = NULL;

    HTRY
    pyGraphicBoraEngine = PyObject_NEW ( PyGraphicBoraEngine, &PyTypeGraphicBoraEngine );
    if ( pyGraphicBoraEngine == NULL ) return NULL;

    pyGraphicBoraEngine->ACCESS_OBJECT = GraphicBoraEngine::grab();
    HCATCH

    return (PyObject*)pyGraphicBoraEngine;
  }


  static PyObject* PyGraphicBoraEngine_getCell ( PyGraphicBoraEngine* self )
  {
    cdebug.log(61) << "PyGraphicBoraEngine_getCell ()" << endl;

    Cell* cell = NULL;
    
    HTRY
    METHOD_HEAD("GraphicBoraEngine.getCell()")
    cell = gtool->getCell ();
    HCATCH

    if (cell == NULL) Py_RETURN_NONE;
    return PyCell_Link(cell);
  }


  GetNameMethod(GraphicBoraEngine, gtool)

  // Standart destroy (Attribute).


  PyMethodDef PyGraphicBoraEngine_Methods[] =
    { { "grab"                , (PyCFunction)PyGraphicBoraEngine_grab                , METH_NOARGS|METH_STATIC
                              , "Returns the GraphicBoraEngine singleton." }
    , { "getName"             , (PyCFunction)PyGraphicBoraEngine_getName             , METH_NOARGS
                              , "Returns the name of the GraphicBoraEngine (class attribute)." }
    , { "getCell"             , (PyCFunction)PyGraphicBoraEngine_getCell             , METH_NOARGS
                              , "Returns the Cell on which this GraphicBoraEngine is attached." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  // ---------------------------------------------------------------
  // PyGraphicBoraEngine Type Methods.


  PythonOnlyDeleteMethod(GraphicBoraEngine)
  PyTypeObjectLinkPyType(GraphicBoraEngine)


#else  // End of Python Module Code Part.


// +=================================================================+
// |         "PyGraphicBoraEngine" Shared Library Code Part          |
// +=================================================================+

  // Link/Creation Method.
  LinkCreateMethod(GraphicBoraEngine)

  PyTypeInheritedObjectDefinitions(GraphicBoraEngine,GraphicTool)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Bora namespace. 
