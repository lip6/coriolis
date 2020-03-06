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
// Class  :  "::LayoutGenerator".

  int  LayoutGenerator::_verboseLevel  = LayoutGenerator::Debug;
  
  
  LayoutGenerator::LayoutGenerator ()
    : _device   (NULL)
    , _box      (NULL)
    , _activeBox(NULL)
    , _matrix   ()
    , _script   (NULL)
  {
    _activeBox = new Hurricane::Box();
    _script    = Script::create();
  }
  
  
  LayoutGenerator::~LayoutGenerator ()
  {
    if (_script) _script->destroy();
  }
  
  
  bool  LayoutGenerator::checkScript()
  {
    if (not _device) {
      cerr << Error( "LayoutGenerator::checkScript(): No device loaded in the generator yet." ) << endl;
      return false;
    }
  
    FILE* fs = fopen( _device->getLayoutScript().c_str(), "r" );
    if (not fs) {
      perror( NULL );
      cerr << Error( "LayoutGenerator::checkScript(): Unable to find/open layout script of device \"%s\".\n"
                     "(\"%s\")"
                   , getString(_device->getName()).c_str()
                   , _device->getLayoutScript().c_str()
                   );
      return false;
    }
  
    fclose( fs );
    return true;
  }
  
  
  bool  LayoutGenerator::checkFunctions()
  {
    if (not _script->getFunction("checkCoherency")) {
      cerr << Error( "LayoutGenerator::drawLayout(): Module \"%s\" miss checkCoherency() function."
                   , _script->getUserModuleName().c_str() ) << endl;
      finalize( ShowTimeTag );
      return false;
    }
  
    if (not _script->getFunction("layout")) {
      cerr << Error( "LayoutGenerator::drawLayout(): Module \"%s\" miss layout() function."
                   , _script->getUserModuleName().c_str() ) << endl;
      finalize( ShowTimeTag );
      return false;
    }
  
    return true;
  }
  
  
  bool  LayoutGenerator::drawLayout ()
  {
    if (_device == NULL) return false;
  
    cdebug_log(500,0) << "LayoutGenerator::drawLayout() " << _device->getDeviceName() << endl;
  
    _device->destroyLayout();
  
    initialize();
  
    if (not _script->getUserModule()) {
      finalize( ShowTimeTag );
      cerr << Error( "LayoutGenerator::drawLayout(): Couldn't load module \"%s\"."
                   , _script->getUserModuleName().c_str() ) << endl;
      return false;
    }
  
    checkFunctions();
  
    PyObject* pyArgs = NULL;
    if (not toPyArguments(pyArgs,NoFlags)) {
      finalize( ShowTimeTag );
      return false;
    }
      
    if (not callCheckCoherency(pyArgs,ShowError)) return false;
    if (not callLayout        (pyArgs)          ) return false;
  
    _device->setAbutmentBox( getDeviceBox() );
  
    finalize( ShowTimeTag|StatusOk );
  
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
  
    _script->setUserModuleName( moduleName );
    return _script->initialize( Script::NoThrow );
  }
  
  
  void  LayoutGenerator::finalize ( unsigned int flags )
  {
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
      cerr << Error( "LayoutGenerator::callCheckCoherency(): An exception may have occured in checkCoherency().\n"
                     "        (\"%s\")"
                   , _script->getFileName()
                   ) << endl;
      finalize( NoFlags );
      return false;
    }

    if ( not PyTuple_Check(pTupleCheck) or (PyTuple_Size(pTupleCheck) != 2) ) {
      cerr << Error( "LayoutGenerator::callCheckCoherency(): checkCoherency() did not return a tuple (bool,str).\n"
                     "        (\"%s\")"
                   , _script->getFileName()
                   ) << endl;
      return false;
    }

    PyObject* pCheckOk = PyTuple_GetItem( pTupleCheck, 0 );
    if (pCheckOk == Py_False) {
      if (flags & ShowError)
        cerr << Error( "%s\n        (\"%s\")"
                     , PyString_AsString(PyTuple_GetItem(pTupleCheck,1))
                     , _script->getFileName()
                     ) << endl;
      return false;
    }
    if (not pCheckOk) {
        cerr << Error( "%s\n        A Python exception has occurred."
                     , _script->getFileName()
                     ) << endl;
      return false;
    }
    return true;
  }
  
  
  bool  LayoutGenerator::callLayout ( PyObject* pArgs )
  {
    _matrix = _script->callFunction( "layout", pArgs );
    if (not _matrix) {
      cerr << Error( "LayoutGenerator::callLayout(): An exception may have occured in layout().\n"
                     "        (\"%s\")"
                   , _script->getFileName()
                   ) << endl;
      finalize( NoFlags );
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
    PyObject* pyBox = getParamValue(getDic(getRow(0), 0), "box");
    if (not pyBox) {
      finalize( NoFlags );
      cerr << Error( "LayoutGenerator::getDeviceBox(): No \"box\" key in returned dictionary.\n"
                     "        (\"%s\")"
                   , _script->getFileName()
                   ) << endl;
      return Box();
    }
  
    if (not IsPyBox(pyBox)) {
      cerr << Error( "LayoutGenerator::getDeviceBox(): Value associated with \"box\" key is *not* of Box type.\n"
                     "        (\"%s\")"
                   , _script->getFileName()
                   ) << endl;
      finalize( NoFlags );
      return Box();
    }

    return *PYBOX_O(pyBox);
  }
  
  
  Box  LayoutGenerator::getActiveBox ()
  {
    PyObject* pyBox = getParamValue( getDic(getRow(0),0), "globalActiveBox" );
    if (not pyBox) {
      cerr << Error( "LayoutGenerator::getDeviceBox(): No \"globalActiveBox\" key in returned dictionary.\n"
                     "        (\"%s\")"
                   , _script->getFileName()
                   ) << endl;
      finalize( NoFlags );
      return Box();
    }
  
    if (not IsPyBox(pyBox)) {
      cerr << Error( "LayoutGenerator::getDeviceBox(): Value associated with \"globalActiveBox\" key is *not* of Box type.\n"
                     "        (\"%s\")"
                   , _script->getFileName()
                   ) << endl;
      finalize( NoFlags );
    }

    return *PYBOX_O(pyBox);
  }
  
  
  double  LayoutGenerator::getParameterValue ( unsigned i, unsigned j, string paramName, bool& found )
  {
    PyObject* pyValue = getParamValue( getDic(getRow(i),j), paramName );
    if (not pyValue){
      found = false;
      return 0.0;     
    }
    found = true; 
    return PyFloat_AsDouble( pyValue );
  }
  
  
  PyObject* LayoutGenerator::getRow ( unsigned i )
  {
    if (_matrix and (PyList_Check(_matrix) == 1) and ((int)i < PyList_Size(_matrix))) {
      return PyList_GetItem( _matrix, i );
    } 
    return NULL; 
  }
  
  
  PyObject* LayoutGenerator::getDic ( PyObject* row, unsigned j )
  {
    if (row and (PyList_Check(row) == 1) and ((int)j < PyList_Size(row))) {
      return PyList_GetItem( row, j );
    }
    return NULL;
  }
  
  
  PyObject* LayoutGenerator::getParamValue ( PyObject* dict, string paramName )
  {
    if (dict
       and (PyDict_Check(dict) == 1)
       and (PyDict_Contains(dict,PyString_FromString(paramName.c_str()))) )
      return PyDict_GetItemString( dict, paramName.c_str() );
    return NULL;
  }


}  // Analog namespace.
