// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyGraphicKatanaEngine.cpp"                   |
// +-----------------------------------------------------------------+


#include "katana/PyGraphicKatanaEngine.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/Cell.h"


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define METHOD_HEAD(function)    GENERIC_METHOD_HEAD(GraphicKatanaEngine,gtool,function)


namespace  Katana {

using namespace Hurricane;
using namespace Isobar;

extern "C" {


// +=================================================================+
// |         "PyGraphicKatanaEngine" Python Module Code Part         |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  // +-------------------------------------------------------------+
  // |          "PyGraphicKatanaEngine" Attribute Methods          |
  // +-------------------------------------------------------------+


  static PyObject* PyGraphicKatanaEngine_grab ( PyObject* )
  {
    cdebug_log(40,0) << "PyGraphicKatanaEngine_grab()" << endl;
    PyGraphicKatanaEngine* pyGraphicKatanaEngine = NULL;

    HTRY
    pyGraphicKatanaEngine = PyObject_NEW ( PyGraphicKatanaEngine, &PyTypeGraphicKatanaEngine );
    if ( pyGraphicKatanaEngine == NULL ) return NULL;

    pyGraphicKatanaEngine->ACCESS_OBJECT = GraphicKatanaEngine::grab();
    HCATCH

    return (PyObject*)pyGraphicKatanaEngine;
  }


  static PyObject* PyGraphicKatanaEngine_getCell ( PyGraphicKatanaEngine* self )
  {
    cdebug_log(40,0) << "PyGraphicKatanaEngine_getCell ()" << endl;

    Cell* cell = NULL;
    
    HTRY
    METHOD_HEAD("GraphicKatanaEngine.getCell()")
    cell = gtool->getCell ();
    HCATCH

    if (cell == NULL) Py_RETURN_NONE;
    return PyCell_Link(cell);
  }


  GetNameMethod(GraphicKatanaEngine, gtool)

  // Standart destroy (Attribute).


  PyMethodDef PyGraphicKatanaEngine_Methods[] =
    { { "grab"                , (PyCFunction)PyGraphicKatanaEngine_grab                , METH_NOARGS|METH_STATIC
                              , "Returns the GraphicKatanaEngine singleton." }
    , { "getName"             , (PyCFunction)PyGraphicKatanaEngine_getName             , METH_NOARGS
                              , "Returns the name of the GraphicKatanaEngine (class attribute)." }
    , { "getCell"             , (PyCFunction)PyGraphicKatanaEngine_getCell             , METH_NOARGS
                              , "Returns the Cell on which this GraphicKatanaEngine is attached." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  // ---------------------------------------------------------------
  // PyGraphicKatanaEngine Type Methods.


  PythonOnlyDeleteMethod(GraphicKatanaEngine)
  PyTypeObjectLinkPyType(GraphicKatanaEngine)


#else  // End of Python Module Code Part.


// +=================================================================+
// |        "PyGraphicKatanaEngine" Shared Library Code Part         |
// +=================================================================+

  // Link/Creation Method.
  LinkCreateMethod(GraphicKatanaEngine)

  PyTypeInheritedObjectDefinitions(GraphicKatanaEngine,GraphicTool)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // CRL namespace. 
