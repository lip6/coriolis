// ****************************************************************************************************
// File: Property.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Property.h"
#include "DBo.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Property implementation
// ****************************************************************************************************

Property::Property()
// *****************
{
}

Property::~Property()
// ******************
{
}

void Property::destroy()
// ********************
{
    _preDestroy();

    delete this;
}

string Property::_getString() const
// ********************************
{
    string s = "<" + _getTypeName() + ">";
    s.insert(s.length() - 1, " " + getString(getName()));
    return s;
}

Record* Property::_getRecord() const
// ***************************
{
    return new Record(getString(this));
}



// ****************************************************************************************************
// PrivateProperty implementation
// ****************************************************************************************************

PrivateProperty::PrivateProperty()
// *******************************
:    Inherit(),
    _owner(NULL)
{
}

void PrivateProperty::_preDestroy()
// *******************************
{
    Inherit::_preDestroy();

    if (_owner) _owner->_onDeleted(this);
}

void PrivateProperty::onCapturedBy(DBo* owner)
// *******************************************
{
    _owner = owner;
}

void PrivateProperty::onReleasedBy(DBo* owner)
// *******************************************
{
    if (_owner == owner) onNotOwned();
}

void PrivateProperty::onNotOwned()
// *******************************
{
    destroy();
}

string PrivateProperty::_getString() const
// ***************************************
{
    return Inherit::_getString();
}

Record* PrivateProperty::_getRecord() const
// **********************************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->Add(getSlot("Owner", _owner));
    }
    return record;
}



// ****************************************************************************************************
// SharedProperty implementation
// ****************************************************************************************************

SharedProperty::SharedProperty()
// *****************************
:    Inherit(),
    _ownerSet()
{
}

void SharedProperty::_preDestroy()
// ******************************
{
    Inherit::_preDestroy();

    while (!_ownerSet.empty()) {
        DBo* owner = *_ownerSet.begin();
        _ownerSet.erase(owner);
        owner->_onDeleted(this);
    }
}

void SharedProperty::onCapturedBy(DBo* owner)
// ******************************************
{
    _ownerSet.insert(owner);
}

void SharedProperty::onReleasedBy(DBo* owner)
// ******************************************
{
    _ownerSet.erase(owner);

    if (_ownerSet.empty()) onNotOwned();
}

void SharedProperty::onNotOwned()
// ******************************
{
    destroy();
}

string SharedProperty::_getString() const
// **************************************
{
    return Inherit::_getString();
}

Record* SharedProperty::_getRecord() const
// *********************************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->Add(getSlot("Owners", &_ownerSet));
    }
    return record;
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
