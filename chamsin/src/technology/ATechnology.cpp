#include "hurricane/Technology.h"
using namespace Hurricane;

#include "ATechnology.h"

namespace {
static Name ATechnologyPropertyName("ATechnologyProperty");

void printPhysicalRule(const ATechnology::PhysicalRule* physicalRule) {
    cout << "  - name = " << physicalRule->_name << 
        ", value = " << physicalRule->_value <<
        ", ref = " << physicalRule->_reference << endl;
}

void printPhysicalRules(const ATechnology::PhysicalRules& physicalRules) {
    for (ATechnology::PhysicalRules::const_iterator prit = physicalRules.begin();
            prit != physicalRules.end();
            prit++) {
        ATechnology::PhysicalRule* physicalRule = *prit;
        printPhysicalRule(physicalRule);
    }
}

void printPhysicalRules(const ATechnology::TwoLayersPhysicalRulesSet& physicalRules) {
    for (ATechnology::TwoLayersPhysicalRulesSet::const_iterator prit = physicalRules.begin();
            prit != physicalRules.end();
            prit++) {
        ATechnology::PhysicalRule* physicalRule = *prit;
        printPhysicalRule(physicalRule);
    }
}

}

string ATechnology::PhysicalRule::_getTypeName() const {
    return "PhysicalRule";
}

string ATechnology::PhysicalRule::_getString() const {
    return "<" + _getTypeName() + " " + getString(_name) + ">";
}

Record* ATechnology::PhysicalRule::_getRecord() const {
    Record* record = new Record(getString(this));
    record->add(getSlot("Name", &_name));
    record->add(getSlot("Value", &_value));
    record->add(getSlot("Reference", &_reference));
    return record;
}

string ATechnology::TwoLayersPhysicalRule::_getTypeName() const {
    return "TwoLayersPhysicalRule";
}

string ATechnology::TwoLayersPhysicalRule::_getString() const {
    return "<" + _getTypeName() + " " + getString(_name) + ">";
}

Record* ATechnology::TwoLayersPhysicalRule::_getRecord() const {
    Record* record = Inherit::_getRecord();
    if (record) {
        record->add(getSlot("Symetric", _symetric));
    }
    return record;
}

Name ATechnology::getName() const {
    return ATechnologyPropertyName;
}

string ATechnology::_getTypeName() const {
    return _TName("ATechnologyProperty");
}

Record* ATechnology::_getRecord() const {
    Record* record = Inherit::_getRecord();
    if (record) {
        record->add(getSlot("NoLayerPhysicalRules", &_noLayerPhysicalRules));
        record->add(getSlot("OneLayerPhysicalRules", &_oneLayerPhysicalRules));
        record->add(getSlot("TwoLayersPhysicalRules", &_twoLayersPhysicalRules));
    }
    return record;
}

void ATechnology::addPhysicalRule(const Name& name, DbU::Unit value, const string& reference) {
    PhysicalRule searchPR(name, 0, "");
    PhysicalRules::iterator prit = _noLayerPhysicalRules.find(&searchPR);
    if (prit != _noLayerPhysicalRules.end()) {
        throw Error("");
    }
    PhysicalRule* newPhysicalRule = new PhysicalRule(name, value, reference);
    _noLayerPhysicalRules.insert(newPhysicalRule); 
}

