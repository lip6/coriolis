// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2010-2018, All Rights Reserved
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
#include "crlcore/PyCatalog.h"
#include "crlcore/PyCellGauge.h"
#include "crlcore/PyRoutingGauge.h"
#include "crlcore/PyAllianceLibrary.h"
#include "crlcore/PyAllianceFramework.h"


namespace  CRL {

  using std::cerr;
  using std::endl;
  using std::hex;
  using std::ostringstream;
  using Hurricane::tab;
  using Hurricane::Exception;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::DataBase;
  using Isobar::__cs;
  using Isobar::Converter;
  using Isobar::ProxyProperty;
  using Isobar::ProxyError;
  using Isobar::ConstructorError;
  using Isobar::HurricaneError;
  using Isobar::HurricaneWarning;
  using Isobar::PyAny_AsLong;
  using Isobar::getPyHash;
  using Isobar::ParseOneArg;
  using Isobar::ParseTwoArg;
  using Isobar::PyLibrary;
  using Isobar::PyLibrary_Link;
  using Isobar::PyCell;
  using Isobar::PyCell_Link;


  PyObject* AllianceLibsToList ( const AllianceLibraries& libs )
  {
    PyObject* pyList = PyList_New( libs.size() );
    for ( size_t i=0 ; i<libs.size() ; ++i )
      PyList_SetItem( pyList, i, PyAllianceLibrary_Link(libs[i]) );
    return pyList;
  }


extern "C" {


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(AllianceFramework,af,function)


// x=================================================================x
// |         "PyAllianceFramework" Python Module Code Part           |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  static PyObject* PyAllianceFramework_create ( PyObject*, PyObject* args )
  {
    cdebug_log(30,0) << "PyAllianceFramework_create()" << endl;

    AllianceFramework* af    = NULL;
    unsigned long      flags = AllianceFramework::NoFlags;
    
    HTRY
      PyObject* arg0;
      if (ParseOneArg("AllianceFramework.create()", args, INT_ARG, &arg0)) {
        flags = PyLong_AsUnsignedLongMask(arg0);
      }
      af = AllianceFramework::create( flags );
    HCATCH

    return PyAllianceFramework_Link( af );
  }


  static PyObject* PyAllianceFramework_get ( PyObject* args )
  {
    cdebug_log(30,0) << "PyAllianceFramework_get()" << endl;

    AllianceFramework* af = NULL;
    HTRY
      af = AllianceFramework::get();
    HCATCH
    return PyAllianceFramework_Link( af );
  }


  static PyObject* PyAllianceFramework_bindLibraries ( PyAllianceFramework* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyAllianceFramework_bindLibraries()" << endl;

    HTRY
      METHOD_HEAD("AllianceFramework.bindLibraries()")
      af->bindLibraries();
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyAllianceFramework_getEnvironment ( PyAllianceFramework* self )
  {
    cdebug_log(30,0) << "PyAllianceFramework_getEnvironment ()" << endl;

    Environment* env = NULL;

    HTRY
    METHOD_HEAD("AllianceFramework.getEnvironment()")

    env = af->getEnvironment();
    if (env == NULL) return NULL;
    
    HCATCH

    return PyEnvironment_Link(env);
  }


  static PyObject* PyAllianceFramework_getLibrary ( PyAllianceFramework* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyAllianceFramework_getLibrary()" << endl;

    Library* lib = NULL;

    HTRY
    METHOD_HEAD("AllianceFramework.getLibrary()")

    PyObject* arg0;
    __cs.init ("AllianceFramework.getLibrary");
    if (not PyArg_ParseTuple( args, "O&:AllianceFramework.getLibrary", Converter, &arg0)) {
      PyErr_SetString( ConstructorError, "Invalid number of parameters for AllianceFramework.getLibrary()." );
      return NULL;
    }

    if      (__cs.getObjectIds() == STRING_ARG) lib = af->getLibrary( Name(PyString_AsString(arg0)) );
    else if (__cs.getObjectIds() == INT_ARG   ) lib = af->getLibrary( PyAny_AsLong(arg0) );
    else {
      PyErr_SetString( ConstructorError, "Bad parameter type for AllianceFramework.getLibrary()." );
      return NULL;
    }

    if (lib == NULL) Py_RETURN_NONE;
    HCATCH

    return PyLibrary_Link(lib);
  }


  static PyObject* PyAllianceFramework_getAllianceLibrary ( PyAllianceFramework* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyAllianceFramework_getAllianceLibrary()" << endl;

    AllianceLibrary* alib = NULL;

    HTRY
    METHOD_HEAD("AllianceFramework.getAllianceLibrary()")

    PyObject* arg0;
    PyObject* arg1;
    __cs.init ("AllianceFramework.getAllianceLibrary");
    if (not PyArg_ParseTuple( args
                            , "O&|O&:AllianceFramework.getAllianceLibrary"
                            , Converter, &arg0
                            , Converter, &arg1)) {
      PyErr_SetString( ConstructorError, "Invalid number of parameters for AllianceFramework.getLibrary()." );
      return NULL;
    }

    if      (__cs.getObjectIds() == INT_ARG      ) alib = af->getAllianceLibrary( PyAny_AsLong(arg0) );
    else if (__cs.getObjectIds() == ":string:int") alib = af->getAllianceLibrary( Name(PyString_AsString(arg0)), PyAny_AsLong(arg1) );
    else if (__cs.getObjectIds() == ":library"   ) alib = af->getAllianceLibrary( PYLIBRARY_O(arg0) );
    else {
      PyErr_SetString( ConstructorError, "Bad parameter type for AllianceFramework.getAllianceLibrary()." );
      return NULL;
    }

    if (alib == NULL) Py_RETURN_NONE;
    HCATCH

    return PyAllianceLibrary_Link(alib);
  }


  static PyObject* PyAllianceFramework_getAllianceLibraries ( PyAllianceFramework* self )
  {
    cdebug_log(30,0) << "PyAllianceFramework_getAllianceLibraries()" << endl;
    PyObject* pyList = NULL;
    HTRY
      METHOD_HEAD("AllianceFramework.getAllianceLibraries()")
      __cs.init ("AllianceFramework.getAllianceLibraries");
      pyList = AllianceLibsToList( af->getAllianceLibraries() );
    HCATCH
    return pyList;
  }


  static PyObject* PyAllianceFramework_getCell ( PyAllianceFramework* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyAllianceFramework_getCell ()" << endl;

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


  static PyObject* PyAllianceFramework_saveCell ( PyAllianceFramework* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyAllianceFramework_saveCell ()" << endl;

    HTRY
    
    METHOD_HEAD("AllianceFramework.saveCell()")

    PyObject* arg0;
    PyObject* arg1;

    if ( not ParseTwoArg ( "AllianceFramework.saveCell", args, CELL_INT_ARG, &arg0, &arg1) ) return NULL;

  //if (PyAny_AsLong(arg1) & CRL::Catalog::State::Logical)
  //  cerr << "saveSell() " << PYCELL_O(arg0) << " Logical set" << endl;
    af->saveCell ( PYCELL_O(arg0),PyAny_AsLong(arg1) );

    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyAllianceFramework_createCell ( PyAllianceFramework* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyAllianceFramework_createCell ()" << endl;

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


  static PyObject* PyAllianceFramework_createLibrary ( PyAllianceFramework* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyAllianceFramework_createLibrary()" << endl;

    AllianceLibrary* alib    = NULL;
    string           libName = "";

    HTRY
    METHOD_HEAD("AllianceFramework.createLibrary()")

    PyObject* arg0;
    PyObject* arg1;
    PyObject* arg2;
    __cs.init ("AllianceFramework.createLibrary");
    if (not PyArg_ParseTuple( args
                            , "O&O&|O&:AllianceFramework.createLibrary"
                            , Converter, &arg0
                            , Converter, &arg1
                            , Converter, &arg2
                            )) {
      PyErr_SetString( ConstructorError, "Invalid number of parameters for AllianceFramework.createLibrary()." );
      return NULL;
    }

    if      (__cs.getObjectIds() == ":string:int"       ) { }
    else if (__cs.getObjectIds() == ":string:int:string") libName = PyString_AsString(arg2);
    else {
      PyErr_SetString( ConstructorError, "Bad parameter type for AllianceFramework.createLibrary()." );
      return NULL;
    }

    alib = af->createLibrary( PyString_AsString(arg0), PyAny_AsLong(arg1), libName );
    if (alib == NULL) Py_RETURN_NONE;
    HCATCH

    return PyAllianceLibrary_Link(alib);
  }


  static PyObject* PyAllianceFramework_wrapLibrary ( PyAllianceFramework* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyAllianceFramework_wrapLibrary()" << endl;

    AllianceLibrary* alib    = NULL;
    HTRY
      METHOD_HEAD("AllianceFramework.wrapLibrary()")
      PyObject* arg0;
      PyObject* arg1;
      __cs.init( "AllianceFramework.wrapLibrary" );
      if (not PyArg_ParseTuple( args
                              , "O&O&:AllianceFramework.wrapLibrary"
                              , Converter, &arg0
                              , Converter, &arg1
                              )) {
        PyErr_SetString( ConstructorError, "AllianceFramework.wrapLibrary(): Takes exactly two arguments." );
        return NULL;
      }
      if (__cs.getObjectIds() != ":library:int") {
        PyErr_SetString( ConstructorError, "AllianceFramework.wrapLibrary(): Bad parameter()s type, must be (Library,int)." );
        return NULL;
      }
      alib = af->wrapLibrary( PYLIBRARY_O(arg0), PyAny_AsLong(arg1) );
      if (not alib) Py_RETURN_NONE;
    HCATCH
    return PyAllianceLibrary_Link(alib);
  }


  static PyObject* PyAllianceFramework_isPad ( PyAllianceFramework* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyAllianceFramework_isPad ()" << endl;

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


  static PyObject* PyAllianceFramework_isRegister ( PyAllianceFramework* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyAllianceFramework_isRegister ()" << endl;
    char* name = NULL;
    HTRY
      METHOD_HEAD("AllianceFramework.isRegister()")
      if ( not PyArg_ParseTuple(args,"s",&name) ) {
        PyErr_SetString ( ConstructorError, "invalid number of parameters for Cell AllianceFramework.isRegister().");
        return NULL;
      }
      if (af->isRegister(name)) Py_RETURN_TRUE;
    HCATCH
    Py_RETURN_FALSE;
  }


  static PyObject* PyAllianceFramework_isCLOCK ( PyAllianceFramework* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyAllianceFramework_isCLOCK ()" << endl;

    char* name = NULL;
    HTRY
      METHOD_HEAD( "AllianceFramework.isCLOCK()" )
      if (not PyArg_ParseTuple(args,"s",&name)) {
        PyErr_SetString ( ConstructorError, "AllianceFramework.isCLOCK(): Invalid number or bad type of parameters.");
        return NULL;
      }
      if (af->isCLOCK(name)) Py_RETURN_TRUE;
    HCATCH
    Py_RETURN_FALSE;
  }


  static PyObject* PyAllianceFramework_getCatalog ( PyAllianceFramework* self )
  {
    cdebug_log(30,0) << "PyAllianceFramework_getCatalog ()" << endl;

    Catalog* catalog = NULL;
    HTRY
      METHOD_HEAD("AllianceFramework.getCatalog()")
      catalog = af->getCatalog();
    HCATCH
    return PyCatalog_Link(catalog);
  }


  static PyObject* PyAllianceFramework_isInCatalog ( PyAllianceFramework* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyAllianceFramework_isInCatalog ()" << endl;

    char* name = NULL;
    HTRY
      METHOD_HEAD("AllianceFramework.isInCatalog()")
      if ( not PyArg_ParseTuple(args,"s",&name) ) {
        PyErr_SetString( ConstructorError, "AllianceFramework.isInCatalog(): Invalid number or bad type of parameters.");
        return NULL;
      }
      if (af->isInCatalog(Name(name))) Py_RETURN_TRUE;
    HCATCH
    Py_RETURN_FALSE;
  }


  static PyObject* PyAllianceFramework_addRoutingGauge ( PyAllianceFramework* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyAllianceFramework_addRoutingGauge ()" << endl;

    HTRY
    METHOD_HEAD("AllianceFramework.addRoutingGauge()")
    PyObject* arg0;
    if ( not ParseOneArg("AllianceFramework.addRoutingGauge", args, ":routGauge", &arg0) )
      return NULL;
    af->addRoutingGauge ( PYROUTINGGAUGE_O(arg0) );
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyAllianceFramework_getRoutingGauge ( PyAllianceFramework* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyAllianceFramework_getRoutingGauge ()" << endl;

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


  static PyObject* PyAllianceFramework_setRoutingGauge ( PyAllianceFramework* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyAllianceFramework_setRoutingGauge()" << endl;

    HTRY
      METHOD_HEAD("AllianceFramework.setRoutingGauge()")
      PyObject* arg0;
      __cs.init ("AllianceFramework.setRoutingGauge");
      if (not PyArg_ParseTuple( args, "O&:AllianceFramework.setRoutingGauge", Converter, &arg0 )) {
        PyErr_SetString( ConstructorError, "Invalid number of parameters for AllianceFramework.setRoutingGauge()." );
        return NULL;
      }
      if (__cs.getObjectIds() == ":string") af->setRoutingGauge( Name(PyString_AsString(arg0)) );
      else {
        PyErr_SetString( ConstructorError, "Bad parameter type for AllianceFramework.setRoutingGauge()." );
        return NULL;
      }
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyAllianceFramework_addCellGauge ( PyAllianceFramework* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyAllianceFramework_addCellGauge ()" << endl;

    HTRY
    METHOD_HEAD("AllianceFramework.addCellGauge()")
    PyObject* arg0;
    if ( not ParseOneArg("AllianceFramework.addCellGauge", args, ":cellGauge", &arg0) )
      return NULL;
    af->addCellGauge ( PYCELLGAUGE_O(arg0) );
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyAllianceFramework_getCellGauge ( PyAllianceFramework* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyAllianceFramework_getCellGauge ()" << endl;

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


  static PyObject* PyAllianceFramework_setCellGauge ( PyAllianceFramework* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyAllianceFramework_setCellGauge()" << endl;

    HTRY
      METHOD_HEAD("AllianceFramework.setCellGauge()")
      PyObject* arg0;
      __cs.init ("AllianceFramework.setCellGauge");
      if (not PyArg_ParseTuple( args, "O&:AllianceFramework.setCellGauge", Converter, &arg0 )) {
        PyErr_SetString( ConstructorError, "Invalid number of parameters for AllianceFramework.setCellGauge()." );
        return NULL;
      }
      if (__cs.getObjectIds() == ":string") af->setCellGauge( Name(PyString_AsString(arg0)) );
      else {
        PyErr_SetString( ConstructorError, "Bad parameter type for AllianceFramework.setCellGauge()." );
        return NULL;
      }
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyAllianceFramework_matchCellGauge ( PyAllianceFramework* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyAllianceFramework_matchCellGauge()" << endl;

    CellGauge* cg = NULL;
    HTRY
      METHOD_HEAD( "AllianceFramework.matchCellGauge()" )
      
      PyObject* arg0;
      PyObject* arg1;
      __cs.init( "AllianceFramework.matchCellGauge" );
      if (not PyArg_ParseTuple( args
                              , "O&O&|O&:AllianceFramework.matchCellGauge"
                              , Converter, &arg0
                              , Converter, &arg1
                              )) {
        PyErr_SetString( ConstructorError, "Invalid number of parameters for AllianceFramework.matchCellGauge()." );
        return NULL;
      }
      
      if (__cs.getObjectIds() == ":int:int") {
        cg = af->matchCellGauge( PyAny_AsLong(arg0), PyAny_AsLong(arg1) );
      } else {
        PyErr_SetString( ConstructorError, "Bad parameter type for AllianceFramework.matchCelGauge()." );
        return NULL;
      }
    HCATCH

    if (cg == NULL) Py_RETURN_NONE;
    return PyCellGauge_Link(cg);
  }


  static PyObject* PyAllianceFramework_loadLibraryCells ( PyAllianceFramework* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyAllianceFramework_loadLibraryCells()" << endl;

    unsigned int count = 0;

    HTRY
    METHOD_HEAD("AllianceFramework.loadLibraryCells()")

    PyObject* arg0;
    __cs.init ("AllianceFramework.loadLibraryCells");
    if (not PyArg_ParseTuple( args, "O&:AllianceFramework.loadLibraryCells", Converter, &arg0)) {
      PyErr_SetString( ConstructorError, "Invalid number of parameters for AllianceFramework.loadLibraryCells()." );
      return NULL;
    }

    if      (__cs.getObjectIds() == STRING_ARG) count = af->loadLibraryCells( Name(PyString_AsString(arg0)) );
    else if (__cs.getObjectIds() == ":library") count = af->loadLibraryCells( PYLIBRARY_O(arg0) );
    else {
      PyErr_SetString( ConstructorError, "Bad parameter type for AllianceFramework.loadLibraryCells()." );
      return NULL;
    }

    HCATCH

    return Py_BuildValue( "I", count );
  }

  
  // Standart Accessors (Attributes).


  // Standart Destroy (Attribute).
  DBoDestroyAttribute(PyAllianceFramework_destroy,PyAllianceFramework)


  PyMethodDef PyAllianceFramework_Methods[] =
    { { "create"               , (PyCFunction)PyAllianceFramework_create               , METH_VARARGS|METH_STATIC
                               , "Gets the Alliance Framework." }                      
    , { "get"                  , (PyCFunction)PyAllianceFramework_get                  , METH_NOARGS|METH_STATIC
                               , "Gets the Alliance Framework." }                      
    , { "getEnvironment"       , (PyCFunction)PyAllianceFramework_getEnvironment       , METH_NOARGS
                               , "Gets the Alliance Environment." }
    , { "getCatalog"           , (PyCFunction)PyAllianceFramework_getCatalog           , METH_NOARGS
                               , "Gets the libraries composite catalog." }
    , { "bindLibraries"        , (PyCFunction)PyAllianceFramework_bindLibraries        , METH_NOARGS
                               , "Bind Alliance libraries to Hurricane one. This is a one-time only methods." }
    , { "getLibrary"           , (PyCFunction)PyAllianceFramework_getLibrary           , METH_VARARGS
                               , "Gets a Library, by index." }                         
    , { "getAllianceLibrary"   , (PyCFunction)PyAllianceFramework_getAllianceLibrary   , METH_VARARGS
                               , "Gets an AllianceLibrary, by index, name or Hurricane Library." }                         
    , { "getAllianceLibraries" , (PyCFunction)PyAllianceFramework_getAllianceLibraries , METH_NOARGS
                               , "Returns the AllianceLibraries vector as a list." }                         
    , { "getCell"              , (PyCFunction)PyAllianceFramework_getCell              , METH_VARARGS
                               , "Gets an Alliance Cell." }                            
    , { "saveCell"             , (PyCFunction)PyAllianceFramework_saveCell             , METH_VARARGS
                               , "Saves an Alliance Cell." }                           
    , { "createCell"           , (PyCFunction)PyAllianceFramework_createCell           , METH_VARARGS
                               , "Create a Cell in the Alliance framework." }
    , { "createLibrary"        , (PyCFunction)PyAllianceFramework_createLibrary        , METH_VARARGS
                               , "Create a Library in the Alliance framework." }
    , { "wrapLibrary"          , (PyCFunction)PyAllianceFramework_wrapLibrary          , METH_VARARGS
                               , "Wrap an Alliance Library around an existing Hurricane Library." }
    , { "loadLibraryCells"     , (PyCFunction)PyAllianceFramework_loadLibraryCells     , METH_VARARGS
                               , "Load in memory all Cells from an Alliance Library." }                           
    , { "isPad"                , (PyCFunction)PyAllianceFramework_isPad                , METH_VARARGS
                               , "Tells if a cell name is a Pad." }
    , { "isRegister"           , (PyCFunction)PyAllianceFramework_isRegister           , METH_VARARGS
                               , "Tells if a cell name is a register (flip-flop)." }
    , { "isCLOCK"              , (PyCFunction)PyAllianceFramework_isCLOCK              , METH_VARARGS
                               , "Tells if a net name matches the clock pattern." }
    , { "isInCatalog"          , (PyCFunction)PyAllianceFramework_isInCatalog          , METH_VARARGS
                               , "Tells if a cell name is referenced in the Catalog." }
    , { "addCellGauge"         , (PyCFunction)PyAllianceFramework_addCellGauge         , METH_VARARGS
                               , "Add a new cell gauge." }
    , { "getCellGauge"         , (PyCFunction)PyAllianceFramework_getCellGauge         , METH_VARARGS
                               , "Get a cell gauge (without a name, return the default)." }          
    , { "setCellGauge"         , (PyCFunction)PyAllianceFramework_setCellGauge         , METH_VARARGS
                               , "Select the default cell gauge." }          
    , { "matchCellGauge"       , (PyCFunction)PyAllianceFramework_matchCellGauge       , METH_VARARGS
                               , "Find the first CellGauge comptible with width and height." }          
    , { "addRoutingGauge"      , (PyCFunction)PyAllianceFramework_addRoutingGauge      , METH_VARARGS
                               , "Add a new routing gauge." }
    , { "getRoutingGauge"      , (PyCFunction)PyAllianceFramework_getRoutingGauge      , METH_VARARGS
                               , "Get a routing gauge (without a name, return the default)." }          
    , { "setRoutingGauge"      , (PyCFunction)PyAllianceFramework_setRoutingGauge      , METH_VARARGS
                               , "Select the default routing gauge." }          
    , { "destroy"              , (PyCFunction)PyAllianceFramework_destroy              , METH_NOARGS
                               , "Destroy the Framework, Hurricane-level objects remains." }
    , {NULL, NULL, 0, NULL}    /* sentinel */
    };


  DBoDeleteMethod(AllianceFramework)
  PyTypeObjectLinkPyType(AllianceFramework)


#else  // End of Python Module Code Part.


// x=================================================================x
// |          "PyAllianceFramework" Shared Library Code Part         |
// x=================================================================x

  // Link/Creation Method.
  DBoLinkCreateMethod(AllianceFramework)
  PyTypeObjectDefinitions(AllianceFramework)


  extern void  PyAllianceFramework_postModuleInit ()
  {
    PyObject* constant;

    LoadObjectConstant(PyTypeAllianceFramework.tp_dict,AllianceFramework::NoPythonInit ,"NoPythonInit" );
    LoadObjectConstant(PyTypeAllianceFramework.tp_dict,AllianceFramework::CreateLibrary,"CreateLibrary");
    LoadObjectConstant(PyTypeAllianceFramework.tp_dict,AllianceFramework::AppendLibrary,"AppendLibrary");
    LoadObjectConstant(PyTypeAllianceFramework.tp_dict,AllianceFramework::HasCatalog   ,"HasCatalog"   );
  }


#endif  // End of Shared Library Code Part.


}  // End of extern "C".


}  // End of CRL namespace.
 
