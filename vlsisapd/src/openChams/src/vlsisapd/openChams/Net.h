/*
 *  Net.h
 *  openChams
 *
 *  Created by damien dupuis on 12/01/10.
 *  Copyright 2010 UPMC / LIP6. All rights reserved.
 *
 */

#ifndef __OPENCHAMS_NET_H__
#define __OPENCHAMS_NET_H__

#include <vector>

#include "vlsisapd/openChams/Name.h"

namespace OpenChams {
class Netlist;
class Port;
class Wire;
class Net {
    public:
    class Connection {
        public:
        Connection(Name instanceName, Name connectorName);

        inline Name getInstanceName() const;
        inline Name getConnectorName() const;

        private:
        Name _instanceName;
        Name _connectorName;
    };

	public:
    Net(Name netName, Name typeName, bool, Netlist*);
        
    void connectTo(Name instanceName, Name connectorName);
    
    inline Name     getName() const;
    inline Name     getType();
    inline bool     isExternal();
    inline Netlist* getNetlist();
    inline bool     hasNoConnections();
    //inline vector<pair<Name, Name> >::iterator getFirstConnectionIt();
    //inline vector<pair<Name, Name> >::iterator getLastConnectionIt();
    inline const std::vector<Net::Connection*>& getConnections();

    // schematic relative methods
    Port* addPort(Name type, unsigned idx, double x, double y, Name orient);
    Wire* addWire();
    inline bool hasNoPorts();
    inline const std::vector<Port*>& getPorts();
    inline bool hasNoWires();
    inline const std::vector<Wire*>& getWires();

    private:
    Name     _name;
    Name     _typeName;
    bool     _isExternal;
    Netlist* _netlist;
    std::vector<Net::Connection*> _connections; // <instanceName, connectorName>
    // schematic relative members
    std::vector<Port*> _ports;
    std::vector<Wire*> _wires;
};
    
inline Name Net::getName() const { return _name; }
inline Name Net::getType() { return _typeName; }
inline bool Net::isExternal() { return _isExternal; }
inline Netlist* Net::getNetlist() { return _netlist; }
inline bool Net::hasNoConnections() { return (_connections.size() == 0)? true : false; }
//inline vector<pair<Name, Name> >::iterator Net::getFirstConnectionIt() { return _connections.begin();};
//inline vector<pair<Name, Name> >::iterator Net::getLastConnectionIt() { return _connections.end();};
inline const std::vector<Net::Connection*>& Net::getConnections() { return _connections; }
inline bool Net::hasNoPorts() { return (_ports.size() == 0)? true : false; }
inline const std::vector<Port*>& Net::getPorts() { return _ports; }
inline bool Net::hasNoWires() { return (_wires.size() == 0)? true : false; }
inline const std::vector<Wire*>& Net::getWires() { return _wires; }

inline Name Net::Connection::getInstanceName() const { return _instanceName; };
inline Name Net::Connection::getConnectorName() const { return _connectorName; };

} // namespace
#endif

