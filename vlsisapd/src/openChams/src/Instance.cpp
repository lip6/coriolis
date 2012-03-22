/*
 *  Instance.cpp
 *  openChams
 *
 *  Created by damien dupuis on 12/01/10.
 *  Copyright 2010 UPMC / LIP6. All rights reserved.
 *
 */

#include <iostream>
using namespace std;

#include "vlsisapd/openChams/Instance.h"
#include "vlsisapd/openChams/Netlist.h"
#include "vlsisapd/openChams/Net.h"
#include "vlsisapd/openChams/OpenChamsException.h"

namespace OpenChams {
Instance::Instance(Name name, Name model, unsigned order, Netlist* netlist)
    : _name(name)
    , _model(model)
    , _order(order)
    , _netlist(netlist)
    , _params()
    , _netMap() {}


  Instance::~Instance ()
  { }


void Instance::addConnector(Name name) {
    // si name n'est pas déjà présent dans la map on ajoute name, NULL (pas de net)
    map<Name, Net*>::iterator it = _netMap.find(name);
    if (it == _netMap.end()) {
        _netMap[name] = NULL;
    }
    else {
        string error("[ERROR] The same instance cannot have several connectors with same name (");
        error += name.getString();
        error += ").";
        throw OpenChamsException(error);
    }
}
    
void Instance::connect(Name connectorName, Name netName) {
    // si connectorName n'est pas déjà présent dans la map :
    //    on cherche le net associé a netName dans la netlist (getNet(Name))
    //    on ajoute cName, net a la map
    map<Name, Net*>::iterator it = _netMap.find(connectorName);
    if (it != _netMap.end()) {
        Net* net = _netlist->getNet(netName);
        if (!net) {
            string error("[ERROR] While connecting instance : net (");
            error += netName.getString();
            error += ") does not exist.";
            throw OpenChamsException(error);
        }
        else
            _netMap[connectorName] = net;
    }
    else {
        string error("[ERROR] While connecting instance : connector (");
        error += connectorName.getString();
        error += ") does not exist.";
        throw OpenChamsException(error);
    }
}
}

