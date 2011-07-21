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

#include <map>
#include <vector>

#include "vlsisapd/openChams/Name.h"
#include "vlsisapd/openChams/Parameters.h"

namespace OpenChams {
class Netlist;
class Net;
class Instance {
	public:
    Instance(Name name, Name model, unsigned, Netlist*);

    virtual ~Instance() {};
    
    void addConnector(Name);
    void connect(Name connectorName, Name netName);
    
    inline void addParameter(Name, double);
    inline void addParameter(Name, std::string);
    inline Name getName() const;
    inline Name getModel();
    inline unsigned getOrder();
    inline Netlist* getNetlist();
    inline Parameters getParameters();
    // pour parcourir les connecteurs
    inline bool hasNoConnectors();
    inline const std::map<Name, Net*>& getConnectors();

	private:
	Name 	   		_name;
    Name 	   		_model;
    unsigned        _order;
    Netlist*   		_netlist;
    Parameters 		_params;
    std::map<Name, Net*>     _netMap; //map associant nom de connecteur a un net
};

inline void Instance::addParameter(Name name, double value) { _params.addParameter(name, value); };
inline void Instance::addParameter(Name name, std::string eqStr) { _params.addParameter(name, eqStr); };
inline Name Instance::getName() const { return _name; };
inline Name Instance::getModel() { return _model; };
inline unsigned Instance::getOrder() { return _order; };
inline Netlist* Instance::getNetlist() { return _netlist; };
inline Parameters Instance::getParameters() { return _params; };
inline bool Instance::hasNoConnectors() { return (_netMap.size() == 0)? true : false; };
inline const std::map<Name, Net*>& Instance::getConnectors() { return _netMap; };
    
} // namespace
#endif

