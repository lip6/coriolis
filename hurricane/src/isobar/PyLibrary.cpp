// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyLibrary.cpp"                          |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyDataBase.h"
#include "hurricane/isobar/PyLibrary.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/isobar/PyCellCollection.h"


namespace Isobar {

using namespace Hurricane;

extern "C" {


#define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Library,lib,function)


// +=================================================================+
// |              "PyLibrary" Python Module Code Part                |
// +=================================================================+

#if defined(__PYTHON_MODULE__)

  GetNameMethod(Library, lib)


  static PyObject* PyLibrary_create ( PyObject *, PyObject *args ) {
      cdebug_log(20,0) << "PyLibrary_create()" << endl;

      PyObject* arg0;
      PyObject* arg1;
      Library* library = NULL;

      HTRY
      __cs.init ("Library.create");
      if (!PyArg_ParseTuple(args,"O&O&:Library.create", Converter, &arg0, Converter, &arg1)) {
        PyErr_SetString ( ConstructorError, "invalid number of parameters for Library constructor." );
        return NULL;
      }
      if (__cs.getObjectIds() == ":db:string") {
          DataBase* db = PYDATABASE_O(arg0);
          library = Library::create(db, Name(PyString_AsString(arg1)));
      } else if (__cs.getObjectIds() == ":library:string") {
          Library* masterLibrary = PYLIBRARY_O(arg0);
          library = Library::create(masterLibrary, Name(PyString_AsString(arg1)));
      } else {
          PyErr_SetString( ConstructorError, "invalid number of parameters for Library constructor." );
          return NULL;
      }
      HCATCH

      return PyLibrary_Link( library );
  }


  static PyObject* PyLibrary_getSubLibrary ( PyLibrary *self, PyObject* args ) {
    cdebug_log(20,0) << "PyLibrary_getLibrary ()" << endl;

    Library* subLibrary = NULL;

    HTRY
      METHOD_HEAD ( "Library.getLibrary()" )
      char* name = NULL;
      if (PyArg_ParseTuple(args,"s:Library.getLibrary", &name)) {
        subLibrary = lib->getLibrary( Name(name) );
      } else {
        PyErr_SetString( ConstructorError, "Library.getLibrary(Name): Invalid number of parameters." );
        return NULL;
      }
    HCATCH
    
    return PyLibrary_Link( subLibrary );
  }


  static PyObject* PyLibrary_getCell ( PyLibrary* self, PyObject* args ) {
      cdebug_log(20,0) << "PyLibrary_getCell ()" << endl;
      
      Cell* cell = NULL;

      HTRY
      METHOD_HEAD("Library.getCell()")
      char* name = NULL;
      if (PyArg_ParseTuple(args,"s:Library.getCell", &name)) {
          cell = lib->getCell (Name(name));
      } else {
          PyErr_SetString ( ConstructorError, "invalid number of parameters for getCell." );
          return NULL;
      }
      HCATCH

      return PyCell_Link(cell);
  }


  static PyObject* PyLibrary_getCells(PyLibrary *self) {
    cdebug_log(20,0) << "PyLibrary_getCells()" << endl;

    METHOD_HEAD ( "Library.getCells()" )

    PyCellCollection* pyCellCollection = NULL;

    HTRY
    Cells* cells = new Cells(lib->getCells ());

    pyCellCollection = PyObject_NEW(PyCellCollection, &PyTypeCellCollection);
    if (pyCellCollection == NULL) { 
        return NULL;
    }

    pyCellCollection->_object = cells;
    HCATCH
    
    return (PyObject*)pyCellCollection;
  }


  // Standart Destroy (Attribute).
  DBoDestroyAttribute(PyLibrary_destroy, PyLibrary)


  PyMethodDef PyLibrary_Methods[] =
    { { "create"    , (PyCFunction)PyLibrary_create       , METH_VARARGS|METH_STATIC
                    , "Creates a new library." }
    , { "getName"   , (PyCFunction)PyLibrary_getName      , METH_NOARGS , "Returns the name of the library." }
    , { "getLibrary", (PyCFunction)PyLibrary_getSubLibrary, METH_VARARGS, "Get the sub-library named <name>" }
    , { "getCell"   , (PyCFunction)PyLibrary_getCell      , METH_VARARGS, "Get the cell of name <name>" }
    , { "getCells"  , (PyCFunction)PyLibrary_getCells     , METH_NOARGS , "Returns the collection of all cells of the library." }
    , { "destroy"   , (PyCFunction)PyLibrary_destroy      , METH_NOARGS
                    , "Destroy associated hurricane object The python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };


  DBoDeleteMethod(Library)
  PyTypeObjectLinkPyType(Library)


#else  // End of Python Module Code Part.


// +=================================================================+
// |             "PyLibrary" Shared Library Code Part                |
// +=================================================================+


  // Link/Creation Method.
  DBoLinkCreateMethod(Library)
  PyTypeObjectDefinitions(Library)


#endif  // End of Shared Library Code Part.


}  // extern "C".

}  // Isobar namespace.
