
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2010, All Rights Reserved
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
// |  C++ Header  :  "./hurricane/viewer/StratusScript.h"            |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef  __VIEWER_STRATUS_SCRIPT__
#define  __VIEWER_STRATUS_SCRIPT__

#include  "hurricane/viewer/Script.h"


namespace Hurricane {


  class StratusScript {
    public:
      static StratusScript* create  ( const std::string& scriptName, CellViewer* editor=NULL );
             bool           run     ();
             void           destroy ();
    private:
      std::string     _scriptName;
      std::string     _scriptDirectory;
      Isobar::Script* _script;
    private:
       StratusScript ( const std::string& scriptName, CellViewer* editor );
      ~StratusScript ();
  };


} // End of Hurricane namespace.


#endif  // __VIEWER_STRATUS_SCRIPT__
