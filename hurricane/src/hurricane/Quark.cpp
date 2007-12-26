// ****************************************************************************************************
// File: Quark.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Quark.h"
#include "SharedPath.h"
#include "Entity.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Variables
// ****************************************************************************************************

typedef SharedPath::QuarkMap Quark_QuarkMap;

static Quark_QuarkMap* NULL_SHARED_PATH_QUARK_MAP = NULL;



// ****************************************************************************************************
// Quark implementation
// ****************************************************************************************************

Quark::Quark(const Occurrence& occurrence)
// *************************************
:	Inherit(),
	_occurrence(occurrence),
	_nextOfSharedPathQuarkMap(NULL)
{
	if (!_occurrence.IsValid())
		throw Error("Can't create " + _TName("Quark") + " : invalid occurrence");

	if (_occurrence._GetQuark())
		throw Error("Can't create " + _TName("Quark") + " : already exists");
}

Quark* Quark::_Create(const Occurrence& occurrence)
// **********************************************
{
	Quark* quark = new Quark(occurrence);

	quark->_PostCreate();

	return quark;
}

void Quark::_PostCreate()
// **********************
{
	SharedPath* sharedPath = _occurrence._GetSharedPath();

	if (sharedPath)
		sharedPath->_GetQuarkMap()._Insert(this);
	else {
		if (!NULL_SHARED_PATH_QUARK_MAP) NULL_SHARED_PATH_QUARK_MAP = new Quark_QuarkMap();
		NULL_SHARED_PATH_QUARK_MAP->_Insert(this);
	}

	Inherit::_PostCreate();
}

void Quark::_PreDelete()
// *********************
{
// trace << "entering Quark::_PreDelete: " << this << endl;
// trace_in();

	Inherit::_PreDelete();

	SharedPath* sharedPath = _occurrence._GetSharedPath();

	if (sharedPath)
		sharedPath->_GetQuarkMap()._Remove(this);
	else
		if (NULL_SHARED_PATH_QUARK_MAP) NULL_SHARED_PATH_QUARK_MAP->_Remove(this);

// trace << "exiting Quark::_PreDelete:" << endl;
// trace_out();
}

string Quark::_GetString() const
// *****************************
{
	string s = Inherit::_GetString();
	s.insert(s.length() - 1, " " + GetString(_occurrence));
	return s;
}

Record* Quark::_GetRecord() const
// ************************
{
	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("Occurrence", &_occurrence));
	}
	return record;
}



// ****************************************************************************************************
// Entity implementation (located here to access the local variables)
// ****************************************************************************************************

Quark* Entity::_GetQuark(SharedPath* sharedPath) const
// ***************************************************
{
	if (sharedPath)
		return sharedPath->_GetQuark(this);
	else {
		if (!NULL_SHARED_PATH_QUARK_MAP) return NULL;
		return NULL_SHARED_PATH_QUARK_MAP->GetElement(this);
	}
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
