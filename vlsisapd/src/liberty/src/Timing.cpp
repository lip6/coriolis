#include<iostream>
#include<fstream>
using namespace std;

#include "vlsisapd/liberty/Name.h"
#include "vlsisapd/liberty/Attribute.h"
#include "vlsisapd/liberty/Timing.h"

namespace LIB {
Timing::Timing(): _attributes() {};

void Timing::addAttribute(Name attrName, Attribute::Type attrType, string& attrValue) {
    Attribute* attr = new Attribute(attrName, attrType, attrValue);
    map<Name, Attribute*>::iterator it = _attributes.find(attrName);
    if (it != _attributes.end()) {
        cerr << "[ERROR] Cannot define Timing with several attributes with the same name : " << attrName.getString() << endl;
        exit(1);
    }
    _attributes[attrName] = attr;
}

void Timing::print() {
    // Timing's attributes
    cout << "|         Timing's attributes :" << endl;
    for(map<Name, Attribute*>::const_iterator it=_attributes.begin() ; it!=_attributes.end() ; ++it) {
        cout << "|           name= " << (*it).first.getString()
             << ", type= " << (*it).second->typeToString((*it).second->getType())
             << ", value= " << (*it).second->valueAsString() << endl;
    }
}

bool Timing::write(ofstream &file) {
    file << "            timing() {" << endl;
    // Timing's attributes
    for(map<Name, Attribute*>::const_iterator it=_attributes.begin() ; it!=_attributes.end() ; ++it) {
        file << "                ";
        (*it).second->write(file);
    }
    file << "            }" << endl;
    return true;
}

} // namespace
