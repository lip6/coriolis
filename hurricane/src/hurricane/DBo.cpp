// ****************************************************************************************************
// File: DBo.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "hurricane/DBo.h"
#include "hurricane/Property.h"
#include "hurricane/Quark.h"
#include "hurricane/Error.h"

namespace Hurricane {



// ****************************************************************************************************
// DBo implementation
// ****************************************************************************************************

DBo::DBo()
// *******
:    _propertySet()
{
}

DBo::~DBo()
// ********
{
}

void DBo::destroy()
// ***************
{
// trace << "entering DBo::destroy: " << this << endl;
// trace_in();

    _preDestroy();

    delete this;

// trace << "exiting DBo::destroy:" << endl;
// trace_out();
}

Property* DBo::getProperty(const Name& name) const
// ***********************************************
{
    PropertySet::const_iterator iterator = _propertySet.begin();
    while (iterator != _propertySet.end()) {
        Property* property = *iterator;
        if (property->getName() == name) return property;
        ++iterator;
    }
    return NULL;
}

void DBo::put(Property* property)
// ******************************
{
    if (!property)
        throw Error("Can't put property : null property");

    Property* oldProperty = getProperty(property->getName());
    if (property != oldProperty) {
        if (oldProperty) {
            _propertySet.erase(oldProperty);
            oldProperty->onReleasedBy(this);
        }
        _propertySet.insert(property);
        property->onCapturedBy(this);
    }
}

void DBo::remove(Property* property)
// *********************************
{
    if (!property)
        throw Error("Can't remove property : null property");

    if (_propertySet.find(property) != _propertySet.end()) {
        _propertySet.erase(property);
        property->onReleasedBy(this);
        if (is_a<Quark*>(this) && _propertySet.empty()) destroy();
    }
}

void DBo::removeProperty(const Name& name)
// ***************************************
{
    Property* property = getProperty(name);
    if (property) {
        _propertySet.erase(property);
        property->onReleasedBy(this);
        if (is_a<Quark*>(this) && _propertySet.empty()) destroy();
    }
}

void DBo::clearProperties()
// ************************
{
// trace << "entering DBo::ClearProperties: " << this << endl;
// trace_in();

    while (!_propertySet.empty()) {
        Property* property = *_propertySet.begin();
// trace << property << endl;
        _propertySet.erase(property);
        property->onReleasedBy(this);
    }

// trace << "exiting DBo::ClearProperties:" << endl;
// trace_out();
}

void DBo::_postCreate()
// ********************
{
}

void DBo::_preDestroy()
// *******************
{
// trace << "entering DBo::_Predestroy: " << this << endl;
// trace_in();

    clearProperties();


// trace << "exiting DBo::_Predestroy:" << endl;
// trace_out();
}

string DBo::_getString() const
// ***************************
{
    return "<" + _getTypeName() + ">";
}

Record* DBo::_getRecord() const
// **********************
{
     Record* record = new Record(getString(this));
    if (record) {
        record->add(getSlot("Properties", &_propertySet));
    }
    return record;
}

void DBo::_onDestroyed(Property* property)
// *************************************
{
    if (property && (_propertySet.find(property) != _propertySet.end())) {
        _propertySet.erase(property);
        if (is_a<Quark*>(this) && _propertySet.empty()) destroy();
    }
}

} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
