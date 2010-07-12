
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2010-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyAllianceFramework.cpp"                |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include "hurricane/isobar/PyCell.h"
#include "hurricane/isobar/PyLibrary.h"
#include "crlcore/Catalog.h"  // TEMPORARY
#include "crlcore/PyAllianceFramework.h"


namespace  CRL {

  using std::cerr;
  using std::endl;
  using std::hex;
  using std::ostringstream;
  using Hurricane::tab;
  using Hurricane::in_trace;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Isobar::ProxyProperty;
  using Isobar::ProxyError;
  using Isobar::ConstructorError;
  using Isobar::HurricaneError;
  using Isobar::HurricaneWarning;
  using Isobar::ParseOneArg;
  using Isobar::ParseTwoArg;
  using Isobar::PyLibrary;
  using Isobar::PyLibrary_Link;
  using Isobar::PyCell;
  using Isobar::PyCell_Link;


extern "C" {


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(AllianceFramework,af,function)


// x=================================================================x
// |         "PyAllianceFramework" Python Module Code Part           |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  extern PyObject* PyAllianceFramework_getLibrary ( PyAllianceFramework* self, PyObject* args )
  {
    trace << "PyAllianceFramework_getLibrary ()" << endl;

    Library* lib = NULL;

    HTRY
    METHOD_HEAD("AllianceFramework.getLibrary()")

    PyObject* arg0;
    if ( not ParseOneArg ( "AllianceFramework.getLibrary()", args, INT_ARG, &arg0 ) ) return NULL;

    lib = af->getLibrary ( PyInt_AsLong(arg0) );
    
    if ( lib == NULL ) Py_RETURN_NONE;
    HCATCH

    return PyLibrary_Link(lib);
  }


  extern PyObject* PyAllianceFramework_getCell ( PyAllianceFramework* self, PyObject* args )
  {
    trace << "PyAllianceFramework_getCell ()" << endl;

    char* name = NULL;
    Cell* cell = NULL;
    int   mode = 0;
    
    HTRY
    METHOD_HEAD("AllianceFramework.getCell()")

    if ( not PyArg_ParseTuple(args,"si",&name,&mode) ) {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Cell AllianceFramework.getCell().");
      return NULL;
    }

    cell = af->getCell ( name, mode );
    HCATCH

    return PyCell_Link(cell);
  }


  extern PyObject* PyAllianceFramework_saveCell ( PyAllianceFramework* self, PyObject* args )
  {
    trace << "PyAllianceFramework_saveCell ()" << endl;

    HTRY
    
    METHOD_HEAD("AllianceFramework.saveCell()")

    PyObject* arg0;
    PyObject* arg1;

    if ( not ParseTwoArg ( "AllianceFramework.saveCell", args, CELL_INT_ARG, &arg0, &arg1) ) return NULL;

    af->saveCell ( PYCELL_O(arg0),PyInt_AsLong(arg1) );

    HCATCH

    Py_RETURN_NONE;
  }


  extern PyObject* PyAllianceFramework_createCell ( PyAllianceFramework* self, PyObject* args )
  {
    trace << "PyAllianceFramework_createCell ()" << endl;

    char* name = NULL;
    Cell* cell = NULL;
    
    HTRY
    METHOD_HEAD("AllianceFramework.createCell()")

    if ( not PyArg_ParseTuple(args,"s",&name) ) {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Cell AllianceFramework.createCell().");
      return NULL;
    }

    cell = af->createCell ( name );
    HCATCH

    return PyCell_Link(cell);
  }

  
  // Standart Accessors (Attributes).


  // Standart Destroy (Attribute).
  // DBoDestroyAttribute(PyAllianceFramework_destroy,PyAllianceFramework)


  PyMethodDef PyAllianceFramework_Methods[] =
    { { "getLibrary"   , (PyCFunction)PyAllianceFramework_getLibrary, METH_VARARGS, "Gets a Library, by index." }
    , { "getCell"      , (PyCFunction)PyAllianceFramework_getCell   , METH_VARARGS, "Gets an Alliance Cell." }
    , { "saveCell"     , (PyCFunction)PyAllianceFramework_saveCell  , METH_VARARGS, "Saves an Alliance Cell." }
    , { "createCell"   , (PyCFunction)PyAllianceFramework_createCell, METH_VARARGS, "Create a Cell in the Alliance framework." }
  //, { "destroy"      , (PyCFunction)PyAllianceFramework_destroy   , METH_NOARGS
  //                   , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };


  PythonOnlyDeleteMethod(AllianceFramework)
  PyTypeObjectLinkPyType(AllianceFramework)


#else  // End of Python Module Code Part.


// x=================================================================x
// |          "PyAllianceFramework" Shared Library Code Part         |
// x=================================================================x


  PyObject* PyAllianceFramework_get ( PyObject *module )
  {
    trace << "PyAllianceFramework_get()" << endl;

    AllianceFramework*   af   = NULL;
    PyAllianceFramework* pyAf = NULL;
    
    HTRY
    af = AllianceFramework::get ();

    pyAf = PyObject_NEW ( PyAllianceFramework, &PyTypeAllianceFramework );
    if ( pyAf == NULL ) return NULL;

    pyAf->_object = af;
    HCATCH

    return (PyObject*)pyAf;
  }



  // Link/Creation Method.
  PyTypeObjectDefinitions(AllianceFramework)


#endif  // End of Shared Library Code Part.


}  // End of extern "C".


}  // End of CRL namespace.
 
