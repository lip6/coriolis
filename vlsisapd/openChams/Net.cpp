/*
 *  Net.cpp
 *  openChams
 *
 *  Created by damien dupuis on 12/01/10.
 *  Copyright 2010 UPMC / LIP6. All rights reserved.
 *
 */

using namespace std;

#include "Net.h"
#include "Netlist.h"

namespace OpenChams {
Net::Net(Name netName, Name typeName, bool isExternal, Netlist* netlist) 
    : _name(netName)
    , _typeName(typeName)
    , _isExternal(isExternal)
    , _netlist(netlist) {}
    
void Net::connectTo(Name instanceName, Name connectorName) {
    _connections.push_back(pair<Name, Name>(instanceName, connectorName));
}
} // namespace
