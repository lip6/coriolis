
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2013, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyGraphicKiteEngine.cpp"                     |
// +-----------------------------------------------------------------+


#include "kite/PyGraphicKiteEngine.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/Cell.h"


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define METHOD_HEAD(function)    GENERIC_METHOD_HEAD(GraphicKiteEngine,gtool,function)


namespace  Kite {

using namespace Hurricane;
using namespace Isobar;

extern "C" {


// +=================================================================+
// |          "PyGraphicKiteEngine" Python Module Code Part          |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  // +-------------------------------------------------------------+
  // |           "PyGraphicKiteEngine" Attribute Methods           |
  // +-------------------------------------------------------------+


  static PyObject* PyGraphicKiteEngine_grab ( PyObject* )
  {
    trace << "PyGraphicKiteEngine_grab()" << endl;
    PyGraphicKiteEngine* pyGraphicKiteEngine = NULL;

    HTRY
    pyGraphicKiteEngine = PyObject_NEW ( PyGraphicKiteEngine, &PyTypeGraphicKiteEngine );
    if ( pyGraphicKiteEngine == NULL ) return NULL;

    pyGraphicKiteEngine->ACCESS_OBJECT = GraphicKiteEngine::grab();
    HCATCH

    return (PyObject*)pyGraphicKiteEngine;
  }


  static PyObject* PyGraphicKiteEngine_getCell ( PyGraphicKiteEngine* self )
  {
    trace << "PyGraphicKiteEngine_getCell ()" << endl;

    Cell* cell = NULL;
    
    HTRY
    METHOD_HEAD("GraphicKiteEngine.getCell()")
    cell = gtool->getCell ();
    HCATCH

    if (cell == NULL) Py_RETURN_NONE;
    return PyCell_Link(cell);
  }


  GetNameMethod(GraphicKiteEngine, gtool)

  // Standart destroy (Attribute).


  PyMethodDef PyGraphicKiteEngine_Methods[] =
    { { "grab"                , (PyCFunction)PyGraphicKiteEngine_grab                , METH_NOARGS|METH_STATIC
                              , "Returns the GraphicKiteEngine singleton." }
    , { "getName"             , (PyCFunction)PyGraphicKiteEngine_getName             , METH_NOARGS
                              , "Returns the name of the GraphicKiteEngine (class attribute)." }
    , { "getCell"             , (PyCFunction)PyGraphicKiteEngine_getCell             , METH_NOARGS
                              , "Returns the Cell on which this GraphicKiteEngine is attached." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  // ---------------------------------------------------------------
  // PyGraphicKiteEngine Type Methods.


  PythonOnlyDeleteMethod(GraphicKiteEngine)
  PyTypeObjectLinkPyType(GraphicKiteEngine)


#else  // End of Python Module Code Part.


// +=================================================================+
// |         "PyGraphicKiteEngine" Shared Library Code Part          |
// +=================================================================+

  // Link/Creation Method.
  LinkCreateMethod(GraphicKiteEngine)

  PyTypeInheritedObjectDefinitions(GraphicKiteEngine,GraphicTool)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // CRL namespace. 
