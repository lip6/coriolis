#include "Technology.h"
#include "Property.h"
using namespace Hurricane;

#include "ATechnology.h"

namespace {

static Name ATechnologyPropertyName("ATechnologyProperty");

class ATechnologyProperty : public PrivateProperty {
    public:
        typedef PrivateProperty Inherit;
        static ATechnologyProperty* create(Technology* technology);
        ATechnologyProperty();
        virtual Name getName() const {
            return ATechnologyPropertyName;
        }
        virtual string _getTypeName() const {
            return _TName("ATechnologyProperty");
        }
};


ATechnologyProperty::ATechnologyProperty():
    Inherit()
    {}

ATechnologyProperty* ATechnologyProperty::create(Technology* technology) {
    ATechnologyProperty* prop = new ATechnologyProperty();

    prop->_postCreate();

    technology->put(prop);

    return prop;
}

}





