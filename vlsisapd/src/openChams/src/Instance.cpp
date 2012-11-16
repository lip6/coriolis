
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
// |  C++ Module  :  "./Instance.cpp"                                |
// +-----------------------------------------------------------------+


#include <iostream>
using namespace std;

#include "vlsisapd/openChams/Instance.h"
#include "vlsisapd/openChams/Netlist.h"
#include "vlsisapd/openChams/Net.h"
#include "vlsisapd/openChams/OpenChamsException.h"


namespace OpenChams {

  Instance::Instance(const string& name, const string& model, unsigned order, Netlist* netlist)
    : _name(name)
    , _model(model)
    , _order(order)
    , _netlist(netlist)
    , _params()
    , _netMap()
  { }


  Instance::~Instance ()
  { }


  void Instance::addConnector(const string& name)
  {
  // si name n'est pas déjà présent dans la map on ajoute name, NULL (pas de net)
    map<string, Net*>::iterator it = _netMap.find(name);
    if (it == _netMap.end()) {
      _netMap[name] = NULL;
    }
    else {
      string error("[ERROR] The same instance cannot have several connectors with same name (");
      error += name;
      error += ").";
      throw OpenChamsException(error);
    }
  }


  void Instance::connect(const string& connectorName, const string& netName)
  {
  // si connectorName n'est pas déjà présent dans la map :
  //    on cherche le net associé a netName dans la netlist (getNet(const string&))
  //    on ajoute cName, net a la map
    map<string, Net*>::iterator it = _netMap.find(connectorName);
    if (it != _netMap.end()) {
      Net* net = _netlist->getNet(netName);
      if (!net) {
        string error("[ERROR] While connecting instance : net (");
        error += netName;
        error += ") does not exist.";
        throw OpenChamsException(error);
      }
      else
        _netMap[connectorName] = net;
    }
    else {
      string error("[ERROR] While connecting instance : connector (");
      error += connectorName;
      error += ") does not exist.";
      throw OpenChamsException(error);
    }
  }


}  // OpenChams namespace.

