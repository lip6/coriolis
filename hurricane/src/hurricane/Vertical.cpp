// ****************************************************************************************************
// File: Vertical.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Vertical.h"
#include "Net.h"
#include "BasicLayer.h"
#include "CompositeLayer.h"
#include "Plug.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Vertical implementation
// ****************************************************************************************************

Vertical::Vertical(Net* net, Component* source, Component* target, Layer* layer, const Unit& x, const Unit& width, const Unit& dySource, const Unit& dyTarget)
// ****************************************************************************************************
:  Inherit(net, source, target, layer, width),
    _x(x),
    _dySource(dySource),
    _dyTarget(dyTarget)
{
}

Vertical* Vertical::create(Net* net, Layer* layer, const Unit& x, const Unit& width, const Unit& dySource, const Unit& dyTarget)
// ****************************************************************************************************
{
    if (!net)
        throw Error("Can't create " + _TName("Vertical") + " : null net");

    Vertical* vertical = new Vertical(net, NULL, NULL, layer, x, width, dySource, dyTarget);

    vertical->_postCreate();

    return vertical;
}

Vertical* Vertical::create(Component* source, Component* target, Layer* layer, const Unit& x, const Unit& width, const Unit& dySource, const Unit& dyTarget)
// ****************************************************************************************************
{
    if (!source)
        throw Error("Can't create " + _TName("Vertical") + " : null source");

    if (!target)
        throw Error("Can't create " + _TName("Vertical") + " : null target");

    Vertical* vertical =
        new Vertical(source->getNet(), source, target, layer, x, width, dySource, dyTarget);

    vertical->_postCreate();

    return vertical;
}

Box Vertical::getBoundingBox() const
// *********************************
{
    Unit size = getHalfWidth() + _getSize();
    Unit extention = _getExtention();

    return Box(_x, getSourceY(), _x, getTargetY()).inflate(size, extention);
}

Box Vertical::getBoundingBox(const BasicLayer* basicLayer) const
// *******************************************************
{
    if (!getLayer()->contains(basicLayer)) return Box();

    Unit size = getHalfWidth() + _getSize(basicLayer);
    Unit extention = _getExtention(basicLayer);

    return Box(_x, getSourceY(), _x, getTargetY()).inflate(size, extention);
}

Unit Vertical::getSourceY() const
// ******************************
{
    Component* source = getSource();
    return (source) ? source->getY() + _dySource : _dySource;
}

Unit Vertical::getTargetY() const
// ******************************
{
    Component* target = getTarget();
    return (target) ? target->getY() + _dyTarget : _dyTarget;
}

Unit Vertical::getLength() const
// *****************************
{
    return abs(getSourceY() - getTargetY());
}

Point Vertical::getCenter() const
// *******************************
{
        return Point ( getX(), (getSourceY()+getTargetY())/2 );
}

void Vertical::translate(const Unit& dx, const Unit& dy)
// *****************************************************
{
    if (dx != 0) {
        invalidate(true);
        _x += dx;
    }
}

void Vertical::setX(const Unit& x)
// *******************************
{
    if (x != _x) {
        invalidate(true);
        _x = x;
    }
}

void Vertical::setDySource(const Unit& dySource)
// *********************************************
{
    if (dySource != _dySource) {
        invalidate(false);
        _dySource = dySource;
    }
}

void Vertical::setDyTarget(const Unit& dyTarget)
// *********************************************
{
    if (dyTarget != _dyTarget) {
        invalidate(false);
        _dyTarget = dyTarget;
    }
}

void Vertical::translate(const Unit& dx)
// *************************************
{
    if (dx != 0) {
        invalidate(true);
        _x += dx;
    }
}

string Vertical::_getString() const
// ********************************
{
    return Inherit::_getString();
}

Record* Vertical::_getRecord() const
// ***************************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->add(getSlot("X", &_x));
        record->add(getSlot("DySource", &_dySource));
        record->add(getSlot("DyTarget", &_dyTarget));
    }
    return record;
}

} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
