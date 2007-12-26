// ****************************************************************************************************
// File: Go.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Go.h"
#include "QuadTree.h"

namespace Hurricane {



// ****************************************************************************************************
// Go implementation
// ****************************************************************************************************

static bool AUTO_MATERIALIZATION_IS_ENABLED = true;

Go::Go()
// *****
:	Inherit(),
	_quadTree(NULL),
	_nextOfQuadTreeGoSet(NULL)
{
}

bool Go::AutoMaterializationIsDisabled()
// *************************************
{
	return !AUTO_MATERIALIZATION_IS_ENABLED;
}

void Go::EnableAutoMaterialization()
// *********************************
{
	AUTO_MATERIALIZATION_IS_ENABLED = true;
}

void Go::DisableAutoMaterialization()
// **********************************
{
	AUTO_MATERIALIZATION_IS_ENABLED = false;
}

void Go::_PostCreate()
// *******************
{
	Inherit::_PostCreate();

	if (!AutoMaterializationIsDisabled()) Materialize(); // materialized after entire post creation
}

void Go::_PreDelete()
// ******************
{
// trace << "entering Go::_PreDelete: " << this << endl;
// trace_in();

	Unmaterialize(); // unmaterialized before starting pre destruction

	Inherit::_PreDelete();

// trace << "exiting Go::_PreDelete:" << endl;
// trace_out();
}

string Go::_GetString() const
// **************************
{
	return Inherit::_GetString();
}

Record* Go::_GetRecord() const
// *********************
{
	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("QuadTree", _quadTree));
	}
	return record;
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
