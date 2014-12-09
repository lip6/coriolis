
// -*- C++ -*-
//
// This file is part of the VLSI SAPD Software.
// Copyright (c) UPMC 2010-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  V L S I   S A P D                              |
// |             OpenChams Circuit Data Base                         |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Schematic.cpp"                               |
// +-----------------------------------------------------------------+


#include "vlsisapd/openChams/Schematic.h"
#include "vlsisapd/openChams/OpenChamsException.h"
using namespace std;

namespace OpenChams {


  Schematic::Schematic (Circuit* circuit )
    : _circuit(circuit)
  { }

    
  void Schematic::addInstance ( const string& instanceName
                              , double             x
                              , double             y
                              , const string& orient )
  {
    map<string, Schematic::Infos*>::iterator it = _instances.find(instanceName);
    if (it != _instances.end()) {
      string error("[ERROR] Cannot set same instance twice in 'schematic' (");
      error += instanceName;
      error += ").";
      throw OpenChamsException(error);
    }
    _instances[instanceName] = new Schematic::Infos(x, y, orient);
  }

  Schematic::Infos::Infos(double x, double y, const string& orient)
    : _x     (x)
    , _y     (y)
    , _orient(orient)
  {}


} // OpenChams namespace.
