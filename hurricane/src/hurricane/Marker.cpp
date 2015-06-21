// ****************************************************************************************************
// File: ./Marker.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify it under the  terms  of the  GNU
// Lesser General Public License as published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without  even
// the implied warranty of MERCHANTABILITY or FITNESS FOR A  PARTICULAR  PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser GNU General Public License along  with  Hurricane.  If
// not, see <http://www.gnu.org/licenses/>.
// ****************************************************************************************************

#include "hurricane/Marker.h"
#include "hurricane/Cell.h"
#include "hurricane/Slice.h"
#include "hurricane/Error.h"

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
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
// ****************************************************************************************************
