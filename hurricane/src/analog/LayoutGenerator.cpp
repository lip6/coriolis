// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2009-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./LayoutGenerator.cpp"                         |
// +-----------------------------------------------------------------+


#include <Python.h>
#include <unistd.h>
#include <cerrno>
#include <cstdio>
#include <iostream>
#include <fstream>
#include "hurricane/Warning.h"
#include "hurricane/isobar/PyBox.h"
#include "hurricane/viewer/Script.h"
#include "hurricane/analog/Device.h"
#include "hurricane/analog/PyDevice.h"
#include "hurricane/analog/LayoutGenerator.h"


namespace Analog {

  using namespace std;
  using namespace Hurricane;
  using namespace Isobar;


// -------------------------------------------------------------------
// Class  :  "::LayoutGenerator::Logger".

  LayoutGenerator::Logger::Logger ( LayoutGenerator* generator )
    : _generator(generator)
  { }
  
  
  LayoutGenerator::Logger::~Logger ()
  { }
  
  
  void  LayoutGenerator::Logger::popStatus ( const string& text )
  {
    if (_generator->getVerboseLevel() >= LayoutGenerator::Verbose)
      cerr << text << endl;
  }


  void  LayoutGenerator::Logger::popError ( const string& text )
  { cerr << text << endl; }


  void  LayoutGenerator::Logger::popScriptError ()
  {
    string scriptError = "! An error occured while creating layout. Please check the python console for more information.";
    cerr << scriptError << endl;
  }


// -------------------------------------------------------------------
// Class  :  "::LayoutGenerator".

