#ifndef ATECHNOLOGY_H_
#define ATECHNOLOGY_H_

#include "Property.h"
using namespace Hurricane;

namespace Hurricane {
    class Technology;
}

class ATechnology : public PrivateProperty {
    public:
        typedef PrivateProperty Inherit;

        class PhysicalRule {
            public:
                PhysicalRule(const string& name,
                        double value,
                        const string& reference):
                    _name(name),
                    _value(value),
                    _reference(reference) {}
                PhysicalRule(const PhysicalRule& physicalRule):
                    _name(physicalRule._name),
                    _value(physicalRule._value),
                    _reference(physicalRule._reference) {}
                const string _name;
                const double _value;
                const string _reference;
                double getValue() const { return _value; }
        };
        typedef map<string, ATechnology::PhysicalRule*> PhysicalRules;
        static ATechnology* create(Hurricane::Technology* technology);
        static ATechnology* getATechnology(Hurricane::Technology* technology);
        const PhysicalRule* getPhysicalRule(const string& name);
        void addPhysicalRule(const string& name, double value, const string& reference);
        void print();

        virtual Name getName() const;

        virtual string _getTypeName() const;

        ATechnology():
            Inherit(),
            _physicalRules() {}

    private:
        PhysicalRules _physicalRules;
};

#endif /* ATECHNOLOGY_H_*/
