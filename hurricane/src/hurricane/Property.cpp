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

void Property::Delete()
// ********************
{
	_PreDelete();

	delete this;
}

void Property::_PostCreate()
// *************************
{
}

void Property::_PreDelete()
// ************************
{
}

string Property::_GetString() const
// ********************************
{
	string s = "<" + _GetTypeName() + ">";
	s.insert(s.length() - 1, " " + GetString(GetName()));
	return s;
}

Record* Property::_GetRecord() const
// ***************************
{
	return new Record(GetString(this));
}



// ****************************************************************************************************
// PrivateProperty implementation
// ****************************************************************************************************

PrivateProperty::PrivateProperty()
// *******************************
:	Inherit(),
	_owner(NULL)
{
}

void PrivateProperty::_PreDelete()
// *******************************
{
	Inherit::_PreDelete();

	if (_owner) _owner->_OnDeleted(this);
}

void PrivateProperty::OnCapturedBy(DBo* owner)
// *******************************************
{
	_owner = owner;
}

void PrivateProperty::OnReleasedBy(DBo* owner)
// *******************************************
{
	if (_owner == owner) OnNotOwned();
}

void PrivateProperty::OnNotOwned()
// *******************************
{
	Delete();
}

string PrivateProperty::_GetString() const
// ***************************************
{
	return Inherit::_GetString();
}

Record* PrivateProperty::_GetRecord() const
// **********************************
{
	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("Owner", _owner));
	}
	return record;
}



// ****************************************************************************************************
// SharedProperty implementation
// ****************************************************************************************************

SharedProperty::SharedProperty()
// *****************************
:	Inherit(),
	_ownerSet()
{
}

void SharedProperty::_PreDelete()
// ******************************
{
	Inherit::_PreDelete();

	while (!_ownerSet.empty()) {
		DBo* owner = *_ownerSet.begin();
		_ownerSet.erase(owner);
		owner->_OnDeleted(this);
	}
}

void SharedProperty::OnCapturedBy(DBo* owner)
// ******************************************
{
	_ownerSet.insert(owner);
}

void SharedProperty::OnReleasedBy(DBo* owner)
// ******************************************
{
	_ownerSet.erase(owner);

	if (_ownerSet.empty()) OnNotOwned();
}

void SharedProperty::OnNotOwned()
// ******************************
{
	Delete();
}

string SharedProperty::_GetString() const
// **************************************
{
	return Inherit::_GetString();
}

Record* SharedProperty::_GetRecord() const
// *********************************
{
	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("Owners", &_ownerSet));
	}
	return record;
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
