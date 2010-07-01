
// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2009, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify
// it under the terms of the GNU  Lesser  General  Public  License  as
// published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will  be  useful,  but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHAN-
// TABILITY or FITNESS FOR A PARTICULAR PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser  GNU  General  Public
// License along with Hurricane. If not, see
//                                     <http://www.gnu.org/licenses/>.
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Script.cpp"                                  |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include  <sstream>
#include  "hurricane/Error.h"
#include  "hurricane/Cell.h"
#include  "hurricane/isobar/PyCell.h"
#include  "hurricane/isobar/Script.h"


namespace Isobar {

  using std::string;
  using Hurricane::Cell;


// -------------------------------------------------------------------
// Class  :  "Isobar::Script".


  vector<string>  Script::_pathes;


  void  Script::addPath ( const string& path )
  { _pathes.push_back ( path ); }


  Script::Script ( const string& name )
    : _moduleName(name)
    , _sysModule (NULL)
    , _userModule(NULL)
  {
  }


  Script::~Script ()
  {
    _destroyModules();
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


  bool  Script::runFunction ( const std::string& function, Cell* cell )
  {
    bool returnCode = true;

    if ( cell == NULL )
      throw Error("Script::runFunction(): NULL Cell as argument");

    Py_Initialize ();
    _importSys    ();
    _importModule ( "Hurricane" );

    _userModule = PyImport_ImportModule ( const_cast<char*>(_moduleName.c_str()) );

    if ( _userModule == NULL ) {
      if ( PyErr_Occurred() ) {
        PyErr_Print ();
      }
      throw Error("Cannot load python module: <%s>",_moduleName.c_str());
    }

    PyObject* pyFunction = PyObject_GetAttrString(_userModule, const_cast<char*>(function.c_str()));
    if ( (pyFunction == NULL) or not PyCallable_Check(pyFunction) ) {
      _destroyModules ();
      throw Error("Python module <%s> doesn't contains any <%s> function."
                 ,_moduleName.c_str(),function.c_str());
    }

    PyObject* pyArgs  = PyTuple_New(1);
    PyTuple_SetItem ( pyArgs, 0, (PyObject*)PyCell_Link(cell) );

    PyObject* pyResult = PyEval_CallObject ( pyFunction, pyArgs );

    if ( pyResult == NULL ) {
      cerr << "Something has gone slightly wrong" << endl;
    } else
      Py_DECREF ( pyResult );

    if ( PyErr_Occurred() ) {
      PyErr_Print ();
      returnCode = false;
    }

    Py_DECREF ( pyFunction );
    Py_DECREF ( pyArgs );

    _destroyModules ();
    Py_Finalize ();

    return true;
  }


  void  Script::_importSys ()
  {
    _sysModule = _importModule ( "sys" );

    PyObject* path = PyObject_GetAttrString ( _sysModule, "path" );
    if ( path == NULL )
      throw Error("Script::_importSys(): No \"sys.path\" attribute.");

    vector<string>::iterator ipath = _pathes.begin();

    for ( ; ipath != _pathes.end() ; ++ipath ) {
      cerr << "PYTHONPATH:" << (*ipath) << endl;

      PyObject* element = PyString_FromString ( const_cast<char*>((*ipath).c_str()) );
      PyList_Insert ( path, 0, element );
    }

    int size = PySequence_Size(path);
    for ( int i=0 ; i<size ; ++i )
      PyObject* element = PySequence_GetItem(path,i);
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


  void  Script::_destroyModules ()
  {
    if ( not Py_IsInitialized() ) return;

    if ( _userModule != NULL ) Py_DECREF ( _userModule );
    if ( _sysModule  != NULL ) Py_DECREF ( _sysModule );

    _userModule = NULL;
    _sysModule  = NULL;
  }


} // End of Isobar namespace.
