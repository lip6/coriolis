/*
 *  Circuit.cpp
 *  openChams
 *
 *  Created by damien dupuis on 18/12/09.
 *  Copyright 2009 UPMC / LIP6. All rights reserved.
 *
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
using namespace std;

#include "vlsisapd/openChams/Circuit.h"
#include "vlsisapd/openChams/Netlist.h"
#include "vlsisapd/openChams/Instance.h"
#include "vlsisapd/openChams/Device.h"
#include "vlsisapd/openChams/Net.h"
#include "vlsisapd/openChams/Schematic.h"
#include "vlsisapd/openChams/SimulModel.h"
#include "vlsisapd/openChams/Sizing.h"
#include "vlsisapd/openChams/Layout.h"
#include "vlsisapd/openChams/Transistor.h"
#include "vlsisapd/openChams/Operator.h"
#include "vlsisapd/openChams/Port.h"
#include "vlsisapd/openChams/Wire.h"
#include "vlsisapd/openChams/OpenChamsException.h"

namespace {
	template<class T> T getValue(xmlChar* str) {
	    std::istringstream iss;
    	iss.str((const char*) str);
	    T res;
    	iss >> res;
    	return res;
	}
}

namespace OpenChams {
    
static bool readSubCircuitsPathsDone  = false;
static bool readCircuitParametersDone = false;
static bool readSimulModelsDone       = false;
static bool readNetListDone           = false;
static bool readInstancesDone         = false;
static bool readNetsDone              = false;
static bool readSchematicDone         = false;
static bool readSizingDone            = false;
static bool readLayoutDone            = false;
    
Circuit::Circuit(Name name, Name techno) : _name(name)
                                         , _absolutePath("")
                                         , _techno(techno)
                                         , _netlist(NULL)
                                         , _schematic(NULL)
                                         , _sizing(NULL)
                                         , _layout(NULL)
{
    readSubCircuitsPathsDone  = false;
    readCircuitParametersDone = false;
    readSimulModelsDone       = false;
    readNetListDone           = false;
    readInstancesDone         = false;
    readNetsDone              = false;
    readSchematicDone         = false;
    readSizingDone            = false;
    readLayoutDone            = false;
}

// COMPARISON FUNCTION //
bool ConnectionsSort(const Net::Connection* c1, const Net::Connection* c2) {
    return c1->getInstanceName() < c2->getInstanceName();
}

bool InstanceNameSort(const Instance* i1, const Instance* i2) {
    return i1->getName() < i2->getName();
}

bool NetNameSort(const Net* n1, const Net* n2) {
    return n1->getName() < n2->getName();
}

// USEFUL //
void Circuit::check_uppercase(string& str, vector<string>& compares, string message) {
    transform(str.begin(), str.end(), str.begin(), ::toupper);
    bool equal = false;
    for (size_t i = 0 ; i < compares.size() ; i++) {
        if (str == compares[i]) {
            equal = true;
        }
    }
    if (!equal) {
        throw OpenChamsException(message);
    }
}
    
void Circuit::check_lowercase(string& str, vector<string>& compares, string message) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    bool equal = false;
    for (size_t i = 0 ; i < compares.size() ; i++) {
        if (str == compares[i]) {
            equal = true;
        }
    }
    if (!equal) {
        throw OpenChamsException(message);
    }
}
    
void Circuit::addSimulModel(unsigned id, SimulModel::Base base, SimulModel::Version version, std::string filePath) {
    SimulModel* sim = new SimulModel(id, base, version, filePath);
    map<unsigned, SimulModel*>::iterator it = _simulModels.find(id);
    if (it != _simulModels.end())
        throw OpenChamsException("[ERROR] Cannot define two SimulModels' models with the same ID.");
    _simulModels[id] = sim;
}

Name Circuit::readParameter(xmlNode* node, double& value) {
    xmlChar* paramNameC = xmlGetProp(node, (xmlChar*)"name");
    xmlChar* valueC     = xmlGetProp(node, (xmlChar*)"value");
    if (paramNameC && valueC) {
        Name name((const char*)paramNameC);
        value = ::getValue<double>(valueC);
        return name;
    } else {
        throw OpenChamsException("[ERROR] 'parameter' node must have 'name' and 'value' properties.");
    }
}
    
Name Circuit::readParameterEq(xmlNode* node, string& eqStr) {
    xmlChar* paramNameC = xmlGetProp(node, (xmlChar*)"name");
    xmlChar* equationC  = xmlGetProp(node, (xmlChar*)"equation");
    if (paramNameC && equationC) {
        Name name((const char*)paramNameC);
        eqStr = string ((const char*)equationC);
        return name;
    } else {
        throw OpenChamsException("[ERROR] 'parameterEq' node must have 'name' and 'equation' properties.");
    }
}
    
Name Circuit::readConnector(xmlNode* node) {
    xmlChar* connectorNameC = xmlGetProp(node, (xmlChar*)"name");
    if (connectorNameC) {
        Name name((const char*)connectorNameC);
        return name;
    } else {
        throw OpenChamsException("[ERROR] 'connector' node must have 'name' property.");
        //return Name("");
    }
}

// CIRCUIT //
void Circuit::readSubCircuitsPaths(xmlNode* node) {
    if (readSubCircuitsPathsDone) {
        cerr << "[WARNING] Only one 'subCircuitsPaths' node is allowed in circuit, others will be ignored." << endl;
        return;
    }
    if (node->type == XML_ELEMENT_NODE && node->children) {
        for (xmlNode* pathNode = node->children ; pathNode ; pathNode = pathNode->next) {
            if (pathNode->type == XML_ELEMENT_NODE) {
                if (xmlStrEqual(pathNode->name, (xmlChar*)"path")) {
                    xmlChar* pathC = xmlGetProp(pathNode, (xmlChar*)"path");
                    if (pathC) {
                        string path((const char*)pathC);
                        if (path[0] != '/') { // this is not an absolute path
                            path = _absolutePath+"/"+path;
                        }
                        _subCircuitsPaths.push_back(path);
                    } else {
                        throw OpenChamsException("[ERROR] 'path' node must have 'path' property.");
                    }
                } else {
                    cerr << "[WARNING] Only 'path' nodes are allowed under 'subCircuitsPaths' node." << endl;
                    return;
                }
            }
        }
    }
    readSubCircuitsPathsDone = true;
}

void Circuit::readCircuitParameters(xmlNode* node) {
    if (readCircuitParametersDone) {
        cerr << "[WARNING] Only one 'parameters' node is allowed in circuit, others will be ignored." << endl;
        return;
    }
    if (node->type == XML_ELEMENT_NODE && node->children) {
        for (xmlNode* paramNode = node->children ; paramNode ; paramNode = paramNode->next) {
            if (paramNode->type == XML_ELEMENT_NODE) {
                if (xmlStrEqual(paramNode->name, (xmlChar*)"parameter")) {
                    double value = 0.0;
                    Name paramName = readParameter(paramNode, value);
                    if (paramName == Name("")) return; // error
                    addParameter(paramName, value);
                } else if (xmlStrEqual(paramNode->name, (xmlChar*)"parameterEq")) {
                    string eqStr = "";
                    Name paramName = readParameterEq(paramNode, eqStr);
                    if (paramName == Name("")) return; // error
        			addParameter(paramName, eqStr);
                } else {
                    cerr << "[WARNING] Only 'parameter' and 'parameterEq' nodes are allowed under 'parameters' node." << endl;
                    return;
                }
            }
        }
    }     
    readCircuitParametersDone = true;
}

void Circuit::readSimulModels(xmlNode* node) {
    if (readSimulModelsDone) {
        cerr << "[WARNING] Only one 'simulModels' node is allowed in circuit, others will be ignored." << endl;
        return;
    }
    if (node->type == XML_ELEMENT_NODE && node->children) {
        for (xmlNode* modelNode = node->children ; modelNode ; modelNode = modelNode->next) {
            if (modelNode->type == XML_ELEMENT_NODE) {
                if (xmlStrEqual(modelNode->name, (xmlChar*)"model")) {
                    xmlChar* mIdC       = xmlGetProp(modelNode, (xmlChar*)"id");
                    xmlChar* mBaseC     = xmlGetProp(modelNode, (xmlChar*)"base");
                    xmlChar* mVersionC  = xmlGetProp(modelNode, (xmlChar*)"version");
                    xmlChar* mFilePathC = xmlGetProp(modelNode, (xmlChar*)"filePath");
                    if (mIdC && mBaseC && mVersionC && mFilePathC) {
                        unsigned id = ::getValue<unsigned>(mIdC);
                        SimulModel::Base base = SimulModel::BSIM3V3;
                        string mBase((const char*)mBaseC);
                        string baseComp[3] = { "BSIM3V3", "BSIM4", "PSP" };
                        vector<string> baseComps(baseComp, baseComp+3);
                        check_uppercase(mBase, baseComps, "[ERROR] SimulModels models' base property must be \"BSIM3V3\", \"BSIM4\" or \"PSP\".");
                        if (mBase == "BSIM3V3") {
                            base = SimulModel::BSIM3V3;
                        } else if (mBase == "BSIM4") {
                            base = SimulModel::BSIM4;
                        } else if (mBase == "PSP") {
                            base = SimulModel::PSP;
                        } else {
                            throw OpenChamsException("[ERROR] SimulModels models' base property must be \"BSIM3V3\", \"BSIM4\" or \"PSP\" (check_uppercase should have filtered that).");
                            return;
                        }
                        SimulModel::Version version = SimulModel::UNDEFINED;
                        string mVersion((const char*)mVersionC);
                        string verComp[4] = { "UNDEFINED", "SVT", "HVT", "LVT" };
                        vector<string> verComps(verComp, verComp+4);
                        check_uppercase(mVersion, verComps, "[ERROR] SimulModels model's version property must be \"UNDEFINED\", \"SVT\", \"HVT\" or \"LVT\".");
                        if (mVersion == "UNDEFINED") {
                            version = SimulModel::UNDEFINED;
                        } else if (mVersion == "SVT") {
                            version = SimulModel::SVT;
                        } else if (mVersion == "HVT") {
                            version = SimulModel::HVT;
                        } else if (mVersion == "LVT") {
                            version = SimulModel::LVT;
                        } else {
                            throw OpenChamsException("[ERROR] SimulModels models' version property must be \"UNDEFINED\", \"SVT\", \"HVT\" or \"LVT\" (check_uppercase should have filtered that).");
                            return;
                        }
                        string filePath((const char*)mFilePathC);
                        addSimulModel(id, base, version, filePath);
                        // Ce simuModel DOIT être rataché au circuit !!!
                    } else {
                        throw OpenChamsException("[ERROR] 'model' node must have 'id', 'base', 'version' and 'filePath' properties.");
                    }
                } else {
                    cerr << "[WARNING] Only 'model' nodes are allowed under 'simulModels' node." << endl;
                    return;
                }
            }
        }
    }     
    readSimulModelsDone = true;
}

// NETLIST //
void Circuit::readNetList(xmlNode* node) {
    if (readNetListDone) {
        cerr << "[WARNING] Only one 'netlist' node is allowed in circuit, others will be ignored." << endl;
        return;
    }
    Netlist* netlist = new Netlist(this);
    xmlNode* child = node->children;
    for (xmlNode* node = child; node; node = node->next) {
        if (node->type == XML_ELEMENT_NODE) {
            if (xmlStrEqual(node->name, (xmlChar*)"instances")) {
                readInstances(node, netlist);
            } else if (xmlStrEqual(node->name, (xmlChar*)"nets")) {
                readNets(node, netlist);
            } else {
                cerr << "[WARNING] Only 'instances' and 'nets' nodes are allowed in 'netlist', others will be ignored." << endl;
            }
        }
    }
    readNetListDone = true;
    _netlist = netlist;
}

// INSTANCES //
void Circuit::readInstances(xmlNode* node, Netlist* netlist) {
    if (readInstancesDone) {
        cerr << "[WARNING] Only one 'instances' node is allowed in 'netlist', others will be ignored." << endl;
        return;
    }
    xmlNode* child = node->children;
    for (xmlNode* node = child; node; node = node->next) {
        if (node->type == XML_ELEMENT_NODE) {
            if (xmlStrEqual(node->name, (xmlChar*)"instance")) {
                readInstance(node, netlist);
            } else {
                cerr << "[WARNING] Only 'instance' nodes are allowed in 'instances', others will be ignored." << endl;
            }
        }
    }
    readInstancesDone = true;
}

Instance* Circuit::readInstance(xmlNode* node, Netlist* netlist) {
    xmlChar* iNameC  = xmlGetProp(node, (xmlChar*)"name");
    xmlChar* iModelC = xmlGetProp(node, (xmlChar*)"model");
    xmlChar* iMOSC   = xmlGetProp(node, (xmlChar*)"mostype");
    xmlChar* iSBCC   = xmlGetProp(node, (xmlChar*)"sourceBulkConnected");
    Instance* inst = NULL;
    if (iNameC && iModelC && iMOSC && iSBCC) { // this is a device
        Name instanceName((const char*)iNameC);
        Name modelName((const char*)iModelC);
        string mosStr((const char*)iMOSC);
        string mosComp[2] = {"NMOS", "PMOS"};
        vector<string> mosComps (mosComp, mosComp+2);
        check_uppercase(mosStr, mosComps, "[ERROR] In 'instance', 'mostype' must be 'NMOS' or 'PMOS'.");
        string sourceBulkStr((const char*)iSBCC);
        string sbcComp[4] = {"true", "false", "on", "off"};
        vector<string> sbcComps(sbcComp, sbcComp+4);
        check_lowercase(sourceBulkStr, sbcComps, "[ERROR] In 'instance', 'sourceBulkConnected' must 'true', 'false', 'on' or 'off'.");
        bool sourceBulkConnected = ((sourceBulkStr == "true") || (sourceBulkStr == "on")) ? true : false;
        inst = (Instance*)netlist->addDevice(instanceName, modelName, Name(mosStr), sourceBulkConnected);
    } else if (iNameC && iModelC && !iMOSC && !iSBCC) { // this is a subcircuit
        Name instanceName((const char*)iNameC);
        Name modelName((const char*)iModelC);
        inst = netlist->addInstance(instanceName, modelName);
    } else {
        throw OpenChamsException("[ERROR] 'instance' node must have ('name' and 'model') or ('name', 'model', 'mostype' and 'sourceBulkConnected') properties.");
        //return inst;
    }

    xmlNode* child = node->children;
    for (xmlNode* node = child; node; node = node->next) {
        if (node->type == XML_ELEMENT_NODE) {
            if (xmlStrEqual(node->name, (xmlChar*)"connectors")) {
                readInstanceConnectors(node, inst);
            } else if (xmlStrEqual(node->name, (xmlChar*)"parameters")) {
                readInstanceParameters(node, inst);
            } else if (xmlStrEqual(node->name, (xmlChar*)"transistors")) {
                if (!dynamic_cast<Device*>(inst))
                    throw OpenChamsException("[ERROR] Only device 'instance' (with 'mostype' and 'sourceBulkConnected' properties) can have 'transistors' section.");
                readInstanceTransistors(node, static_cast<Device*>(inst));
            } else {
                //cerr << "[WARNING] Only 'parameters' node is allowed in 'instance', others will be ignored." << endl;
                cerr << "[WARNING] Only 'conectors', 'transistors' and 'parameters' nodes are allowed in 'instance', others will be ignored." << endl;
            }
        }
    }
    return inst;
}
 
void Circuit::readInstanceConnectors(xmlNode* node, Instance* inst) {
	xmlNode* child = node->children;
    for (xmlNode* node = child; node; node = node->next) {
        if (node->type == XML_ELEMENT_NODE) {
            if (xmlStrEqual(node->name, (xmlChar*)"connector")) {
                Name connectorName = readConnector(node);
                if (connectorName == Name("")) return; // error
                inst->addConnector(connectorName);
            }
        }
    }
}
    
void Circuit::readInstanceParameters(xmlNode* node, Instance* inst) {
    xmlNode* child = node->children;
    for (xmlNode* node = child; node; node = node->next) {
        if (node->type == XML_ELEMENT_NODE) {
            if (xmlStrEqual(node->name, (xmlChar*)"parameter")) {
                double value = 0.0;
                Name paramName = readParameter(node, value);
                if (paramName == Name("")) return; // error
                inst->addParameter(paramName, value);
            } else if (xmlStrEqual(node->name, (xmlChar*)"parameterEq")) {
                string eqStr = "";
                Name paramName = readParameterEq(node, eqStr);
                if (paramName == Name("")) return; // error
                inst->addParameter(paramName, eqStr);
            } else {
                cerr << "[WARNING] Only 'parameter' and 'parameterEq' nodes are allowed under 'instance' node." << endl;
                return;
            }
        }
    }
}

void Circuit::readInstanceTransistors(xmlNode* node, Device* dev) {
    xmlNode* child = node->children;
    for (xmlNode* node = child; node; node = node->next) {
        if (node->type == XML_ELEMENT_NODE) {
            if (xmlStrEqual(node->name, (xmlChar*)"transistor")) {
                readTransistor(node, dev);
            } else {
                cerr << "[WARNING] Only 'transistor' nodes are allowed in 'transistors', others will be ignored." << endl;
            }

        }
    }
}
    
void Circuit::readTransistor(xmlNode* node, Device* dev) {
    xmlChar* tNameC  = xmlGetProp(node, (xmlChar*)"name");
    Transistor* trans = NULL;
    if (tNameC) {
        Name tName((const char*)tNameC);
        trans = dev->addTransistor(tName);
    } else {
        throw OpenChamsException("[ERROR] 'transistor' node must have 'name' property.");
        //return inst;
    }
    
    xmlNode* child = node->children;
    for (xmlNode* node = child; node; node = node->next) {
        if (node->type == XML_ELEMENT_NODE) {
            if (xmlStrEqual(node->name, (xmlChar*)"connection")) {
                readTransistorConnection(node, trans);
            } else if (xmlStrEqual(node->name, (xmlChar*)"parameters")) {
                cerr << "Transistor parameters NOT SUPPORTED YET" << endl;
            } else {
                cerr << "[WARNING] Only 'conectors', 'transistors' and 'parameters' nodes are allowed in 'instance', others will be ignored." << endl;
            }
        }
    }
}

void Circuit::readTransistorConnection(xmlNode* node, Transistor* trans) {
    xmlChar* gateC   = xmlGetProp(node, (xmlChar*)"gate");
    xmlChar* sourceC = xmlGetProp(node, (xmlChar*)"source");
    xmlChar* drainC  = xmlGetProp(node, (xmlChar*)"drain");
    xmlChar* bulkC   = xmlGetProp(node, (xmlChar*)"bulk");
    if (gateC && sourceC && drainC && bulkC) {
        Name gateN  ((const char*)gateC);
        Name sourceN((const char*)sourceC);
        Name drainN ((const char*)drainC);
        Name bulkN  ((const char*)bulkC);
        trans->setGate(gateN);
        trans->setSource(sourceN);
        trans->setDrain(drainN);
        trans->setBulk(bulkN);
    } else {
        throw OpenChamsException("[ERROR] 'connection' node must have 'gate', 'source', 'drain' and 'bulk' properties.");
    }

}
 
// NETS //
void Circuit::readNets(xmlNode* node, Netlist* netlist) {
    if (readNetsDone) {
        cerr << "[WARNING] Only one 'nets' node is allowed in 'netlist', others will be ignored." << endl;
        return;
    }
    xmlNode* child = node->children;
    for (xmlNode* node = child; node; node = node->next) {
        if (node->type == XML_ELEMENT_NODE) {
            if (xmlStrEqual(node->name, (xmlChar*)"net")) {
                readNet(node, netlist);
            } else {
                cerr << "[WARNING] Only 'net' nodes are allowed in 'nets', others will be ignored." << endl;
            }
        }
    }
    readNetsDone = true;
}
    
Net* Circuit::readNet(xmlNode* node, Netlist* netlist) {
    xmlChar* nNameC   = xmlGetProp(node, (xmlChar*)"name");
    xmlChar* nTypeC   = xmlGetProp(node, (xmlChar*)"type");
    xmlChar* nExternC = xmlGetProp(node, (xmlChar*)"isExternal");
    Net* net = NULL;
    if (nNameC && nTypeC && nExternC) {
        Name netName((const char*)nNameC);
        string typeStr((const char*)nTypeC);
        string typeComp[3] = {"power", "ground", "logical"};
        vector<string> typeComps(typeComp, typeComp+3);
        check_lowercase(typeStr, typeComps, "[ERROR] In 'net', 'type' must be 'power', 'ground' or 'logical'.");
        string externStr((const char*)nExternC);
        string extComp[4] = {"true", "false", "on", "off"};
        vector<string> extComps(extComp, extComp+4);
        check_lowercase(externStr, extComps, "[ERROR] In 'net', 'isExternal' must be 'true', 'false', 'on' or 'off'.");
        bool isExternal = ((externStr == "true") || (externStr == "on")) ? true : false;
        net = netlist->addNet(netName, Name(typeStr), isExternal);
    } else {
        throw OpenChamsException("[ERROR] 'net' node must have 'name', 'type' and 'isExternal' properties.");
        //return net;
    }
    
    xmlNode* child = node->children;
    for (xmlNode* node = child; node; node = node->next) {
        if (node->type == XML_ELEMENT_NODE) {
            if (xmlStrEqual(node->name, (xmlChar*)"connector")) {
                readNetConnector(node, net);
            } else {
                cerr << "[WARNING] Only 'connector' nodes are allowed in 'net', others will be ignored." << endl;
                return NULL;
            }
        }
    }
    return net;
}
    
void Circuit::readNetConnector(xmlNode* node, Net* net) {
    xmlChar* instanceNameC  = xmlGetProp(node, (xmlChar*)"instance");
    xmlChar* connectorNameC = xmlGetProp(node, (xmlChar*)"name");
    if (instanceNameC && connectorNameC) {
        Name iName((const char*)instanceNameC);
        Name cName((const char*)connectorNameC);
        Instance* inst = net->getNetlist()->getInstance(iName);
        if (!inst) {
            string error("[ERROR] no instance named \"");
            error += iName.getString();
            error += "\" in connector of net \"";
            error += net->getName().getString();
            error += "\".";
            throw OpenChamsException(error);
            //return;
        }
        //inst->connect(cName, net->getName());
        net->connectTo(iName, cName);
    } else {
        throw OpenChamsException("[ERROR] 'connector' node must have 'instance' and 'name' properties (for net).");
    }
}

// SCHEMATIC //
void Circuit::readSchematic(xmlNode* node) {
    if (readSchematicDone) {
        cerr << "[WARNING] Only one 'schematic' node is allowed in circuit, others will be ignored." << endl;
        return;
    }

    Schematic* schematic = new Schematic(this);
    xmlNode* child = node->children;
    for (xmlNode* node = child; node; node = node->next) {
        if (node->type == XML_ELEMENT_NODE) {
            if (xmlStrEqual(node->name, (xmlChar*)"instance")) {
                readInstanceSchematic(node, schematic);
            } else if (xmlStrEqual(node->name, (xmlChar*)"net")) {
                readNetSchematic(node, this);
            } else {
                cerr << "[WARNING] Only 'instance' and 'nets' nodes are allowed in 'schematic', others will be ignored." << endl;
            }
        }
    }
    readSchematicDone = true;
    _schematic = schematic;
}
    
void Circuit::readInstanceSchematic(xmlNode* node, Schematic* schematic) {
    xmlChar* nameC   = xmlGetProp(node, (xmlChar*)"name");
    xmlChar* xC      = xmlGetProp(node, (xmlChar*)"x");
    xmlChar* yC      = xmlGetProp(node, (xmlChar*)"y");
    xmlChar* orientC = xmlGetProp(node, (xmlChar*)"orient");
    if (nameC && xC && yC && orientC) {
        Name   iName((const char*)nameC);
        double x = ::getValue<double>(xC);
        double y = ::getValue<double>(yC);
        string orientStr((const char*)orientC);
        string orientComp[8] = {"ID", "R1", "R2", "R3", "MX", "XR", "MY", "YR"};
    	vector<string> orientComps (orientComp, orientComp+8);
        check_uppercase(orientStr, orientComps, "[ERROR] In 'schematic'.'instance', 'orient' must be 'ID', 'R1', 'R2', 'R3', 'MX', 'XR', 'MY' or 'YR'.");
        schematic->addInstance(iName, x, y, Name(orientStr));
    } else {
        throw OpenChamsException("[ERROR] 'instance' node in 'schematic' must have 'name', 'x', 'y' and 'orient' properties.");
    }
}

void Circuit::readNetSchematic(xmlNode* node, Circuit* circuit) {
    xmlChar* nameC = xmlGetProp(node, (xmlChar*)"name");
    if (nameC) {
        Name nName((const char*)nameC);
        Net* net = circuit->getNetlist()->getNet(nName);
        if (!net) {
            string error ("[ERROR] In 'schematic' section cannot specify wires for net ");
            error += nName.getString();
            error += " since it has not been defined in netlist section.";
            throw OpenChamsException(error);
        }
        xmlNode* child = node->children;
        for (xmlNode* node = child; node; node = node->next) {
            if (node->type == XML_ELEMENT_NODE) {
                if (xmlStrEqual(node->name, (xmlChar*)"port")) {
                    readPortSchematic(node, net);
                } else if (xmlStrEqual(node->name, (xmlChar*)"wire")) {
                    readWireSchematic(node, net);
                } else {
                    cerr << "[WARNING] Only 'port' and 'wire' nodes are allowed in 'schematic'.'net', others will be ignored." << endl;
                }
            }
        }
    } else {
        throw OpenChamsException("[ERROR] 'net' node in schematic must have 'name' property.");
    }
}

void Circuit::readPortSchematic(xmlNode* node, Net* net) {
    xmlChar* typeC   = xmlGetProp(node, (xmlChar*)"type");
    xmlChar* idxC    = xmlGetProp(node, (xmlChar*)"idx");
    xmlChar* xC      = xmlGetProp(node, (xmlChar*)"x");
    xmlChar* yC      = xmlGetProp(node, (xmlChar*)"y");
    xmlChar* orientC = xmlGetProp(node, (xmlChar*)"orient");
    if (typeC && idxC && xC && yC && orientC) {
        Name pType((const char*)typeC);
        unsigned idx = ::getValue<unsigned>(idxC);
        double   x   = ::getValue<double>(xC);
        double   y   = ::getValue<double>(yC);
        string orientStr((const char*)orientC);
        string orientComp[8] = {"ID", "R1", "R2", "R3", "MX", "XR", "MY", "YR"};
    	vector<string> orientComps (orientComp, orientComp+8);
        check_uppercase(orientStr, orientComps, "[ERROR] In 'schematic'.'port', 'orient' must be 'ID', 'R1', 'R2', 'R3', 'MX', 'XR', 'MY' or 'YR'.");
        net->addPort(pType, idx, x, y, Name(orientStr));
    } else {
        throw OpenChamsException("[ERROR] 'schematic'.'port' must have 'type', 'idx', 'x', 'y' and 'orient' properties.");
    }
}

void Circuit::readWireSchematic(xmlNode* node, Net* net) {
    Wire* wire = net->addWire();
    xmlNode* child = node->children;
    for (xmlNode* node = child; node; node = node->next) {
        if (node->type == XML_ELEMENT_NODE) {
            if (xmlStrEqual(node->name, (xmlChar*)"connector")) {
                xmlChar* nameC = xmlGetProp(node, (xmlChar*)"name");
                xmlChar* plugC = xmlGetProp(node, (xmlChar*)"plug");
                xmlChar* idxC  = xmlGetProp(node, (xmlChar*)"idx");
                if (nameC && plugC) {
                    Name name((const char*)nameC);
                    Name plug((const char*)plugC);
                    if (!wire->getStartPoint()) {
                        wire->setStartPoint(name, plug);
                    } else if (!wire->getEndPoint()) {
                        wire->setEndPoint(name, plug);
                    } else {
                        throw OpenChamsException("[ERROR] In 'schematic' a 'wire' must have exactly 2 connectors (not more).");
                    }
                } else if (idxC) {
                    unsigned idx = ::getValue<unsigned>(idxC);
                    if (!wire->getStartPoint()) {
                        wire->setStartPoint(idx);
                    } else if (!wire->getEndPoint()) {
                        wire->setEndPoint(idx);
                    } else {
                        throw OpenChamsException("[ERROR] In 'schematic' a 'wire' must have exactly 2 connectors (not more).");
                    }

                } else {
                    throw OpenChamsException("[ERROR] 'schematic'.'net'.'connector' node must have 'name' & 'plug' OR 'idx' properties. ");
                }
            } else if (xmlStrEqual(node->name, (xmlChar*)"point")) {
                xmlChar* xC = xmlGetProp(node, (xmlChar*)"x");
                xmlChar* yC = xmlGetProp(node, (xmlChar*)"y");
                if (xC && yC) {
                    double x = ::getValue<double>(xC);
                    double y = ::getValue<double>(yC);
                    wire->addIntermediatePoint(x, y); // check is done inside the method (start/end points)
                } else {
                    throw OpenChamsException("[ERROR] 'schematic'.'net'.'point' node must have 'x' and 'y' properties.");
                }
            } else {
                cerr << "[WARNING] Only 'connector' and 'points' nodes are allowed in 'schematic'.'net'.'wire', others will be ignored." << endl;
            }
        }
    }
}

// SIZING //
void Circuit::readSizing(xmlNode* node) {
    if (readSizingDone) {
        cerr << "[WARNING] Only one 'sizing' node is allowed in circuit, others will be ignored." << endl;
        return;
    }
    
    Sizing* sizing = new Sizing(this);
    xmlNode* child = node->children;
    for (xmlNode* node = child; node; node = node->next) {
        if (node->type == XML_ELEMENT_NODE) {
            if (xmlStrEqual(node->name, (xmlChar*)"instance")) {
                readInstanceSizing(node, sizing);
            } else if (xmlStrEqual(node->name, (xmlChar*)"equations")) {
                readEquations(node, sizing);
            } else {
                cerr << "[WARNING] Only 'instance' and 'equations' nodes are allowed in 'sizing', others will be ignored." << endl;
            }
        }
    }
    readSizingDone = true;
    _sizing = sizing;
}
    
void Circuit::readInstanceSizing(xmlNode* node, Sizing* sizing) {
    xmlChar* nameC     = xmlGetProp(node, (xmlChar*)"name");
    xmlChar* operatorC = xmlGetProp(node, (xmlChar*)"operator");
    xmlChar* simulModC = xmlGetProp(node, (xmlChar*)"simulModel");
    xmlChar* orderC    = xmlGetProp(node, (xmlChar*)"callOrder");
    if (nameC && operatorC && simulModC && orderC) {
        Name     iName   ((const char*)nameC);
        string   opStr   ((const char*)operatorC);
        transform(opStr.begin(), opStr.end(), opStr.begin(), ::toupper);
        Name     opName  (opStr);
        Name     simulMod((const char*)simulModC);
        unsigned callOrder = ::getValue<unsigned>(orderC);
        Operator* op = sizing->addOperator(iName, opName, simulMod, callOrder);
        xmlNode* child = node->children;
        for (xmlNode* node = child; node; node = node->next) {
            if (node->type == XML_ELEMENT_NODE) {
                if (xmlStrEqual(node->name, (xmlChar*)"constraint")) {
                    readConstraint(node, op);
                } else {
                    cerr << "[WARNING] Only 'constraint' nodes are allowed in 'instance' of 'sizing', others will be ignored." << endl;
                }
            }
        }
        
    } else {
        throw OpenChamsException("[ERROR] 'instance' node in 'sizing' must have 'name', 'operator', 'simulModel' and 'callOrder' properties.");
    }
}
    
void Circuit::readConstraint(xmlNode* node, Operator* op) {
    // attributes of constraint may be :
    //    param ref refParam [factor]
    //    param refEquation [factor]
    xmlChar* paramC    = xmlGetProp(node, (xmlChar*)"param");
    xmlChar* refC      = xmlGetProp(node, (xmlChar*)"ref");
    xmlChar* refParamC = xmlGetProp(node, (xmlChar*)"refParam");
    xmlChar* refEqC    = xmlGetProp(node, (xmlChar*)"refEquation");
    xmlChar* factorC   = xmlGetProp(node, (xmlChar*)"factor");
    if (paramC && refC && refParamC) {
        Name param    ((const char*)paramC);
        Name ref      ((const char*)refC);
        Name refParam ((const char*)refParamC);
        double factor = 1.0;
        if (factorC) {
            factor = ::getValue<double>(factorC);
        }
        op->addConstraint(param, ref, refParam, factor);
    } else if (paramC && refEqC) {
        Name param ((const char*)paramC);
        Name refEq ((const char*)refEqC);
        double factor = 1.0;
        if (factorC) {
            factor = ::getValue<double>(factorC);
        }
        op->addConstraint(param, refEq, factor);
    } else {
        throw OpenChamsException("[ERROR] 'constraint' node must have 'param, ref, refParam, [factor]' or 'param, refEq, [factor]' properties.");
    }
}
    
void Circuit::readEquations(xmlNode* node, Sizing* sizing) {
    xmlNode* child = node->children;
    for (xmlNode* node = child; node; node = node->next) {
        if (node->type == XML_ELEMENT_NODE) {
            if (xmlStrEqual(node->name, (xmlChar*)"eq")) {
                readEquation(node, sizing);
            } else {
                throw OpenChamsException("[ERROR] Only 'eq' nodes are allowed in 'equations'.");
            }
        }
    }
}
    
void Circuit::readEquation(xmlNode* node, Sizing* sizing) {
    xmlChar* nameC     = xmlGetProp(node, (xmlChar*)"name");
    xmlChar* equationC = xmlGetProp(node, (xmlChar*)"equation");
    if (nameC && equationC) {
        Name   eName ((const char*)nameC);
        string eqStr ((const char*)equationC);
        sizing->addEquation(eName, eqStr);
    } else {
        throw OpenChamsException("[ERROR] 'eq' node in 'equations' must have 'name' and 'equation' properties.");
    }
}
    
// LAYOUT //
void Circuit::readLayout(xmlNode* node) {
    if (readLayoutDone) {
        cerr << "[WARNING] Only one 'layout' node is allowed in circuit, others will be ignored." << endl;
        return;
    }
    
    Layout* layout = new Layout(this);
    xmlNode* child = node->children;
    for (xmlNode* node = child; node; node = node->next) {
        if (node->type == XML_ELEMENT_NODE) {
            if (xmlStrEqual(node->name, (xmlChar*)"instance")) {
                readInstanceLayout(node, layout);
            } else {
                cerr << "[WARNING] Only 'instance' nodes are allowed in 'sizing', others will be ignored." << endl;
            }
        }
    }
    readLayoutDone = true;
    _layout = layout;
}
    
void Circuit::readInstanceLayout(xmlNode* node, Layout* layout) {
    xmlChar* nameC  = xmlGetProp(node, (xmlChar*)"name");
    xmlChar* styleC = xmlGetProp(node, (xmlChar*)"style");
    if (nameC && styleC) {
        Name     iName     ((const char*)nameC);
        Name     styleName ((const char*)styleC);
        layout->addInstance(iName, styleName);
    } else {
        throw OpenChamsException("[ERROR] 'instance' node in 'layout' must have 'name' and 'style' properties.");
    }
}

void Circuit::setAbsolutePath(const string filePath) {
    if (filePath[0] == '/')
        _absolutePath = filePath;
    else {
        _absolutePath = string(getenv("PWD"))+"/"+filePath;
    }
    size_t found = _absolutePath.find_last_of("/");
    _absolutePath = _absolutePath.substr(0, found);
}
    
Circuit* Circuit::readFromFile(const string filePath) {
    LIBXML_TEST_VERSION;
    Circuit* cir = NULL;

    xmlDoc* doc = xmlReadFile(filePath.c_str(), NULL, 0);
    if (doc == NULL) {
        string error ("[ERROR] Failed to parse: ");
        error += filePath;
        throw OpenChamsException(error);
        //return NULL;
    }
    xmlNode* rootElement = xmlDocGetRootElement(doc);
    if (rootElement->type == XML_ELEMENT_NODE && xmlStrEqual(rootElement->name, (xmlChar*)"circuit")) {
        xmlChar* circuitNameC = xmlGetProp(rootElement, (xmlChar*)"name");
        xmlChar* technoNameC  = xmlGetProp(rootElement, (xmlChar*)"techno");
        
        if (circuitNameC && technoNameC) {
            Name circuitName ((const char*)circuitNameC);
            Name technoName  ((const char*)technoNameC);
            cir = new Circuit(circuitName, technoName);
        } else {
            throw OpenChamsException("[ERROR] 'circuit' node must have 'name' and 'techno' properties.");
            return NULL;
        }
        cir->setAbsolutePath(filePath);
        
        xmlNode* child = rootElement->children;
        for (xmlNode* node = child; node; node = node->next) {
            if (node->type == XML_ELEMENT_NODE) {
                if (xmlStrEqual(node->name, (xmlChar*)"subCircuitsPaths")) {
                    cir->readSubCircuitsPaths(node);
                }
                else if (xmlStrEqual(node->name, (xmlChar*)"parameters")) {
                    cir->readCircuitParameters(node);
                }
                else if (xmlStrEqual(node->name, (xmlChar*)"simulModels")) {
                    cir->readSimulModels(node);
                }
                else if (xmlStrEqual(node->name, (xmlChar*)"netlist")) {
                    cir->readNetList(node);
                }
                else if (xmlStrEqual(node->name, (xmlChar*)"schematic")) {
                    cir->readSchematic(node);
                }
                else if (xmlStrEqual(node->name, (xmlChar*)"sizing")) {
                    cir->readSizing(node);
                }
                else if (xmlStrEqual(node->name, (xmlChar*)"layout")) {
                    cir->readLayout(node);
                }
                else {
                    string error("[ERROR] Unknown section ");
                    error += string((const char*)node->name);
                    error += " in circuit description.";
                    throw OpenChamsException(error);
                    return NULL;
                }
            }
        }
    }
    if (!readNetListDone) {
        throw OpenChamsException("[ERROR] no <netlist> section was found in parsed file !");
    }
    return cir;
}
    
Netlist* Circuit::createNetlist() {
    if (_netlist)
        throw OpenChamsException("[ERROR] Cannot create two netlists in one circuit.");

    _netlist = new Netlist(this);
    if (!_netlist)
        throw OpenChamsException("[ERROR] Cannot create netlist.");

    return _netlist;
}

Schematic* Circuit::createSchematic() {
    if (_schematic)
        throw OpenChamsException("[ERROR] Cannot create two scheamtics in one circuit.");

    _schematic = new Schematic(this);
    if (!_schematic)
        throw OpenChamsException("[ERROR] Cannot create schematic.");

    return _schematic;
}

Sizing* Circuit::createSizing() {
    if (_sizing)
        throw OpenChamsException("[ERROR] Cannot create two sizings in one circuit.");

    _sizing = new Sizing(this);
    if (!_sizing)
        throw OpenChamsException("[ERROR] Cannot create sizing.");

    return _sizing;
}

Layout* Circuit::createLayout() {
    if (_layout)
        throw OpenChamsException("[ERROR] Cannot create two layouts in one circuit.");

    _layout = new Layout(this);
    if (!_layout)
        throw OpenChamsException("[ERROR] Cannot create layout.");

    return _layout;
}

bool Circuit::writeToFile(string filePath) {
    ofstream file;
    file.open(filePath.c_str());
    if (!file.is_open()) {
        string error("[ERROR] Cannot open file ");
        error += filePath;
        error += " for writting.";
        throw OpenChamsException(error);
    }
    // checks before do anything
    if (!_netlist) {
        //cerr << "no netlist" << endl; cerr.flush();
        throw OpenChamsException("[ERROR] Cannot writeToFile since no netlist is defined !");
        //return false;
    }    
    if (_netlist->hasNoInstances()) {
        //cerr << "no instances" << endl; cerr.flush();
        throw OpenChamsException("[ERROR] Cannot writeToFile since no instance is defined in netlist !");
        //return false;
    }
    if (_netlist->hasNoNets()) {
        //cerr << "no nets" << endl; cerr.flush();
        throw OpenChamsException("[ERROR] Cannot writeToFile since no net is defined in netlist !");
        //return false;
    }
    
    file << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" << endl
         << "<circuit name=\"" << _name.getString() << "\" techno=\"" << _techno.getString() << "\">" << endl;
    if (_subCircuitsPaths.size() != 0) {
        file << "  <subCircuitsPaths>"  << endl;
        for (size_t i = 0 ; i < _subCircuitsPaths.size() ; i++ ) {
            file << "    <path path=\"" << _subCircuitsPaths[i] << "\"/>" << endl;
        }
        file << "  </subCircuitsPaths>" << endl;
    }
    if (!_params.isEmpty()) {
        file << "  <parameters>" << endl;
    	for (map<Name, double>::const_iterator it = _params.getValues().begin() ; it != _params.getValues().end() ; ++it) {
        	file << "    <parameter name=\"" << (*it).first.getString() << "\" value=\"" << (*it).second << "\"/>" << endl;
    	}
        for (map<Name, string>::const_iterator it = _params.getEqValues().begin() ; it != _params.getEqValues().end() ; ++it) {
            file << "    <parameterEq name=\"" << (*it).first.getString() << "\" equation=\"" << (*it).second << "\"/>" << endl;
        }
        file << "  </parameters>" << endl;
    }
    file << "  <netlist>" << endl
         << "    <instances>" << endl;
    vector<Instance*> instances = _netlist->getInstances();
    sort(instances.begin(), instances.end(), InstanceNameSort); // sort based on instances' names
    for (vector<Instance*>::iterator it = instances.begin() ; it != instances.end() ; ++it) {
        Instance* inst = (*it);
        Device*   dev  = dynamic_cast<Device*>(inst);
        if (inst->hasNoConnectors()) {
            string error("[ERROR] Cannot writeToFile since instance (");
            error += inst->getName().getString();
            error += ") has no connectors !";
            throw OpenChamsException(error);
            //return false;
        }
        if (dev && dev->hasNoTransistors()) {
            string error("[ERROR] Cannot writeToFile since device instance (");
            error += dev->getName().getString();
            error += ") has no transistors !";
            throw OpenChamsException(error);
        }
        if (dev) {
            string sourceBulkStr = (dev->isSourceBulkConnected()) ? "True" : "False";
            file << "      <instance name=\"" << dev->getName().getString() << "\" model=\"" << dev->getModel().getString() << "\" mostype=\"" << dev->getMosType().getString() << "\" sourceBulkConnected=\"" << sourceBulkStr << "\">" << endl;
        } else {
            file << "      <instance name=\"" << inst->getName().getString() << "\" model=\"" << inst->getModel().getString() << "\">" << endl;
        }
        file << "        <connectors>" << endl;
        for (map<Name, Net*>::const_iterator it = inst->getConnectors().begin() ; it != inst->getConnectors().end() ; ++it) {
            file << "          <connector name=\"" << (*it).first.getString() << "\"/>" << endl;
        }
        file << "        </connectors>" << endl;
        if (dev) {
            file << "        <transistors>" << endl;
            for (vector<Transistor*>::const_iterator it = dev->getTransistors().begin() ; it != dev->getTransistors().end() ; ++it ) {
                Transistor* tr = (*it);
                file << "          <transistor name=\"" << tr->getName().getString() << "\">" << endl
                     << "            <connection gate=\"" << tr->getGate().getString() << "\" source=\"" << tr->getSource().getString() << "\" drain=\"" << tr->getDrain().getString() << "\" bulk=\"" << tr->getBulk().getString() << "\"/>" << endl
                     << "          </transistor>" << endl;
            }
            file << "        </transistors>" << endl;
        }
        if (!inst->getParameters().isEmpty()) {
            Parameters params = inst->getParameters();
            file << "        <parameters>" << endl;
            for (map<Name, double>::const_iterator it = params.getValues().begin() ; it != params.getValues().end() ; ++it) {
                file << "          <parameter name=\"" << (*it).first.getString() << "\" value=\"" << (*it).second << "\"/>" << endl;
            }
            file << "        </parameters>" << endl;
        }
		file << "      </instance>" << endl;
    }
    file << "    </instances>" << endl
         << "    <nets>" << endl;
    bool schematicNets = false; // to know if net sections in schematic are needed
    vector<Net*> nets = _netlist->getNets();
    sort(nets.begin(), nets.end(), NetNameSort); // sort based on nets' names
    for (vector<Net*>::iterator it = nets.begin() ; it != nets.end() ; ++it) {
        Net* net = (*it);
        if (net->hasNoConnections()) {
            string error("[ERROR] Cannot writeToFile since net (");
            error += net->getName().getString();
            error += ") has no connectors !";
            throw OpenChamsException(error);
            //return false;
        }
        if (!net->hasNoPorts() || !net->hasNoWires())
            schematicNets = true;
        string externStr = (net->isExternal()) ? "True" : "False";
        file << "      <net name=\"" << net->getName().getString() << "\" type=\"" << net->getType().getString() << "\" isExternal=\"" << externStr << "\">" << endl;
        vector<Net::Connection*> connections = net->getConnections();
        sort(connections.begin(), connections.end(), ConnectionsSort);
        for (vector<Net::Connection*>::iterator it = connections.begin() ; it != connections.end() ; ++it) {
            file << "        <connector instance=\"" << (*it)->getInstanceName().getString() << "\" name=\"" << (*it)->getConnectorName().getString() << "\"/>" << endl;
        }
		file << "      </net>" << endl;
    }
    file << "    </nets>" << endl;
    file << "  </netlist>" << endl;
    if (_schematic && !_schematic->hasNoInstances()) {
        file << "  <schematic>" << endl;
        for (map<Name, Schematic::Infos*>::const_iterator it = _schematic->getInstances().begin() ; it != _schematic->getInstances().end(); ++it ) {
            Schematic::Infos* infos = (*it).second;
            file << "    <instance name=\"" << ((*it).first).getString() << "\" x=\"" << infos->getX() << "\" y=\"" << infos->getY() << "\" orient=\"" << infos->getOrientation().getString() << "\"/>" << endl;
        }
        if (schematicNets) {
            for (size_t i = 0 ; i < nets.size() ; i++) {
                Net* net = nets[i];
                if (net->hasNoPorts() && net->hasNoWires())
                    continue;
                file << "    <net name=\"" << net->getName().getString() << "\">" << endl;
                for (size_t j = 0 ; j < net->getPorts().size() ; j++) {
                    Port* port = net->getPorts()[j];
                    if (!port)
                        continue;
                    file << "      <port type=\"" << port->getType().getString() << "\" idx=\"" << port->getIndex() << "\" x=\"" << port->getX() << "\" y=\"" << port->getY() << "\" orient=\"" << port->getOrientation().getString() << "\"/>" << endl;
                }
                for (size_t j = 0 ; j < net->getWires().size() ; j++) {
                    Wire* wire = net->getWires()[j];
                    file << "      <wire>" << endl;
                    WirePoint* start = wire->getStartPoint();
                    WirePoint* end   = wire->getEndPoint();
                    // start point
                    if (dynamic_cast<InstancePoint*>(start)) {
                        InstancePoint* iP = static_cast<InstancePoint*>(start);
                        file << "        <connector name=\"" << iP->getName().getString() << "\" plug=\"" << iP->getPlug().getString() << "\"/>" << endl;
                    } else if (dynamic_cast<PortPoint*>(start)) {
                        PortPoint* pP = static_cast<PortPoint*>(start);
                        file << "        <connector idx=\"" << pP->getIndex() << "\"/>" << endl;
                    } else {
                        throw OpenChamsException("[ERROR] Wire start point is nor an InstancePoint nor a PortPoint.");
                    }
                    // intermediate points
                    for (size_t k = 0 ; k < wire->getIntermediatePoints().size() ; k++) {
                        IntermediatePoint* iP = wire->getIntermediatePoints()[k];
                        file << "        <point x=\"" << iP->getX() << "\" y=\"" << iP->getY() << "\"/>" << endl;
                    }
                    // end point
                    if (dynamic_cast<InstancePoint*>(end)) {
                        InstancePoint* iP = static_cast<InstancePoint*>(end);
                        file << "        <connector name=\"" << iP->getName().getString() << "\" plug=\"" << iP->getPlug().getString() << "\"/>" << endl;
                    } else if (dynamic_cast<PortPoint*>(end)) {
                        PortPoint* pP = static_cast<PortPoint*>(end);
                        file << "        <connector idx=\"" << pP->getIndex() << "\"/>" << endl;
                    } else {
                        throw OpenChamsException("[ERROR] Wire end point is nor an InstancePoint nor a PortPoint.");
                    }
                    file << "      </wire>" << endl;
                }
                file << "    </net>" << endl;
            }
        }
        file << "  </schematic>" << endl;
    }
    if (_sizing && !_sizing->hasNoOperators()) {
        file << "  <sizing>" << endl;
        for (map<Name, Operator*>::const_iterator it = _sizing->getOperators().begin() ; it != _sizing->getOperators().end() ; ++it) {
            Operator* op = (*it).second;
            string opName = op->getName().getString();
            transform(opName.begin(), opName.end(), opName.begin(), ::toupper);
            file << "    <instance name=\"" << ((*it).first).getString() << "\" operator=\"" << opName << "\" simulModel=\"" << op->getSimulModel().getString() << "\" callOrder=\"" << op->getCallOrder() << "\">" << endl;
            if (!op->hasNoConstraints()) {
                for (map<Name, Operator::Constraint*>::const_iterator cit = op->getConstraints().begin() ; cit != op->getConstraints().end() ; ++cit) {
                    Operator::Constraint* cn = (*cit).second;
                    Name ref = cn->getRef();
                    if (ref == Name("")) {
                        file << "      <constraint param=\"" << ((*cit).first).getString() << "\" refEquation=\"" << cn->getRefParam().getString() << "\" factor=\"" << cn->getFactor() << "\"/>" << endl;
                    } else {
                        file << "      <constraint param=\"" << ((*cit).first).getString() << "\" ref=\"" << cn->getRef().getString() << "\" refParam=\"" << cn->getRefParam().getString() << "\" factor=\"" << cn->getFactor() << "\"/>" << endl;
                    }
                }
            }
            file << "    </instance>" << endl;
        }
        if (!_sizing->hasNoEquations()) {
            file << "    <equations>" << endl;
            for (map<Name, string>::const_iterator it = _sizing->getEquations().begin() ; it != _sizing->getEquations().end() ; ++it) {
                file << "      <eq name=\"" << ((*it).first).getString() << "\" equation=\"" << (*it).second << "\"/>" << endl;
            }
            file << "    </equations>" << endl;
        }
        file << "  </sizing>" << endl;
    }
    if (_layout && !_layout->hasNoInstance()) {
        file << "  <layout>" << endl;
        for (map<Name, Name>::const_iterator it = _layout->getInstances().begin() ; it != _layout->getInstances().end() ; ++it) {
            file << "    <instance name=\"" << ((*it).first).getString() << "\" style=\"" << ((*it).second).getString() << "\"/>" << endl;
        }
        file << "  </layout>" << endl;
    }
    file << "</circuit>" << endl;
    file.close();
	return true;        
}

}

