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

void Marker::materialize()
// ***********************
{
    if (!isMaterialized()) {
        Cell* cell = getCell();
        QuadTree* quadTree = cell->_getQuadTree();
        quadTree->insert(this);
        cell->_fit(quadTree->getBoundingBox());
    }
}

void Marker::unmaterialize()
// *************************
{
    if (isMaterialized()) {
        Cell* cell = getCell();
        cell->_unfit(getBoundingBox());
        cell->_getQuadTree()->remove(this);
    }
}

void Marker::_postCreate()
// ***********************
{
    _cell->_getMarkerSet()._insert(this);

    Inherit::_postCreate();
}

void Marker::_preDestroy()
// **********************
{
    Inherit::_preDestroy();

    _cell->_getMarkerSet()._remove(this);
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
        record->add(getSlot("Cell", _cell));
    }
    return record;
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
