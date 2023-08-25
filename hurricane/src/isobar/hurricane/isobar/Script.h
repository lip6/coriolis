// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/viewer/Script.h"                   |
// +-----------------------------------------------------------------+


#if !defined(__PYTHON_MODULE__)

#ifndef  ISOBAR_SCRIPT_H
#define  ISOBAR_SCRIPT_H

#include <vector>

extern "C" {
  struct  _object;
  typedef _object  PyObject;
}

namespace Hurricane {
  class Cell;
  class CellViewer;
}


namespace Isobar {


  class Script {
    public:
      enum   Flags     { NoFlags      = 0x0000
                       , NoScriptArgs = 0x0001
                       , Initialized  = 0x0002
                       , NoThrow      = 0x0004
                       };
    public:
      static void         addPath            ( const std::string& path );
      static void         removePath         ( const std::string& path );
      static Script*      create             ( const std::string& name="" );
             void         destroy            ();
      inline std::string  getUserModuleName  () const;
      inline std::string  getFileName        () const;
      inline PyObject*    getSysModule       ();
      inline PyObject*    getHurricaneModule ();
      inline PyObject*    getUserModule      ();
             bool         addInitModuleName  ( std::string );
             void         setUserModuleName  ( const std::string& );
             PyObject*    getFunction        ( std::string function );
             bool         runFunction        ( const std::string& function, Hurricane::Cell* cell, unsigned int flags=0 );
             PyObject*    callFunction       ( const std::string& function, PyObject* pyArgs );
             void         addKwArgument      ( const char* key, PyObject* object );
             bool         initialize         ( unsigned int flags=NoFlags );
             void         finalize           ();
    protected:
      static std::vector<std::string>  _pathes;
             std::vector<std::string>  _initModuleNames;
             std::vector<PyObject*>    _initModules;
             std::string               _moduleName;
             PyObject*                 _sysModule;
             PyObject*                 _hurricaneModule;
             PyObject*                 _userModule;
             PyObject*                 _pyFunction;
             PyObject*                 _pyArgs;
             PyObject*                 _pyKw;
             PyObject*                 _pyResult;
             PyThreadState*            _globalState;
             PyThreadState*            _subInterpreter;
             long                      _flags;
    protected:                            
                       Script           ( const std::string& name );
                      ~Script           ();
                       Script           ( const Script& );
             Script&   operator=        ( const Script& );
             PyObject* _importSys       ( unsigned int flags );
      inline PyObject* _importHurricane ( unsigned int flags );
      inline PyObject* _importUser      ( unsigned int flags );
             PyObject* _importModule    ( const std::string&, unsigned int flags );
  };


// Inline Methods.
  inline std::string  Script::getUserModuleName  () const { return _moduleName; }
  inline PyObject*    Script::getSysModule       () { return _sysModule; }
  inline PyObject*    Script::getHurricaneModule () { return _hurricaneModule; }
  inline PyObject*    Script::getUserModule      () { return _userModule; }

  inline std::string  Script::getFileName () const
  {
    if (not _userModule) return getUserModuleName(); 
    PyObject* pyBytes = PyUnicode_AsASCIIString( PyModule_GetFilenameObject(_userModule) ); 
    std::string fileName = PyBytes_AsString( pyBytes );
    Py_DECREF( pyBytes );
    return fileName;
  }

  inline PyObject* Script::_importHurricane ( unsigned int flags )
  { return _hurricaneModule = _importModule("coriolis.Hurricane",flags); }

  inline PyObject* Script::_importUser ( unsigned int flags )
  { return _userModule = _importModule(_moduleName,flags); }


} // Isobar namespace.

#endif // ISOBAR_SCRIPT_H

#endif
