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

#include "Instance.h"
#include "Netlist.h"
#include "Net.h"

namespace OpenChams {
Instance::Instance(Name name, Name model, Name mosType, Netlist* netlist)
    : _name(name)
    , _model(model)
    , _mosType(mosType)
    , _netlist(netlist) {}

void Instance::addConnector(Name name) {
    // si name n'est pas déjà présent dans la map on ajoute name, NULL (pas de net)
    map<Name, Net*>::iterator it = _netMap.find(name);
    if (it == _netMap.end())
        _netMap[name] = NULL;
    else
        cerr << "[ERROR] The same instance cannot have several connectors with same name (" << name.getString() << ")." << endl;
}
    
void Instance::connect(Name connectorName, Name netName) {
    // si connectorName n'est pas déjà présent dans la map :
    //    on cherche le net associé a netName dans la netlist (getNet(Name))
    //    on ajoute cName, net a la map
    map<Name, Net*>::iterator it = _netMap.find(connectorName);
    if (it != _netMap.end()) {
        Net* net = _netlist->getNet(netName);
        if (!net)
            cerr << "[ERROR] While connecting instance : net (" << netName.getString() << ") does not exist." << endl;
        else
            _netMap[connectorName] = net;
    }
    else
        cerr << "[ERROR] While connecting instance : connector (" << connectorName.getString() << ") does not exist." << endl;
}
}

