#ifndef ATECHNOLOGY_H_
#define ATECHNOLOGY_H_

#include "Property.h"
using namespace Hurricane;

namespace Hurricane {
    class Technology;
    class Layer;
}

class ATechnology : public PrivateProperty {
    public:
        typedef PrivateProperty Inherit;

        class PhysicalRule {
            public:
                PhysicalRule(const Name& name,
                        double value,
                        const string& reference):
                    _name(name),
                    _value(value),
                    _reference(reference) {}
                PhysicalRule(const PhysicalRule& physicalRule):
                    _name(physicalRule._name),
                    _value(physicalRule._value),
                    _reference(physicalRule._reference) {}
                const Name _name;
                const double _value;
                const string _reference;
                double getValue() const { return _value; }
        };
        typedef map<Name, ATechnology::PhysicalRule*> PhysicalRules;
        typedef map<Layer*, PhysicalRules> OneLayerPhysicalRules;
        typedef pair<Layer*, Layer*> LayerPair;
        typedef map<LayerPair, PhysicalRules> TwoLayersPhysicalRules;
        static ATechnology* create(Hurricane::Technology* technology);
        static ATechnology* getATechnology(Hurricane::Technology* technology);
        const PhysicalRule* getPhysicalRule(const Name& name);
        void addPhysicalRule(const Name& name, double value, const string& reference);
        void addPhysicalRule(const Name& name, const Name& layerName, double value, const string& reference);
        void addPhysicalRule(const Name& name, const Name& layer1Name,
                const Name& layer2Name, double value, const string& reference);
        Layer* getLayer(const Name& layerName);
        void print();

        virtual Name getName() const;

        virtual string _getTypeName() const;

        ATechnology():
            Inherit(),
            _noLayerPhysicalRules(),
            _oneLayerPhysicalRules(),
            _twoLayersPhysicalRules() {}

    private:
        PhysicalRules _noLayerPhysicalRules;
        OneLayerPhysicalRules _oneLayerPhysicalRules;
        TwoLayersPhysicalRules _twoLayersPhysicalRules;
};

#endif /* ATECHNOLOGY_H_*/
