// ****************************************************************************************************
// File: Occurrence.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************

#include "Occurrence.h"
#include "Entity.h"
#include "Cell.h"
#include "SharedPath.h"
#include "Property.h"
#include "Quark.h"
#include "Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Occurrence implementation
// ****************************************************************************************************

Occurrence::Occurrence(const Entity* entity)
// *********************************
  :	_entity(const_cast<Entity*>(entity)),
	_sharedPath(NULL)
{
}

Occurrence::Occurrence(const Entity* entity, const Path& path)
// ***************************************************
  :	_entity(const_cast<Entity*>(entity)),
	_sharedPath(path._GetSharedPath())
{
	if (!_entity) {
		throw Error("Can't create " + _TName("Occurrence") + " : null entity");
	}
	if (_sharedPath)
		if (_entity->GetCell() != _sharedPath->GetMasterCell())
			throw Error("Can't create " + _TName("Occurrence") + " : incompatible path");
}

Occurrence::Occurrence(const Occurrence& occurrence)
// *********************************************
:	_entity(occurrence._entity),
	_sharedPath(occurrence._sharedPath)
{
}

Occurrence& Occurrence::operator=(const Occurrence& occurrence)
// ********************************************************
{
	_entity = occurrence._entity;
	_sharedPath = occurrence._sharedPath;
	return *this;
}

bool Occurrence::operator==(const Occurrence& occurrence) const
// *********************************************************
{
	return _entity && occurrence._entity &&
			 (_entity == occurrence._entity) &&
			 (_sharedPath == occurrence._sharedPath);
}

bool Occurrence::operator!=(const Occurrence& occurrence) const
// *********************************************************
{
	return !_entity ||
			 !occurrence._entity ||
			 (_entity != occurrence._entity) ||
			 (_sharedPath != occurrence._sharedPath);
}

bool Occurrence::operator<(const Occurrence& occurrence) const
// ********************************************************
{
	return ((_entity < occurrence._entity) ||
			  ((_entity == occurrence._entity) && (_sharedPath < occurrence._sharedPath)));
}

Cell* Occurrence::GetOwnerCell() const
// **********************************
{
	if (!_entity) return NULL;
	return (_sharedPath) ? _sharedPath->GetOwnerCell() : _entity->GetCell();
}

Cell* Occurrence::GetMasterCell() const
// ***********************************
{
	return (_entity) ? _entity->GetCell() : NULL;
}

Property* Occurrence::GetProperty(const Name& name) const
// *****************************************************
{
	if (_entity) {
        //DBo* quark = _GetQuark(); 
		Quark* quark = _GetQuark();
		if (quark) return quark->GetProperty(name);
	}
	return NULL;
}

Properties Occurrence::GetProperties() const
// ****************************************
{
	if (_entity) {
		Quark* quark = _GetQuark();
		if (quark) return quark->GetProperties();
	}
	return Properties();
}

Box Occurrence::GetBoundingBox() const
// **********************************
{
	if (!_entity) return Box();
	if (!_sharedPath) return _entity->GetBoundingBox();
	return _sharedPath->GetTransformation().GetBox(_entity->GetBoundingBox());
}

bool Occurrence::HasProperty() const
// ********************************
{
	return (_GetQuark() != NULL);
}

void Occurrence::MakeInvalid()
// **************************
{
	_entity = NULL;
	_sharedPath = NULL;
}

void Occurrence::Put(Property* property)
// ************************************
{
	if (!_entity)
		throw Error("Can't put property : invalid occurrence");

	if (!property)
		throw Error("Can't put property : null property");

	Quark* quark = _GetQuark();
	if (!quark) quark = Quark::_Create(*this);
	quark->Put(property);
}

void Occurrence::Remove(Property* property)
// ***************************************
{
	if (!_entity)
		throw Error("Can't remove property : invalid occurrence");

	if (!property)
		throw Error("Can't remove property : null property");

	Quark* quark = _GetQuark();
	if (quark) quark->Remove(property);
}

void Occurrence::RemoveProperty(const Name& name)
// *********************************************
{
	if (!_entity)
		throw Error("Can't remove property : invalid occurrence");

	Quark* quark = _GetQuark();
	if (quark) quark->RemoveProperty(name);
}

void Occurrence::ClearProperties()
// ******************************
{
	Quark* quark = _GetQuark();
	if (quark) quark->Delete();
}

string Occurrence::_GetString() const
// *********************************
{
	string s = "<" + _TName("Occurrence");
	if (_entity) {
		s += " ";
                s += GetString(GetOwnerCell());
                s += ":";
		if (_sharedPath) s += GetString(_sharedPath->GetName()) + ":";
		s += GetString(_entity);
	}
	s += ">";
	return s;
}

Record* Occurrence::_GetRecord() const
// ****************************
{
 	Record* record = NULL;
	if (_entity) {
  		record = new Record(GetString(this));
		record->Add(GetSlot("Entity", _entity));
		record->Add(GetSlot("SharedPath", _sharedPath));
		Quark* quark = _GetQuark();
		if (quark) record->Add(GetSlot("Quark", quark));
	}
	return record;
}

//DBo* ...
Quark* Occurrence::_GetQuark() const
// ********************************
{
	return (_entity) ? _entity->_GetQuark(_sharedPath) : NULL;
}

string Occurrence::GetName() const
// *******************************
{
    string description;

    if (_sharedPath)
        description=_sharedPath->GetName()+SharedPath::GetNameSeparator();

    if (Plug* plug= dynamic_cast<Plug*>(_entity))
        description += plug->GetName();
    else if (Pin* pin= dynamic_cast<Pin*>(_entity))
        description += GetString(pin->GetName());
    else if (Net* net= dynamic_cast<Net*>(_entity))
        description += GetString(net->GetName());
    else if (Cell* cell= dynamic_cast<Cell*>(_entity))
        description += GetString(cell->GetName());
    else if (Instance* instance= dynamic_cast<Instance*>(_entity))
        description += GetString(instance->GetName());
    else
        description+= GetString(_entity);
        //throw Error("[Occurrence::GetName] No Name for "+GetString(_entity));

    return description;
}


} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
