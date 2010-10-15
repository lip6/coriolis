#include<iostream>
#include<fstream>
#include<sstream>
using namespace std;

#include "vlsisapd/liberty/WireLoad.h"

namespace LIB {
WireLoad::WireLoad(Name name): _name(name), _attributes() {};

Attribute* WireLoad::getAttribute(Name attrName) {
    Attribute* attr = NULL;
    map<Name, Attribute*>::iterator it = _attributes.find(attrName);
    if (it == _attributes.end()) {
        cerr << "[ERROR] WireLoad " << _name.getString() << " has no attribute named " << attrName.getString() << endl;
        exit(1);
    }
    attr= (*it).second;
    return attr;
}

void WireLoad::addAttribute(Name attrName, Attribute::Type attrType, const string& attrValue, const string& attrValue2) {
    Attribute* attr = new Attribute(attrName, attrType, attrValue, "", attrValue2);
    map<Name, Attribute*>::iterator it = _attributes.find(attrName);
    if (it != _attributes.end()) {
        cerr << "[ERROR] Cannot define WireLoad with several attributes with the same name : " << attrName.getString() << endl;
        exit(1);
    }
    _attributes[attrName] = attr;
}

const string WireLoad::getString() const{
    ostringstream chaine;
    // WireLoad's attributes
    chaine << "|     Wireload : " << _name.getString() << endl
           << "|       Attributes :" << endl;
    for(map<Name, Attribute*>::const_iterator it=_attributes.begin() ; it!=_attributes.end() ; ++it) {
        chaine << "|         name= " << (*it).first.getString()
               << ", type= " << (*it).second->typeToString()
               << ", value= " << (*it).second->valueAsString();
        string value2=(*it).second->secondValueAsString();
        if(!value2.empty())
            chaine << ", value2= " << value2;
        chaine << endl;
    }
    return chaine.str();
}

void WireLoad::print() {
    cout << WireLoad::getString();
}

bool WireLoad::write(ofstream &file) {
    file << "    wire_load(\"" << _name.getString() << "\") {" << endl;

    for(map<Name, Attribute*>::const_iterator it=_attributes.begin() ; it!=_attributes.end() ; ++it ) {
        file << "        ";
        (*it).second->write(file);
    }

    file << "}" << endl;

    return true;
}
} // namespace
