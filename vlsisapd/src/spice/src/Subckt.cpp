#include <iostream>
using namespace std;

#include "vlsisapd/spice/Subckt.h"

namespace SPICE {
void Subckt::addParameter(string name, string value) {
    map<string, string>::iterator pit = _parameters.find(name);
    if (pit != _parameters.end()) {
        cerr << "[WARNING] SPICE:: Cannot add parameter " << name
             << " to subckt " << _name << " since it already exists." << endl;
        return;
    }
    _parameters[name] = value;
}
}
