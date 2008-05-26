#include "hurricane/Technology.h"
using namespace Hurricane;

#include "ATechnology.h"

namespace {
static Name ATechnologyPropertyName("ATechnologyProperty");

void printPhysicalRules(ATechnology::PhysicalRules& physicalRules) {
    for (ATechnology::PhysicalRules::iterator prit = physicalRules.begin();
            prit != physicalRules.end();
            prit++) {
        ATechnology::PhysicalRule* physicalRule = *prit;
        cout << "  - name = " << physicalRule->_name << 
            ", value = " << physicalRule->_value <<
            ", ref = " << physicalRule->_reference << endl;
    }
}

}

Name ATechnology::getName() const {
    return ATechnologyPropertyName;
}

string ATechnology::_getTypeName() const {
    return _TName("ATechnologyProperty");
}

void ATechnology::addPhysicalRule(const Name& name, double value, const string& reference) {
    PhysicalRule searchPR(name, 0, "");
    PhysicalRules::iterator prit = _noLayerPhysicalRules.find(&searchPR);
    if (prit != _noLayerPhysicalRules.end()) {
        throw Error("");
    }
    PhysicalRule* newPhysicalRule = new PhysicalRule(name, value, reference);
    _noLayerPhysicalRules.insert(newPhysicalRule); 
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
        olprit->second.insert(newPhysicalRule);
    } else {
        PhysicalRules& physicalRules = olprit->second;
        PhysicalRule searchPR(name, 0, "");
        if (physicalRules.find(&searchPR) != physicalRules.end()) {
            throw Error("duplicate rule");
        }
        PhysicalRule* newPhysicalRule = new PhysicalRule(name, value, reference);
        physicalRules.insert(newPhysicalRule);
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
    printPhysicalRules(_noLayerPhysicalRules);
    cout << endl;
    cout << " o One Layer Physical Rules" << endl;
    for (OneLayerPhysicalRules::iterator olprit = _oneLayerPhysicalRules.begin();
            olprit != _oneLayerPhysicalRules.end();
            olprit++) {
        Layer* layer = olprit->first;
        cout << "   o layer " << layer << endl; 
        printPhysicalRules(olprit->second);
        cout << endl;
    }
    for (TwoLayersPhysicalRules::iterator tlprit = _twoLayersPhysicalRules.begin();
            tlprit != _twoLayersPhysicalRules.end();
            tlprit++) {
        Layer* layer1 = tlprit->first.first;
        Layer* layer2 = tlprit->first.second;
        cout << "   o layer1 " << layer1 << endl; 
        cout << "   o layer2 " << layer2 << endl; 
        printPhysicalRules(tlprit->second);
        cout << endl;
    }
}

const ATechnology::PhysicalRule* ATechnology::getPhysicalRule(const Name& name) {
    PhysicalRule searchPR(name, 0, "");
    PhysicalRules::iterator prit = _noLayerPhysicalRules.find(&searchPR);
    if (prit == _noLayerPhysicalRules.end()) {
        throw Error("Cannot find Physical Rule " + getString(name));
    }
    return *prit;
}

Layer* ATechnology::getLayer(const Name& layerName) {
    Technology* technology = static_cast<Technology*>(getOwner());
    Layer* layer = technology->getLayer(layerName);
    if (!layer) {
        throw Error("cannot find layer " + getString(layerName));
    }
    return layer;
}
