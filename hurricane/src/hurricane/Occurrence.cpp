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
  :    _entity(const_cast<Entity*>(entity)),
    _sharedPath(NULL)
{
}

Occurrence::Occurrence(const Entity* entity, const Path& path)
// ***************************************************
  :    _entity(const_cast<Entity*>(entity)),
    _sharedPath(path._getSharedPath())
{
    if (!_entity) {
        throw Error("Can't create " + _TName("Occurrence") + " : null entity");
    }
    if (_sharedPath)
        if (_entity->getCell() != _sharedPath->getMasterCell())
            throw Error("Can't create " + _TName("Occurrence") + " : incompatible path");
}

Occurrence::Occurrence(const Occurrence& occurrence)
// *********************************************
:    _entity(occurrence._entity),
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

Cell* Occurrence::getOwnerCell() const
// **********************************
{
    if (!_entity) return NULL;
    return (_sharedPath) ? _sharedPath->getOwnerCell() : _entity->getCell();
}

Cell* Occurrence::getMasterCell() const
// ***********************************
{
    return (_entity) ? _entity->getCell() : NULL;
}

Property* Occurrence::getProperty(const Name& name) const
// *****************************************************
{
    if (_entity) {
        //DBo* quark = _getQuark(); 
        Quark* quark = _getQuark();
        if (quark) return quark->getProperty(name);
    }
    return NULL;
}

Properties Occurrence::getProperties() const
// ****************************************
{
    if (_entity) {
        Quark* quark = _getQuark();
        if (quark) return quark->getProperties();
    }
    return Properties();
}

Box Occurrence::getBoundingBox() const
// **********************************
{
    if (!_entity) return Box();
    if (!_sharedPath) return _entity->getBoundingBox();
    return _sharedPath->getTransformation().getBox(_entity->getBoundingBox());
}

bool Occurrence::hasProperty() const
// ********************************
{
    return (_getQuark() != NULL);
}

void Occurrence::makeInvalid()
// **************************
{
    _entity = NULL;
    _sharedPath = NULL;
}

void Occurrence::put(Property* property)
// ************************************
{
    if (!_entity)
        throw Error("Can't put property : invalid occurrence");

    if (!property)
        throw Error("Can't put property : null property");

    Quark* quark = _getQuark();
    if (!quark) quark = Quark::_create(*this);
    quark->put(property);
}

void Occurrence::remove(Property* property)
// ***************************************
{
    if (!_entity)
        throw Error("Can't remove property : invalid occurrence");

    if (!property)
        throw Error("Can't remove property : null property");

    Quark* quark = _getQuark();
    if (quark) quark->remove(property);
}

void Occurrence::removeProperty(const Name& name)
// *********************************************
{
    if (!_entity)
        throw Error("Can't remove property : invalid occurrence");

    Quark* quark = _getQuark();
    if (quark) quark->removeProperty(name);
}

void Occurrence::clearProperties()
// ******************************
{
    Quark* quark = _getQuark();
    if (quark) quark->destroy();
}

string Occurrence::_getString() const
// *********************************
{
    string s = "<" + _TName("Occurrence");
    if (_entity) {
        s += " ";
                s += getString(getOwnerCell());
                s += ":";
        if (_sharedPath) s += getString(_sharedPath->getName()) + ":";
        s += getString(_entity);
    }
    s += ">";
    return s;
}

Record* Occurrence::_getRecord() const
// ****************************
{
     Record* record = NULL;
    if (_entity) {
          record = new Record(getString(this));
        record->Add(getSlot("Entity", _entity));
        record->Add(getSlot("SharedPath", _sharedPath));
        Quark* quark = _getQuark();
        if (quark) record->Add(getSlot("Quark", quark));
    }
    return record;
}

//DBo* ...
Quark* Occurrence::_getQuark() const
// ********************************
{
    return (_entity) ? _entity->_getQuark(_sharedPath) : NULL;
}

string Occurrence::getName() const
// *******************************
{
    string description;

    if (_sharedPath)
        description=_sharedPath->getName()+SharedPath::getNameSeparator();

    if (Plug* plug= dynamic_cast<Plug*>(_entity))
        description += plug->getName();
    else if (Pin* pin= dynamic_cast<Pin*>(_entity))
        description += getString(pin->getName());
    else if (Net* net= dynamic_cast<Net*>(_entity))
        description += getString(net->getName());
    else if (Cell* cell= dynamic_cast<Cell*>(_entity))
        description += getString(cell->getName());
    else if (Instance* instance= dynamic_cast<Instance*>(_entity))
        description += getString(instance->getName());
    else
        description+= getString(_entity);
        //throw Error("[Occurrence::getName] No Name for "+getString(_entity));

    return description;
}


} // End of Hurricane namespace.

// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2004, All Rights Reserved
// ****************************************************************************************************
