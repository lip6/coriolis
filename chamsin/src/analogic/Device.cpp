#include "hurricane/UpdateSession.h"
using namespace Hurricane;

#include "Device.h"

Device::Device(Library* library, const Name& name):
    AnalogComponent(library, name)
{}

Device* Device::create(Library* library, const Name& name) {
    Device* device = new Device(library, name);

    device->_postCreate();

    return device;
}

void Device::_postCreate() {
    Inherit::_postCreate();

    UpdateSession::open();
    /////
    UpdateSession::close();
}
