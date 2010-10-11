#include<iostream>
#include<fstream>
#include<ctime>
using namespace std;

#include "vlsisapd/liberty/Name.h"
#include "vlsisapd/liberty/Attribute.h"
#include "vlsisapd/liberty/WireLoad.h"
#include "vlsisapd/liberty/WireLoadSelection.h"
#include "vlsisapd/liberty/Cell.h"
#include "vlsisapd/liberty/Library.h"

namespace LIB {
Library::Library(Name name): _name(name), _attributes(), _wires_load(), _wire_load_selection(NULL), _cells() {};

Cell* Library::getCell(Name cellName) {
    Cell* cell = NULL;
    map<Name, Cell*>::iterator it = _cells.find(cellName);
    if (it == _cells.end()) {
        cerr << "[ERROR] No cell named " << cellName.getString() << endl;
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

void Library::addAttribute(Name attrName, Attribute::Type attrType, string& attrValue, const string& attrUnit) {
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

void Library::print() {
    cout << "+-----------------------------+" << endl
         << "| Library : " << _name.getString() << endl;

    // Library's attributes
    cout << "|   Attributes :" << endl;
    for(map<Name, Attribute*>::const_iterator it=_attributes.begin() ; it!=_attributes.end() ; ++it) {
        cout << "|     name=" << (*it).first.getString()
             << ", type=" << (*it).second->typeToString()
             << ", value=" << (*it).second->valueAsString();
        string unit = (*it).second->getUnit();
        if(!unit.empty())
            cout << ", unit=" << unit;
        cout << endl;
    }

    map<Name, Attribute*> attributes;
    map<Name, Pin*> pins;
    vector<Timing*> timings;
    FlipFlop* ff;

    // WireLoad
    for(map<Name, WireLoad*>::const_iterator it=_wires_load.begin() ; it!=_wires_load.end() ; ++it) {
//        (*it).second->print();
        cout << "|     Wireload : " << (*it).first.getString() << endl;
        // Wireload's attributes
        cout << "|       Attributes :" << endl;
        attributes = (*it).second->getAttributes();
        for(map<Name, Attribute*>::const_iterator it2=attributes.begin() ; it2 != attributes.end() ; ++it2) {
            if ((*it2).second == NULL) {
                cerr << "NULL attribute !" << endl;
                exit(12);
            }
            cout << "|         name=" << (*it2).first.getString()
                 << ", type=" << (*it2).second->typeToString()
                 << ", value=" << (*it2).second->valueAsString();
            string value2=(*it2).second->secondValueAsString();
            if(!value2.empty())
                cout << ", value2=" << value2;
            cout << endl;            
        }
    }

    // WireLoadSelection
//    _wire_load_selection->print();
    cout << "|     WireLoadSection name= " << _wire_load_selection->getName().getString() << endl;
    vector<WireLoadArea*> wires_load_area = _wire_load_selection->getWiresLoadArea();
    for(size_t i = 0 ; i < wires_load_area.size() ; i++) {
        cout << "|       wire_load_from_area name= " << wires_load_area[i]->getName().getString()
             << ", min= "  << wires_load_area[i]->getMin()
             << ", max= "  << wires_load_area[i]->getMax()
             << endl;
    }

    // Cell
    for(map<Name, Cell*>::const_iterator it=_cells.begin() ; it!=_cells.end() ; ++it) {
//        (*it).second->print();
        cout << "|     Cell name= " << (*it).first.getString() << endl;

        // Cell's attributes
        cout << "|       Attributes :" << endl;
        attributes=(*it).second->getAttributes();
        for(map<Name, Attribute*>::const_iterator it2=attributes.begin() ; it2!=attributes.end() ; ++it2) {
            cout << "|         name= " << (*it2).first.getString()
                 << ", type= " << (*it2).second->typeToString()
                 << ", value= " << (*it2).second->valueAsString() << endl;
        }
        // Cell's pins
        pins=(*it).second->getPins();
        for(map<Name, Pin*>::const_iterator it2=pins.begin() ; it2!=pins.end() ; ++it2) {
//            (*it2).second->print();
            cout << "|       Pin name= " << (*it2).first.getString() << endl;
            // Pin's attributes
            cout << "|         Attributes :" << endl;
            attributes=(*it2).second->getAttributes();
            for(map<Name, Attribute*>::const_iterator it3=attributes.begin() ; it3!=attributes.end() ; ++it3) {
                cout << "|           name= " << (*it3).first.getString()
                     << ", type= " << (*it3).second->typeToString()
                     << ", value= " << (*it3).second->valueAsString() << endl;
            }
            // Timing
            timings=(*it2).second->getTimings();
            for (size_t i = 0 ; i < timings.size() ; i++) {
                attributes=timings[i]->getAttributes();
                // Timing's attributes
                cout << "|         Timing's attributes :" << endl;
                for(map<Name, Attribute*>::const_iterator it3=attributes.begin() ; it3!=attributes.end() ; ++it3) {
                    cout << "|           name= " << (*it3).first.getString()
                         << ", type= " << (*it3).second->typeToString()
                         << ", value= " << (*it3).second->valueAsString() << endl;
                }
            }
        }
        // FF
//        if(_ff)
//            _ff->print();
        ff = (*it).second->getFF();
        if(ff) {
            cout << "|       FF noninverting= " << ff->getNonInverting().getString() << ", inverting= " << ff->getInverting().getString() << endl;
            cout << "|         Attributes :" << endl;
            attributes=ff->getAttributes();
            for(map<Name, Attribute*>::const_iterator it=attributes.begin() ; it!=attributes.end() ; ++it) {
                cout << "|           name= " << (*it).first.getString()
                     << ", type= " << (*it).second->typeToString()
                    << ", value= " << (*it).second->valueAsString() << endl;
            }
        }
        // test_cell
//        if(_testCell)
//            _testCell->print();
        Cell* testCell = (*it).second->getTestCell();
        if(testCell) {
            cout << "|     Test Cell" << endl;

            // Cell's attributes
            cout << "|       Attributes :" << endl;
            attributes=testCell->getAttributes();
            for(map<Name, Attribute*>::const_iterator it2=attributes.begin() ; it2!=attributes.end() ; ++it2) {
                cout << "|         name= " << (*it2).first.getString()
                     << ", type= " << (*it2).second->typeToString()
                     << ", value= " << (*it2).second->valueAsString() << endl;
            }
            // Cell's pins
            pins=(*it).second->getPins();
            for(map<Name, Pin*>::const_iterator it2=pins.begin() ; it2!=pins.end() ; ++it2) {
//                (*it2).second->print();
                cout << "|       Pin name= " << (*it2).first.getString() << endl;
                // Pin's attributes
                cout << "|         Attributes :" << endl;
                attributes=(*it2).second->getAttributes();
                for(map<Name, Attribute*>::const_iterator it3=attributes.begin() ; it3!=attributes.end() ; ++it3) {
                    cout << "|           name= " << (*it3).first.getString()
                         << ", type= " << (*it3).second->typeToString()
                         << ", value= " << (*it3).second->valueAsString() << endl;
                }
                // Timing
                timings=(*it2).second->getTimings();
                for (size_t i = 0 ; i < timings.size() ; i++) {
                    attributes=timings[i]->getAttributes();
                    // Timing's attributes
                    cout << "|         Timing's attributes :" << endl;
                    for(map<Name, Attribute*>::const_iterator it3=attributes.begin() ; it3!=attributes.end() ; ++it3) {
                        cout << "|           name= " << (*it3).first.getString()
                             << ", type= " << (*it3).second->typeToString()
                             << ", value= " << (*it3).second->valueAsString() << endl;
                    }
                }
            }
            // FF
//            if(_ff)
//                _ff->print();
            ff = (*it).second->getFF();
            if(ff) {
                cout << "|       FF noninverting= " << ff->getNonInverting().getString() << ", inverting= " << ff->getInverting().getString() << endl;
                cout << "|         Attributes :" << endl;
                attributes=ff->getAttributes();
                for(map<Name, Attribute*>::const_iterator it2=attributes.begin() ; it2!=attributes.end() ; ++it2) {
                    cout << "|           name= " << (*it2).first.getString()
                         << ", type= " << (*it2).second->typeToString()
                        << ", value= " << (*it2).second->valueAsString() << endl;
                }
            }
        }
    }
    cout << "+-----------------------------+" << endl;
};

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
