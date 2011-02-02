
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2011, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// +-----------------------------------------------------------------+ 
// |                                                                 |
// |                   C O R I O L I S                               |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./StratusScript.cpp"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// +-----------------------------------------------------------------+


#include  <iostream>
using namespace std;

#include  <boost/filesystem/operations.hpp>
namespace bfs = boost::filesystem;

#include  "hurricane/Warning.h"
#include  "hurricane/viewer/StratusScript.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "StratusScript".


  StratusScript::StratusScript ( const std::string& scriptName, CellViewer* editor )
    : _scriptName     (scriptName)
    , _scriptDirectory()
    , _script         (NULL)
  {
    bfs::path userStratus   = _scriptName;
    bfs::path userDirectory = userStratus.branch_path();

    if ( not userDirectory.is_complete() )
      userDirectory = bfs::current_path() / userDirectory;
    userDirectory.normalize();

    _scriptDirectory = userDirectory.string();
    Isobar::Script::addPath ( _scriptDirectory );

    _script = Isobar::Script::create ( userStratus.leaf() );
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
