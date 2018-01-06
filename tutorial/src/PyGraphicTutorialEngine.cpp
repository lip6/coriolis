// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2017-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |        T o o l E n g i n e   T u t o r i a l                    |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyGraphicTutorialEngine.cpp"                 |
// +-----------------------------------------------------------------+


#include "tutorial/PyGraphicTutorialEngine.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/Cell.h"


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT            _baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define METHOD_HEAD(function)    GENERIC_METHOD_HEAD(GraphicTutorialEngine,gtool,function)


namespace  Tutorial {

using namespace Hurricane;
using namespace Isobar;

extern "C" {


// +=================================================================+
// |        "PyGraphicTutorialEngine" Python Module Code Part        |
// +=================================================================+

#if defined(__PYTHON_MODULE__)

  static PyObject* PyGraphicTutorialEngine_grab ( PyObject* )
  {
    cdebug_log(40,0) << "PyGraphicTutorialEngine_grab()" << endl;
    PyGraphicTutorialEngine* pyGraphicTutorialEngine = NULL;

    HTRY
    pyGraphicTutorialEngine = PyObject_NEW ( PyGraphicTutorialEngine, &PyTypeGraphicTutorialEngine );
    if ( pyGraphicTutorialEngine == NULL ) return NULL;

    pyGraphicTutorialEngine->ACCESS_OBJECT = GraphicTutorialEngine::grab();
    HCATCH

    return (PyObject*)pyGraphicTutorialEngine;
  }


  static PyObject* PyGraphicTutorialEngine_getCell ( PyGraphicTutorialEngine* self )
  {
    cdebug_log(40,0) << "PyGraphicTutorialEngine_getCell ()" << endl;

    Cell* cell = NULL;
    
    HTRY
    METHOD_HEAD("GraphicTutorialEngine.getCell()")
    cell = gtool->getCell ();
    HCATCH

    if (cell == NULL) Py_RETURN_NONE;
    return PyCell_Link(cell);
  }


  GetNameMethod(GraphicTutorialEngine, gtool)

  // Standart destroy (Attribute).


  PyMethodDef PyGraphicTutorialEngine_Methods[] =
    { { "grab"                , (PyCFunction)PyGraphicTutorialEngine_grab                , METH_NOARGS|METH_STATIC
                              , "Returns the GraphicTutorialEngine singleton." }
    , { "getName"             , (PyCFunction)PyGraphicTutorialEngine_getName             , METH_NOARGS
                              , "Returns the name of the GraphicTutorialEngine (class attribute)." }
    , { "getCell"             , (PyCFunction)PyGraphicTutorialEngine_getCell             , METH_NOARGS
                              , "Returns the Cell on which this GraphicTutorialEngine is attached." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  // ---------------------------------------------------------------
  // PyGraphicTutorialEngine Type Methods.


  PythonOnlyDeleteMethod(GraphicTutorialEngine)
  PyTypeObjectLinkPyType(GraphicTutorialEngine)


#else  // End of Python Module Code Part.


// +=================================================================+
// |       "PyGraphicTutorialEngine" Shared Library Code Part        |
// +=================================================================+

  // Link/Creation Method.
  LinkCreateMethod(GraphicTutorialEngine)

  PyTypeInheritedObjectDefinitions(GraphicTutorialEngine,GraphicTool)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Tutorial namespace. 
