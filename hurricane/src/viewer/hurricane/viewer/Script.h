
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2012, All Rights Reserved
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


# ifndef  __ISOBAR_SCRIPT__
# define  __ISOBAR_SCRIPT__

#include <vector>

namespace Hurricane {
  class Cell;
  class CellViewer;
}


namespace Isobar {


  class Script {
    public:
      enum   Flags     { NoScriptArgs=0x1, Initialized=0x10 };
    public:
      static void      addPath            ( const std::string& path );
      static void      removePath         ( const std::string& path );
      static Script*   create             ( const std::string& name );
             void      destroy            ();
      inline PyObject* getSysModule       ();
      inline PyObject* getHurricaneModule ();
      inline PyObject* getUserModule      ();
             void      setEditor          ( Hurricane::CellViewer* );
             bool      runFunction        ( const std::string& function, Hurricane::Cell* cell, unsigned int flags=0 );
    protected:
      static std::vector<std::string>  _pathes;
             std::string               _moduleName;
             PyObject*                 _sysModule;
             PyObject*                 _hurricaneModule;
             PyObject*                 _userModule;
             PyObject*                 _pyFunction;
             PyObject*                 _pyArgs;
             PyObject*                 _pyResult;
             Hurricane::CellViewer*    _cellViewer;
             PyThreadState*            _globalState;
             PyThreadState*            _subInterpreter;
             long                      _flags;
    protected:                            
                Script           ( const std::string& name );
               ~Script           ();
                Script           ( const Script& );
      Script&   operator=        ( const Script& );
      void      _importSys       ();
      void      _importHurricane ();
      PyObject* _importModule    ( const std::string& );
      void      _initialize      ();
      void      _finalize        ();
      void      _setEditor       ();
  };


// Inline Methods.
  inline PyObject* Script::getSysModule       () { return _sysModule; }
  inline PyObject* Script::getHurricaneModule () { return _hurricaneModule; }
  inline PyObject* Script::getUserModule      () { return _userModule; }


} // End of Isobar namespace.


# endif // __ISOBAR_SCRIPT__
