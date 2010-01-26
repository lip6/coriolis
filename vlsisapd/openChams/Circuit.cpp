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
using namespace std;

#include "Circuit.h"

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
    
Circuit::Circuit(Name name, Name techno) : _name(name), _techno(techno) {}

Name Circuit::readParameter(xmlNode* node, double& value) {
    xmlChar* paramNameC = xmlGetProp(node, (xmlChar*)"name");
    xmlChar* valueC     = xmlGetProp(node, (xmlChar*)"value");
    if (paramNameC && valueC) {
        Name name((const char*)paramNameC);
        value = ::getValue<double>(valueC);
        return name;
    } else {
        cerr << "[ERROR] 'parameter' node must have 'name' and 'value' properties." << endl;
        return Name("");
    }
}
    
Name Circuit::readConnector(xmlNode* node) {
    xmlChar* connectorNameC = xmlGetProp(node, (xmlChar*)"name");
    if (connectorNameC) {
        Name name((const char*)connectorNameC);
        return name;
    } else {
        cerr << "[ERROR] 'connector' node must have 'name' property." << endl;
        return Name("");
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
    Instance* inst = NULL;
    if (iNameC && iModelC && iMOSC) {
        Name instanceName((const char*)iNameC);
        Name modelName((const char*)iModelC);
        Name mosName((const char*)iMOSC);
        inst = new Instance(instanceName, modelName, mosName, netlist);
    } else {
        cerr << "[ERROR] 'instance' node must have 'name', 'model' and 'mos' properties." << endl;
        return inst;
    }

    xmlNode* child = node->children;
    for (xmlNode* node = child; node; node = node->next) {
        if (node->type == XML_ELEMENT_NODE) {
            if (xmlStrEqual(node->name, (xmlChar*)"connectors")) {
                readInstanceConnectors(node, inst);
            } else if (xmlStrEqual(node->name, (xmlChar*)"parameters")) {
                readInstanceParameters(node, inst);
            } else {
                cerr << "[WARNING] Only 'conectors' and 'parameters' nodes are allowed in 'instance', others will be ignored." << endl;
                return NULL;
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
        Name typeName((const char*)nTypeC);
        string externStr((const char*)nExternC);
        bool isExternal = (externStr == "True") ? true : false;
        net = new Net(netName, typeName, isExternal, netlist);
        netlist->addNet(net);
    } else {
        cerr << "[ERROR] 'net' node must have 'name', 'type' and 'isExternal' properties." << endl;
        return net;
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
            cerr << "[ERROR] no instance named \"" << iName.getString() << "\" in connector of net \"" << net->getName().getString() << "\"." << endl;
            return;
        }
        inst->connect(cName, net->getName());
        net->connectTo(iName, cName);
    } else {
        cerr << "[ERROR] 'connector' node must have 'instance' and 'name' properties (for net)." << endl;
    }
}
    
Circuit* Circuit::readFromFile(string filePath) {
    LIBXML_TEST_VERSION;
    Circuit* cir = NULL;
    xmlDoc* doc = xmlReadFile(filePath.c_str(), NULL, 0);
    if (doc == NULL) {
        cerr << "[ERROR] Failed to parse: " << filePath << endl;
        return NULL;
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
            cerr << "[ERROR] 'circuit' node must have 'name' and 'techno' properties." << endl;
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
            }
        }
    }
    return cir;
}
    
bool Circuit::writeToFile(string filePath) {
    ofstream file;
    file.open(filePath.c_str());
    // checks before do anything
    if (!_netlist) {
        cerr << "[ERROR] Cannot writeToFile since no netlist is defined !" << endl;
        return false;
    }    
    if (_netlist->hasNoInstances()) {
        cerr << "[ERROR] Cannot writeToFile since no instance is defined in netlist !" << endl;
        return false;
    }
    if (_netlist->hasNoNets()) {
        cerr << "[ERROR] Cannot writeToFile since no net is defined in netlist !" << endl;
        return false;
    }
    
    file << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>" << endl
         << "<circuit name=\"" << _name.getString() << "\" techno=\"" << _techno.getString() << "\">" << endl;
    if (!_params.isEmpty()) {
        file << "  <parameters>" << endl;
    	for (map<Name, double>::iterator it = _params.getFirstIt() ; it != _params.getLastIt() ; ++it) {
        	file << "    <parameter name=\"" << (*it).first.getString() << "\" value=\"" << (*it).second << "\"/>" << endl;
    	}
        file << "  </parameters>" << endl;
    }
    file << "  <netlist>" << endl
         << "    <instances>" << endl;
    for (vector<Instance*>::iterator it = _netlist->getFirstInstanceIt() ; it != _netlist->getLastInstanceIt() ; ++it) {
        Instance* inst = (*it);
        if (inst->hasNoConnectors()) {
            cerr << "[ERROR] Cannot writeToFile since instance (" << inst->getName().getString() << ") has no connectors !" << endl;
            return false;
        }
        file << "      <instance name=\"" << inst->getName().getString() << "\" model=\"" << inst->getModel().getString() << "\" mostype=\"" << inst->getMosType().getString() << "\">" << endl;
        file << "        <connectors>" << endl;
        for (map<Name, Net*>::iterator it = inst->getFirstConnectorIt() ; it != inst->getLastConnectorIt() ; ++it) {
            file << "          <connector name=\"" << (*it).first.getString() << "\"/>" << endl;
        }
        file << "        </connectors>" << endl;
        if (!inst->getParameters().isEmpty()) {
            Parameters params = inst->getParameters();
            file << "        <parameters>" << endl;
            for (map<Name, double>::iterator it = params.getFirstIt() ; it != params.getLastIt() ; ++it) {
                file << "          <parameter name=\"" << (*it).first.getString() << "\" value=\"" << (*it).second << "\"/>" << endl;
            }
            file << "        </parameters>" << endl;
        }
		file << "      </instance>" << endl;
    }
    file << "    </instances>" << endl
         << "    <nets>" << endl;
    for (vector<Net*>::iterator it = _netlist->getFirstNetIt() ; it != _netlist->getLastNetIt() ; ++it) {
        Net* net = (*it);
        if (net->hasNoConnectors()) {
            cerr << "[ERROR] Cannot writeToFile since net (" << net->getName().getString() << ") has no connectors !" << endl;
            return false;
        }
        string externStr = (net->isExternal()) ? "True" : "False";
        file << "      <net name=\"" << net->getName().getString() << "\" type=\"" << net->getType().getString() << "\" isExternal=\"" << externStr << "\">" << endl;
        for (vector<pair<Name, Name> >::iterator it = net->getFirstConnectionIt() ; it != net->getLastConnectionIt() ; ++it) {
            file << "        <connector instance=\"" << (*it).first.getString() << "\" name=\"" << (*it).second.getString() << "\"/>" << endl;
        }
		file << "      </net>" << endl;
    }
    file << "    </nets>" << endl;
    file << "  </netlist>" << endl;
    file << "</circuit>" << endl;
    file.close();
	return true;        
}

}

