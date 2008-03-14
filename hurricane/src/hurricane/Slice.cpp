// ****************************************************************************************************
// File: Slice.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Slice.h"
#include "Cell.h"
#include "Component.h"
#include "Marker.h"
#include "Layer.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Slice implementation
// ****************************************************************************************************

Slice::Slice(Cell* cell, Layer* layer)
// ***********************************
:    _cell(cell),
    _layer(layer),
    _quadTree(),
    _nextOfCellSliceMap(NULL)
{
    if (!_cell)
        throw Error("Can't create " + _TName("Slice") + " : null cell");

    if (!_layer)
        throw Error("Can't create " + _TName("Slice") + " : null layer");

    if (_cell->GetSlice(_layer))
        throw Error("Can't create " + _TName("Slice") + " : already exists");

    _cell->_GetSliceMap()._Insert(this);
}

Slice::~Slice()
// ************
{
    _cell->_GetSliceMap()._Remove(this);
}

Components Slice::GetComponents() const
// ************************************
{
    // return _quadTree.GetGos().GetSubSet<Component*>();
    return SubTypeCollection<Go*, Component*>(_quadTree.GetGos());
}

Components Slice::GetComponentsUnder(const Box& area) const
// ********************************************************
{
    // return _quadTree.GetGosUnder(area).GetSubSet<Component*>();
    return SubTypeCollection<Go*, Component*>(_quadTree.GetGosUnder(area));
}

Markers Slice::GetMarkers() const
// ******************************
{
    // return _quadTree.GetGos().GetSubSet<Marker*>();
    return SubTypeCollection<Go*, Marker*>(_quadTree.GetGos());
}

Markers Slice::GetMarkersUnder(const Box& area) const
// **************************************************
{
    // return _quadTree.GetGosUnder(area).GetSubSet<Marker*>();
    return SubTypeCollection<Go*, Marker*>(_quadTree.GetGosUnder(area));
}

Slice* Slice::_Create(Cell* cell, Layer* layer)
// ********************************************
{
    Slice* slice = new Slice(cell, layer);

    return slice;
}

void Slice::_Delete()
// ******************
{
    delete this;
}

string Slice::_GetString() const
// *****************************
{
    string s = "<" + _TName("Slice");
    // s += " " + GetString(_cell->GetName());
    s += " " + GetString(_layer->getName());
    s += ">";
    return s;
}

Record* Slice::_GetRecord() const
// ************************
{
     Record* record = new Record(GetString(this));
    if (record) {
        record->Add(GetSlot("Cell", _cell));
        record->Add(GetSlot("Layer", _layer));
        record->Add(GetSlot("QuadTree", &_quadTree));
    }
    return record;
}

} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
