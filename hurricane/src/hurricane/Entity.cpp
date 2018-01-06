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


  unsigned long  Entity::_flags     = 0;
  unsigned int   Entity::_nextId    = 0;
  unsigned int   Entity::_idCounter = 1;


  unsigned int  Entity::getIdCounter ()
  { return _idCounter; }


  bool  Entity::inForcedIdMode ()
  { return _flags & ForcedIdMode; }


  void  Entity::enableForcedIdMode ()
  {
    if (_flags & ForcedIdMode) return;
    if (_idCounter != 1) {
      throw Error( "Entity::enableForcedIdMode(): DataBase must be reset before forcind ids." );
    }
    _flags |= ForcedIdMode;
  }


  void  Entity::disableForcedIdMode ()
  {
    if (not (_flags & ForcedIdMode)) return;
    _flags &= ~ForcedIdMode;
  }


  void  Entity::setNextId ( unsigned int nid )
  {
    if (not (_flags & ForcedIdMode)) {
      cerr << Error("Entity::setNextId(): Not in forced id mode, ignored.") << endl;
      return;
    }
    _nextId = nid;
    if (nid > _idCounter) _idCounter = nid;
    _flags |= NextIdSet;
  }


  unsigned int  Entity::getNextId ()
  {
    if (_flags & ForcedIdMode) {
      if (_flags & NextIdSet) {
        _flags &= ~NextIdSet;
        cdebug_log(18,0) << demangle(typeid(*this).name())
                   << "::getNextId(): Consuming the preset id:" << _nextId << endl;
        return _nextId;
      } else {
        throw Error("Entity::getNextId(): Next id is not set, while in forced id mode.");
      }
    }

    return _idCounter++;
  }


  Entity::Entity()
    : Inherit()
    , _id    (getNextId())
  {
    if (_idCounter == std::numeric_limits<unsigned int>::max()) {
      throw Error( "Entity::Entity(): Identifier counter has reached it's limit (%d bits)."
                 , std::numeric_limits<unsigned int>::digits );
    }
  }


  void Entity::_preDestroy()
  {
  //ltrace(10) << "Entity::_preDestroy() - " << (void*)this << endl;
  //ltracein(10);

    vector<Entity*> slaveEntities;
    SlaveEntityMap::iterator  it;
    SlaveEntityMap::iterator  end;
    getCell()->_getSlaveEntities(this,it,end);
    for(; it != end ; it++)
      slaveEntities.push_back(it->second);
    for(; slaveEntities.size() ; slaveEntities.pop_back()) {
      slaveEntities.back()->destroy();
    }

    Quark* quark = _getQuark();
    if (quark) quark->destroy();

    stack<SharedPath*> sharedPathStack;
    for_each_instance(instance, getCell()->getSlaveInstances()) {
      SharedPath* sharedPath = instance->_getSharedPath(NULL);
      if (sharedPath) sharedPathStack.push(sharedPath);
      end_for;
    }
    while (!sharedPathStack.empty()) {
      SharedPath* sharedPath = sharedPathStack.top();
      sharedPathStack.pop();
      Quark* quark = _getQuark(sharedPath);
      if (quark) quark->destroy();
      Cell* cell = sharedPath->getOwnerCell();
      for_each_instance(instance, cell->getSlaveInstances()) {
        SharedPath* sharedPath2 = instance->_getSharedPath(sharedPath);
        if (sharedPath2) sharedPathStack.push(sharedPath2);
        end_for;
      }
    }

    Inherit::_preDestroy();

  //ltrace(10) << "Entity::_preDestroy() - exit" << endl;
  //ltraceout(10);
  }


  void  Entity::setId ( unsigned int id )
  {
    if (_flags & ForcedIdMode) {
      _id = id;
      if (_id > _idCounter) _idCounter = _id;
    } else {
      throw Error("Entity::setId(): Attempt to set id while not in forced id mode.");
    }
  }


  void  Entity::_toJson ( JsonWriter* writer ) const
  {
    Inherit::_toJson( writer );

    jsonWrite( writer, "_id", getId() );
  }


  string Entity::_getString() const
  {
    string s = Inherit::_getString();
    s.insert( 1, "id:"+getString(_id)+" " );
    return s;
  }


  Record* Entity::_getRecord() const
  {
    Record* record = Inherit::_getRecord();
    if (record) {
      record->add( getSlot("_id", _id) );
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
