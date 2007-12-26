// ****************************************************************************************************
// File: Marker.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Marker.h"
#include "Cell.h"
#include "Slice.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Marker implementation
// ****************************************************************************************************

Marker::Marker(Cell* cell)
// ***********************
:  Inherit(),
	_cell(cell),
	_nextOfCellMarkerSet(NULL)
{
	if (!_cell)
		throw Error("Can't create " + _TName("Marker") + " : null cell");
}

void Marker::Materialize()
// ***********************
{
	if (!IsMaterialized()) {
		Cell* cell = GetCell();
		QuadTree* quadTree = cell->_GetQuadTree();
		quadTree->Insert(this);
		cell->_Fit(quadTree->GetBoundingBox());
	}
}

void Marker::Unmaterialize()
// *************************
{
	if (IsMaterialized()) {
		Cell* cell = GetCell();
		cell->_Unfit(GetBoundingBox());
		cell->_GetQuadTree()->Remove(this);
	}
}

void Marker::_PostCreate()
// ***********************
{
	_cell->_GetMarkerSet()._Insert(this);

	Inherit::_PostCreate();
}

void Marker::_PreDelete()
// **********************
{
	Inherit::_PreDelete();

	_cell->_GetMarkerSet()._Remove(this);
}

string Marker::_GetString() const
// ******************************
{
	string s = Inherit::_GetString();
	s.insert(s.length() - 1, " " + GetString(_cell->GetName()));
	return s;
}

Record* Marker::_GetRecord() const
// *************************
{
	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("Cell", _cell));
	}
	return record;
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
