#include<cstdlib>

#include<iostream>
#include<fstream>
#include<sstream>
using namespace std;

#include "vlsisapd/liberty/FlipFlop.h"

namespace LIB {
FlipFlop::FlipFlop(Name noninverting, Name inverting): _noninverting(noninverting), _inverting(inverting), _attributes() {};

Attribute* FlipFlop::getAttribute(Name attrName) {
    Attribute* attr = NULL;
    map<Name, Attribute*>::iterator it = _attributes.find(attrName);
    if (it == _attributes.end()) {
        cerr << "[ERROR] FlipFLop has no attribute named " << attrName.getString() << endl;
        exit(1);
    }
    attr= (*it).second;
    return attr;
}

void FlipFlop::addAttribute(Name attrName, Attribute::Type attrType, const string& attrValue) {
    Attribute* attr = new Attribute(attrName, attrType, attrValue);
    map<Name, Attribute*>::iterator it = _attributes.find(attrName);
    if (it != _attributes.end()) {
        cerr << "[ERROR] Cannot define FlipFlop with several attributes with the same name: " << attrName.getString() << endl;
        exit(1);
    }
    _attributes[attrName] = attr;
}

const string FlipFlop::getString() const{
    ostringstream chaine;
    chaine << "|       FF noninverting= " << _noninverting.getString() << ", inverting= " << _inverting.getString() << endl;

    // FlipFlop's attributes
    chaine << "|         Attributes :" << endl;
    for(map<Name, Attribute*>::const_iterator it=_attributes.begin() ; it!=_attributes.end() ; ++it) {
        chaine << "|           name= " << (*it).first.getString()
               << ", type= " << (*it).second->typeToString()
               << ", value= " << (*it).second->valueAsString() << endl;
    }
    return chaine.str();
}

void FlipFlop::print() {
    cout << FlipFlop::getString();
}

bool FlipFlop::write(ofstream &file) {
    file << "        ff(\"" << _noninverting.getString() << "\",\"" <<  _inverting.getString() << "\") {" << endl;
    for(map<Name, Attribute*>::const_iterator it=_attributes.begin() ; it!=_attributes.end() ; ++it) {
        file << "            ";
        (*it).second->write(file);
    }
    file << "        }" << endl;
    return true;
}

} // namespace
