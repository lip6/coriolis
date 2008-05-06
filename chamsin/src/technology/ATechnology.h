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
                    name_(name),
                    value_(value),
                    reference_(reference) {}
                PhysicalRule(const PhysicalRule& physicalRule):
                    name_(physicalRule.name_),
                    value_(physicalRule.value_),
                    reference_(physicalRule.reference_) {}
                const string name_;
                const double value_;
                const string reference_;
        };
        typedef map<string, ATechnology::PhysicalRule*> PhysicalRules;
        static ATechnology* create(Hurricane::Technology* technology);
        static ATechnology* getATechnology(Hurricane::Technology* technology);
        const PhysicalRule* getPhysicalRule(string name);
        void addPhysicalRule(const string& name, double value, const string& reference);
        void print();
        virtual Name getName() const;
        virtual string _getTypeName() const;

        ATechnology():
            Inherit(),
            physicalRules_() {}

    private:
        PhysicalRules physicalRules_;
};

#endif /* ATECHNOLOGY_H_*/
