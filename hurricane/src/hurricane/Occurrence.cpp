// ****************************************************************************************************
// File: ./Occurrence.cpp
// Authors: R. Escassut
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
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

#include "hurricane/Occurrence.h"
#include "hurricane/Entity.h"
#include "hurricane/Cell.h"
#include "hurricane/SharedPath.h"
#include "hurricane/Property.h"
#include "hurricane/Quark.h"
#include "hurricane/Error.h"

namespace Hurricane {



// ****************************************************************************************************
// Occurrence implementation
// ****************************************************************************************************

Occurrence::Occurrence(const Entity* entity)
// *****************************************
  : _entity(const_cast<Entity*>(entity))
  , _sharedPath(NULL)
{
}

Occurrence::Occurrence(const Entity* entity, const Path& path)
// ***********************************************************
  : _entity(const_cast<Entity*>(entity))
  , _sharedPath(path._getSharedPath())
{
  if (not _entity)
    throw Error( "Occurrence::Occurrence(): NULL Entity." );

  if (_sharedPath) {
    if (_entity->getCell() != _sharedPath->getMasterCell())
      throw Error( "Occurrence::Occurrence(): Entity incompatible with the path.\n"
                   "        * Entity %s, master cell %s\n"
                   "        * Path %s, master cell %s"
                 , getString(entity).c_str()
                 , getString(entity->getCell()).c_str()
                 , getString(path).c_str()
                 , getString(_sharedPath->getMasterCell()).c_str()
                 );
  }
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
  cdebug_log(0,0) << "Occurrence::operator<()" << endl;
  cdebug_log(0,0) << "| lhs=" << *this << endl;
  cdebug_log(0,0) << "| rhs=" << occurrence << endl;
  if ((not _sharedPath) xor (not occurrence._sharedPath)) return not _sharedPath;
  if ((not _entity    ) xor (not occurrence._entity    )) return not _entity;
  if (_entity and (_entity->getId() != occurrence._entity->getId()))
    return _entity->getId() < occurrence._entity->getId();
  if (not _sharedPath) return false;

  // if (not _sharedPath) return true;
  // if (not occurrence._sharedPath) return false;
  // if (not _sharedPath and not occurrence._sharedPath) return false;
  // if (not _sharedPath) return true;
  // if (not occurrence._sharedPath) return false;

  // if (not _entity and not occurrence._entity) return false;
  // if (not _entity) return true;
  // if (not occurrence._entity) return false;

  // if (_entity->getId() < occurrence._entity->getId()) return true;
  // if (_entity->getId() > occurrence._entity->getId()) return false;

  return _sharedPath->getHash() < occurrence._sharedPath->getHash();
  
//return ((_entity  < occurrence._entity) or 
//       ((_entity == occurrence._entity) and (_sharedPath < occurrence._sharedPath)));
}

bool Occurrence::isBelowTerminalNetlist() const
// ********************************************
{
  SharedPath* tail = _sharedPath;
  while ( tail ) {
    Instance* instance = tail->getHeadInstance();
    tail = tail->getTailSharedPath();
    
    if (tail and instance->isTerminalNetlist()) return true;
  }
  return false;
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

Occurrence Occurrence::getNetOccurrence() const
// ********************************************
{
  if (isValid()) {
    Entity* entity = getEntity();
    if (dynamic_cast<Net*>(entity)) return *this;
    else {
      Rubber* rubber = dynamic_cast<Rubber*>(entity);
      if (rubber) return Occurrence(rubber->getNet(), getPath());
      else {
        Component* component = dynamic_cast<Component*>(entity);
        if (component) return Occurrence(component->getNet(), getPath());
      }
    }
  }

  return Occurrence();
}



Box Occurrence::getBoundingBox() const
// **********************************
{
    if (!_entity) return Box();
    if (!_sharedPath) return _entity->getBoundingBox();
    return _sharedPath->getTransformation().getBox(_entity->getBoundingBox());
}

Box Occurrence::getBoundingBox(const BasicLayer* basicLayer) const
// ***************************************************************
{
  const Component* component = dynamic_cast<const Component*>(_entity);
  if ( not component ) return getBoundingBox();
    
  if (!_sharedPath) return component->getBoundingBox(basicLayer);
  return _sharedPath->getTransformation().getBox(component->getBoundingBox(basicLayer));
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

string Occurrence::getCompactString() const
// ****************************************
{
  string s;
  if (_entity) {
    s += getString(getOwnerCell()->getName());
    s += ":";
    if (_sharedPath) s += getString(_sharedPath->getName());
    s += ":";
    Instance* instance = dynamic_cast<Instance*>(_entity);
    if (instance) {
      s += "I."+getString(instance->getName());
    } else {
      Net* net = dynamic_cast<Net*>(_entity);
      if (net) {
        s += "N."+getString(net->getName());
      } else {
        s += getString(_entity);
      }
    }
  }
  return s;
}

void  Occurrence::toJson(JsonWriter* w) const
// ******************************************
{
  w->startObject();
  jsonWrite( w, "@typename", "Occurrence"         );
  jsonWrite( w, "_path"    , getPath().getJsonString(w->getFlags()) );

  w->key( "_entity" );
  if (not w->issetFlags(JsonWriter::DesignBlobMode)) {
    getEntity()->toJsonSignature( w );
  } else {
    jsonWrite( w, getEntity()->getId() );
  }

  w->endObject();
}

Record* Occurrence::_getRecord() const
// ****************************
{
     Record* record = NULL;
    if (_entity) {
          record = new Record(getString(this));
        record->add(getSlot("Entity", _entity));
        record->add(getSlot("SharedPath", _sharedPath));
        Quark* quark = _getQuark();
        if (quark) record->add(getSlot("Quark", quark));
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


Initializer<JsonOccurrence>  jsonOccurrenceInit ( 0 );


void  JsonOccurrence::initialize()
// *******************************
{ JsonTypes::registerType( new JsonOccurrence (JsonWriter::RegisterMode) ); }

JsonOccurrence::JsonOccurrence(unsigned long flags)
// ************************************************
  : JsonObject(flags)
{
  add( ".Cell"  , typeid(Cell*)  );
  add( "_path"  , typeid(string) );

  if (issetFlags(JsonWriter::DesignBlobMode)) {
    add( "_entity", typeid(uint64_t) );
  } else {
    add( "_entity", typeid(Entity*) );
  }
}

JsonOccurrence* JsonOccurrence::clone(unsigned long flags) const
// *************************************************************
{ return new JsonOccurrence ( flags ); }

string JsonOccurrence::getTypeName() const
// ***************************************
{ return "Occurrence"; }

void JsonOccurrence::toData(JsonStack& stack)
// ******************************************
{
  check( stack, "JsonOccurrence::toData" );

  Path    path;
  Entity* entity = NULL;
  if (issetFlags(JsonWriter::DesignBlobMode)) {
    entity = stack.getEntity<Entity*>( get<int64_t>(stack,"_entity") );

  //Cell*         cell      = get<Cell*>( stack, ".Cell" );
    Instance*     instance  = NULL;
    char          separator = SharedPath::getNameSeparator();
    string        pathIds   = get<string>( stack, "_path" );
    unsigned long id;
    size_t        dot       = pathIds.find( separator );

    if (dot != string::npos) {
      id      = stoul( pathIds.substr( 0, dot ) );
      pathIds =        pathIds.substr( dot+1 );
    } else
      id = stol( pathIds );

    instance = stack.getEntity<Instance*>(id);
    if (not instance)
      throw Error( "JsonOccurrence::toData(): No Instance id:lu% (or not an Instance) in stack LUT.", id );
    path = Path( instance );

    while ( dot != string::npos ) {
      dot = pathIds.find( separator );
      if (dot != string::npos) {
        id      = stoul( pathIds.substr( 0, dot ) );
        pathIds =        pathIds.substr( dot+1 );
      } else
        id = stol( pathIds );

      instance = stack.getEntity<Instance*>(id);
      if (not instance)
        throw Error( "JsonOccurrence::toData(): No Instance id:lu% (or not an Instance) in stack LUT.", id );
      path = Path( path, instance );
    }
  } else {
    entity = get<Entity*>(stack,"_entity");
    path   = Path( get<Cell*>(stack,".Cell"), get<string>(stack,"_path") );
  }

  Occurrence occurrence ( entity, path );
  update( stack, occurrence );
}

} // End of Hurricane namespace.


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
