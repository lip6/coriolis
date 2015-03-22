// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./hurricane/viewer/Script.cpp"                 |
// +-----------------------------------------------------------------+


#if !defined(__PYTHON_MODULE__)

#include  <Python.h>
#include  <sstream>
#include  "hurricane/Error.h"
#include  "hurricane/Cell.h"
#include  "hurricane/isobar/PyCell.h"
#include  "hurricane/viewer/PyCellViewer.h"
#include  "hurricane/viewer/Script.h"


namespace Isobar {

  using std::string;
  using Hurricane::Cell;
  using Hurricane::CellViewer;


// -------------------------------------------------------------------
// Class  :  "Isobar::Script".


  vector<string>  Script::_pathes;


  void  Script::addPath ( const string& path )
  { _pathes.push_back ( path ); }


  void  Script::removePath ( const string& path )
  {
    vector<string>::iterator ipath = _pathes.begin();
    for ( ; ipath != _pathes.end() ; ++ipath ) {
      if ( (*ipath) == path ) {
        _pathes.erase ( ipath );
        return;
      }
    }
  }


  Script::Script ( const string& name )
    : _initModuleNames()
    , _initModules    ()
    , _moduleName     (name)
    , _sysModule      (NULL)
    , _hurricaneModule(NULL)
    , _userModule     (NULL)
    , _pyFunction     (NULL)
    , _pyArgs         (NULL)
    , _pyKw           (NULL)
    , _pyResult       (NULL)
    , _globalState    (NULL)
    , _subInterpreter (NULL)
    , _flags          (0)
  { }


  Script::~Script ()
  { finalize(); }


  Script* Script::create ( const std::string& name )
  {
    Script* script = new Script ( name );
    return script;
  }


  void  Script::destroy ()
  {
    delete this;
  }


  void  Script::setUserModuleName ( const std::string& name )
  {
    finalize();
    _moduleName = name;
  }


  bool  Script::addInitModuleName ( std::string name )
  {
    for ( auto moduleName : _initModuleNames ) {
      if (name == moduleName) return false;
    }
    _initModuleNames.push_back( name );
    return true;
  }


  PyObject* Script::getFunction ( std::string function )
  {
    if (not _userModule) return NULL;

    PyObject* pyFunction = PyObject_GetAttrString( _userModule, const_cast<char*>(function.c_str()) );
    if ( (pyFunction == NULL) or not PyCallable_Check(pyFunction) )
      return NULL;

    return pyFunction;
  }


  bool  Script::runFunction ( const std::string& function, Cell* cell, unsigned int flags )
  {
    bool returnCode = true;

    initialize();

    _pyFunction = PyObject_GetAttrString( _userModule, const_cast<char*>(function.c_str()) );
    if ( (_pyFunction == NULL) or not PyCallable_Check(_pyFunction) ) {
      finalize();
      throw Error( "Python module %s doesn't contains any %s function."
                 , _moduleName.c_str(),function.c_str());
    }

    _pyArgs   = PyTuple_New( 0 );
    _pyResult = PyObject_Call( _pyFunction, _pyArgs, _pyKw );
    _pyArgs   = NULL;
    _pyKw     = NULL;

    if (_pyResult == NULL) {
      cerr << "Something has gone slightly wrong" << endl;
    }

    if (PyErr_Occurred()) {
      PyErr_Print();
      returnCode = false;
    }

    finalize();

    return returnCode;
  }


  PyObject* Script::callFunction ( const std::string& function, PyObject* pyArgs )
  {
    if (_userModule == NULL) {
      if (PyErr_Occurred()) PyErr_Print();
      return NULL;
    }

    _pyFunction = PyObject_GetAttrString( _userModule, const_cast<char*>(function.c_str()) );
    if ( (_pyFunction == NULL) or not PyCallable_Check(_pyFunction) ) {
      cerr << Error( "Python module %s doesn't contains any %s function."
                   , _moduleName.c_str(),function.c_str()) << endl;
      return NULL;
    }

    _pyResult = PyObject_Call( _pyFunction, pyArgs, NULL );

    if (_pyResult == NULL) {
      cerr << "Something has gone slightly wrong" << endl;
      return NULL;
    }

    if (PyErr_Occurred()) PyErr_Print();

    return _pyResult;
  }


