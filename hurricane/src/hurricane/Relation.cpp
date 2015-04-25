// ****************************************************************************************************
// File: ./Relation.cpp
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

#include "hurricane/Relation.h"
#include "hurricane/DBo.h"
#include "hurricane/Error.h"

#include "hurricane/Filter.h"

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
        record->add(getSlot("MasterOwner", _masterOwner));
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
        record->add(getSlot("Name", &_name));
    }
    return record;
}



} // End of Hurricane namespace.


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2009, All Rights Reserved
// ****************************************************************************************************
