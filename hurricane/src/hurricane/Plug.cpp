// ****************************************************************************************************
// File: Plug.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Plug.h"
#include "Net.h"
#include "Cell.h"
#include "Instance.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Filters declaration & implementation
// ****************************************************************************************************

class Plug_IsConnectedFilter : public Filter<Plug*> {
// ************************************************

    public: Plug_IsConnectedFilter() {};

    public: Plug_IsConnectedFilter(const Plug_IsConnectedFilter& filter) {};

    public: Plug_IsConnectedFilter& operator=(const Plug_IsConnectedFilter& filter) {return *this;};

    public: virtual Filter<Plug*>* getClone() const {return new Plug_IsConnectedFilter(*this);};

    public: virtual bool accept(Plug* plug) const {return plug->isConnected();};

    public: virtual string _getString() const {return "<" + _TName("Plug::IsConnectedFilter>");};

};



// ****************************************************************************************************
// Plug implementation
// ****************************************************************************************************

Plug::Plug(Instance* instance, Net* masterNet)
// *******************************************
:    Inherit(NULL, true),
    _instance(instance),
    _masterNet(masterNet),
    _nextOfInstancePlugMap(NULL)
{
    if (!_instance)
        throw Error("Can't create " + _TName("Plug") + " : null instance");

    if (!_masterNet)
        throw Error("Can't create " + _TName("Plug") + " : null master net");

    if (_masterNet->getCell() != _instance->getMasterCell())
        throw Error("Can't create " + _TName("Plug") + " : incompatible master net");

    if (!_masterNet->isExternal())
        throw Error("Can't create " + _TName("Plug") + " : not an external master net");

    if (_instance->getPlug(_masterNet))
        throw Error("Can't create " + _TName("Plug") + " : already exists");
}

void Plug::destroy()
// ****************
{
    throw Error("Abnormal deletion of " + _TName("Plug"));
}

Cell* Plug::getCell() const
// ************************
{
    return _instance->getCell();
}

Unit Plug::getX() const
// ********************
{
    return _instance->getTransformation().getX(_masterNet->getPosition());
}

Unit Plug::getY() const
// ********************
{
    return _instance->getTransformation().getY(_masterNet->getPosition());
}

Point Plug::getPosition() const
// ****************************
{
    return _instance->getTransformation().getPoint(_masterNet->getPosition());
}

Box Plug::getBoundingBox() const
// *****************************
{
    return _instance->getTransformation().getBox(_masterNet->getPosition());
}

Box Plug::getBoundingBox(const BasicLayer* basicLayer) const
// ***************************************************
{
    return Box();
}

PlugFilter Plug::getIsConnectedFilter()
// ************************************
{
    return Plug_IsConnectedFilter();
}

PlugFilter Plug::getIsUnconnectedFilter()
// **************************************
{
    return !Plug_IsConnectedFilter();
}

void Plug::setNet(Net* net)
// ************************
{
    if (net != getNet()) {

        if (net && (getCell() != net->getCell()))
            throw Error("Can't change net of plug : net : " + getString(net) + "does not belong to the cell : " + getString(getCell()));

        if (!getBodyHook()->getSlaveHooks().IsEmpty())
            throw Error("Can't change net of plug : not empty slave hooks");

        _setNet(net);
    }
}

Plug* Plug::_create(Instance* instance, Net* masterNet)
// ****************************************************
{
    Plug* plug = new Plug(instance, masterNet);

    plug->_postCreate();

    return plug;
}

void Plug::_postCreate()
// *********************
{
    _instance->_getPlugMap()._Insert(this);

    Inherit::_postCreate();
}

void Plug::_destroy()
// *****************
{
    _preDestroy();

    delete this;
}

void Plug::_preDestroy()
// ********************
{
// trace << "entering Plug::_preDestroy: " << this << endl;
// trace_in();

    Inherit::_preDestroy();

    _instance->_getPlugMap()._Remove(this);

// trace << "exiting Plug::_preDestroy:" << endl;
// trace_out();
}

string Plug::_getString() const
// ****************************
{
    string s = Inherit::_getString();
    s.insert(s.length() - 1, " " + getName());
    return s;
}

Record* Plug::_getRecord() const
// ***********************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->Add(getSlot("Instance", _instance));
        record->Add(getSlot("MasterNet", _masterNet));
    }
    return record;
}

string Plug::getName() const
// *************************
{
    return getString(_instance->getName())
         + "."
         + getString(_masterNet->getName());
}

} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
