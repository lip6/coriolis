#include<cstdlib>

#include<iostream>
#include<fstream>
#include<sstream>
using namespace std;

#include "vlsisapd/liberty/Timing.h"
#include "vlsisapd/liberty/Pin.h"

namespace LIB {
Pin::Pin(Name name): _name(name), _attributes(), _timings() {};

Attribute* Pin::getAttribute(Name attrName) {
    Attribute* attr = NULL;
    map<Name, Attribute*>::iterator it = _attributes.find(attrName);
    if (it == _attributes.end()) {
        cerr << "[ERROR] Pin " << _name.getString() << " has no attribute named " << attrName.getString() << endl;
        exit(1);
    }
    attr= (*it).second;
    return attr;
}

Timing* Pin::getTiming(Name pinName) {
    for (size_t i = 0 ; i < _timings.size() ; i++) {
        if (pinName.getString() == _timings[i]->getAttribute(Name("related_pin"))->valueAsString()) {
            return _timings[i];
        }
    }
    cerr << "[ERROR] Pin " << _name.getString() << " has no timing related to pin named " << pinName.getString() << endl;
    exit(1);
}

void Pin::addAttribute(Name attrName, Attribute::Type attrType, const string& attrValue) {
    Attribute* attr = new Attribute(attrName, attrType, attrValue);
    map<Name, Attribute*>::iterator it = _attributes.find(attrName);
    if (it != _attributes.end()) {
        cerr << "[ERROR] Cannot define Pin " << _name.getString() << " with several attributes with the same name: " << attrName.getString() << endl;
        exit(1);
    }
    _attributes[attrName] = attr;
}

void Pin::addTiming() {
    Timing* timing = new Timing();
    _timings.push_back(timing);
}

const string Pin::getString() const{
    ostringstream chaine;
    chaine << "|       Pin name= " << _name.getString() << endl;

    // Pin's attributes
    chaine << "|         Attributes :" << endl;
    for(map<Name, Attribute*>::const_iterator it=_attributes.begin() ; it!=_attributes.end() ; ++it) {
        chaine << "|           name= " << (*it).first.getString()
               << ", type= " << (*it).second->typeToString()
               << ", value= " << (*it).second->valueAsString() << endl;
    }
    // Timing
    for (size_t i = 0 ; i < _timings.size() ; i++) {
        chaine << "|         Timing's attributes :" << endl;
        for(map<Name, Attribute*>::const_iterator it=_timings[i]->getAttributes().begin() ; it!=_timings[i]->getAttributes().end() ; ++it) {
            chaine << "|           name= " + (*it).first.getString()
                   << ", type= " + (*it).second->typeToString()
                << ", value= " + (*it).second->valueAsString() << endl;
        }
    }
    return chaine.str();
}

void Pin::print() {
    cout << Pin::getString();
}

bool Pin::write(ofstream &file) {
    file << "        pin(" << _name.getString() << ") {" << endl;
    //Pin's attributes
    for(map<Name, Attribute*>::const_iterator it=_attributes.begin() ; it!=_attributes.end() ; ++it) {
        file << "            ";
        (*it).second->write(file);
    }
    // Timing
    for (size_t i = 0 ; i < _timings.size() ; i++)
        _timings[i]->write(file);
    file << "        }" << endl;
    return true;
}

} // namespace
