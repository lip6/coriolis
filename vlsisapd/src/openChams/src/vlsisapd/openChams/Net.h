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

#include "Name.h"
#include "Instance.h"

namespace OpenChams {
class Netlist;
class Net {
	public:
    Net(Name netName, Name typeName, bool, Netlist*);
        
    void connectTo(Name instanceName, Name connectorName);
    
    inline Name     getName() const;
    inline Name     getType();
    inline bool     isExternal();
    inline Netlist* getNetlist();
    inline bool     hasNoConnectors();
    //inline vector<pair<Name, Name> >::iterator getFirstConnectionIt();
    //inline vector<pair<Name, Name> >::iterator getLastConnectionIt();
    inline const std::vector<std::pair<Name, Name> >& getConnections();

    private:
    Name     _name;
    Name     _typeName;
    bool     _isExternal;
    Netlist* _netlist;
    std::vector<std::pair<Name, Name> > _connections; // <instanceName, connectorName>
};
    
inline Name Net::getName() const { return _name; };
inline Name Net::getType() { return _typeName; };
inline bool Net::isExternal() { return _isExternal; };
inline Netlist* Net::getNetlist() { return _netlist; };
inline bool Net::hasNoConnectors() { return (_connections.size() == 0)? true : false; };
//inline vector<pair<Name, Name> >::iterator Net::getFirstConnectionIt() { return _connections.begin();};
//inline vector<pair<Name, Name> >::iterator Net::getLastConnectionIt() { return _connections.end();};
inline const std::vector<std::pair<Name, Name> >& Net::getConnections() { return _connections; };    

} // namespace
#endif

