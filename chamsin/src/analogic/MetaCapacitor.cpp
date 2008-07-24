#include "hurricane/UpdateSession.h"
using namespace Hurricane;

#include "Capacitor.h"
#include "MetaCapacitor.h"

MetaCapacitor::MetaCapacitor(Library* library, const Name& name):
    Device(library, name)
{}

MetaCapacitor* MetaCapacitor::create(Library* library, const Name& name) {
    MetaCapacitor* mCapacitor = new MetaCapacitor(library, name);

    mCapacitor->_postCreate();

    return mCapacitor;
}

void MetaCapacitor::_postCreate() {
    Inherit::_postCreate();

    setTerminal(false);
}
