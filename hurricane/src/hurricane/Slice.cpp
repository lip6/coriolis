// ****************************************************************************************************
// File: ./Slice.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
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

#include "hurricane/Slice.h"
#include "hurricane/Cell.h"
#include "hurricane/Component.h"
#include "hurricane/Marker.h"
#include "hurricane/Layer.h"
#include "hurricane/Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Slice implementation
// ****************************************************************************************************

Slice::Slice(Cell* cell, const Layer* layer)
// *****************************************
:    _cell(cell),
    _layer(layer),
    _quadTree(),
    _nextOfCellSliceMap(NULL)
{
    if (!_cell)
        throw Error("Can't create " + _TName("Slice") + " : null cell");

    if (!_layer)
        throw Error("Can't create " + _TName("Slice") + " : null layer");

    if (_cell->getSlice(_layer))
        throw Error("Can't create " + _TName("Slice") + " : already exists");

    _cell->_getSliceMap()->_insert(this);
}

Slice::~Slice()
// ************
{
    _cell->_getSliceMap()->_remove(this);
}

Components Slice::getComponents() const
// ************************************
{
    // return _quadTree.getGos().getSubSet<Component*>();
    return SubTypeCollection<Go*, Component*>(_quadTree.getGos());
}

Components Slice::getComponentsUnder(const Box& area, DbU::Unit threshold) const
// *****************************************************************************
{
    // return _quadTree.getGosUnder(area).getSubSet<Component*>();
    return SubTypeCollection<Go*, Component*>(_quadTree.getGosUnder(area,threshold));
}

Markers Slice::getMarkers() const
// ******************************
{
    // return _quadTree.getGos().getSubSet<Marker*>();
    return SubTypeCollection<Go*, Marker*>(_quadTree.getGos());
}

Markers Slice::getMarkersUnder(const Box& area) const
// **************************************************
{
    // return _quadTree.getGosUnder(area).getSubSet<Marker*>();
    return SubTypeCollection<Go*, Marker*>(_quadTree.getGosUnder(area));
}

Slice* Slice::_create(Cell* cell, const Layer* layer)
// ********************************************
{
    Slice* slice = new Slice(cell, layer);

    return slice;
}

void Slice::_destroy()
// ******************
{
    delete this;
}

string Slice::_getString() const
// *****************************
{
    string s = "<" + _TName("Slice");
    // s += " " + getString(_cell->getName());
    s += " " + getString(_layer->getName());
    s += ">";
    return s;
}

Record* Slice::_getRecord() const
// ************************
{
     Record* record = new Record(getString(this));
    if (record) {
        record->add(getSlot("Cell", _cell));
        record->add(getSlot("Layer", _layer));
        record->add(getSlot("QuadTree", &_quadTree));
    }
    return record;
}

} // End of Hurricane namespace.


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
