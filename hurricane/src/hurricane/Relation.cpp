// ****************************************************************************************************
// File: Relation.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Relation.h"
#include "DBo.h"
#include "Error.h"

#include "Filter.h"

namespace Hurricane {



// ****************************************************************************************************
// Filters declaration & implementation
// ****************************************************************************************************

class Relation_OwnerIsSlave : public Filter<DBo*> {
// **********************************************

    private: const Relation* _relation;

    public: Relation_OwnerIsSlave(const Relation* relation)
    // ****************************************************
    : _relation(relation)
    {
    };

    public: Relation_OwnerIsSlave(const Relation_OwnerIsSlave& filter)
    // ***************************************************************
    : _relation(filter._relation)
    {
    };

    public: Relation_OwnerIsSlave& operator=(const Relation_OwnerIsSlave& filter)
    // **************************************************************************
    {
        _relation = filter._relation;
        return *this;
    };

    public: virtual Filter<DBo*>* getClone() const
    // *******************************************
    {
        return new Relation_OwnerIsSlave(*this);
    };

    public: virtual bool accept(DBo* owner) const
    // ******************************************
    {
        return (owner != _relation->getMasterOwner());
    };

    public: virtual string _getString() const
    // **************************************
    {
        return "<" + _TName("Relation::OwnerIsSlave") + " " + getString(_relation) + ">";
    };

};



// ****************************************************************************************************
// Relation implementation
// ****************************************************************************************************

Relation::Relation(DBo* masterOwner)
// *********************************
:    Inherit(),
    _masterOwner(masterOwner)
{
    if (!_masterOwner)
        throw Error("Can't create " + _TName("Relation") + " : null master owner");
}

DBos Relation::getSlaveOwners() const
// **********************************
{
    return getOwners().getSubSet(Relation_OwnerIsSlave(this));
}

void Relation::onReleasedBy(DBo* owner)
// ************************************
{
    _getOwnerSet().erase(owner);

    if (owner == _masterOwner) destroy();
}

void Relation::_postCreate()
// *************************
{
    Inherit::_postCreate();

    _masterOwner->put(this);
}

string Relation::_getString() const
// ********************************
{
    return "<" + _TName("Relation") + " " + getString(getMasterOwner()) + " " + getString(getName()) + ">";
}

Record* Relation::_getRecord() const
// ***************************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->Add(getSlot("MasterOwner", _masterOwner));
    }
    return record;
}



// ****************************************************************************************************
// StandardRelation implementation
// ****************************************************************************************************

StandardRelation::StandardRelation(DBo* masterOwner, const Name& name)
// *******************************************************************
:    Inherit(masterOwner),
    _name(name)
{
}

StandardRelation* StandardRelation::create(DBo* masterOwner, const Name& name)
// ***************************************************************************
{
    StandardRelation* standardRelation = new StandardRelation(masterOwner, name);

    standardRelation->_postCreate();

    return standardRelation;
};

string StandardRelation::_getString() const
// ****************************************
{
    return Inherit::_getString();
}

Record* StandardRelation::_getRecord() const
// ***********************************
{
    Record* record = Inherit::_getRecord();
    if (record) {
        record->Add(getSlot("Name", &_name));
    }
    return record;
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
