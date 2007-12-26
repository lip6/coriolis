// ****************************************************************************************************
// File: SharedName.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "SharedName.h"

namespace Hurricane {



// ****************************************************************************************************
// SharedName implementation
// ****************************************************************************************************

SharedName::SharedNameMap* SharedName::_SHARED_NAME_MAP = NULL;

SharedName::SharedName(const string& s)
// ************************************
:	_count(0),
	_string(s)
{
	if (!_SHARED_NAME_MAP) _SHARED_NAME_MAP = new SharedNameMap();
	(*_SHARED_NAME_MAP)[&_string] = this;
}

SharedName::~SharedName()
// **********************
{
	_SHARED_NAME_MAP->erase(&_string);
}

void SharedName::Capture()
// ***********************
{
	_count++;
}

void SharedName::Release()
// ***********************
{
	if (!--_count) delete this;
}

string SharedName::_GetString() const
// **********************************
{
	return "<" + _TName("SharedName") + " " + GetString(_count) + " " + _string + ">";
}

Record* SharedName::_GetRecord() const
// *****************************
{
	Record* record = new Record(GetString(this));
	record->Add(GetSlot("Count", &_count));
	record->Add(GetSlot("String", &_string));
	return record;
}



// ****************************************************************************************************
// SharedName::SharedNameMapComparator
// ****************************************************************************************************

bool SharedName::SharedNameMapComparator::operator()(string* s1, string* s2) const
// *******************************************************************************
{
	return (*s1) < (*s2);
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
