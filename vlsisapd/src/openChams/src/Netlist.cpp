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
    
void Netlist::addInstance(Instance* inst) {
    for (vector<Instance*>::iterator it = _instances.begin() ; it != _instances.end() ; ++it) {
        if ((*it)->getName() == inst->getName()) {
            string error("[ERROR] Cannot define two instances with the same name in netlist (");
            error += inst->getName().getString();
            error += ").";
            throw OpenChamsException(error);
        }
    }
    _instances.push_back(inst);
}

void Netlist::addNet(Net* net) {
	for (vector<Net*>::iterator it = _nets.begin() ; it != _nets.end() ; ++it ) {
        if ((*it)->getName() == net->getName()) {
            string error("[ERROR] Cannot define two nets with the same name in netlist (");
            error += net->getName().getString();
            error += ").";
            throw OpenChamsException(error);
        }
    }
    _nets.push_back(net);
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

