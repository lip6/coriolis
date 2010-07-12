/*
 *  Techno.cpp
 *  DTR
 *
 *  Created by damien dupuis on 01/04/10.
 *  Copyright 2010 UPMC / LIP6. All rights reserved.
 *
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
using namespace std;

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "vlsisapd/dtr/Techno.h"
#include "vlsisapd/dtr/Rules.h"   
#include "vlsisapd/dtr/DTRException.h"

namespace {
	template<class T> T getValue(xmlChar* str) {
	    std::istringstream iss;
    	iss.str((const char*) str);
	    T res;
    	iss >> res;
    	return res;
	}
}

namespace DTR {
Techno::Techno(Name name, Name unit) : _name(name), _unit(unit) {}

Rule* Techno::addRule (Name name, double value, Name ref, Name layer1, Name layer2) {
    Rule* rule = new Rule(name, value, ref, layer1, layer2);
    _rules.push_back(rule);
    return rule;
}

ARule* Techno::addARule (Name name, double value, Name ref, Name layer1, Name layer2) {
    ARule* arule = new ARule(name, value, ref, layer1, layer2);
    _rules.push_back(arule);
    return arule;
}

Rule* Techno::getRule(Name name, Name layer1, Name layer2) {
    //bool testL1   = (layer1 == Name("")) ? false : true;
    //bool testL2   = (layer2 == Name("")) ? false : true;

    for (size_t i = 0 ; i < _rules.size() ; i++) {
        Rule* rule = _rules[i];
    //    if (rule->getName() == name) {
    //        if (testL1) {
    //            if (rule->getLayer1() == layer1) {
    //                if (testL2) {
    //                    if (rule->getLayer2() == layer2) {
    //                        return rule;
    //                    }
    //                } else {
    //                    return rule;
    //                }
    //            }
    //        } else {
    //            return rule;
    //        }
    //    }
        if ((rule->getName() == name) && (rule->getLayer1() == layer1) && (rule->getLayer2() == layer2))
            return rule;
    }
    string error ("[ERROR] Could not find rule: ");
    error += name.getString();
    error += ".";
    error += layer1.getString();
    error += ".";
    error += layer2.getString();
    error += ".";
    throw DTRException(error);
}

double Techno::getValue(Name name, Name layer1, Name layer2) {
    return getRule(name, layer1, layer2)->getValue();
}

const string& Techno::getValueAsString(Name name, Name layer1, Name layer2) {
    return getRule(name, layer1, layer2)->getValueAsString();
}

Techno* Techno::readFromFile(const string filePath) {
    LIBXML_TEST_VERSION;
    Techno* techno = NULL;
    xmlDoc* doc = xmlReadFile(filePath.c_str(), NULL, 0);
    if (doc == NULL) {
        string error ("[ERROR] Failed to parse: ");
        error += filePath;
        throw DTRException(error);
        //return NULL;
    }
    xmlNode* rootElement = xmlDocGetRootElement(doc);
    if (rootElement->type == XML_ELEMENT_NODE && xmlStrEqual(rootElement->name, (xmlChar*)"technology")) {
        xmlChar* technoNameC = xmlGetProp(rootElement, (xmlChar*)"name");
        xmlChar* technoUnitC = xmlGetProp(rootElement, (xmlChar*)"unit");
        
        if (technoNameC && technoUnitC) {
            Name name ((const char*)technoNameC);
            Name unit ((const char*)technoUnitC);
            techno = new Techno(name, unit);
        } else {
            throw DTRException("[ERROR] 'technology' node must have 'name' and 'unit' properties.");
            return NULL;
        }

        
        xmlNode* child = rootElement->children;
        for (xmlNode* node = child; node; node = node->next) {
            if (node->type == XML_ELEMENT_NODE) {
                if (!xmlStrEqual(node->name, (xmlChar*)"physical_rules")) {
                    throw DTRException("[ERROR] only 'physical_rules' node is allowed under 'technology' node.");
                    return NULL;
                }
                for (xmlNode* subnode = node->children ; subnode ; subnode = subnode->next) {
                    if (subnode->type == XML_ELEMENT_NODE) {
                        if (xmlStrEqual(subnode->name, (xmlChar*)"rule")) {
                            xmlChar* nameC   = xmlGetProp(subnode, (xmlChar*)"name"  );
                            xmlChar* layerC  = xmlGetProp(subnode, (xmlChar*)"layer" );
                            xmlChar* layer1C = xmlGetProp(subnode, (xmlChar*)"layer1");
                            xmlChar* layer2C = xmlGetProp(subnode, (xmlChar*)"layer2");
                            xmlChar* valueC  = xmlGetProp(subnode, (xmlChar*)"value" );
                            xmlChar* refC    = xmlGetProp(subnode, (xmlChar*)"ref"   );
                            xmlChar* typeC   = xmlGetProp(subnode, (xmlChar*)"type"  );

                            Rule* rule = NULL;
                            if (nameC && layer1C && layer2C && valueC && refC) { // rule with two layers
                                Name name   ((const char*)nameC);
                                Name ref    ((const char*)refC);
                                Name layer1 ((const char*)layer1C);
                                Name layer2 ((const char*)layer2C);
                                double value = ::getValue<double>(valueC);
                                rule = techno->addRule(name, value, ref, layer1, layer2);
                            } else if (nameC && layerC && valueC && refC) {// rule with only one layer
                                Name name  ((const char*)nameC);
                                Name ref   ((const char*)refC);
                                Name layer ((const char*)layerC);
                                double value = ::getValue<double>(valueC);
                                rule = techno->addRule(name, value, ref, layer);
                            } else if (nameC && valueC && refC) { // rule without layer
                                Name name ((const char*)nameC);
                                Name ref  ((const char*)refC);
                                double value = ::getValue<double>(valueC);
                                rule = techno->addRule(name, value, ref);
                            } else { // invalid case
                                throw DTRException("[ERROR] properties of 'rule' node must be ('name', 'value', 'ref') or ('name', 'layer', 'value', 'ref') or ('name', 'layer1', 'layer2', 'value', 'ref').");
                                return NULL;
                            }
                            if (typeC) {
                                Name type ((const char*)typeC);
                                rule->setType(type);
                            }
                        } else if (xmlStrEqual(subnode->name, (xmlChar*)"arule")) {
                            xmlChar* nameC   = xmlGetProp(subnode, (xmlChar*)"name"  );
                            xmlChar* layer1C = xmlGetProp(subnode, (xmlChar*)"layer1");
                            xmlChar* layer2C = xmlGetProp(subnode, (xmlChar*)"layer2");
                            xmlChar* valueC  = xmlGetProp(subnode, (xmlChar*)"value" );
                            xmlChar* refC    = xmlGetProp(subnode, (xmlChar*)"ref"   );
                            if (nameC && layer1C && layer2C && valueC && refC) {
                                Name name   ((const char*)nameC);
                                Name layer1 ((const char*)layer1C);
                                Name layer2 ((const char*)layer2C);
                                Name ref    ((const char*)refC);
                                double value = ::getValue<double>(valueC);
                                techno->addARule(name, value, ref, layer1, layer2);
                            } else {
                                throw DTRException("[ERROR] 'arule' node must have 'name', 'layer1', 'layer2', 'value' and 'ref' properties.");
                                return NULL;
                            }
                        } else {
                            throw DTRException("[ERROR] only 'rule' and 'arule' nodes are allowed under 'physical_rules' node.");
                        }
                    }
                }
            }
        }
    }
    return techno;
}
    
bool Techno::writeToFile(string filePath) {
    ofstream file;
    file.open(filePath.c_str());
    if (!file.is_open()) {
        string error("[ERROR] Cannot open file ");
        error += filePath;
        error += " for writting.";
        throw DTRException(error);
    }
    // checks before do anything
    if (_rules.size() == 0) {
        throw DTRException("[ERROR] Cannot writeToFile since no rule is defined!");
    }    
    
    file << "<technology name=\"" << _name.getString() << "\" unit=\"" << _unit.getString() << "\">" << endl
         << "  <physical_rules>" << endl;

    for (size_t i = 0 ; i < _rules.size() ; i++) {
        Rule* rule = _rules[i];
        if (dynamic_cast<ARule*>(rule)) {
            file << "    <arule name=\"" << rule->getName().getString() << "\" ";
        } else {
            file << "    <rule name=\"" << rule->getName().getString() << "\" ";
        }
        if (rule->getType() != Name("")) {
            file << "type=\"" << rule->getType().getString() << "\" ";
        }
        if (rule->getLayer1() != Name("")) {
            if (rule->getLayer2() != Name("")) {
                file << "layer1=\"" << rule->getLayer1().getString() << "\" layer2=\"" << rule->getLayer2().getString() << "\" ";
            } else {
                file << "layer=\"" << rule->getLayer1().getString() << "\" ";
            }
        }
        file << "value=\"" << rule->getValue() << "\" ref=\"" << rule->getRef().getString() << "\"/>" << endl;
    }
    file << "  </physical_rules>" << endl
         << "</technology>" << endl;
    file.close();
	return true;        
}

}

