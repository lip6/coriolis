#include "Technology.h"
#include "Property.h"
using namespace Hurricane;

#include "ATechnology.h"

namespace {

static Name ATechnologyPropertyName("ATechnologyProperty");


class ATechnologyProperty : public PrivateProperty {
    public:
        typedef PrivateProperty Inherit;
        typedef map<string, ATechnology::PhysicalRule*> PhysicalRules;
        static ATechnologyProperty* create(Technology* technology);
        ATechnologyProperty();
        virtual Name getName() const {
            return ATechnologyPropertyName;
        }
        virtual string _getTypeName() const {
            return _TName("ATechnologyProperty");
        }
        void addPhysicalRule(ATechnology::PhysicalRule& physicalRule) {
            PhysicalRules::iterator prit = physicalRules_.find(physicalRule.name_);
            if (prit != physicalRules_.end()) {
                throw Error("");
            }
            ATechnology::PhysicalRule* newPhysicalRule = new ATechnology::PhysicalRule(physicalRule);
            physicalRules_[newPhysicalRule->name_] = newPhysicalRule; 
        }
    private:
        PhysicalRules physicalRules_;
};


ATechnologyProperty::ATechnologyProperty():
    Inherit(),
    physicalRules_()
    {}

ATechnologyProperty* ATechnologyProperty::create(Technology* technology) {
    ATechnologyProperty* prop = new ATechnologyProperty();

    prop->_postCreate();

    technology->put(prop);

    return prop;
}

}





