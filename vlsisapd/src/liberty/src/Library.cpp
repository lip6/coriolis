#include<iostream>
#include<fstream>
#include<sstream>
#include<ctime>
using namespace std;

#include "vlsisapd/liberty/WireLoad.h"
#include "vlsisapd/liberty/WireLoadSelection.h"
#include "vlsisapd/liberty/WireLoadArea.h"
#include "vlsisapd/liberty/Cell.h"
#include "vlsisapd/liberty/Pin.h"
#include "vlsisapd/liberty/Timing.h"
#include "vlsisapd/liberty/FlipFlop.h"

#include "vlsisapd/liberty/Library.h"

namespace LIB {
Library::Library(Name name): _name(name), _attributes(), _wires_load(), _wire_load_selection(NULL), _cells() {};

Attribute* Library::getAttribute(Name attrName) {
    Attribute* attr = NULL;
    map<Name, Attribute*>::iterator it = _attributes.find(attrName);
    if (it == _attributes.end()) {
        cerr << "[ERROR] Library " << _name.getString() << " has no attribute named " << attrName.getString() << endl;
        exit(1);
    }
    attr= (*it).second;
    return attr;
}

Cell* Library::getCell(Name cellName) {
    Cell* cell = NULL;
    map<Name, Cell*>::iterator it = _cells.find(cellName);
    if (it == _cells.end()) {
        cerr << "[ERROR] Library : No cell named " << cellName.getString() << endl;
        exit(1);
    }
    cell= (*it).second;
    return cell;
}

WireLoad* Library::getWireLoad(Name wireLoadName) {
    WireLoad* wire = NULL;
    map<Name, WireLoad*>::iterator it = _wires_load.find(wireLoadName);
    if (it == _wires_load.end()) {
        cerr << "[ERROR] No wire_load named " << wireLoadName.getString() << endl;
        exit(1);
    }
    wire= (*it).second;
    return wire;
}

void Library::addAttribute(Name attrName, Attribute::Type attrType, const string& attrValue, const string& attrUnit) {
    Attribute* attr = new Attribute(attrName, attrType, attrValue, attrUnit);
    map<Name, Attribute*>::iterator it = _attributes.find(attrName);
    if (it != _attributes.end()) {
        cerr << "[ERROR] Cannot define two Libraries's attributes with the same name: " << attrName.getString() << endl;
        exit(1);
    }
    _attributes[attrName] = attr;
}

void Library::addWireLoad(Name wireLoadName) {
    WireLoad* wire = new WireLoad(wireLoadName);
    map<Name, WireLoad*>::iterator it = _wires_load.find(wireLoadName);
    if (it != _wires_load.end()) {
        cerr << "[ERROR] Cannot define two Libraries's wire_load with the same name: " << wireLoadName.getString() << endl;
        exit(1);
    }
    _wires_load[wireLoadName] = wire;
}

void Library::addWireLoadSelection(Name wireLoadSelectionName) {
    WireLoadSelection* wire = new WireLoadSelection(wireLoadSelectionName);
    _wire_load_selection = wire;
}

void Library::addCell(Name cellName) {
    Cell* cell = new Cell(cellName);
    map<Name, Cell*>::iterator it = _cells.find(cellName);
    if (it != _cells.end()) {
        cerr << "[ERROR] Cannot define two Libraries's cells with the same name: " << cellName.getString() << endl;
        exit(1);
    }
    _cells[cellName] = cell;
}

const string Library::getString() const{
    ostringstream chaine;
    chaine << "+-----------------------------+" << endl
           << "| Library : " << _name.getString() << endl;
    // Library's attributes
    chaine << "|   Attributes :" << endl;
    for(map<Name, Attribute*>::const_iterator it=_attributes.begin() ; it!=_attributes.end() ; ++it) {
        chaine << "|     name=" << (*it).first.getString()
               << ", type=" << (*it).second->typeToString()
               << ", value=" << (*it).second->valueAsString();
        string unit = (*it).second->getUnit();
        if(!unit.empty())
            chaine << ", unit=" << unit;
        chaine << endl;
    }

    // WireLoad
    for(map<Name, WireLoad*>::const_iterator it=_wires_load.begin() ; it!=_wires_load.end() ; ++it) {
//        (*it).second->print();
        chaine << "|     Wireload : " << (*it).first.getString() << endl;
        // Wireload's attributes
        chaine << "|       Attributes :" << endl;
        for(map<Name, Attribute*>::const_iterator it2=(*it).second->getAttributes().begin() ; it2 != (*it).second->getAttributes().end() ; ++it2) {
            if ((*it2).second == NULL) {
                cerr << "NULL attribute !" << endl;
                exit(12);
            }
            chaine << "|         name=" << (*it2).first.getString()
                 << ", type=" << (*it2).second->typeToString()
                 << ", value=" << (*it2).second->valueAsString();
            string value2=(*it2).second->secondValueAsString();
            if(!value2.empty())
                chaine << ", value2=" << value2;
            chaine << endl;            
        }
    }

    // WireLoadSelection
//    _wire_load_selection->print();
    chaine << "|     WireLoadSection name= " << _wire_load_selection->getName().getString() << endl;
    vector<WireLoadArea*> wires_load_area = _wire_load_selection->getWiresLoadArea();
    for(size_t i = 0 ; i < wires_load_area.size() ; i++) {
        chaine << "|       wire_load_from_area name= " << wires_load_area[i]->getName().getString()
               << ", min= "  << wires_load_area[i]->getMin()
               << ", max= "  << wires_load_area[i]->getMax()
               << endl;
    }

    // Cell
    for(map<Name, Cell*>::const_iterator it=_cells.begin() ; it!=_cells.end() ; ++it) {
//        (*it).second->print();
        chaine << "|     Cell name= " << (*it).first.getString() << endl;

        // Cell's attributes
        chaine << "|       Attributes :" << endl;
        for(map<Name, Attribute*>::const_iterator it2=(*it).second->getAttributes().begin() ; it2!=(*it).second->getAttributes().end() ; ++it2) {
            chaine << "|         name= " << (*it2).first.getString()
                   << ", type= " << (*it2).second->typeToString()
                   << ", value= " << (*it2).second->valueAsString() << endl;
        }
        // Cell's pins
        for(map<Name, Pin*>::const_iterator it2=(*it).second->getPins().begin() ; it2!=(*it).second->getPins().end() ; ++it2) {
//            (*it2).second->print();
            chaine << "|       Pin name= " << (*it2).first.getString() << endl;
            // Pin's attributes
            chaine << "|         Attributes :" << endl;
            for(map<Name, Attribute*>::const_iterator it3=(*it2).second->getAttributes().begin() ; it3!=(*it2).second->getAttributes().end() ; ++it3) {
                chaine << "|           name= " << (*it3).first.getString()
                       << ", type= " << (*it3).second->typeToString()
                       << ", value= " << (*it3).second->valueAsString() << endl;
            }
            // Timing
            for (size_t i = 0 ; i < (*it2).second->getTimings().size() ; i++) {
                // Timing's attributes
                chaine << "|         Timing's attributes :" << endl;
                for(map<Name, Attribute*>::const_iterator it3=(*it2).second->getTimings()[i]->getAttributes().begin() ; it3!=(*it2).second->getTimings()[i]->getAttributes().end() ; ++it3) {
                    chaine << "|           name= " << (*it3).first.getString()
                           << ", type= " << (*it3).second->typeToString()
                           << ", value= " << (*it3).second->valueAsString() << endl;
                }
            }
        }
        // FF
//        if(_ff)
//            _ff->print();
        if((*it).second->getFF()) {
            chaine << "|       FF noninverting= " << (*it).second->getFF()->getNonInverting().getString() << ", inverting= " << (*it).second->getFF()->getInverting().getString() << endl;
            chaine << "|         Attributes :" << endl;
            for(map<Name, Attribute*>::const_iterator it2=(*it).second->getFF()->getAttributes().begin() ; it2!=(*it).second->getFF()->getAttributes().end() ; ++it2) {
                chaine << "|           name= " << (*it2).first.getString()
                       << ", type= " << (*it2).second->typeToString()
                       << ", value= " << (*it2).second->valueAsString() << endl;
            }
        }
        // test_cell
//        if(_testCell)
//            _testCell->print();
        Cell* testCell = (*it).second->getTestCell();
        if(testCell) {
            chaine << "|     Test Cell" << endl;

            // Cell's attributes
            chaine << "|       Attributes :" << endl;
            for(map<Name, Attribute*>::const_iterator it2=testCell->getAttributes().begin() ; it2!=testCell->getAttributes().end() ; ++it2) {
                chaine << "|         name= " << (*it2).first.getString()
                       << ", type= " << (*it2).second->typeToString()
                       << ", value= " << (*it2).second->valueAsString() << endl;
            }
            // Cell's pins
            for(map<Name, Pin*>::const_iterator it2=(*it).second->getPins().begin() ; it2!=(*it).second->getPins().end() ; ++it2) {
//                (*it2).second->print();
                chaine << "|       Pin name= " << (*it2).first.getString() << endl;
                // Pin's attributes
                chaine << "|         Attributes :" << endl;
                for(map<Name, Attribute*>::const_iterator it3=(*it2).second->getAttributes().begin() ; it3!=(*it2).second->getAttributes().end() ; ++it3) {
                    chaine << "|           name= " << (*it3).first.getString()
                           << ", type= " << (*it3).second->typeToString()
                           << ", value= " << (*it3).second->valueAsString() << endl;
                }
                // Timing
                for (size_t i = 0 ; i < (*it2).second->getTimings().size() ; i++) {
                    // Timing's attributes
                    chaine << "|         Timing's attributes :" << endl;
                    for(map<Name, Attribute*>::const_iterator it3=(*it2).second->getTimings()[i]->getAttributes().begin() ; it3!=(*it2).second->getTimings()[i]->getAttributes().end() ; ++it3) {
                        chaine << "|           name= " << (*it3).first.getString()
                               << ", type= " << (*it3).second->typeToString()
                               << ", value= " << (*it3).second->valueAsString() << endl;
                    }
                }
            }
            // FF
//            if(_ff)
//                _ff->print();
            if((*it).second->getFF()) {
                chaine << "|       FF noninverting= " << (*it).second->getFF()->getNonInverting().getString() << ", inverting= " << (*it).second->getFF()->getInverting().getString() << endl;
                chaine << "|         Attributes :" << endl;
                for(map<Name, Attribute*>::const_iterator it2=(*it).second->getFF()->getAttributes().begin() ; it2!=(*it).second->getFF()->getAttributes().end() ; ++it2) {
                    chaine << "|           name= " << (*it2).first.getString()
                           << ", type= " << (*it2).second->typeToString()
                           << ", value= " << (*it2).second->valueAsString() << endl;
                }
            }
        }
    }
    chaine << "+-----------------------------+" << endl;
    return chaine.str();
}

void Library::print() {
    cout << Library::getString();
}

bool Library::writeToFile(string filename) {
    time_t curtime = time(0);
    tm now = *localtime(&curtime);
    char date[BUFSIZ]={0};
    const char format[]="%y-%m-%d  %H:%M:%S";
    if (!strftime(date, sizeof(date)-1, format, &now)>0)
        cerr << "[Liberty DRIVE ERROR]: cannot build current date." << endl;

    ofstream file;
    file.open(filename.c_str(), ios::out);
    // Header
    file << "/* Generated by vlsisapd driver */" << endl
         << "/* date : " << date << " */" << endl;
    file << "library (" << _name.getString() << ") {" << endl;

    // Library's attributes
    for(map<Name, Attribute*>::const_iterator it=_attributes.begin() ; it!=_attributes.end() ; ++it) {
        file << "    ";
        (*it).second->write(file);
    }
    // For each WireLoad : write wireload
    for(map<Name, WireLoad*>::const_iterator it=_wires_load.begin() ; it!=_wires_load.end() ; ++it) {
        (*it).second->write(file);
    }
    // write wireloadselection
    _wire_load_selection->write(file);
    // for each Cell : write cell
    for(map<Name, Cell*>::const_iterator it=_cells.begin() ; it!=_cells.end() ; ++it) {
        (*it).second->write(file);
    }
    // Footer
    file << "}" << endl;

    file.close();
    return true;
}
} // namespace
