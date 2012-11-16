
// -*- C++ -*-
//
// This file is part of the VLSI SAPD Software.
// Copyright (c) UPMC/LIP6 2009-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  V L S I   S A P D                              |
// |             OpenChams Circuit Data Base                         |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Parameters.cpp"                              |
// +-----------------------------------------------------------------+


#include <iostream>
using namespace std;

#include "vlsisapd/openChams/Parameters.h"
#include "vlsisapd/openChams/OpenChamsException.h"

namespace OpenChams {
    

  void  Parameters::addParameter ( const string& name, const string& value )
  {
	map<string,string>::iterator it = _params.find(name);
    if ( it != _params.end() ) {
      string error("[ERROR] Cannot addParameter ");
      error += name;
      error += " because it already exists !";
      throw OpenChamsException(error);
    }
    _params[name] = value;
  }


  const string& Parameters::getValue ( const string& name )
  {
    map<string,string>::iterator it = _params.find(name);
    if (it == _params.end()) {
      string error("[ERROR] No parameters named ");
      error += name;
      throw OpenChamsException(error);
    }
    return (*it).second;
  }


} // OpenChams namespace.

