// ****************************************************************************************************
// File: ./Entity.cpp
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

#include <limits>
#include "hurricane/Error.h"
#include "hurricane/Entity.h"
#include "hurricane/Quark.h"
#include "hurricane/Cell.h"
#include "hurricane/Instance.h"
#include "hurricane/SharedPath.h"

namespace Hurricane {


// ****************************************************************************************************
// Entity implementation
// ****************************************************************************************************


  Entity::Entity()
    : Inherit()
  { }


  Entity::~Entity() throw(Error)
  { }
  

  void Entity::_postCreate()
  {
    Inherit::_postCreate();

  //cerr << _getString() << endl;
  }


  void Entity::_preDestroy()
  {
  //ltrace(10) << "Entity::_preDestroy() - " << (void*)this << endl;
  //ltracein(10);

    vector<Entity*> slaveEntities;
    SlaveEntityMap::iterator  it;
    SlaveEntityMap::iterator  end;
    getCell()->_getSlaveEntities(this,it,end);
    for( ; it != end ; it++)
      slaveEntities.push_back(it->second);
    for( ; slaveEntities.size() ; slaveEntities.pop_back()) {
      slaveEntities.back()->destroy();
    }

    Quark* quark = _getQuark();
    if (quark) quark->destroy();

    stack<SharedPath*> sharedPathStack;
    for ( Instance* instance : getCell()->getSlaveInstances() ) {
      SharedPath* sharedPath = instance->_getSharedPath(NULL);
      if (sharedPath) sharedPathStack.push(sharedPath);
    }
    while (not sharedPathStack.empty()) {
      SharedPath* sharedPath = sharedPathStack.top();
      sharedPathStack.pop();
      Quark* quark = _getQuark(sharedPath);
      if (quark) quark->destroy();
      Cell* cell = sharedPath->getOwnerCell();
      for ( Instance* instance : cell->getSlaveInstances() ) {
        SharedPath* sharedPath2 = instance->_getSharedPath(sharedPath);
        if (sharedPath2) sharedPathStack.push(sharedPath2);
      }
    }

    Inherit::_preDestroy();

  //ltrace(10) << "Entity::_preDestroy() - exit" << endl;
  //ltraceout(10);
  }


  void  Entity::_toJson ( JsonWriter* writer ) const
  {
    Inherit::_toJson( writer );

    jsonWrite( writer, "_id", getId() );
  }


  string Entity::_getString() const
  {
    string s = Inherit::_getString();
    return s;
  }


  Record* Entity::_getRecord() const
  {
    Record* record = Inherit::_getRecord();
    if (record) {
      Occurrence occurrence = Occurrence(this);
      if (occurrence.hasProperty())
        record->add( getSlot("Occurrence", occurrence) );
    }
    return record;
  }


  JsonEntity::JsonEntity (unsigned long flags)
    : JsonDBo(flags)
  {
    add( "_id", typeid(uint64_t) );
  }


// -------------------------------------------------------------------
// Class  :  "JsonEntityRef".

  Initializer<JsonEntityRef>  jsonEntityRefInit ( 0 );


  void  JsonEntityRef::initialize ()
  { JsonTypes::registerType( new JsonEntityRef (JsonWriter::RegisterMode) ); }


  JsonEntityRef::JsonEntityRef ( unsigned long flags )
    : JsonObject(flags)
  {
    if (flags & JsonWriter::RegisterMode) return;

    add( "_id", typeid(int64_t) );
  }


  string  JsonEntityRef::getTypeName () const
  { return "&Entity"; }


  JsonEntityRef* JsonEntityRef::clone( unsigned long flags ) const
  { return new JsonEntityRef ( flags ); }


  void JsonEntityRef::toData ( JsonStack& stack )
  {
    cdebug_tabw(19,1);

    check( stack, "JsonEntityRef::toData" );

    unsigned int jsonId = get<int64_t>( stack, "_id" );
    Entity*      entity = stack.getEntity<Entity*>( jsonId );

    cdebug_log(19,0) << "jsonId:" << jsonId << " entity:" << entity << endl;

    if (entity) {
      JsonBaseArray<Entity*>* array = jget< JsonBaseArray<Entity*> >( stack );
      if (array) array->push_back( entity );
      else
        cerr << Error( "JsonEntityRef::toData(): Missing JsonBaseArray in stack context." ) << endl;
    } else {
      cerr << Error( "JsonEntityRef::toData(): No Entity id:%u in stack LUT.", jsonId ) << endl;
    }

    update( stack, NULL );

    cdebug_tabw(19,-1);
  }

} // End of Hurricane namespace.


// ****************************************************************************************************
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
// ****************************************************************************************************
