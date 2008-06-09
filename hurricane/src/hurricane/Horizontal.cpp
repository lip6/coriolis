// ****************************************************************************************************
// File: Horizontal.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "hurricane/Horizontal.h"
#include "hurricane/Layer.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/Net.h"
#include "hurricane/Plug.h"
#include "hurricane/Error.h"


namespace Hurricane {



// ****************************************************************************************************
// Horizontal implementation
// ****************************************************************************************************

Horizontal::Horizontal(Net* net, Component* source, Component* target, const Layer* layer, const DbU::Unit& y, const DbU::Unit& width, const DbU::Unit& dxSource, const DbU::Unit& dxTarget)
// ****************************************************************************************************
:  Inherit(net, source, target, layer, width),
    _y(y),
    _dxSource(dxSource),
    _dxTarget(dxTarget)
{
}

Horizontal* Horizontal::create(Net* net, const Layer* layer, const DbU::Unit& y, const DbU::Unit& width, const DbU::Unit& dxSource, const DbU::Unit& dxTarget)
// ****************************************************************************************************
{
    if (!net)
        throw Error("Can't create " + _TName("Horizontal") + " : null net");

    Horizontal* horizontal = new Horizontal(net, NULL, NULL, layer, y, width, dxSource, dxTarget);

    horizontal->_postCreate();

    return horizontal;
}

Horizontal* Horizontal::create(Component* source, Component* target, const Layer* layer, const DbU::Unit& y, const DbU::Unit& width, const DbU::Unit& dxSource, const DbU::Unit& dxTarget)
// ****************************************************************************************************
{
    if (!source)
        throw Error("Can't create " + _TName("Horizontal") + " : null source");

    if (!target)
        throw Error("Can't create " + _TName("Horizontal") + " : null target");

    Horizontal* horizontal =
        new Horizontal(source->getNet(), source, target, layer, y, width, dxSource, dxTarget);

    horizontal->_postCreate();

    return horizontal;
}

Box Horizontal::getBoundingBox() const
// ***********************************
{
  DbU::Unit size      = getLayer()->getExtentionWidth() + getHalfWidth();
  DbU::Unit extention = getLayer()->getExtentionCap  ();

    return Box(getSourceX(), _y, getTargetX(), _y).inflate(extention, size);
}

Box Horizontal::getBoundingBox(const BasicLayer* basicLayer) const
// *********************************************************
{
    if (!getLayer()->contains(basicLayer)) return Box();

    DbU::Unit size      = getLayer()->getExtentionWidth(basicLayer) + getHalfWidth();
    DbU::Unit extention = getLayer()->getExtentionCap  (basicLayer);

    return Box(getSourceX(), _y, getTargetX(), _y).inflate(extention, size);
}

DbU::Unit Horizontal::getSourceX() const
// ********************************
{
    Component* source = getSource();
    return (source) ? source->getX() + _dxSource : _dxSource;
}

DbU::Unit Horizontal::getTargetX() const
// ********************************
{
    Component* target = getTarget();
    return (target) ? target->getX() + _dxTarget : _dxTarget;
}

DbU::Unit Horizontal::getLength() const
// *******************************
{
    return abs(getSourceX() - getTargetX());
}

Point Horizontal::getCenter() const
// *******************************
{
    return Point ( (getSourceX()+getTargetX())/2, getY() );
}

void Horizontal::translate(const DbU::Unit& dx, const DbU::Unit& dy)
// *******************************************************
{
    if (dy != 0) {
        invalidate(true);
        _y += dy;
    }
}

void Horizontal::setY(const DbU::Unit& y)
// *********************************
{
    if (y != _y) {
        invalidate(true);
        _y = y;
    }
}

void Horizontal::setDxSource(const DbU::Unit& dxSource)
// ***********************************************
{
    if (dxSource != _dxSource) {
        invalidate(false);
        _dxSource = dxSource;
    }
}

void Horizontal::setDxTarget(const DbU::Unit& dxTarget)
// ***********************************************
{
    if (dxTarget != _dxTarget) {
        invalidate(false);
        _dxTarget = dxTarget;
    }
}

void Horizontal::translate(const DbU::Unit& dy)
// ***************************************
{
    if (dy != 0) {
        invalidate(true);
        _y += dy;
    }
}

string Horizontal::_getString() const
// **********************************
{
    return Inherit::_getString();
}

Record* Horizontal::_getRecord() const
// *****************************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->add(getSlot("Y", &_y));
        record->add(getSlot("DxSource", &_dxSource));
        record->add(getSlot("DxTarget", &_dxTarget));
    }
    return record;
}

} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
