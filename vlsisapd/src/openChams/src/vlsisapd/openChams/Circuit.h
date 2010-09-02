/*
 *  Circuit.h
 *  openChams
 *
 *  Created by damien dupuis on 18/12/09.
 *  Copyright 2009 UPMC / LIP6 All rights reserved.
 *
 */

#ifndef __OPENCHAMS_CIRCUIT_H__
#define __OPENCHAMS_CIRCUIT_H__

#include <vector>
#include <map>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "vlsisapd/openChams/Name.h"
#include "vlsisapd/openChams/Parameters.h"
#include "vlsisapd/openChams/SimulModel.h"

namespace OpenChams {
class Netlist;
class Instance;
class Net;
class Schematic;
class Sizing;
class Transistor;
class Operator;
class Layout;

class Circuit {
    public:
    Circuit(Name name, Name techno);
    
    inline Name       getName();
    inline Name       getTechno();
    inline double     getValue(Name);
    inline Netlist*   getNetlist();
    inline Schematic* getSchematic();
    inline Sizing*    getSizing();
    inline Layout*    getLayout();
    inline void       addParameter(Name, double);
    inline void       addParameter(Name, std::string);
    inline Parameters getParameters();

    void addSimulModel(unsigned, SimulModel::Base, SimulModel::Version, std::string);
    
    inline void       setSizing(Sizing*);
    inline void       setLayout(Layout*);

    Netlist*   createNetlist();
    Schematic* createSchematic(double);
    Sizing*    createSizing();
    Layout*    createLayout();
    
    bool writeToFile(std::string filePath);
    static Circuit* readFromFile(const std::string filePath);
    
	private:
    Name      readParameter(xmlNode*, double&);
    Name      readParameterEq(xmlNode*, std::string&);
    Name      readConnector(xmlNode*);
    void      readCircuitParameters(xmlNode*);
    void      readSimulModels(xmlNode*);
    void      readNetList(xmlNode*);
    void      readInstances(xmlNode*, Netlist*);
    Instance* readInstance (xmlNode*, Netlist*);
    void      readInstanceParameters(xmlNode*, Instance*);
    void      readInstanceConnectors(xmlNode*, Instance*);
    void      readInstanceTransistors(xmlNode*, Instance*);
    void      readTransistor(xmlNode*, Instance*);
    void      readTransistorConnection(xmlNode*, Transistor*);
    void      readNets(xmlNode*, Netlist*);
    Net*      readNet (xmlNode*, Netlist*);
    void      readNetConnector(xmlNode*, Net*);
    void      readSchematic(xmlNode*);
    void      readInstanceSchematic(xmlNode*, Schematic*);
    void      readSizing(xmlNode*);
    void      readInstanceSizing(xmlNode*, Sizing*);
    void      readConstraint(xmlNode*, Operator*);
    void      readEquations(xmlNode*, Sizing*);
    void      readEquation(xmlNode*, Sizing*);
    void      readLayout(xmlNode*);
    void      readInstanceLayout(xmlNode*, Layout*);
    
    void      check_uppercase(std::string& str, std::vector<std::string>& compares, std::string message);
    void      check_lowercase(std::string& str, std::vector<std::string>& compares, std::string message);
    
    Name 	   _name;
    Name 	   _techno;
    Parameters _params;
    Netlist*   _netlist;
    Schematic* _schematic;
    Sizing*    _sizing;
    Layout*    _layout;
    std::map<unsigned, SimulModel*> _simulModels;
};
    
inline Name       Circuit::getName()    	   { return _name; };
inline Name       Circuit::getTechno()         { return _techno; };
inline double     Circuit::getValue(Name name) { return _params.getValue(name); };
inline Netlist*   Circuit::getNetlist()        { return _netlist; };
inline Schematic* Circuit::getSchematic()      { return _schematic; };
inline Sizing*    Circuit::getSizing()         { return _sizing; };
inline Layout*    Circuit::getLayout()         { return _layout; };
inline void       Circuit::addParameter(Name name, double value) { _params.addParameter(name, value); };
inline void       Circuit::addParameter(Name name, std::string eqStr) { _params.addParameter(name, eqStr); };
inline Parameters Circuit::getParameters()     { return _params; };
    
    
} // namespace OpenChams
#endif

