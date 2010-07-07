/*
 *  Netlist.cpp
 *  openChams
 *
 *  Created by damien dupuis on 12/01/10.
 *  Copyright 2010 UPMC / LIP6. All rights reserved.
 *
 */

#include <iostream>
#include <algorithm>
using namespace std;

#include "vlsisapd/openChams/Netlist.h"
#include "vlsisapd/openChams/Instance.h"
#include "vlsisapd/openChams/Net.h"
#include "vlsisapd/openChams/Circuit.h"
#include "vlsisapd/openChams/OpenChamsException.h"

namespace OpenChams {
Netlist::Netlist(Circuit* circuit) : _circuit(circuit) {}
    
Instance* Netlist::addInstance(Name name, Name model, Name mosType, bool sourceBulkConnected) {
    for (vector<Instance*>::iterator it = _instances.begin() ; it != _instances.end() ; ++it) {
        if ((*it)->getName() == name) {
            string error("[ERROR] Cannot define two instances with the same name in netlist (");
            error += name.getString();
            error += ").";
            throw OpenChamsException(error);
        }
    }
    Instance* inst = new Instance(name, model, mosType, sourceBulkConnected, this);
    if (!inst)
        throw OpenChamsException("[ERROR] Cannot creeate instance.");
    _instances.push_back(inst);

    return inst;
}

Net* Netlist::addNet(Name name, Name type, bool external) {
	for (vector<Net*>::iterator it = _nets.begin() ; it != _nets.end() ; ++it ) {
        if ((*it)->getName() == name) {
            string error("[ERROR] Cannot define two nets with the same name in netlist (");
            error += name.getString();
            error += ").";
            throw OpenChamsException(error);
        }
    }
    Net* net = new Net(name, type, external, this);
    if (!net)
        throw OpenChamsException("[ERROR] Cannot create net.");

    _nets.push_back(net);

    return net;
}
 
Instance* Netlist::getInstance(Name instanceName) {
    for (size_t i = 0 ; i < _instances.size() ; ++i) {
        if (_instances[i]->getName() == instanceName) {
            return _instances[i];
        }
    }
    return NULL;
}
    
Net* Netlist::getNet(Name netName) {
    for (size_t i = 0; i < _nets.size(); ++i) {
        if (_nets[i]->getName() == netName) {
            return _nets[i];
        }
    }
    return NULL;
}
} // namespace

