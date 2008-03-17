// ****************************************************************************************************
// File: Horizontal.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Horizontal.h"
#include "Net.h"
#include "BasicLayer.h"
#include "CompositeLayer.h"
#include "Plug.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Horizontal implementation
// ****************************************************************************************************

Horizontal::Horizontal(Net* net, Component* source, Component* target, Layer* layer, const Unit& y, const Unit& width, const Unit& dxSource, const Unit& dxTarget)
// ****************************************************************************************************
:  Inherit(net, source, target, layer, width),
    _y(y),
    _dxSource(dxSource),
    _dxTarget(dxTarget)
{
}

Horizontal* Horizontal::Create(Net* net, Layer* layer, const Unit& y, const Unit& width, const Unit& dxSource, const Unit& dxTarget)
// ****************************************************************************************************
{
    if (!net)
        throw Error("Can't create " + _TName("Horizontal") + " : null net");

    Horizontal* horizontal = new Horizontal(net, NULL, NULL, layer, y, width, dxSource, dxTarget);

    horizontal->_postCreate();

    return horizontal;
}

Horizontal* Horizontal::Create(Component* source, Component* target, Layer* layer, const Unit& y, const Unit& width, const Unit& dxSource, const Unit& dxTarget)
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
    Unit size = getHalfWidth() + _getSize();
    Unit extention = _getExtention();

    return Box(getSourceX(), _y, getTargetX(), _y).inflate(extention, size);
}

Box Horizontal::getBoundingBox(const BasicLayer* basicLayer) const
// *********************************************************
{
    if (!getLayer()->contains(basicLayer)) return Box();

    Unit size = getHalfWidth() + _getSize(basicLayer);
    Unit extention = _getExtention(basicLayer);

    return Box(getSourceX(), _y, getTargetX(), _y).inflate(extention, size);
}

Unit Horizontal::getSourceX() const
// ********************************
{
    Component* source = getSource();
    return (source) ? source->getX() + _dxSource : _dxSource;
}

Unit Horizontal::getTargetX() const
// ********************************
{
    Component* target = getTarget();
    return (target) ? target->getX() + _dxTarget : _dxTarget;
}

Unit Horizontal::getLength() const
// *******************************
{
    return abs(getSourceX() - getTargetX());
}

Point Horizontal::getCenter() const
// *******************************
{
    return Point ( (getSourceX()+getTargetX())/2, getY() );
}

void Horizontal::Translate(const Unit& dx, const Unit& dy)
// *******************************************************
{
    if (dy != 0) {
        Invalidate(true);
        _y += dy;
    }
}

void Horizontal::SetY(const Unit& y)
// *********************************
{
    if (y != _y) {
        Invalidate(true);
        _y = y;
    }
}

void Horizontal::SetDxSource(const Unit& dxSource)
// ***********************************************
{
    if (dxSource != _dxSource) {
        Invalidate(false);
        _dxSource = dxSource;
    }
}

void Horizontal::SetDxTarget(const Unit& dxTarget)
// ***********************************************
{
    if (dxTarget != _dxTarget) {
        Invalidate(false);
        _dxTarget = dxTarget;
    }
}

void Horizontal::Translate(const Unit& dy)
// ***************************************
{
    if (dy != 0) {
        Invalidate(true);
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
        record->Add(getSlot("Y", &_y));
        record->Add(getSlot("DxSource", &_dxSource));
        record->Add(getSlot("DxTarget", &_dxTarget));
    }
    return record;
}

} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
