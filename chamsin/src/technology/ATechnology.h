#ifndef ATECHNOLOGY_H_
#define ATECHNOLOGY_H_

#include "hurricane/Property.h"
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
                        DbU::Unit value,
                        const string& reference):
                    _name(name),
                    _value(value),
                    _reference(reference) {}
                PhysicalRule(const PhysicalRule& physicalRule):
                    _name(physicalRule._name),
                    _value(physicalRule._value),
                    _reference(physicalRule._reference) {}
                const Name _name;
                const DbU::Unit _value;
                const string _reference;
                double getValue() const { return _value; }

                string _getTypeName() const;
                string _getString() const;
                Record* _getRecord() const;
        };

        class TwoLayersPhysicalRule : public PhysicalRule {
            public:
                typedef PhysicalRule Inherit;

                TwoLayersPhysicalRule(const Name& name,
                        DbU::Unit value,
                        const string& reference,
                        bool symetric):
                    PhysicalRule(name, value, reference),
                    _symetric(symetric) {}

                bool isSymetric() const { return _symetric; } 
                const bool _symetric; 

                string _getTypeName() const;
                string _getString() const;
                Record* _getRecord() const;
        };

        struct PhysicalRuleNameCompare:
            public std::binary_function<const PhysicalRule*, const PhysicalRule*, bool> {
                bool operator()(const PhysicalRule* pr1, const PhysicalRule* pr2) const {
                    return pr1->_name < pr2->_name;
                }
            };

        typedef pair<const Layer*, const Layer*> LayerPair;

        typedef set<ATechnology::PhysicalRule*, PhysicalRuleNameCompare> PhysicalRules;
        typedef set<ATechnology::TwoLayersPhysicalRule*, PhysicalRuleNameCompare> TwoLayersPhysicalRulesSet;
        typedef map<const Layer*, PhysicalRules> OneLayerPhysicalRules;
        typedef map<LayerPair, TwoLayersPhysicalRulesSet> TwoLayersPhysicalRules;

        static ATechnology* create(Hurricane::Technology* technology);
        static ATechnology* getATechnology(Hurricane::Technology* technology);
        const PhysicalRule* getPhysicalRule(const Name& name) const;
        const PhysicalRule* getPhysicalRule(const Name& name, const Layer* layer) const;
        const PhysicalRule* getPhysicalRule(const Name& name, const Layer* layer1, const Layer* layer2) const;
        void addPhysicalRule(const Name& name, DbU::Unit value, const string& reference);
        void addPhysicalRule(const Name& name, const Name& layerName, DbU::Unit value, const string& reference);
        void addPhysicalRule(const Name& name, const Name& layer1Name,
                const Name& layer2Name, bool symetric, DbU::Unit value, const string& reference);
        Layer* getLayer(const Name& layerName);
        void print();

        virtual Name getName() const;

        virtual string _getTypeName() const;
        virtual Record* _getRecord() const;

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


// -------------------------------------------------------------------
// Inspector Support for  :  "ATechnology::LayerPair".

template<>
inline std::string getString<ATechnology::LayerPair> (ATechnology::LayerPair lp) {
    return "<LayerPair layer1=" + getString(lp.first) + ", layer2=" + getString(lp.second) + ">";
}

INSPECTOR_P_SUPPORT(ATechnology);
INSPECTOR_P_SUPPORT(ATechnology::PhysicalRule);
INSPECTOR_P_SUPPORT(ATechnology::TwoLayersPhysicalRule);

#endif /* ATECHNOLOGY_H_*/
