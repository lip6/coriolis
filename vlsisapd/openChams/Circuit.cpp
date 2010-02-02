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

#include "Circuit.h"
#include "OpenChamsException.h"

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
    
static bool readCircuitParametersDone = false;
static bool readNetListDone = false;
static bool readInstancesDone = false;
static bool readNetsDone = false;
static bool readSchematicDone = false;
    
Circuit::Circuit(Name name, Name techno) : _name(name), _techno(techno), _netlist(NULL), _schematic(NULL) {
    readCircuitParametersDone = false;
    readNetListDone           = false;
    readInstancesDone         = false;
    readNetsDone              = false;
    readSchematicDone         = false;
}

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
    
Name Circuit::readParameter(xmlNode* node, double& value) {
    xmlChar* paramNameC = xmlGetProp(node, (xmlChar*)"name");
    xmlChar* valueC     = xmlGetProp(node, (xmlChar*)"value");
    if (paramNameC && valueC) {
        Name name((const char*)paramNameC);
        value = ::getValue<double>(valueC);
        return name;
    } else {
        throw OpenChamsException("[ERROR] 'parameter' node must have 'name' and 'value' properties.");
        //return Name("");
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
                } else {
                    cerr << "[WARNING] Only 'parameter' nodes are authorized under 'parameters' node." << endl;
                    return;
                }
            }
        }
    }     
    readCircuitParametersDone = true;
}

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
    
