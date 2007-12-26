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

	public: virtual Filter<DBo*>* GetClone() const
	// *******************************************
	{
		return new Relation_OwnerIsSlave(*this);
	};

	public: virtual bool Accept(DBo* owner) const
	// ******************************************
	{
		return (owner != _relation->GetMasterOwner());
	};

	public: virtual string _GetString() const
	// **************************************
	{
		return "<" + _TName("Relation::OwnerIsSlave") + " " + GetString(_relation) + ">";
	};

};



// ****************************************************************************************************
// Relation implementation
// ****************************************************************************************************

Relation::Relation(DBo* masterOwner)
// *********************************
:	Inherit(),
	_masterOwner(masterOwner)
{
	if (!_masterOwner)
		throw Error("Can't create " + _TName("Relation") + " : null master owner");
}

DBos Relation::GetSlaveOwners() const
// **********************************
{
	return GetOwners().GetSubSet(Relation_OwnerIsSlave(this));
}

void Relation::OnReleasedBy(DBo* owner)
// ************************************
{
	_GetOwnerSet().erase(owner);

	if (owner == _masterOwner) Delete();
}

void Relation::_PostCreate()
// *************************
{
	Inherit::_PostCreate();

	_masterOwner->Put(this);
}

string Relation::_GetString() const
// ********************************
{
	return "<" + _TName("Relation") + " " + GetString(GetMasterOwner()) + " " + GetString(GetName()) + ">";
}

Record* Relation::_GetRecord() const
// ***************************
{
	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("MasterOwner", _masterOwner));
	}
	return record;
}



// ****************************************************************************************************
// StandardRelation implementation
// ****************************************************************************************************

StandardRelation::StandardRelation(DBo* masterOwner, const Name& name)
// *******************************************************************
:	Inherit(masterOwner),
	_name(name)
{
}

StandardRelation* StandardRelation::Create(DBo* masterOwner, const Name& name)
// ***************************************************************************
{
	StandardRelation* standardRelation = new StandardRelation(masterOwner, name);

	standardRelation->_PostCreate();

	return standardRelation;
};

string StandardRelation::_GetString() const
// ****************************************
{
	return Inherit::_GetString();
}

Record* StandardRelation::_GetRecord() const
// ***********************************
{
	Record* record = Inherit::_GetRecord();
	if (record) {
		record->Add(GetSlot("Name", &_name));
	}
	return record;
}



} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
