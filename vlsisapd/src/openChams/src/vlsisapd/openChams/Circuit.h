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
#include <fstream>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "vlsisapd/openChams/Name.h"
#include "vlsisapd/openChams/Parameters.h"
#include "vlsisapd/openChams/SimulModel.h"

namespace OpenChams {
class Netlist;
class Instance;
class Device;
class Net;
class Schematic;
class Sizing;
class Transistor;
class Operator;
class Layout;
class Node;

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
    inline void       addSubCircuitPath(std::string);
    inline std::vector<std::string>& getSubCircuitPaths();

    void addSimulModel(unsigned, SimulModel::Base, SimulModel::Version, std::string);
    
    inline void       setSizing(Sizing*);
    inline void       setLayout(Layout*);

    Netlist*   createNetlist();
    Schematic* createSchematic();
    Sizing*    createSizing();
    Layout*    createLayout();

    void driveHBTree(std::ofstream&, Node*, unsigned);
    
    bool writeToFile(std::string filePath);
    static Circuit* readFromFile(const std::string filePath);
    
	private:
    Name      readParameter(xmlNode*, double&);
    Name      readParameterEq(xmlNode*, std::string&);
    Name      readConnector(xmlNode*);
    void      readSubCircuitsPaths(xmlNode*);
    void      readCircuitParameters(xmlNode*);
    void      readSimulModels(xmlNode*);
    void      readNetList(xmlNode*);
    void      readInstances(xmlNode*, Netlist*);
    Instance* readInstance (xmlNode*, Netlist*);
    void      readInstanceParameters(xmlNode*, Instance*);
    void      readInstanceConnectors(xmlNode*, Instance*);
    void      readInstanceTransistors(xmlNode*, Device*);
    void      readTransistor(xmlNode*, Device*);
    void      readTransistorConnection(xmlNode*, Transistor*);
    void      readNets(xmlNode*, Netlist*);
    Net*      readNet (xmlNode*, Netlist*);
    void      readNetConnector(xmlNode*, Net*);
    void      readSchematic(xmlNode*);
    void      readInstanceSchematic(xmlNode*, Schematic*);
    void      readNetSchematic(xmlNode*, Circuit*);
    void      readPortSchematic(xmlNode*, Net*);
    void      readWireSchematic(xmlNode*, Net*);
    void      readSizing(xmlNode*);
    void      readInstanceSizing(xmlNode*, Sizing*);
    void      readConstraint(xmlNode*, Operator*);

    void      readEquations(xmlNode*, Sizing*);
    void      readEquation_CircuitLevel(xmlNode*, Sizing*);
    void      readEquation_NRC(xmlNode*, Sizing*);
    void      readEquation_DDPs(xmlNode*, Sizing*);
    void      readEquation_DesignerCstr(xmlNode*, Sizing*);

    void      readLayout(xmlNode*);
    void      readInstanceLayout(xmlNode*, Layout*);
    void      readHBTree(xmlNode*, Layout*);
    Node*     readNodeOrBloc(xmlNode*, Node* parent = NULL);
    void      setAbsolutePath(const std::string filePath);
    
    void      check_uppercase(std::string& str, std::vector<std::string>& compares, std::string message);
    void      check_lowercase(std::string& str, std::vector<std::string>& compares, std::string message);
    
    Name 	    _name;
    std::string _absolutePath;
    Name 	    _techno;
    Parameters  _params;
    Netlist*    _netlist;
    Schematic*  _schematic;
    Sizing*     _sizing;
    Layout*     _layout;
    std::vector<std::string> _subCircuitsPaths;
    std::map<unsigned, SimulModel*> _simulModels;
};
    
inline Name       Circuit::getName()    	   { return _name; }
inline Name       Circuit::getTechno()         { return _techno; }
inline double     Circuit::getValue(Name name) { return _params.getValue(name); }
inline Netlist*   Circuit::getNetlist()        { return _netlist; }
inline Schematic* Circuit::getSchematic()      { return _schematic; }
inline Sizing*    Circuit::getSizing()         { return _sizing; }
inline Layout*    Circuit::getLayout()         { return _layout; }
inline void       Circuit::addParameter(Name name, double value) { _params.addParameter(name, value); }
inline void       Circuit::addParameter(Name name, std::string eqStr) { _params.addParameter(name, eqStr); }
inline Parameters Circuit::getParameters()     { return _params; }
inline void       Circuit::addSubCircuitPath(std::string path) { _subCircuitsPaths.push_back(path); }
inline std::vector<std::string>& Circuit::getSubCircuitPaths() { return _subCircuitsPaths; }
    
    
} // namespace OpenChams
#endif

