#include "hurricane/UpdateSession.h"
using namespace Hurricane;

#include "Resistor.h"

Resistor::Resistor(Library* library, const Name& name):
    AnalogComponent(library, name)
{}

Resistor* Resistor::create(Library* library, const Name& name) {
    Resistor* resistor = new Resistor(library, name);

    resistor->_postCreate();

    return resistor;
}

void Resistor::_postCreate() {
    Inherit::_postCreate();

}
