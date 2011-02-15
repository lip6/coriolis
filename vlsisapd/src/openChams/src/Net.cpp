/*
 *  Net.cpp
 *  openChams
 *
 *  Created by damien dupuis on 12/01/10.
 *  Copyright 2010 UPMC / LIP6. All rights reserved.
 *
 */

using namespace std;

#include "vlsisapd/openChams/Net.h"
#include "vlsisapd/openChams/Instance.h"
#include "vlsisapd/openChams/Netlist.h"
#include "vlsisapd/openChams/Port.h"
#include "vlsisapd/openChams/Wire.h"
#include "vlsisapd/openChams/OpenChamsException.h"

namespace OpenChams {
Net::Net(Name netName, Name typeName, bool isExternal, Netlist* netlist) 
    : _name(netName)
    , _typeName(typeName)
    , _isExternal(isExternal)
    , _netlist(netlist)
    , _connections()
    , _ports()
    , _wires()
{}
    
void Net::connectTo(Name instanceName, Name connectorName) {
    _connections.push_back(new Net::Connection(instanceName, connectorName));
    Instance* inst = _netlist->getInstance(instanceName);
    if (!inst) {
        string error ("[ERROR] While connecting net ");
        error += _name.getString();
        error += " to instance ";
        error += instanceName.getString();
        error += ": instance does not exist in netlist !";
        throw OpenChamsException(error);
    } else {
        inst->connect(connectorName, _name);
    }
}

Port* Net::addPort(Name type, unsigned idx, double x, double y, Name sym) {
    while (_ports.size() <= idx)
        _ports.push_back(NULL);
    if (_ports[idx])
        throw OpenChamsException("[ERROR] Net::addPort: cannot add port since another one with the same id already exists.");
    _ports[idx] = new Port(type, idx, x, y, sym);

    return _ports[idx];
}

Wire* Net::addWire() {
    Wire* w = new Wire();
    _wires.push_back(w);
    return w;
}

Net::Connection::Connection(Name instanceName, Name connectorName) : _instanceName(instanceName), _connectorName(connectorName) {}
} // namespace
