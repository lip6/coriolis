#include<iostream>
#include<fstream>
#include<sstream>
using namespace std;

#include "vlsisapd/liberty/Timing.h"

namespace LIB {
Timing::Timing(): _attributes() {};

Attribute* Timing::getAttribute(Name attrName) {
    Attribute* attr = NULL;
    map<Name, Attribute*>::iterator it = _attributes.find(attrName);
    if (it == _attributes.end()) {
        cerr << "[ERROR] Timing has no attribute named " << attrName.getString() << endl;
        exit(1);
    }
    attr= (*it).second;
    return attr;
}

void Timing::addAttribute(Name attrName, Attribute::Type attrType, const string& attrValue) {
    Attribute* attr = new Attribute(attrName, attrType, attrValue);
    map<Name, Attribute*>::iterator it = _attributes.find(attrName);
    if (it != _attributes.end()) {
        cerr << "[ERROR] Cannot define Timing with several attributes with the same name : " << attrName.getString() << endl;
        exit(1);
    }
    _attributes[attrName] = attr;
}

const string Timing::getString() const{
    // Timing's attributes
    ostringstream chaine;
    chaine << "|         Timing's attributes :" << endl;
    for(map<Name, Attribute*>::const_iterator it=_attributes.begin() ; it!=_attributes.end() ; ++it) {
        chaine << "|           name= " + (*it).first.getString()
               << ", type= " + (*it).second->typeToString()
               << ", value= " + (*it).second->valueAsString() << endl;
    }
    return chaine.str();
}

void Timing::print() {
    cout << Timing::getString();
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
