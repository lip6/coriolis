#include <iostream>
using namespace std;

#include "vlsisapd/spice/Instance.h"
#include "vlsisapd/spice/Value.h"

namespace SPICE {
Instance::Instance(string name, string model) : _name(name), _model(model)
{}

string Instance::getParameterValue(string name) {
    map<string, string>::iterator pit = _parameters.find(name);
    if (pit != _parameters.end()) {
        return (*pit).second;
    } else {
        return "";
    }
}

void Instance::addParameter(string name, string value) {
    map<string, string>::iterator pit = _parameters.find(name);
    if (pit != _parameters.end()) {
        cerr << "[WARNING] SPICE:: Cannot add parameter " << name
             << " to instance " << _name << " since it already exists." << endl;
        return;
    }
    _parameters[name] = value;
}
}