  int  LayoutGenerator::_verboseLevel  = LayoutGenerator::Debug;
  
  
  LayoutGenerator::LayoutGenerator ()
    : _logger   (NULL)
    , _device   (NULL)
    , _box      (NULL)
    , _activeBox(NULL)
    , _matrix   ()
    , _script   (NULL)
  {
    _activeBox = new Hurricane::Box();
    _script    = Script::create();
    setLogger ( new Logger(this) );
  }
  
  
  LayoutGenerator::~LayoutGenerator ()
  {
    if (_script) _script->destroy();
    if (_logger) delete _logger;
  }
  
  
  bool  LayoutGenerator::checkScript()
  {
    if (not _device) {
      popError( "Try to check for script but device does not exist." );
      return false;
    }
  
    FILE* fs = fopen( _device->getLayoutScript().c_str(), "r" );
    if (not fs) {
      perror( NULL );
      popError( "Cannot load script : file does not exist or bad access rights." );
      return false;
    }
  
    fclose( fs );
    popStatus( _device->getLayoutScript()+" found." );
    return true;
  }
  
  
  bool  LayoutGenerator::checkFunctions()
  {
    if (not _script->getFunction("checkCoherency")) {
      cerr << Error( "LayoutGenerator::drawLayout(): Module <%s> miss checkCoherency()."
                   , _script->getUserModuleName().c_str() ) << endl;
      finalize( ShowTimeTag );
      return false;
    }
  
    if (not _script->getFunction("layout")) {
      cerr << Error( "LayoutGenerator::drawLayout(): Module <%s> miss layout()."
                   , _script->getUserModuleName().c_str() ) << endl;
      finalize( ShowTimeTag );
      return false;
    }
  
    return true;
  }
  
  
  bool  LayoutGenerator::drawLayout ()
  {
      if (_device == NULL) return false;
  
    //checkScript();
  
      cdebug_log(500,0) << "LayoutGenerator::drawLayout() " << _device->getDeviceName() << endl;
  
      _device->destroyLayout();
  
      initialize();
  
      if (not _script->getUserModule()) {
        finalize( ShowTimeTag );
        cerr << Error( "LayoutGenerator::drawLayout(): Couldn't load module <%s>"
                     , _script->getUserModuleName().c_str() ) << endl;
        return false;
      }
  
      checkFunctions();
  
      PyObject* pyArgs = NULL;
      if (not toPyArguments(pyArgs,NoFlags)) {
        finalize( ShowTimeTag );
        return false;
      }
      
      if (not callCheckCoherency(pyArgs,ShowError)) {
        return false;
      }
  
      if (not callLayout(pyArgs)) {
        cerr << "Layout failed" << endl; cerr.flush();
        return false;
      }
  
    // Eric passed by here
    //cerr << "Python driven Layout successfully drawn." << endl;
  
      _device->setAbutmentBox( getDeviceBox() );
  
      finalize( ShowTimeTag|StatusOk );
  
    //string message = _device->checkLayoutOnPhysicalGrid(); 
    //if (not message.empty())
    //  popError( message.c_str() );
      
    // Eric passed by here
    //popStatus( "Layout done." );
  
      return true;
  }
  
  
  bool  LayoutGenerator::initialize ()
  {
    string moduleFullPath = _device->getLayoutScript();
    size_t slash          = moduleFullPath.rfind( '/' );
  
    slash = (slash!=string::npos) ? slash+1 : 0;
    string moduleName = moduleFullPath.substr( slash );
    string modulePath = moduleFullPath.substr( 0, slash-1 );
    size_t dot        = moduleName.rfind( '.' );
  
    dot = (dot!=string::npos) ? dot : moduleName.size();
    moduleName = moduleName.substr( 0, dot );
  
  // Eric passed by here
  //cerr << "Path: " << modulePath << endl;
  //cerr << "Name: " << moduleName << endl;
  
    _script->setUserModuleName( moduleName );
    return _script->initialize( Script::NoThrow );
  }
  
  
  void  LayoutGenerator::finalize ( unsigned int flags )
  {
    if (flags & ShowTimeTag) {
      string code = "";
    // Eric passed by here
    //if (flags & StatusOk) code = "import time; print ' -- Script SUCCESS2 --', time.strftime('%H:%M:%S',time.localtime())\n";
      if (flags & StatusOk) code = "";
      else                  code = "import time; print ' -- Script FAILED --' , time.strftime('%H:%M:%S',time.localtime())\n";
      PyRun_SimpleString( code.c_str() );
    }
    _script->finalize();
  }
  
  
  bool  LayoutGenerator::toPyArguments ( PyObject*& pyArgs, unsigned int flags )
  {
    pyArgs= PyTuple_New( 2 );
    PyTuple_SetItem( pyArgs, 0, PyDevice_Link(_device) );
    PyTuple_SetItem( pyArgs, 1, (flags & ComputeBbOnly) ? Py_True : Py_False );
  
    return true;
  }
  
  
  bool  LayoutGenerator::callCheckCoherency ( PyObject* pArgsCheck, unsigned int flags )
  {
    PyObject* pTupleCheck = _script->callFunction( "checkCoherency", pArgsCheck );
    
    if (not pTupleCheck) {
      string code = "print ' -- Script FAILED --', time.strftime('%H:%M:%S',time.localtime())\n";
      PyRun_SimpleString( code.c_str() );
      finalize( NoFlags );
      popScriptError();
      return false;
    }
    if ( not PyTuple_Check(pTupleCheck) or (PyTuple_Size(pTupleCheck) != 2) ) {
      popError( "checkCoherency function must return a tuple: (bool,errorMessage)" );
      return false;
    }
    PyObject* pCheckOk = PyTuple_GetItem( pTupleCheck, 0 );
    if (pCheckOk == Py_False) {
      if (flags & ShowError)
        popError( string(PyString_AsString(PyTuple_GetItem(pTupleCheck,1))) );
      return false;
    }
    return true;
  }
  
  
  bool  LayoutGenerator::callLayout ( PyObject* pArgs )
  {
    _matrix = _script->callFunction( "layout", pArgs );
    if (not _matrix) {
      string code = "print ' -- Script FAILED --', time.strftime('%H:%M:%S',time.localtime())";
      PyRun_SimpleString( code.c_str() );
      finalize( NoFlags );
      popScriptError();
      cerr << "There was a problem running layout function" << endl;
      return false;
    }
    return true;
  }
  
  
  unsigned  LayoutGenerator::getNumberTransistor ()
  {
    PyObject* row = getRow(0);
    if (row and PyList_Check(row) == 1)
      return PyList_Size(row) - 1; // -1 because of the first column for global params
    return 0; // else return 0;
  }
  
  
  unsigned  LayoutGenerator::getNumberStack ()
  {
    if (_matrix and PyList_Check(_matrix) == 1)
      return PyList_Size(_matrix) - 1; // -1 because of the first row for global params
    return 0;
  }
  
  
  Box  LayoutGenerator::getDeviceBox ()
  {
    PyObject* pBox = getParamValue(getDic(getRow(0), 0), "box");
    if (pBox == NULL) {
  	finalize( NoFlags );
      popError("Layout function did not returned a valid device box 2!");
  	return Box();
    }
  
    if (pBox->ob_type != &PyTypeBox) {
      finalize( NoFlags );
      popError("Layout function did not returned a valid device box!");
    }
    return *((PyBox*)pBox)->_object; //get the hurricane box
  }
  
  
  Box  LayoutGenerator::getActiveBox ()
  {
    PyObject* pBox = getParamValue(getDic(getRow(0), 0), "globalActiveBox");
    if (!pBox) {
  	finalize( NoFlags );
      popError("Layout function did not returned a valid active box 2!");
  	return Box();
    }
  
    if (pBox->ob_type != &PyTypeBox) {
      finalize( NoFlags );
      popError("Layout function did not returned a valid active box!");
    }
    return *((PyBox*)pBox)->_object; //get the hurricane box
  }
  
  
  double  LayoutGenerator::getParameterValue ( unsigned i, unsigned j, string paramName, bool& ok )
  {
    PyObject* pValue = getParamValue(getDic(getRow(i),j), paramName);
    if (pValue == NULL){
      ok = false;
      return 0.0;     
    }
    ok = true; 
    return (PyFloat_AsDouble(pValue));
  }
  
  
  PyObject* LayoutGenerator::getRow ( unsigned i )
  {
    if (_matrix and PyList_Check(_matrix) == 1 and ((int)i < PyList_Size(_matrix))){
      return PyList_GetItem(_matrix, i);
    } 
    return NULL; 
  }
  
  
  PyObject* LayoutGenerator::getDic ( PyObject* row, unsigned j )
  {
    if (row and PyList_Check(row) == 1 and ((int)j < PyList_Size(row))){
      return PyList_GetItem(row, j);
    }
    return NULL;
  }
  
  
  PyObject* LayoutGenerator::getParamValue ( PyObject* dic, string paramName )
  {
    if (dic and (PyDict_Check(dic) == 1) and (PyDict_Contains(dic,PyString_FromString(paramName.c_str()))))
      return PyDict_GetItemString(dic,paramName.c_str());
    return NULL;
  }


}  // Analog namespace.
