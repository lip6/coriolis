// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2010-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyAllianceFramework.cpp"                     |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyCell.h"
#include "hurricane/isobar/PyLibrary.h"
#include "hurricane/DataBase.h"
#include "crlcore/PyEnvironment.h"
#include "crlcore/PyCellGauge.h"
#include "crlcore/PyRoutingGauge.h"
#include "crlcore/PyAllianceFramework.h"
#include "crlcore/GraphicsParser.h"
#include "crlcore/SymbolicTechnologyParser.h"
#include "crlcore/RealTechnologyParser.h"


namespace  CRL {

  using std::cerr;
  using std::endl;
  using std::hex;
  using std::ostringstream;
  using Hurricane::tab;
  using Hurricane::in_trace;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::DataBase;
  using Isobar::ProxyProperty;
  using Isobar::ProxyError;
  using Isobar::ConstructorError;
  using Isobar::HurricaneError;
  using Isobar::HurricaneWarning;
  using Isobar::PyAny_AsLong;
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


  static PyObject* PyAllianceFramework_get ( PyObject* )
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


  extern PyObject* PyAllianceFramework_getEnvironment ( PyAllianceFramework* self )
  {
    trace << "PyAllianceFramework_getEnvironment ()" << endl;

    Environment* env = NULL;

    HTRY
    METHOD_HEAD("AllianceFramework.getEnvironment()")

    env = af->getEnvironment();
    if (env == NULL) return NULL;
    
    HCATCH

    return PyEnvironment_Link(env);
  }


  extern PyObject* PyAllianceFramework_getLibrary ( PyAllianceFramework* self, PyObject* args )
  {
    trace << "PyAllianceFramework_getLibrary ()" << endl;

    Library* lib = NULL;

    HTRY
    METHOD_HEAD("AllianceFramework.getLibrary()")

    PyObject* arg0;
    if ( not ParseOneArg ( "AllianceFramework.getLibrary()", args, INT_ARG, &arg0 ) ) return NULL;

    lib = af->getLibrary ( PyAny_AsLong(arg0) );
    
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

    af->saveCell ( PYCELL_O(arg0),PyAny_AsLong(arg1) );

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


  static PyObject* PyAllianceFramework_isPad ( PyAllianceFramework* self, PyObject* args )
  {
    trace << "PyAllianceFramework_isPad ()" << endl;

    char* name = NULL;
    
    HTRY
    METHOD_HEAD("AllianceFramework.isPad()")

    if ( not PyArg_ParseTuple(args,"s",&name) ) {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Cell AllianceFramework.isPad().");
      return NULL;
    }

    if (af->isPad(name)) Py_RETURN_TRUE;
    HCATCH

    Py_RETURN_FALSE;
  }


  extern PyObject* PyAllianceFramework_addRoutingGauge ( PyAllianceFramework* self, PyObject* args )
  {
    trace << "PyAllianceFramework_addRoutingGauge ()" << endl;

    HTRY
    METHOD_HEAD("AllianceFramework.addRoutingGauge()")
    PyObject* arg0;
    if ( not ParseOneArg("AllianceFramework.addRoutingGauge", args, ":routGauge", &arg0) )
      return NULL;
    af->addRoutingGauge ( PYROUTINGGAUGE_O(arg0) );
    HCATCH

    Py_RETURN_NONE;
  }


  extern PyObject* PyAllianceFramework_getRoutingGauge ( PyAllianceFramework* self, PyObject* args )
  {
    trace << "PyAllianceFramework_getRoutingGauge ()" << endl;

    RoutingGauge* rg = NULL;

    HTRY
    METHOD_HEAD("AllianceFramework.getRoutingGauge()")
    char* name  = NULL;
    Name  hName = "";
    if (PyArg_ParseTuple( args, "|s:RoutingGauge.getRoutingGauge", &name)) {
      if ( name != NULL ) hName = Name(name);
      rg = af->getRoutingGauge(hName);
    } else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for AllianceFramework.getRoutingGauge()." );
      return NULL;
    }
    HCATCH

    if ( rg == NULL ) Py_RETURN_NONE;
    return PyRoutingGauge_Link(rg);
  }


