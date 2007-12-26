// ****************************************************************************************************
// File: DBo.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "DBo.h"
#include "Property.h"
#include "Quark.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// DBo implementation
// ****************************************************************************************************

DBo::DBo()
// *******
:	_propertySet()
{
}

DBo::~DBo()
// ********
{
}

void DBo::Delete()
// ***************
{
// trace << "entering DBo::Delete: " << this << endl;
// trace_in();

	_PreDelete();

	delete this;

// trace << "exiting DBo::Delete:" << endl;
// trace_out();
}

Property* DBo::GetProperty(const Name& name) const
// ***********************************************
{
	PropertySet::const_iterator iterator = _propertySet.begin();
	while (iterator != _propertySet.end()) {
		Property* property = *iterator;
		if (property->GetName() == name) return property;
		++iterator;
	}
	return NULL;
}

void DBo::Put(Property* property)
// ******************************
{
	if (!property)
		throw Error("Can't put property : null property");

	Property* oldProperty = GetProperty(property->GetName());
	if (property != oldProperty) {
		if (oldProperty) {
			_propertySet.erase(oldProperty);
			oldProperty->OnReleasedBy(this);
		}
		_propertySet.insert(property);
		property->OnCapturedBy(this);
	}
}

void DBo::Remove(Property* property)
// *********************************
{
	if (!property)
		throw Error("Can't remove property : null property");

	if (_propertySet.find(property) != _propertySet.end()) {
		_propertySet.erase(property);
		property->OnReleasedBy(this);
		if (is_a<Quark*>(this) && _propertySet.empty()) Delete();
	}
}

void DBo::RemoveProperty(const Name& name)
// ***************************************
{
	Property* property = GetProperty(name);
	if (property) {
		_propertySet.erase(property);
		property->OnReleasedBy(this);
		if (is_a<Quark*>(this) && _propertySet.empty()) Delete();
	}
}

void DBo::ClearProperties()
// ************************
{
// trace << "entering DBo::ClearProperties: " << this << endl;
// trace_in();

	while (!_propertySet.empty()) {
		Property* property = *_propertySet.begin();
// trace << property << endl;
		_propertySet.erase(property);
		property->OnReleasedBy(this);
	}

// trace << "exiting DBo::ClearProperties:" << endl;
// trace_out();
}

void DBo::_PostCreate()
// ********************
{
}

void DBo::_PreDelete()
// *******************
{
// trace << "entering DBo::_PreDelete: " << this << endl;
// trace_in();

	ClearProperties();


// trace << "exiting DBo::_PreDelete:" << endl;
// trace_out();
}

string DBo::_GetString() const
// ***************************
{
	return "<" + _GetTypeName() + ">";
}

Record* DBo::_GetRecord() const
// **********************
{
 	Record* record = new Record(GetString(this));
	if (record) {
		record->Add(GetSlot("Properties", &_propertySet));
	}
	return record;
}

void DBo::_OnDeleted(Property* property)
// *************************************
{
	if (property && (_propertySet.find(property) != _propertySet.end())) {
		_propertySet.erase(property);
		if (is_a<Quark*>(this) && _propertySet.empty()) Delete();
	}
}

} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
