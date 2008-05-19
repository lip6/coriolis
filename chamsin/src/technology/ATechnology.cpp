#include "Technology.h"
#include "Property.h"
using namespace Hurricane;

#include "ATechnology.h"

namespace {
static Name ATechnologyPropertyName("ATechnologyProperty");
}

Name ATechnology::getName() const {
    return ATechnologyPropertyName;
}

string ATechnology::_getTypeName() const {
    return _TName("ATechnologyProperty");
}

void ATechnology::addPhysicalRule(const string& name, double value, const string& reference) {
    PhysicalRules::iterator prit = _physicalRules.find(name);
    if (prit != _physicalRules.end()) {
        throw Error("");
    }
    PhysicalRule* newPhysicalRule = new PhysicalRule(name, value, reference);
    _physicalRules[newPhysicalRule->_name] = newPhysicalRule; 
}

ATechnology* ATechnology::create(Technology* technology) {
    ATechnology* prop = new ATechnology();

    prop->_postCreate();

    technology->put(prop);

    return prop;
}

ATechnology* ATechnology::getATechnology(Technology* technology) {
    Property* property = technology->getProperty(ATechnologyPropertyName);
    if (property) {
        ATechnology* aTechnology = static_cast<ATechnology*>(property);
        return aTechnology;
    }
    return NULL;
}

void ATechnology::print() {
    cout << "Printing ATechnology" << endl;
    cout << " o Physical Rules" << endl;
    for (PhysicalRules::iterator prit = _physicalRules.begin();
            prit != _physicalRules.end();
            prit++) {
        PhysicalRule* physicalRule = prit->second;
        cout << "  - name = " << physicalRule->_name << 
            ", value = " << physicalRule->_value <<
            ", ref = " << physicalRule->_reference << endl;
    }
}

const ATechnology::PhysicalRule* ATechnology::getPhysicalRule(const string& name) {
    PhysicalRules::iterator prit = _physicalRules.find(name);
    if (prit == _physicalRules.end()) {
        throw Error("Cannot find Physical Rule " + name);
    }
    return prit->second;
}
