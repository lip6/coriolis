/*
 *  Instance.h
 *  openChams
 *
 *  Created by damien dupuis on 12/01/10.
 *  Copyright 2010 UPMC / LIP6. All rights reserved.
 *
 */

#ifndef __OPENCHAMS_INSTANCE_H__
#define __OPENCHAMS_INSTANCE_H__

#include <string>
#include <map>
using namespace std;

#include "Name.h"
#include "Parameters.h"

namespace OpenChams {
class Netlist;
class Net;
class Instance {
	public:
    Instance(Name name, Name model, Name mosType, Netlist*);
    
    void addConnector(Name);
    void connect(Name connectorName, Name netName);
    
    Instance* getInstance(Name);
    
    inline void addParameter(Name, double);
    inline Name getName();
    inline Name getModel();
    inline Name getMosType();
    inline Parameters getParameters();
    // pour parcourir les connecteurs
    inline bool hasNoConnectors();
    inline map<Name, Net*>::iterator getFirstConnectorIt();
    inline map<Name, Net*>::iterator getLastConnectorIt();
        
private:
	Name 	   		_name;
    Name 	   		_model;
    Name 	   		_mosType;
    Netlist*   		_netlist;
    Parameters 		_params;
    map<Name, Net*> _netMap; //map associant nom de connecteur a un net
};

inline void Instance::addParameter(Name name, double value) { _params.addParameter(name, value); };
inline Name Instance::getName() { return _name; };
inline Name Instance::getModel() { return _model; };
inline Name Instance::getMosType() { return _mosType; };
inline Parameters Instance::getParameters() { return _params; };
inline bool Instance::hasNoConnectors() { return (_netMap.size() == 0)? true : false; };
inline map<Name, Net*>::iterator Instance::getFirstConnectorIt() { return _netMap.begin(); };
inline map<Name, Net*>::iterator Instance::getLastConnectorIt() { return _netMap.end(); };
    
} // namespace
#endif