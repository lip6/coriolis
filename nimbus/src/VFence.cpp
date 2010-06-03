
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Date   : 29/01/2004
// Author : Hugo Clément                  <Hugo.Clement@lip6.fr>


#include "nimbus/Nimbus.h"
#include "nimbus/RoutTools.h"
#include "nimbus/VFence.h"

namespace Nimbus {

VFence::VFence (Grid* grid, GCell* gcleft, GCell* gcright)
    : Fence    (grid)
    , _gcleft  (gcleft)
    , _gcright (gcright)
{
    return;
}

VFence::~VFence ()
{
    _gcleft->setRightFence(NULL);
    _gcright->setLeftFence(NULL);

    return;
}

VFence* VFence::create (Grid* grid, GCell* gcleft, GCell* gcright) {

    if ( (!gcleft) && (!gcright) )
        throw Error("cannot create a flying fence");

    unsigned step;
    if (gcleft) {
        step = gcleft->getStep();
    } else if (gcright) {
        step = gcright->getStep();
    } else {
        step = 0;
    }

    VFence* vfence = new VFence (grid, gcleft, gcright);

    if (!vfence)
        throw Error ("cannot create fence");

    vfence->_postCreate();

    return vfence;
}

void VFence::_postCreate() {

    DbU::Unit size = 0;
    if (_gcleft) {
        size = _gcleft->getHeight();
        _gcleft->setRightFence (this);
        _step = _gcleft->getStep();
    }
    if (_gcright) {
        size = _gcright->getHeight();
        _gcright->setLeftFence (this);
        _step = _gcright->getStep();
    }

    setSize(size);

    Inherit::_postCreate();

    return;
}

string VFence::_getString() const {
    return "<" + _TName ( "VFence")
         + " " + getString ( getX() )
         + " " + getString ( getY() ) + ">"; 
}

Record* VFence::_getRecord() const {
    Record* record = Inherit::_getRecord();

    if (record) {
        record->add(getSlot("Left nimbox", _gcleft));
        record->add(getSlot("Right nimbox", _gcright));
    }

    return record;
}


DbU::Unit VFence::getX() const {
    DbU::Unit X;

    if (_gcleft) {
        X = _gcleft->getXMax();
    } else if (_gcright) {
        X = _gcright->getXMin();
    } else {
        throw Error ("Floating fence");
    }

    return X;
}

DbU::Unit VFence::getY() const {
    DbU::Unit Y;

    if (_gcleft) {
        Y = _gcleft->getYCenter();
    } else if (_gcright) {
        Y = _gcright->getYCenter();
    } else {
        throw Error ("Floating fence");
    }

    return Y;
}

DbU::Unit VFence::getYMin() const {

    DbU::Unit Y;

    if (_gcleft) {
        Y = _gcleft->getYMin();
    } else if (_gcright) {
        Y = _gcright->getYMin();
    } else {
        throw Error ("Floating fence");
    }
    
    return Y;
}

DbU::Unit VFence::getYMax() const {

    DbU::Unit Y;

    if (_gcleft) {
        Y = _gcleft->getYMax();
    } else if (_gcright) {
        Y = _gcright->getYMax();
    } else {
        throw Error ("Floating fence");
    }
    
    return Y;
}

Point VFence::getP1() const {

    DbU::Unit X = getX();
    DbU::Unit Y = getYMin();

    return Point(X, Y);
}

Point VFence::getP2() const {

    DbU::Unit X = getX();
    DbU::Unit Y = getYMax();
    
    return Point(X, Y);
}

DbU::Unit VFence::getXCenter() const {

    DbU::Unit X = getX();

    return X;
}

DbU::Unit VFence::getYCenter() const {

    DbU::Unit Y = (getYMin()+getYMax()) / 2;

    return Y;
}

Point VFence::getCenter() const {

    DbU::Unit X = getXCenter();
    DbU::Unit Y = getYCenter();

    return Point(X, Y);
}

void VFence::_moveTo (DbU::Unit target)
{
    Inherit::_moveTo (target);
    getLeftGCell()->setXMax(target);
    getRightGCell()->setXMin(target);
    for_each_splitter (splitter, getSplitters())
    {
        splitter->setX(target);
        end_for;
    }
    return;
}

void VFence::computeCapacity() {

    unsigned capa(0);
    const vector<RoutingLayerGauge*>& gauges = Fence::_routingGauge->getLayerGauges();
    for ( size_t i=0 ; i<gauges.size() ; i++ )
    {
        if (gauges[i]->getType() != Constant::Default)
            continue;
        if (gauges[i]->getDirection() == Constant::Vertical)
            continue;
        capa += gauges[i]->getTrackNumber(getYMin(), getYMax());
    }
    setCapacity(capa);

#if 0
    unsigned pitch = static_cast<unsigned>(getValue (getCDataBase()->getPitch()));
    unsigned nlayers = getGrid()->getNimbus()->getNumberOfRoutingLayers();
    double capa (0);
    double powerrail (0);
    for_each_layer (layer, _grid->getNimbus()->getRoutingLayers())
    {
        double pitch = getValue(layer->getPitch());
        if (pitch == 0) pitch = 5;
        capa += getValue(_size) / pitch / 2;
        powerrail += ( (12 * (getValue(_size) / pitch) /50) );
        end_for;
    }
    
    setCapacity(static_cast<unsigned> ((capa - powerrail)/2));
#endif
    return;
}

} // namespace Nimbus
