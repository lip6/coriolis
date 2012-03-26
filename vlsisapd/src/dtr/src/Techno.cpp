
// -*- C++ -*-
//
// This file is part of the VLSI SAPD Software.
// Copyright (c) UPMC/LIP6 2010-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                  V L S I   S A P D                              |
// |           Design Technological Rules (DTR)                      |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Techno.cpp"                                  |
// +-----------------------------------------------------------------+


#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <limits>
using namespace std;

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "vlsisapd/dtr/Techno.h"
#include "vlsisapd/dtr/Rules.h"   
#include "vlsisapd/dtr/DTRException.h"


namespace {


  template<class T> T getValue(xmlChar* str)
  {
    if (string((const char*)str).empty()) 
      return numeric_limits<T>::quiet_NaN();
    std::istringstream iss;
    iss.str((const char*) str);
    T res;
    iss >> res;
    return res;
  }


}  // Anonymous namespace.


namespace DTR {


  Techno::Techno ( const string& name, const string& unit, const string& version )
    : _name   (name)
    , _unit   (unit)
    , _version(version)
  { }


  Techno::Techno ( const char* name, const char* unit, const char* version )
    : _name   (name)
    , _unit   (unit)
    , _version((version) ? version : "")
  {
  // Cannot initialize a std::string from a NULL pointer.
  }


  Rule* Techno::addRule ( const char* name
                        , double      value
                        , const char* ref
                        , const char* layer1
                        , const char* layer2 )
  {
    Rule* rule = new Rule(name, value, ref, layer1, layer2);
    _rules.push_back(rule);
    return rule;
  }


  ARule* Techno::addARule ( const char* name
                          , double      value
                          , const char* ref
                          , const char* layer1
                          , const char* layer2 )
  {
    ARule* arule = new ARule(name, value, ref, layer1, layer2);
    _rules.push_back(arule);
    return arule;
  }


  Rule* Techno::getRule ( const char* name, const char* layer1, const char* layer2 )
  {
    for (size_t i = 0 ; i < _rules.size() ; i++) {
      Rule* rule = _rules[i];
      if ((rule->getName() == name) && (rule->getLayer1() == layer1) && (rule->getLayer2() == layer2))
        return rule;
    }
    return NULL;
  }


  double Techno::getValue ( const char* name, const char* layer1, const char* layer2 )
  {
    Rule* r =  getRule(name, layer1, layer2);
    if(!r) {
      ostringstream error;
      error << "[ERROR] Could not find rule: " << name << "." << layer1 << "." << layer2 << ".";
      throw DTRException(error.str());
    }
    return r->getValue();
  }


  const string& Techno::getValueAsString ( const char* name, const char* layer1, const char* layer2 )
  {
    Rule* r =  getRule(name, layer1, layer2);
    if(!r) {
      ostringstream error;
      error << "[ERROR] Could not find rule: " << name << "." << layer1 << "." << layer2 << ".";
      throw DTRException(error.str());
    }
    return r->getValueAsString();
  }


  Techno* Techno::readFromFile ( const char* filePath )
  {
    LIBXML_TEST_VERSION;

    Techno* techno = NULL;
    xmlDoc* doc = xmlReadFile(filePath, NULL, 0);
    if (doc == NULL) {
      string error ("[ERROR] Failed to parse: ");
      error += filePath;
      throw DTRException(error);
    }

    xmlNode* rootElement = xmlDocGetRootElement(doc);
    if (rootElement->type == XML_ELEMENT_NODE && xmlStrEqual(rootElement->name, (xmlChar*)"technology")) {
      xmlChar* technoNameC = xmlGetProp(rootElement, (xmlChar*)"name");
      xmlChar* technoUnitC = xmlGetProp(rootElement, (xmlChar*)"unit");
      xmlChar* technoVersC = xmlGetProp(rootElement, (xmlChar*)"version");
        
      if (technoNameC && technoUnitC) {
        techno = new Techno((char*)technoNameC, (char*)technoUnitC, (char*)technoVersC);
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
                if (nameC and layer1C and layer2C and valueC and refC) {
                // Rule with two layers
                  rule = techno->addRule( (char*)nameC
                                        , ::getValue<double>(valueC)
                                        , (char*)refC
                                        , (char*)layer1C
                                        , (char*)layer2C );
                } else if (nameC and layerC and valueC and refC) {
                // Rule with only one layer
                  rule = techno->addRule( (char*)nameC
                                        , ::getValue<double>(valueC)
                                        , (char*)refC
                                        , (char*)layerC );
                } else if (nameC and valueC and refC) {
                // Rule without layer
                  rule = techno->addRule( (char*)nameC
                                        , ::getValue<double>(valueC)
                                        , (char*)refC );
                } else {
                // Invalid case
                  throw DTRException("[ERROR] properties of 'rule' node must be ('name', 'value', 'ref') or ('name', 'layer', 'value', 'ref') or ('name', 'layer1', 'layer2', 'value', 'ref').");
                  return NULL;
                }

                if (typeC) {
                  rule->setType((const char*)typeC);
                }
              } else if (xmlStrEqual(subnode->name, (xmlChar*)"arule")) {
                xmlChar* nameC   = xmlGetProp(subnode, (xmlChar*)"name"  );
                xmlChar* layer1C = xmlGetProp(subnode, (xmlChar*)"layer1");
                xmlChar* layer2C = xmlGetProp(subnode, (xmlChar*)"layer2");
                xmlChar* valueC  = xmlGetProp(subnode, (xmlChar*)"value" );
                xmlChar* refC    = xmlGetProp(subnode, (xmlChar*)"ref"   );

                if (nameC && layer1C && layer2C && valueC && refC) {
                  techno->addARule( (char*)nameC
                                  , ::getValue<double>(valueC)
                                  , (char*)refC
                                  , (char*)layer1C
                                  , (char*)layer2C );
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
    

  bool Techno::writeToFile ( const char* filePath )
  {
    ofstream file;
    file.open(filePath);
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

    file << "<technology name=\"" << _name << "\" unit=\"" << _unit << "\" version=\"" << _version << "\">" << endl
         << "  <physical_rules>" << endl;

    file.setf(ios::left, ios::adjustfield);
    for (size_t i = 0 ; i < _rules.size() ; i++) {
      Rule* rule = _rules[i];
      string name = "\""+rule->getName()+"\"";
      if (dynamic_cast<ARule*>(rule)) {
        file << "    <arule name=" << setw(20) << name  << " ";
      } else {
        file << "    <rule  name=" << setw(20) << name << " ";
      }
      int spacing = 18;
      if (not rule->getType().empty()) {
        string type = "\""+rule->getType()+"\"";
        file << "type=" << setw(12) << type << " ";
        spacing = 0;
      }
      if (not rule->getLayer1().empty()) {
        string l1 = "\""+rule->getLayer1()+"\"";
        if (not rule->getLayer2().empty()) {
          string l2 = "\""+rule->getLayer2()+"\"";
          file << "layer1=" << setw(10) << l1 << " layer2=" << setw(10) << l2 << " ";
        } else {
          file << "layer=" << setw(11+spacing) << l1 << " ";
        }
      } else {
        file << setw(36) << " ";
      }

      ostringstream oss;
      oss << "\"" << rule->getValue() <<"\"";
      file << "value=" << setw(7) << oss.str() << " ref=\"" << rule->getRef() << "\"/>" << endl;
    }
    file << "  </physical_rules>" << endl
         << "</technology>" << endl;
    file.close();
	return true;        
  }


}  // DTR namespace.

