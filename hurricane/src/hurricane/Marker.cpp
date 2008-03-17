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
        Cell* cell = getCell();
        QuadTree* quadTree = cell->_getQuadTree();
        quadTree->Insert(this);
        cell->_Fit(quadTree->getBoundingBox());
    }
}

void Marker::Unmaterialize()
// *************************
{
    if (IsMaterialized()) {
        Cell* cell = getCell();
        cell->_Unfit(getBoundingBox());
        cell->_getQuadTree()->Remove(this);
    }
}

void Marker::_postCreate()
// ***********************
{
    _cell->_getMarkerSet()._Insert(this);

    Inherit::_postCreate();
}

void Marker::_preDestroy()
// **********************
{
    Inherit::_preDestroy();

    _cell->_getMarkerSet()._Remove(this);
}

string Marker::_getString() const
// ******************************
{
    string s = Inherit::_getString();
    s.insert(s.length() - 1, " " + getString(_cell->getName()));
    return s;
}

Record* Marker::_getRecord() const
// *************************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->Add(getSlot("Cell", _cell));
    }
    return record;
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
