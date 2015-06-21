// ****************************************************************************************************
// File: ./Pad.cpp
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

#include "hurricane/Pad.h"
#include "hurricane/Net.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Layer.h"
#include "hurricane/Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Pad implementation
// ****************************************************************************************************

Pad::Pad(Net* net, const Layer* layer, const Box& boundingBox)
// ***********************************************************
:  Inherit(net),
    _layer(layer),
    _boundingBox(boundingBox)
{
    if (!_layer)
        throw Error("Can't create " + _TName("Pad") + " : null layer");

    if (_boundingBox.isEmpty())
        throw Error("Can't create " + _TName("Pad") + " : empty bounding box");
}

Pad* Pad::create(Net* net, const Layer* layer, const Box& boundingBox)
// *******************************************************************
{
    Pad* pad = new Pad(net, layer, boundingBox);

    pad->_postCreate();

    return pad;
}

DbU::Unit Pad::getX() const
// *******************
{
  return 0;
}

DbU::Unit Pad::getY() const
// *******************
{
    return 0;
}

Box Pad::getBoundingBox() const
// ****************************
{
    Box boundingBox = _boundingBox;

    return boundingBox;
}

Box Pad::getBoundingBox(const BasicLayer* basicLayer) const
// **************************************************
{
    if (!_layer->contains(basicLayer)) return Box();

    Box boundingBox = _boundingBox;

    return boundingBox;
}

void Pad::translate(const DbU::Unit& dx, const DbU::Unit& dy)
// ************************************************
{
    if ((dx != 0) || (dy != 0)) {
        invalidate(true);
        _boundingBox.translate(dx, dy);
    }
}

void Pad::setLayer(const Layer* layer)
// ***********************************
{
    if (!layer)
        throw Error("Can't set layer : null layer");

    if (layer != _layer) {
        invalidate(false);
        _layer = layer;
    }
}

void Pad::setBoundingBox(const Box& boundingBox)
// *********************************************
{
    if (_boundingBox.isEmpty())
        throw Error("Can't set bounding box : empty bounding box");

    if (boundingBox != _boundingBox) {
        invalidate(true);
        _boundingBox = boundingBox;
    }
}

string Pad::_getString() const
// ***************************
{
    string s = Inherit::_getString();
    s.insert(s.length() - 1, " " + getString(_layer->getName()));
    s.insert(s.length() - 1, " " + getString(_boundingBox));
    return s;
}

Record* Pad::_getRecord() const
// **********************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->add(getSlot("Layer", _layer));
        record->add(getSlot("BoundingBox", &_boundingBox));
    }
    return record;
}

} // End of Hurricane namespace.


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2015, All Rights Reserved
// ****************************************************************************************************
