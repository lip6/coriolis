// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2007-2023, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |       T r a m o n t a n a  -  Extractor & LVX                   |
// |                                                                 |
// |  Algorithm    :                   Christian MASSON              |
// |  First impl.  :                           Yifei WU              |
// |  Second impl. :                   Jean-Paul CHAPUT              |
// |  E-mail       :           Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module   :  "./PyGraphicTramontanaEngine.cpp"              |
// +-----------------------------------------------------------------+


#include "tramontana/PyGraphicTramontanaEngine.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/Cell.h"


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define METHOD_HEAD(function)    GENERIC_METHOD_HEAD(GraphicTramontanaEngine,gtool,function)


namespace  Tramontana {

using namespace Hurricane;
using namespace Isobar;

extern "C" {


// +=================================================================+
// |       "PyGraphicTramontanaEngine" Python Module Code Part       |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  static PyObject* PyGraphicTramontanaEngine_grab ( PyObject* )
  {
    cdebug_log(40,0) << "PyGraphicTramontanaEngine_grab()" << endl;
    PyGraphicTramontanaEngine* pyGraphicTramontanaEngine = NULL;
    HTRY
      pyGraphicTramontanaEngine = PyObject_NEW ( PyGraphicTramontanaEngine, &PyTypeGraphicTramontanaEngine );
      if ( pyGraphicTramontanaEngine == NULL ) return NULL;
      pyGraphicTramontanaEngine->ACCESS_OBJECT = GraphicTramontanaEngine::grab();
    HCATCH
    return (PyObject*)pyGraphicTramontanaEngine;
  }


  static PyObject* PyGraphicTramontanaEngine_getCell ( PyGraphicTramontanaEngine* self )
  {
    cdebug_log(40,0) << "PyGraphicTramontanaEngine_getCell ()" << endl;
    Cell* cell = NULL;
    HTRY
      METHOD_HEAD("GraphicTramontanaEngine.getCell()")
      cell = gtool->getCell ();
    HCATCH
    if (cell == NULL) Py_RETURN_NONE;
    return PyCell_Link(cell);
  }


  GetNameMethod(GraphicTramontanaEngine, gtool)

  // Standart destroy (Attribute).


  PyMethodDef PyGraphicTramontanaEngine_Methods[] =
    { { "grab"                , (PyCFunction)PyGraphicTramontanaEngine_grab                , METH_NOARGS|METH_STATIC
                              , "Returns the GraphicTramontanaEngine singleton." }
    , { "getName"             , (PyCFunction)PyGraphicTramontanaEngine_getName             , METH_NOARGS
                              , "Returns the name of the GraphicTramontanaEngine (class attribute)." }
    , { "getCell"             , (PyCFunction)PyGraphicTramontanaEngine_getCell             , METH_NOARGS
                              , "Returns the Cell on which this GraphicTramontanaEngine is attached." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  // ---------------------------------------------------------------
  // PyGraphicTramontanaEngine Type Methods.


  PythonOnlyDeleteMethod(GraphicTramontanaEngine)
  PyTypeObjectLinkPyType(GraphicTramontanaEngine)


#else  // End of Python Module Code Part.


// +=================================================================+
// |      "PyGraphicTramontanaEngine" Shared Library Code Part       |
// +=================================================================+

  // Link/Creation Method.
  LinkCreateMethod(GraphicTramontanaEngine)

  PyTypeInheritedObjectDefinitions(GraphicTramontanaEngine,GraphicTool)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // CRL namespace. 
