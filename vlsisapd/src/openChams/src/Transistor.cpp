
// -*- C++ -*-
//
// This file is part of the VLSI SAPD Software.
// Copyright (c) UPMC/LIP6 2010-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  V L S I   S A P D                              |
// |             OpenChams Circuit Data Base                         |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Transistor.cpp"                              |
// +-----------------------------------------------------------------+


#include <iostream>
using namespace std;

#include "vlsisapd/openChams/Transistor.h"
#include "vlsisapd/openChams/Instance.h"
#include "vlsisapd/openChams/Net.h"
#include "vlsisapd/openChams/OpenChamsException.h"


namespace OpenChams {


  Transistor::Transistor(const std::string& name, Instance* instance)
    : _name    (name)
    , _gate    ("")
    , _source  ("")
    , _drain   ("")
    , _bulk    ("")
    , _instance(instance)
  { }


  void Transistor::setGate(const std::string& gate)
  { if (checkConnector(gate)) _gate = gate; }


  void Transistor::setSource(const std::string& source)
  { if (checkConnector(source)) _source = source; }

  void Transistor::setDrain(const std::string& drain)
  { if (checkConnector(drain)) _drain = drain; }

            
  void Transistor::setBulk(const std::string& bulk)
  { if (checkConnector(bulk)) _bulk = bulk; }


  bool Transistor::checkConnector(const std::string& name)
  {
    map<string, Net*>::const_iterator it = _instance->getConnectors().find(name);
    if (it != _instance->getConnectors().end()) {// si le connecteur existe bien
      return true;
    } else {
      string error("[ERROR] Instance ");
      error += _instance->getName();
      error += " has no connector named ";
      error += name;
      error += ".";
      throw OpenChamsException(error);
    }
  }


}  // OpenChams namespace.
