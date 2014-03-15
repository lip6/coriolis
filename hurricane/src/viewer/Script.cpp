
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2012, All Rights Reserved
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


#include  <Python.h>
#include  <sstream>
#include  "hurricane/Error.h"
#include  "hurricane/Cell.h"
#include  "hurricane/isobar/PyCell.h"
#include  "hurricane/viewer/PyCellViewer.h"
#include  "hurricane/viewer/Script.h"


namespace {

  const char* __editorKw = "__editor";

} // End of anonymous namespace.


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
    : _moduleName     (name)
    , _sysModule      (NULL)
    , _hurricaneModule(NULL)
    , _userModule     (NULL)
    , _pyFunction     (NULL)
    , _pyArgs         (NULL)
    , _pyResult       (NULL)
    , _cellViewer     (NULL)
    , _globalState    (NULL)
    , _subInterpreter (NULL)
    , _flags          (0)
  { }


  Script::~Script ()
  {
    _finalize();
  //Py_Finalize ();
  }


  Script* Script::create ( const std::string& name )
  {
    Script* script = new Script ( name );
    return script;
  }


  void  Script::destroy ()
  {
    delete this;
  }


  void  Script::setEditor ( CellViewer* viewer )
  { _cellViewer = viewer; }


  bool  Script::runFunction ( const std::string& function, Cell* cell, unsigned int flags )
  {
    bool returnCode = true;

    _initialize ();

    _userModule = PyImport_ImportModule ( const_cast<char*>(_moduleName.c_str()) );

    if ( _userModule == NULL ) {
      if ( PyErr_Occurred() ) {
        PyErr_Print ();
      }
      _finalize ();
      throw Error("Cannot load python module: <%s>",_moduleName.c_str());
    }

    _setEditor ();

    _pyFunction = PyObject_GetAttrString(_userModule, const_cast<char*>(function.c_str()));
    if ( (_pyFunction == NULL) or not PyCallable_Check(_pyFunction) ) {
      _finalize ();
      throw Error("Python module <%s> doesn't contains any <%s> function."
                 ,_moduleName.c_str(),function.c_str());
    }

    _pyArgs  = PyTuple_New(1);
    if ( cell != NULL ) PyTuple_SetItem ( _pyArgs, 0, (PyObject*)PyCell_Link(cell) );
    else                PyTuple_SetItem ( _pyArgs, 0, Py_None );

    _pyResult = PyEval_CallObject ( _pyFunction, (flags&NoScriptArgs) ? NULL : _pyArgs );

    if ( _pyResult == NULL ) {
      cerr << "Something has gone slightly wrong" << endl;
    }

    if ( PyErr_Occurred() ) {
      PyErr_Print ();
      returnCode = false;
    }

    _finalize();

    return returnCode;
  }


  void  Script::_importSys ()
  {
    if ( not Py_IsInitialized() )
      throw Error ( "Script::_importSys(): Called before Py_Initialize()." );

    _sysModule = _importModule ( "sys" );

    PyObject* path = PyObject_GetAttrString ( _sysModule, "path" );
    if ( path == NULL )
      throw Error("Script::_importSys(): No \"sys.path\" attribute.");

    vector<string>::iterator ipath = _pathes.begin();

    for ( ; ipath != _pathes.end() ; ++ipath ) {
      PyObject* element = PyString_FromString ( const_cast<char*>((*ipath).c_str()) );
      PyList_Insert ( path, 0, element );
    }
  }


  void  Script::_importHurricane ()
  {
    if ( not Py_IsInitialized() )
      throw Error ( "Script::_importHurricane(): Called before Py_Initialize()." );

    _hurricaneModule = PyImport_ImportModule ( "Hurricane" );
    if ( _hurricaneModule == NULL )
      throw Error("Script::_importModule(): No Hurricane module.");

    PyModule_AddObject ( _hurricaneModule, const_cast<char*>(__editorKw), Py_None );
  }


  PyObject* Script::_importModule ( const string& moduleName )
  {
    if ( not Py_IsInitialized() )
      throw Error ( "Script::_importModule(): Called before Py_Initialize() while importing <%s>."
                  , moduleName.c_str() );

    PyObject* module = PyImport_ImportModule ( const_cast<char*>(moduleName.c_str()) );
    if ( module == NULL )
      throw Error("Script::_importModule(): No <%s> module.",moduleName.c_str());

    return module;
  }


  void  Script::_initialize ()
  {
    if (Py_IsInitialized()) {
    // Python is already running. Launch a sub-interpreter.
      _globalState    = PyThreadState_Get();
      _subInterpreter = Py_NewInterpreter();
    } else 
      Py_Initialize();

    _flags |= Initialized;
    _importSys      ();
    _importHurricane();
  }


  void  Script::_finalize ()
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
      
      Py_Finalize ();
      
      _userModule       = NULL;
      _sysModule        = NULL;
      _hurricaneModule  = NULL;
      _pyFunction       = NULL;
      _pyArgs           = NULL;
      _pyResult         = NULL;
    }
  }


  void  Script::_setEditor ()
  {
    if ( _userModule == NULL ) return;

    PyObject* dictionary = PyModule_GetDict ( _userModule );

    if ( _cellViewer != NULL ) {
      PyCellViewer* pyCellViewer = PyObject_NEW ( PyCellViewer, &PyTypeCellViewer );
      pyCellViewer->_object = _cellViewer;

      PyDict_SetItemString ( dictionary, __editorKw, (PyObject*)pyCellViewer );
    } else {
      PyDict_SetItemString ( dictionary, __editorKw, Py_None );
    }
  }


} // End of Isobar namespace.
