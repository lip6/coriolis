#include<iostream>
#include<fstream>
#include<sstream>
using namespace std;

#include "vlsisapd/liberty/WireLoadArea.h"
#include "vlsisapd/liberty/WireLoadSelection.h"

namespace LIB {
WireLoadSelection::WireLoadSelection(Name name): _name(name), _wires_load_area() {};

void WireLoadSelection::addWireLoadArea(double min, double max, Name name) {
    WireLoadArea * area = new WireLoadArea(min, max, name);
    _wires_load_area.push_back(area);
}

const string WireLoadSelection::getString() const{
    ostringstream chaine;
    chaine << "|     WireLoadSection name= " << _name.getString() << endl;
    for(size_t i = 0 ; i < _wires_load_area.size() ; i++) {
        chaine << "|       wire_load_from_area name= " << _wires_load_area[i]->getName().getString()
               << ", min= "  << _wires_load_area[i]->getMin()
               << ", max= "  << _wires_load_area[i]->getMax()
               << endl;
    }
    return chaine.str();
}

void WireLoadSelection::print() {
    cout << WireLoadSelection::getString();
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
