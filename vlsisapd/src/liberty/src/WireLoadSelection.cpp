#include<iostream>
#include<fstream>
using namespace std;

#include "vlsisapd/liberty/Name.h"
#include "vlsisapd/liberty/WireLoadSelection.h"

namespace LIB {
WireLoadSelection::WireLoadSelection(Name name): _name(name), _wires_load_area() {};

void WireLoadSelection::addWireLoadArea(double min, double max, Name name) {
    WireLoadArea * area = new WireLoadArea(min, max, name);
    _wires_load_area.push_back(area);
}

void WireLoadSelection::print() {
    cout << "|     WireLoadSection name= " << _name.getString() << endl;
    for(size_t i = 0 ; i < _wires_load_area.size() ; i++) {
        cout << "|       wire_load_from_area name= " << _wires_load_area[i]->getName().getString()
             << ", min= "  << _wires_load_area[i]->getMin()
             << ", max= "  << _wires_load_area[i]->getMax()
             << endl;
    }
}

bool WireLoadSelection::write(ofstream& file) {
    file << "    wire_load_selection(" << _name.getString() << ") {" << endl;

    for(size_t i = 0 ; i < _wires_load_area.size() ; i++) {
        file << "        ";
        _wires_load_area[i]->write(file); 
    }

    file << "}" << endl;

    return true;
}
} // namespace
