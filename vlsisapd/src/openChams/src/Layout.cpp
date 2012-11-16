
// -*- C++ -*-
//
// This file is part of the VLSI SAPD Software.
// Copyright (c) UPMC/LIP6 2008-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  V L S I   S A P D                              |
// |             OpenChams Circuit Data Base                         |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Layout.cpp"                                  |
// +-----------------------------------------------------------------+


#include "vlsisapd/openChams/Layout.h"
#include "vlsisapd/openChams/Circuit.h"
#include "vlsisapd/openChams/OpenChamsException.h"
using namespace std;


namespace OpenChams {


  Layout::Layout ( Circuit* circuit )
    : _circuit   (circuit)
    , _hbTreeRoot(NULL)
    , _instances ()
  { }


  void Layout::addInstance ( const string& name, const string& style )
  {
    map<std::string, std::string>::iterator it = _instances.find(name);
    if (it != _instances.end()) {
      string error("[ERROR] Cannot set several instances with the same name in 'layout' (");
      error += name;
      error += ").";
      throw OpenChamsException(error);
    }
    _instances[name] = style;
  }
    

} // OpenChams namespace.