void Circuit::readInstances(xmlNode* node, Netlist* netlist) {
    if (readInstancesDone) {
        cerr << "[WARNING] Only one 'instances' node is allowed in 'netlist', others will be ignored." << endl;
        return;
    }
    xmlNode* child = node->children;
    for (xmlNode* node = child; node; node = node->next) {
        if (node->type == XML_ELEMENT_NODE) {
            if (xmlStrEqual(node->name, (xmlChar*)"instance")) {
                Instance* inst =readInstance(node, netlist);
                if (inst)
                	netlist->addInstance(inst);
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
    if (iNameC && iModelC && iMOSC && iSBCC) {
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
        inst = new Instance(instanceName, modelName, Name(mosStr), sourceBulkConnected, netlist);
    } else {
        throw OpenChamsException("[ERROR] 'instance' node must have 'name', 'model', 'mostype' and 'sourceBulkConnected' properties.");
        //return inst;
    }

    xmlNode* child = node->children;
    for (xmlNode* node = child; node; node = node->next) {
        if (node->type == XML_ELEMENT_NODE) {
            //if (xmlStrEqual(node->name, (xmlChar*)"connectors")) {
            //    readInstanceConnectors(node, inst);
            //} else
            if (xmlStrEqual(node->name, (xmlChar*)"parameters")) {
                readInstanceParameters(node, inst);
            } else {
                cerr << "[WARNING] Only 'parameters' node is allowed in 'instance', others will be ignored." << endl;
                //cerr << "[WARNING] Only 'conectors' and 'parameters' nodes are allowed in 'instance', others will be ignored." << endl;
            }
        }
    }
    return inst;
}
 
/*
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
*/
    
void Circuit::readInstanceParameters(xmlNode* node, Instance* inst) {
    xmlNode* child = node->children;
    for (xmlNode* node = child; node; node = node->next) {
        if (node->type == XML_ELEMENT_NODE) {
            if (xmlStrEqual(node->name, (xmlChar*)"parameter")) {
                double value = 0.0;
                Name paramName = readParameter(node, value);
                if (paramName == Name("")) return; // error
                inst->addParameter(paramName, value);
            }
        }
    }
}
    
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
        net = new Net(netName, Name(typeStr), isExternal, netlist);
        netlist->addNet(net);
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
                cerr << "[WARNING] Only 'conector' nodes are allowed in 'net', others will be ignored." << endl;
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
    
void Circuit::readSchematic(xmlNode* node) {
    if (readSchematicDone) {
        cerr << "[WARNING] Only one 'schematic' node is allowed in circuit, others will be ignored." << endl;
        return;
    }
    xmlChar* zoomC = xmlGetProp(node, (xmlChar*)"zoom");
    double zoom = 1.0;
    if (zoomC) {
        zoom = ::getValue<double>(zoomC);
    } else {
        throw OpenChamsException("[ERROR] 'schematic' node must have 'zoom' property.");
    }

    Schematic* schematic = new Schematic(this, zoom);
    xmlNode* child = node->children;
    for (xmlNode* node = child; node; node = node->next) {
        if (node->type == XML_ELEMENT_NODE) {
            if (xmlStrEqual(node->name, (xmlChar*)"instance")) {
                readInstanceSchematic(node, schematic);
            } else {
                cerr << "[WARNING] Only 'instance' nodes are allowed in 'schematic', others will be ignored." << endl;
            }
        }
    }
    readSchematicDone = true;
    _schematic = schematic;
}
    
void Circuit::readInstanceSchematic(xmlNode* node, Schematic* schematic) {
    xmlChar* nameC = xmlGetProp(node, (xmlChar*)"name");
    xmlChar* xC    = xmlGetProp(node, (xmlChar*)"x");
    xmlChar* yC    = xmlGetProp(node, (xmlChar*)"y");
    xmlChar* symC  = xmlGetProp(node, (xmlChar*)"sym");
    if (nameC && xC && yC && symC) {
        Name   iName((const char*)nameC);
        double x = ::getValue<double>(xC);
        double y = ::getValue<double>(yC);
        string symStr((const char*)symC);
        string symComp[8] = {"ID", "R1", "R2", "R3", "MX", "XR", "MY", "YR"};
    	vector<string> symComps (symComp, symComp+8);
        check_uppercase(symStr, symComps, "[ERROR] In 'schematic/instance', 'sym' must be 'ID', 'R1', 'R2', 'R3', 'MX', 'XR', 'MY' or 'YR'.");
        schematic->addInstance(iName, x, y, Name(symStr));
    } else {
        throw OpenChamsException("[ERROR] 'instance' node in 'schematic' must have 'name', 'x', 'y' and 'sym' properties.");
    }
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

        
        xmlNode* child = rootElement->children;
        for (xmlNode* node = child; node; node = node->next) {
            if (node->type == XML_ELEMENT_NODE) {
                if (xmlStrEqual(node->name, (xmlChar*)"parameters")) {
                    cir->readCircuitParameters(node);
                }
                if (xmlStrEqual(node->name, (xmlChar*)"netlist")) {
                    cir->readNetList(node);
                }
                if (xmlStrEqual(node->name, (xmlChar*)"schematic")) {
                    cir->readSchematic(node);
                }
            }
        }
    }
    if (!readNetListDone) {
        throw OpenChamsException("[ERROR] no <netlist> section was found in parsed file !");
    }
    return cir;
}
    
bool Circuit::writeToFile(string filePath) {
    ofstream file;
    file.open(filePath.c_str());
    // checks before do anything
    if (!_netlist) {
        cerr << "no netlist" << endl; cerr.flush();
        throw OpenChamsException("[ERROR] Cannot writeToFile since no netlist is defined !");
        //return false;
    }    
    if (_netlist->hasNoInstances()) {
        cerr << "no instances" << endl; cerr.flush();
        throw OpenChamsException("[ERROR] Cannot writeToFile since no instance is defined in netlist !");
        //return false;
    }
    if (_netlist->hasNoNets()) {
        cerr << "no nets" << endl; cerr.flush();
        throw OpenChamsException("[ERROR] Cannot writeToFile since no net is defined in netlist !");
        //return false;
    }
    
    file << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" << endl
         << "<circuit name=\"" << _name.getString() << "\" techno=\"" << _techno.getString() << "\">" << endl;
    if (!_params.isEmpty()) {
        file << "  <parameters>" << endl;
    	for (map<Name, double>::const_iterator it = _params.getValues().begin() ; it != _params.getValues().end() ; ++it) {
        	file << "    <parameter name=\"" << (*it).first.getString() << "\" value=\"" << (*it).second << "\"/>" << endl;
    	}
        file << "  </parameters>" << endl;
    }
    file << "  <netlist>" << endl
         << "    <instances>" << endl;
    for (vector<Instance*>::const_iterator it = _netlist->getInstances().begin() ; it != _netlist->getInstances().end() ; ++it) {
        Instance* inst = (*it);
        /*if (inst->hasNoConnectors()) {
            string error("[ERROR] Cannot writeToFile since instance (");
            error += inst->getName().getString();
            error += ") has no connectors !";
            throw OpenChamsException(error);
            //return false;
        }*/
        string sourceBulkStr = (inst->isSourceBulkConnected()) ? "True" : "False";
        file << "      <instance name=\"" << inst->getName().getString() << "\" model=\"" << inst->getModel().getString() << "\" mostype=\"" << inst->getMosType().getString() << "\" sourceBulkConnected=\"" << sourceBulkStr << "\">" << endl;
        /*file << "        <connectors>" << endl;
        for (map<Name, Net*>::const_iterator it = inst->getConnectors().begin() ; it != inst->getConnectors().end() ; ++it) {
            file << "          <connector name=\"" << (*it).first.getString() << "\"/>" << endl;
        }
        file << "        </connectors>" << endl;*/
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
    for (vector<Net*>::const_iterator it = _netlist->getNets().begin() ; it != _netlist->getNets().end() ; ++it) {
        Net* net = (*it);
        if (net->hasNoConnectors()) {
            string error("[ERROR] Cannot writeToFile since net (");
            error += net->getName().getString();
            error += ") has no connectors !";
            throw OpenChamsException(error);
            //return false;
        }
        string externStr = (net->isExternal()) ? "True" : "False";
        file << "      <net name=\"" << net->getName().getString() << "\" type=\"" << net->getType().getString() << "\" isExternal=\"" << externStr << "\">" << endl;
        for (vector<pair<Name, Name> >::const_iterator it = net->getConnections().begin() ; it != net->getConnections().end() ; ++it) {
            file << "        <connector instance=\"" << (*it).first.getString() << "\" name=\"" << (*it).second.getString() << "\"/>" << endl;
        }
		file << "      </net>" << endl;
    }
    file << "    </nets>" << endl;
    file << "  </netlist>" << endl;
    if (_schematic && !_schematic->hasNoInstances()) {
        file << "  <schematic zoom=\"" << _schematic->getZoom() << "\">" << endl;
        for (map<Name, Schematic::Infos*>::const_iterator it = _schematic->getInstances().begin() ; it != _schematic->getInstances().end(); ++it ) {
            Schematic::Infos* infos = (*it).second;
            file << "    <instance name=\"" << ((*it).first).getString() << "\" x=\"" << infos->getX() << "\" y=\"" << infos->getY() << "\" sym=\"" << infos->getSymetry().getString() << "\"/>" << endl;
        }
        file << "  </schematic>" << endl;
    }
    file << "</circuit>" << endl;
    file.close();
	return true;        
}

}