  PyObject* Script::_importSys ( unsigned int flags )
  {
    _sysModule = _importModule( "sys", flags );

    PyObject* path = PyObject_GetAttrString( _sysModule, "path" );
    if (path == NULL) {
      if (flags & NoThrow) return NULL;
      throw Error("Script::_importSys(): No \"sys.path\" attribute.");
    }

    vector<string>::iterator ipath = _pathes.begin();

    for ( ; ipath != _pathes.end() ; ++ipath ) {
      PyObject* element = PyString_FromString( const_cast<char*>((*ipath).c_str()) );
      PyList_Insert( path, 0, element );
    }

    return _sysModule;
  }


  PyObject* Script::_importModule ( const string& moduleName, unsigned int flags )
  {
    if (not Py_IsInitialized()) {
      if (flags & NoThrow) return NULL;

      throw Error( "Script::_importModule(): Called before Py_Initialize() while importing <%s>."
                 , moduleName.c_str() );
    }

    PyObject* module = PyImport_ImportModule( const_cast<char*>(moduleName.c_str()) );
    if (module == NULL) {
      if (PyErr_Occurred()) {
        PyErr_Print();
      }
      if (flags & NoThrow) return NULL;

      throw Error( "Script::_importModule(): Unable to load module <%s>.",moduleName.c_str() );
    }

    return module;
  }


  bool  Script::initialize ( unsigned int flags )
  {
    if (Py_IsInitialized()) {
    // Python is already running. Launch a sub-interpreter.
      _globalState    = PyThreadState_Get();
      _subInterpreter = Py_NewInterpreter();
    } else 
      Py_Initialize();

    if (not Py_IsInitialized())  {
      if (flags & NoThrow) return false;

      throw Error( "Script::initialize(): Unable to initialize the Python interpreter (<%s>)."
                 , _moduleName.c_str());
    }

    _flags |= Initialized;
    _importSys      ( flags );

    for ( auto moduleName : _initModuleNames )
      _initModules.push_back( _importModule(moduleName,flags) );

    _importHurricane( flags );
    _importUser     ( flags );

    return true;
  }


  void  Script::finalize ()
  {
    if (not Py_IsInitialized())     return;
    if (not (_flags & Initialized)) return;
    _flags &= ~Initialized;

    if (_subInterpreter != NULL) {
      Py_EndInterpreter(_subInterpreter);
      PyThreadState_Swap(_globalState);
      _subInterpreter = NULL;
    } else {
      if ( _userModule      != NULL ) { Py_DECREF ( _userModule ); }
      if ( _hurricaneModule != NULL ) { Py_DECREF ( _hurricaneModule ); }
      if ( _sysModule       != NULL ) { Py_DECREF ( _sysModule ); }
      if ( _pyResult        != NULL ) { Py_DECREF ( _pyResult ); }
      if ( _pyArgs          != NULL ) { Py_DECREF ( _pyArgs ); }
      if ( _pyFunction      != NULL ) { Py_DECREF ( _pyFunction ); }
      
      for ( size_t i=0 ; i<_initModules.size() ; ++i ) {
        if (_initModules[i]) {
          Py_DECREF( _initModules[i] );
          _initModules[i] = NULL;
        }
      }
      _initModules.clear();

      Py_Finalize ();
      
      _userModule       = NULL;
      _sysModule        = NULL;
      _hurricaneModule  = NULL;
      _pyFunction       = NULL;
      _pyArgs           = NULL;
      _pyResult         = NULL;
    }
  }


  void  Script::addKwArgument ( const char* key, PyObject* object )
  {
    if (_pyKw == NULL) _pyKw = PyDict_New();

    PyObject* pyKey = PyString_FromString( key );
    if (PyDict_Contains(_pyKw,pyKey) == 1) {
      cerr << Error( "Script::addKwArgument(): Attempt to add twice key %s (nothing done)."
                   , key ) << endl;
      return;
    }

    PyDict_SetItem( _pyKw, pyKey, object );
  }


} // End of Isobar namespace.

#endif
