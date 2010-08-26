
// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2010, All Rights Reserved
//
// This file is part of ISOBAR.
//
// ISOBAR is free software: you can redistribute it  and/or  modify
// it under the terms of the GNU  Lesser  General  Public  License  as
// published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// ISOBAR is distributed in the hope that it will  be  useful,  but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHAN-
// TABILITY or FITNESS FOR A PARTICULAR PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser  GNU  General  Public
// License along with ISOBAR. If not, see
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
// |  C++ Header  :  "./hurricane/isobar/Script.h"                   |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


# ifndef  __ISOBAR_SCRIPT__
# define  __ISOBAR_SCRIPT__

#include <vector>
#include <Python.h>
namespace Hurricane {
  class Cell;
  class CellViewer;
}


namespace Isobar {


  class Script {
    public:
      enum Flags { NoScriptArgs=0x1 };
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