  extern PyObject* PyAllianceFramework_addCellGauge ( PyAllianceFramework* self, PyObject* args )
  {
    trace << "PyAllianceFramework_addCellGauge ()" << endl;

    HTRY
    METHOD_HEAD("AllianceFramework.addCellGauge()")
    PyObject* arg0;
    if ( not ParseOneArg("AllianceFramework.addCellGauge", args, ":cellGauge", &arg0) )
      return NULL;
    af->addCellGauge ( PYCELLGAUGE_O(arg0) );
    HCATCH

    Py_RETURN_NONE;
  }


  extern PyObject* PyAllianceFramework_getCellGauge ( PyAllianceFramework* self, PyObject* args )
  {
    trace << "PyAllianceFramework_getCellGauge ()" << endl;

    CellGauge* rg = NULL;

    HTRY
    METHOD_HEAD("AllianceFramework.getCellGauge()")
    char* name  = NULL;
    Name  hName = "";
    if (PyArg_ParseTuple( args, "|s:CellGauge.getCellGauge", &name)) {
      if ( name != NULL ) hName = Name(name);
      rg = af->getCellGauge(hName);
    } else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for AllianceFramework.getCellGauge()." );
      return NULL;
    }
    HCATCH

    if ( rg == NULL ) Py_RETURN_NONE;
    return PyCellGauge_Link(rg);
  }

  
  // Standart Accessors (Attributes).


  // Standart Destroy (Attribute).
  // DBoDestroyAttribute(PyAllianceFramework_destroy,PyAllianceFramework)


  PyMethodDef PyAllianceFramework_Methods[] =
    { { "get"                  , (PyCFunction)PyAllianceFramework_get                  , METH_NOARGS|METH_STATIC
                               , "Gets the Alliance Framework." }                      
    , { "getEnvironment"       , (PyCFunction)PyAllianceFramework_getEnvironment       , METH_NOARGS
                               , "Gets the Alliance Environment." }
    , { "getLibrary"           , (PyCFunction)PyAllianceFramework_getLibrary           , METH_VARARGS
                               , "Gets a Library, by index." }                         
    , { "getCell"              , (PyCFunction)PyAllianceFramework_getCell              , METH_VARARGS
                               , "Gets an Alliance Cell." }                            
    , { "saveCell"             , (PyCFunction)PyAllianceFramework_saveCell             , METH_VARARGS
                               , "Saves an Alliance Cell." }                           
    , { "createCell"           , (PyCFunction)PyAllianceFramework_createCell           , METH_VARARGS
                               , "Create a Cell in the Alliance framework." }
    , { "isPad"                , (PyCFunction)PyAllianceFramework_isPad                 , METH_VARARGS
                               , "Tells if a cell name is a Pad." }
    , { "addCellGauge"         , (PyCFunction)PyAllianceFramework_addCellGauge         , METH_VARARGS
                               , "Add a new cell gauge." }
    , { "getCellGauge"         , (PyCFunction)PyAllianceFramework_getCellGauge         , METH_VARARGS
                               , "Get a cell gauge (whithout a name, return the default)." }          
    , { "addRoutingGauge"      , (PyCFunction)PyAllianceFramework_addRoutingGauge      , METH_VARARGS
                               , "Add a new routing gauge." }
    , { "getRoutingGauge"      , (PyCFunction)PyAllianceFramework_getRoutingGauge      , METH_VARARGS
                               , "Get a routing gauge (whithout a name, return the default)." }          
  //, { "destroy"              , (PyCFunction)PyAllianceFramework_destroy              , METH_NOARGS
  //                           , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL}    /* sentinel */
    };


  PythonOnlyDeleteMethod(AllianceFramework)
  PyTypeObjectLinkPyType(AllianceFramework)


#else  // End of Python Module Code Part.


// x=================================================================x
// |          "PyAllianceFramework" Shared Library Code Part         |
// x=================================================================x



  // Link/Creation Method.
  PyTypeObjectDefinitions(AllianceFramework)


#endif  // End of Shared Library Code Part.


}  // End of extern "C".


}  // End of CRL namespace.
 
