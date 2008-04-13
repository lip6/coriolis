// ****************************************************************************************************
// File: Pad.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
// 21-10-2003 Alignment BULL-LIP6 (to be seen : PROVISOIREMENT in _Draw & _Highlight)

#include "Pad.h"
#include "Net.h"
#include "BasicLayer.h"
#include "CompositeLayer.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Pad implementation
// ****************************************************************************************************

Pad::Pad(Net* net, Layer* layer, const Box& boundingBox)
// *****************************************************
:  Inherit(net),
    _layer(layer),
    _boundingBox(boundingBox)
{
    if (!_layer)
        throw Error("Can't create " + _TName("Pad") + " : null layer");

    if (_boundingBox.isEmpty())
        throw Error("Can't create " + _TName("Pad") + " : empty bounding box");
}

Pad* Pad::create(Net* net, Layer* layer, const Box& boundingBox)
// *************************************************************
{
    Pad* pad = new Pad(net, layer, boundingBox);

    pad->_postCreate();

    return pad;
}

Unit Pad::getX() const
// *******************
{
    return 0;
}

Unit Pad::getY() const
// *******************
{
    return 0;
}

Box Pad::getBoundingBox() const
// ****************************
{
    Box boundingBox = _boundingBox;

    if (is_a<CompositeLayer*>(_layer))
        boundingBox.inflate(((CompositeLayer*)_layer)->getMaximalPadSize());

    return boundingBox;
}

Box Pad::getBoundingBox(const BasicLayer* basicLayer) const
// **************************************************
{
    if (!_layer->contains(basicLayer)) return Box();

    Box boundingBox = _boundingBox;

    if (is_a<CompositeLayer*>(_layer))
        boundingBox.inflate(((CompositeLayer*)_layer)->getPadSize(basicLayer));

    return boundingBox;
}

void Pad::translate(const Unit& dx, const Unit& dy)
// ************************************************
{
    if ((dx != 0) || (dy != 0)) {
        invalidate(true);
        _boundingBox.translate(dx, dy);
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
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
