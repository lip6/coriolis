#include<cstdlib>

#include<iostream>
#include<fstream>
#include<sstream>
using namespace std;

#include "vlsisapd/liberty/Cell.h"
#include "vlsisapd/liberty/Pin.h"
#include "vlsisapd/liberty/FlipFlop.h"

namespace LIB {
Cell::Cell(Name name): _name(name), _attributes(), _pins(), _ff(NULL), _testCell(NULL) {};

Attribute* Cell::getAttribute(Name attrName) {
    Attribute* attr = NULL;
    map<Name, Attribute*>::iterator it = _attributes.find(attrName);
    if (it == _attributes.end()) {
        cerr << "[ERROR] Cell " << _name.getString() << " has no attribute named " << attrName.getString() << endl;
        exit(1);
    }
    attr= (*it).second;
    return attr;
}

Pin* Cell::getPin(Name pinName) {
    Pin* pin = NULL;
    map<Name, Pin*>::iterator it = _pins.find(pinName);
    if (it == _pins.end()) {
        cerr << "[ERROR] Cell " << _name.getString() << " has no pin named " << pinName.getString() << endl;
        exit(1);
    }
    pin= (*it).second;
    return pin;
}

void Cell::addAttribute(Name attrName, Attribute::Type attrType, const string& attrValue) {
    Attribute* attr = new Attribute(attrName, attrType, attrValue);
    map<Name, Attribute*>::iterator it = _attributes.find(attrName);
    if (it != _attributes.end()) {
        cerr << "[ERROR] Cannot define two Cells's attributes with the same name: " << attrName.getString() << endl;
        exit(1);
    }
    _attributes[attrName] = attr;
}

void Cell::addPin(Name pinName) {
    Pin* pin = new Pin(pinName);
    map<Name, Pin*>::iterator it = _pins.find(pinName);
    if (it != _pins.end()) {
        cerr << "[ERROR] Cannot define two Cells's pins with the same name: " << _name.getString() << " -> " << pinName.getString() << endl;
        exit(1);
    }
    _pins[pinName] = pin;
}

void Cell::addFF(Name noninverting, Name inverting) {
    FlipFlop* flipflop = new FlipFlop(noninverting, inverting);
    _ff = flipflop;
}

void Cell::setTestCell(Cell *cell) {
    _testCell = cell;
}

const string Cell::getString() const{
    ostringstream chaine;
    chaine << "|     Cell name= " << _name.getString() << endl;

    // Cell's attributes
    chaine << "|       Attributes :" << endl;
    for(map<Name, Attribute*>::const_iterator it=_attributes.begin() ; it!=_attributes.end() ; ++it) {
        chaine << "|         name= " << (*it).first.getString()
               << ", type= " << (*it).second->typeToString()
               << ", value= " << (*it).second->valueAsString() << endl;
    }
    // Cell's pins
    for(map<Name, Pin*>::const_iterator it=_pins.begin() ; it!=_pins.end() ; ++it) {
        (*it).second->print();
    }
    // FF
    if(_ff) {
        chaine << "|       FF noninverting= " << _ff->getNonInverting().getString() << ", inverting= " << _ff->getInverting().getString() << endl;

        // FlipFlop's attributes
        chaine << "|         Attributes :" << endl;
        for(map<Name, Attribute*>::const_iterator it=_ff->getAttributes().begin() ; it!=_ff->getAttributes().end() ; ++it) {
            chaine << "|           name= " << (*it).first.getString()
                   << ", type= " << (*it).second->typeToString()
                   << ", value= " << (*it).second->valueAsString() << endl;
        }
    }
    // test_cell
    if(_testCell)
        chaine << _testCell->getString();

    return chaine.str();
}

void Cell::print() {
    cout << Cell::getString();
}

bool Cell::write(ofstream &file, bool test) {
    if(_name.getString()=="test_cell") {
        if(test==false)
            return true;
        else
            file << "    test_cell () {" << endl;
    } else {
        file << "    cell (" << _name.getString() << ") {" << endl;
    }
    // Cell's attributes
    for(map<Name, Attribute*>::const_iterator it=_attributes.begin() ; it!=_attributes.end() ; ++it) {
        file << "        ";
        (*it).second->write(file);
    }
    // Cell's pin
    for(map<Name, Pin*>::const_iterator it=_pins.begin() ; it!=_pins.end() ; ++it) {
        (*it).second->write(file);
    }
    // FF
    if(_ff)
        _ff->write(file);
    // test_cell
    if(_testCell)
        _testCell->write(file, true);
    file << "    }" << endl;
    return true;
}
} // namespace
