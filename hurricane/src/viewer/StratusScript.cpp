// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2008-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./StratusScript.cpp"                      |
// +-----------------------------------------------------------------+


#include <Python.h>
#include <iostream>
using namespace std;

#include "hurricane/utilities/Path.h"
#include "hurricane/Warning.h"
#include "hurricane/viewer/StratusScript.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "StratusScript".


  StratusScript::StratusScript ( const std::string& scriptName, CellViewer* editor )
    : _scriptName     (scriptName)
    , _scriptDirectory()
    , _script         (NULL)
  {
    Utilities::Path userStratus   ( _scriptName );
    Utilities::Path userDirectory ( userStratus.dirname() );

    if ( not userDirectory.absolute() )
      userDirectory = Utilities::Path::cwd() / userDirectory;

    _scriptDirectory = userDirectory.string();
    Isobar::Script::addPath ( _scriptDirectory );

    _script = Isobar::Script::create ( userStratus.basename().string() );
    _script->setEditor ( editor );
  }


  StratusScript::~StratusScript ()
  {
    Isobar::Script::removePath ( _scriptDirectory );
    _script->destroy ();
  }


  StratusScript* StratusScript::create  ( const std::string& scriptName, CellViewer* editor )
  {
    return new StratusScript ( scriptName, editor );
  }


  void  StratusScript::destroy ()
  {
    delete this;
  }


  bool  StratusScript::run ()
  {
    return _script->runFunction ( "StratusScript", NULL, Isobar::Script::NoScriptArgs );
  }


} // End of Hurricane namespace.
