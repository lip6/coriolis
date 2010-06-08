
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Date   : 29/01/2004
// Author : Hugo Clément                  <Hugo.Clement@lip6.fr>


#include "crlcore/RoutingLayerGauge.h"
#include "nimbus/NimbusEngine.h"
#include "nimbus/RoutTools.h"
#include "nimbus/HFence.h"

namespace Nimbus {

HFence::HFence ( Grid* grid, GCell* gcup, GCell* gcdown)
    : Fence   (grid)
    , _gcup   (gcup)
    , _gcdown (gcdown)
{
    return;
}

HFence::~HFence ()
{
    _gcup->setDownFence(NULL);
    _gcdown->setUpFence(NULL);

    return;
}

HFence* HFence::create (Grid* grid, GCell* gcup, GCell* gcdown) {

    if ( (!gcup) && (!gcdown) )
        throw Error("cannot create a flying fence");
            
    unsigned step;
    if (gcup) {
        step = gcup->getStep();
    } else if (gcdown) {
        step = gcdown->getStep();
    } else {
        step = 0;
    }

    HFence* hfence = new HFence (grid, gcup, gcdown);

    if (!hfence)
        throw Error("cannot create Fence");

    hfence->_postCreate();

    return hfence;
}

void HFence::_postCreate () {

    DbU::Unit size = 0;
    if (_gcup) {
        size = _gcup->getWidth();
        _gcup->setDownFence (this);
        _step = _gcup->getStep();
    }
    if (_gcdown) {
        size = _gcdown->getWidth();
        _gcdown->setUpFence (this);
        _step = _gcdown->getStep();
    }

    setSize(size);

    Inherit::_postCreate();

    return;
}

string HFence::_getString() const {
    return "<" + _TName ( "HFence")
         + " " + getString ( getX() )
         + " " + getString ( getY() ) + ">"; 
}

Record* HFence::_getRecord() const {
    Record* record = Inherit::_getRecord();

    if (record) {
        record->add(getSlot("Up gcell", _gcup));
        record->add(getSlot("Down gcell", _gcdown));
    }

    return record;
}

DbU::Unit HFence::getY() const {
    DbU::Unit Y;

    if (_gcup) {
        Y = _gcup->getYMin();
    } else if (_gcdown) {
        Y = _gcdown->getYMax();
    } else {
        throw Error ("Floating fence");
    }

    return Y;
}

DbU::Unit HFence::getXMin() const {

    DbU::Unit X;

    if (_gcup) {
        X = _gcup->getXMin();
    } else if (_gcdown) {
        X = _gcdown->getXMin();
    } else {
        throw Error ("Floating fence");
    }
    
    return X;
}

DbU::Unit HFence::getXMax() const {

    DbU::Unit X;

    if (_gcup) {
        X = _gcup->getXMax();
    } else if (_gcdown) {
        X = _gcdown->getXMax();
    } else {
        throw Error ("Floating fence");
    }
    
    return X;
}

DbU::Unit HFence::getX() const {
    DbU::Unit X;

    if (_gcup) {
        X = _gcup->getXCenter();
    } else if (_gcdown) {
        X = _gcdown->getXCenter();
    } else {
        throw Error ("Floating fence");
    }

    return X;
}

Point HFence::getP1() const {

    DbU::Unit X = getXMin();
    DbU::Unit Y = getY();

    return Point(X, Y);
}

Point HFence::getP2() const {

    DbU::Unit X = getXMax();
    DbU::Unit Y = getY();
    
    return Point(X, Y);
}

DbU::Unit HFence::getXCenter() const {

    DbU::Unit X = (getXMin() + getXMax()) / 2;

    return X;
}

DbU::Unit HFence::getYCenter() const {

    DbU::Unit Y = getY();

    return Y;
}

Point HFence::getCenter() const {

    DbU::Unit X = getXCenter();
    DbU::Unit Y = getYCenter();

    return Point(X, Y);
}

void HFence::_moveTo (DbU::Unit target)
{
    Inherit::_moveTo (target);
    getUpGCell()->setYMin(target);
    getDownGCell()->setYMax(target);
    for_each_splitter (splitter, getSplitters())
    {
        splitter->setY(target);
        end_for;
    }
    return;
}

void HFence::computeCapacity() {

    unsigned capa(0);
    const vector<RoutingLayerGauge*>& gauges = Fence::_routingGauge->getLayerGauges();
    for ( size_t i=0 ; i<gauges.size() ; i++)
    {
        if (gauges[i]->getType() != Constant::Default)
            continue;
        if (gauges[i]->getDirection() == Constant::Horizontal)
            continue;
        capa += gauges[i]->getTrackNumber(getXMin(), getXMax());
    }
    setCapacity(capa);
#if 0
    unsigned pitch = static_cast<unsigned>(getValue (getCDataBase()->getPitch()));
    unsigned nlayers = getGrid()->getNimbus()->getNumberOfRoutingLayers();
    
    setCapacity(static_cast<unsigned>((nlayers / 2) * getValue (_size) / pitch));

#endif
    return;
}

} // namespace Nimbus
