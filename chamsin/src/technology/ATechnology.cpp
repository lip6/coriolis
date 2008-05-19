#include "Technology.h"
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

void ATechnology::addPhysicalRule(const Name& name, double value, const string& reference) {
    PhysicalRules::iterator prit = _noLayerPhysicalRules.find(name);
    if (prit != _noLayerPhysicalRules.end()) {
        throw Error("");
    }
    PhysicalRule* newPhysicalRule = new PhysicalRule(name, value, reference);
    _noLayerPhysicalRules[newPhysicalRule->_name] = newPhysicalRule; 
}

void ATechnology::addPhysicalRule(const Name& name, const Name& layerName, double value, const string& reference) {
    Layer* layer = getLayer(layerName);
    OneLayerPhysicalRules::iterator olprit = _oneLayerPhysicalRules.find(layer);
    if (olprit == _oneLayerPhysicalRules.end()) {
        pair<OneLayerPhysicalRules::iterator, bool> result =
            _oneLayerPhysicalRules.insert(make_pair(layer, PhysicalRules()));
        assert(result.second);
        olprit = result.first;
        PhysicalRule* newPhysicalRule = new PhysicalRule(name, value, reference);
        olprit->second[name] = newPhysicalRule;
    } else {
        PhysicalRules& physicalRules = olprit->second;
        if (physicalRules.find(name) != physicalRules.end()) {
            throw Error("duplicate rule");
        }
        PhysicalRule* newPhysicalRule = new PhysicalRule(name, value, reference);
        physicalRules[name] = newPhysicalRule;
    }
}

void ATechnology::addPhysicalRule(const Name& name, const Name& layer1Name,
        const Name& layer2Name, double value, const string& reference) {
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
    cout << " o No Layer Physical Rules" << endl;
    for (PhysicalRules::iterator prit = _noLayerPhysicalRules.begin();
            prit != _noLayerPhysicalRules.end();
            prit++) {
        PhysicalRule* physicalRule = prit->second;
        cout << "  - name = " << physicalRule->_name << 
            ", value = " << physicalRule->_value <<
            ", ref = " << physicalRule->_reference << endl;
    }
}

const ATechnology::PhysicalRule* ATechnology::getPhysicalRule(const Name& name) {
    PhysicalRules::iterator prit = _noLayerPhysicalRules.find(name);
    if (prit == _noLayerPhysicalRules.end()) {
        throw Error("Cannot find Physical Rule " + getString(name));
    }
    return prit->second;
}

Layer* ATechnology::getLayer(const Name& layerName) {
    Technology* technology = static_cast<Technology*>(getOwner());
    Layer* layer = technology->getLayer(layerName);
    if (!layer) {
        throw Error("cannot find layer");
    }
    return layer;
}
