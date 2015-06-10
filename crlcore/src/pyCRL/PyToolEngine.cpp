
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyToolEngine.cpp"                            |
// +-----------------------------------------------------------------+


#include "crlcore/PyToolEngine.h"
#include "crlcore/PyToolEngineCollection.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/Cell.h"


namespace  CRL {

using namespace Hurricane;
using namespace Isobar;

extern "C" {


#define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(ToolEngine,tool,function)


// +=================================================================+
// |             "PyToolEngine" Python Module Code Part              |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  // +-------------------------------------------------------------+
  // |              "PyToolEngine" Attribute Methods               |
  // +-------------------------------------------------------------+


  static PyObject* PyToolEngine_get ( PyObject*, PyObject* args, PyObject* kwArgs )
  {
    trace << "PyToolEngine_get()" << endl;
    
    HTRY
    PyObject*   pyCell = NULL;
    char* name   = NULL;

    static char* keywords[] = { (char*)"cell", (char*)"name", NULL };

    if (PyArg_ParseTupleAndKeywords( args
                                   , kwArgs
                                   , "O|s:ToolEngine.get()"
                                   , keywords
                                   , &pyCell
                                   , &name )) {
      if (not IsPyCell(pyCell)) {
        PyErr_SetString ( ConstructorError, "ToolEngine.get(): First argument is not of type Cell." );
        return NULL;
      }
      Cell* cell = PYCELL_O(pyCell);

      if ( name != NULL ) {
      // Calling C++ overload ToolEngine::get(Cell*,Name);
        PyToolEngine* pyToolEngine = PyObject_NEW ( PyToolEngine, &PyTypeToolEngine );
        if ( pyToolEngine == NULL ) return NULL;

        ToolEngine* tool = ToolEngine::get(cell, Name(name));
        pyToolEngine->_object = tool;

        return (PyObject*)pyToolEngine;
      } else {
      // Calling C++ overload ToolEngine::get(Cell*);
        PyToolEngineCollection* pyToolEngineCollection = PyObject_NEW(PyToolEngineCollection, &PyTypeToolEngineCollection);
        if ( pyToolEngineCollection == NULL ) return NULL;

        ToolEngines* tools = new ToolEngines(ToolEngine::get(cell));
        pyToolEngineCollection->_object = tools;

        return (PyObject*)pyToolEngineCollection;
      }
    } else {
      PyErr_SetString ( ConstructorError, "ToolEngine::get(): Invalid number of parameters." );
      return NULL;
    }

    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyToolEngine_destroyAll ( PyObject* )
  {
    trace << "PyToolEngine_destroyAll()" << endl;
    
    HTRY
    ToolEngine::destroyAll();
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyToolEngine_getCell ( PyToolEngine* self )
  {
    trace << "PyToolEngine_getCell ()" << endl;

    Cell* cell = NULL;
    
    HTRY
    METHOD_HEAD("ToolEngine.getCell()")
    cell = tool->getCell ();
    HCATCH

    if (cell == NULL) Py_RETURN_NONE;
    return PyCell_Link(cell);
  }


  GetNameMethod(ToolEngine, tool)
  DirectGetBoolAttribute(PyToolEngine_getPlacementFlag,placementModificationFlagHasChanged,PyToolEngine,ToolEngine)
  DirectGetBoolAttribute(PyToolEngine_getRoutingFlag  ,routingModificationFlagHasChanged  ,PyToolEngine,ToolEngine)

  // Standart destroy (Attribute).


  PyMethodDef PyToolEngine_Methods[] =
    { { "get"                 , (PyCFunction)PyToolEngine_get                 , METH_VARARGS|METH_STATIC
                              , "Returns either one ToolEngine or the collection of all." }
    , { "destroyAll"          , (PyCFunction)PyToolEngine_destroyAll          , METH_NOARGS|METH_STATIC
                              , "Destroy all instances of this ToolEngine on every Cell." }
    , { "getName"             , (PyCFunction)PyToolEngine_getName             , METH_NOARGS
                              , "Returns the name of the ToolEngine (class attribute)." }
    , { "getCell"             , (PyCFunction)PyToolEngine_getCell             , METH_NOARGS
                              , "Returns the Cell on which this ToolEngine is attached." }
    , { "placementModificationFlagHasChanged"
                              , (PyCFunction)PyToolEngine_getPlacementFlag    , METH_NOARGS
                              , "Returns the state of the placement modification flag." }
    , { "routingModificationFlagHasChanged"
                              , (PyCFunction)PyToolEngine_getRoutingFlag      , METH_NOARGS
                              , "Returns the state of the routing modification flag." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  // ---------------------------------------------------------------
  // PyToolEngine Type Methods.


  DBoDeleteMethod(ToolEngine)
  PyTypeObjectLinkPyType(ToolEngine)


#else  // End of Python Module Code Part.


// +=================================================================+
// |            "PyToolEngine" Shared Library Code Part              |
// +=================================================================+

  // Link/Creation Method.
  DBoLinkCreateMethod(ToolEngine)

  PyTypeRootObjectDefinitions(ToolEngine)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // CRL namespace. 
