/*
 *  NetList.cpp
 *  openChams
 *
 *  Created by damien dupuis on 12/01/10.
 *  Copyright 2010 UPMC / LIP6. All rights reserved.
 *
 */

#include <iostream>
#include <algorithm>
using namespace std;

#include "Netlist.h"
#include "Circuit.h"

namespace OpenChams {
Netlist::Netlist(Circuit* circuit) : _circuit(circuit) {}
    
void Netlist::addInstance(Instance* inst) {
    vector<Instance*>::iterator it = find(_instances.begin(), _instances.end(), inst);
    if (it != _instances.end())
        cerr << "[WARNING] Cannot add instance twice in netlist (" << inst->getName().getString() << ")." << endl;
    else
        _instances.push_back(inst);
}

void Netlist::addNet(Net* net) {
	vector<Net*>::iterator it = find(_nets.begin(), _nets.end(), net);
    if (it != _nets.end())
        cerr << "[WARNING] Cannot add net twice in netlist (" << net->getName().getString() << ")." << endl;
    else
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
