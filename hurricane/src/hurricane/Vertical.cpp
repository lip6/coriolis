// ****************************************************************************************************
// File: Vertical.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "hurricane/Vertical.h"
#include "hurricane/Layer.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Net.h"
#include "hurricane/Plug.h"
#include "hurricane/Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Vertical implementation
// ****************************************************************************************************

Vertical::Vertical(Net* net, Component* source, Component* target, const Layer* layer, const DbU::Unit& x, const DbU::Unit& width, const DbU::Unit& dySource, const DbU::Unit& dyTarget)
// ****************************************************************************************************
:  Inherit(net, source, target, layer, width),
    _x(x),
    _dySource(dySource),
    _dyTarget(dyTarget)
{
}

Vertical* Vertical::create(Net* net, const Layer* layer, const DbU::Unit& x, const DbU::Unit& width, const DbU::Unit& dySource, const DbU::Unit& dyTarget)
// ****************************************************************************************************
{
    if (!net)
        throw Error("Can't create " + _TName("Vertical") + " : null net");

    Vertical* vertical = new Vertical(net, NULL, NULL, layer, x, width, dySource, dyTarget);

    vertical->_postCreate();

    return vertical;
}

Vertical* Vertical::create(Component* source, Component* target, const Layer* layer, const DbU::Unit& x, const DbU::Unit& width, const DbU::Unit& dySource, const DbU::Unit& dyTarget)
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
  DbU::Unit size      = getLayer()->getExtentionWidth() + getHalfWidth();
  DbU::Unit extention = getLayer()->getExtentionCap  ();

    return Box(_x, getSourceY(), _x, getTargetY()).inflate(size, extention);
}

Box Vertical::getBoundingBox(const BasicLayer* basicLayer) const
// *******************************************************
{
  if (!getLayer()->contains(basicLayer)) return Box();


    DbU::Unit size      = getLayer()->getExtentionWidth(basicLayer) + getHalfWidth();
    DbU::Unit extention = getLayer()->getExtentionCap  (basicLayer);

  //cerr << this << " on: " << basicLayer << " " << size << " " << extention << endl;
  //cerr << Box(_x, getSourceY(), _x, getTargetY()).inflate(size, extention) << endl;

    return Box(_x, getSourceY(), _x, getTargetY()).inflate(size, extention);
}

DbU::Unit Vertical::getSourceY() const
// ******************************
{
    Component* source = getSource();
    return (source) ? source->getY() + _dySource : _dySource;
}

DbU::Unit Vertical::getTargetY() const
// ******************************
{
    Component* target = getTarget();
    return (target) ? target->getY() + _dyTarget : _dyTarget;
}

DbU::Unit Vertical::getLength() const
// *****************************
{
    return abs(getSourceY() - getTargetY());
}

Point Vertical::getCenter() const
// *******************************
{
        return Point ( getX(), (getSourceY()+getTargetY())/2 );
}

void Vertical::translate(const DbU::Unit& dx, const DbU::Unit& dy)
// *****************************************************
{
    if (dx != 0) {
        invalidate(true);
        _x += dx;
    }
}

void Vertical::setX(const DbU::Unit& x)
// *******************************
{
    if (x != _x) {
        invalidate(true);
        _x = x;
    }
}

void Vertical::setDySource(const DbU::Unit& dySource)
// *********************************************
{
    if (dySource != _dySource) {
        invalidate(false);
        _dySource = dySource;
    }
}

void Vertical::setDyTarget(const DbU::Unit& dyTarget)
// *********************************************
{
    if (dyTarget != _dyTarget) {
        invalidate(false);
        _dyTarget = dyTarget;
    }
}

void Vertical::translate(const DbU::Unit& dx)
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
