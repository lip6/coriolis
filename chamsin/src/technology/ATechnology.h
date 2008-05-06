#ifndef ATECHNOLOGY_H_
#define ATECHNOLOGY_H_

class ATechnology {
    public:
        class PhysicalRule {
            public:
                PhysicalRule(string& name,
                        double value,
                        string& reference):
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
        static const PhysicalRule& getPhysicalRule(string name);
};

#endif /* ATECHNOLOGY_H_*/
