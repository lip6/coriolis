#include "hurricane/UpdateSession.h"
using namespace Hurricane;

#include "Capacitor.h"

Capacitor::Capacitor(Library* library, const Name& name):
    AnalogComponent(library, name)
{}

Capacitor* Capacitor::create(Library* library, const Name& name) {
    Capacitor* capacitor = new Capacitor(library, name);

    capacitor->_postCreate();

    return capacitor;
}

void Capacitor::_postCreate() {
    Inherit::_postCreate();

    UpdateSession::open();
    /////
    UpdateSession::close();
}