void ATechnology::addPhysicalRule(const Name& name, const Name& layerName, DbU::Unit value, const string& reference) {
    Layer* layer = getLayer(layerName);
    OneLayerPhysicalRules::iterator olprit = _oneLayerPhysicalRules.find(layer);
    if (olprit == _oneLayerPhysicalRules.end()) {
        pair<OneLayerPhysicalRules::iterator, bool> result =
            _oneLayerPhysicalRules.insert(OneLayerPhysicalRules::value_type(layer, PhysicalRules()));
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
        const Name& layer2Name, bool symetric, DbU::Unit value,
        const string& reference) {
    Layer* layer1 = getLayer(layer1Name);
    Layer* layer2 = getLayer(layer2Name);
    LayerPair layerPair(layer1, layer2);
    TwoLayersPhysicalRules::iterator tlprit = _twoLayersPhysicalRules.find(layerPair);
    if (tlprit == _twoLayersPhysicalRules.end()) {
        pair<TwoLayersPhysicalRules::iterator, bool> result =
            _twoLayersPhysicalRules.insert(TwoLayersPhysicalRules::value_type(layerPair, TwoLayersPhysicalRulesSet()));
        tlprit = result.first;
        TwoLayersPhysicalRule* newPhysicalRule = new TwoLayersPhysicalRule(name, value, reference, symetric);
        tlprit->second.insert(newPhysicalRule);
    } else {
        TwoLayersPhysicalRulesSet& physicalRules = tlprit->second;
        TwoLayersPhysicalRule searchPR(name, 0, "", true);
        if (physicalRules.find(&searchPR) != physicalRules.end()) {
            throw Error("duplicate rule");
        }
        TwoLayersPhysicalRule* newPhysicalRule = new TwoLayersPhysicalRule(name, value, reference, symetric);
        physicalRules.insert(newPhysicalRule);
    }
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
        const Layer* layer = olprit->first;
        cout << "   o layer " << layer << endl; 
        printPhysicalRules(olprit->second);
        cout << endl;
    }
    cout << " o Two Layers Physical Rules" << endl;
    for (TwoLayersPhysicalRules::iterator tlprit = _twoLayersPhysicalRules.begin();
            tlprit != _twoLayersPhysicalRules.end();
            tlprit++) {
        const Layer* layer1 = tlprit->first.first;
        const Layer* layer2 = tlprit->first.second;
        cout << "   o layer1 " << layer1 << endl; 
        cout << "   o layer2 " << layer2 << endl; 
        printPhysicalRules(tlprit->second);
        cout << endl;
    }
}

const ATechnology::PhysicalRule* ATechnology::getPhysicalRule(const Name& name) const {
    PhysicalRule searchPR(name, 0, "");
    PhysicalRules::iterator prit = _noLayerPhysicalRules.find(&searchPR);
    if (prit == _noLayerPhysicalRules.end()) {
        throw Error("Cannot find Physical Rule " + getString(name));
    }
    return *prit;
}

const ATechnology::PhysicalRule* ATechnology::getPhysicalRule(const Name& name, const Layer* layer) const {
    OneLayerPhysicalRules::const_iterator olprit = _oneLayerPhysicalRules.find(layer);
    if (olprit == _oneLayerPhysicalRules.end()) {
        throw Error("Cannot find Physical Rules for layer " + getString(layer->getName()));
    }
    const PhysicalRules& physicalRules = olprit->second;
    PhysicalRule searchPR(name, 0, "");
    PhysicalRules::iterator prit = physicalRules.find(&searchPR);
    if (prit == physicalRules.end()) {
        throw Error("Cannot find Physical Rule " + getString(name));
    }
    return *prit;
}

const ATechnology::PhysicalRule* ATechnology::getPhysicalRule(
        const Name& name,
        const Layer* layer1,
        const Layer* layer2) const {
    LayerPair searchLayerPair(layer1, layer2);
    TwoLayersPhysicalRules::const_iterator tlprit = _twoLayersPhysicalRules.find(searchLayerPair);
    if (tlprit != _twoLayersPhysicalRules.end()) {
        const TwoLayersPhysicalRulesSet& physicalRules = tlprit->second;
        TwoLayersPhysicalRule searchPR(name, 0, "", true);
        TwoLayersPhysicalRulesSet::const_iterator prit = physicalRules.find(&searchPR);
        if (prit != physicalRules.end()) {
            return *prit;
        }
    }
    LayerPair reverseSearchLayerPair(layer2, layer1);
    tlprit = _twoLayersPhysicalRules.find(reverseSearchLayerPair);
    if (tlprit != _twoLayersPhysicalRules.end()) {
        const TwoLayersPhysicalRulesSet& physicalRules = tlprit->second;
        TwoLayersPhysicalRule searchPR(name, 0, "", true);
        TwoLayersPhysicalRulesSet::const_iterator prit = physicalRules.find(&searchPR);
        if (prit != physicalRules.end()) {
            if ((*prit)->isSymetric()) {
                return *prit;
            }
        }
    }
    throw Error("Cannot find Physical Rule " +
            getString(name) + " for layers " +
        getString(layer1->getName()) +
        " and " +
        getString(layer2->getName()));
}

Layer* ATechnology::getLayer(const Name& layerName) {
    Technology* technology = static_cast<Technology*>(getOwner());
    Layer* layer = technology->getLayer(layerName);
    if (!layer) {
        throw Error("cannot find layer " + getString(layerName));
    }
    return layer;
}
