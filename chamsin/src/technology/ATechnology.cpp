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
    PhysicalRules::iterator prit = physicalRules_.find(name);
    if (prit != physicalRules_.end()) {
        throw Error("");
    }
    PhysicalRule* newPhysicalRule = new PhysicalRule(name, value, reference);
    physicalRules_[newPhysicalRule->name_] = newPhysicalRule; 
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
    for (PhysicalRules::iterator prit = physicalRules_.begin();
            prit != physicalRules_.end();
            prit++) {
        PhysicalRule* physicalRule = prit->second;
        cout << "  - name = " << physicalRule->name_ << 
            ", value = " << physicalRule->value_ <<
            ", ref = " << physicalRule->reference_ << endl;
    }
}
