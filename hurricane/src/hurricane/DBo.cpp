// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2000-2018, All Rights Reserved
//
// This file is part of Hurricane.
//
// Hurricane is free software: you can redistribute it  and/or  modify
// it under the terms of the GNU  Lesser  General  Public  License  as
// published by the Free Software Foundation, either version 3 of  the
// License, or (at your option) any later version.
//
// Hurricane is distributed in the hope that it will  be  useful,  but
// WITHOUT ANY WARRANTY; without even the implied warranty of MERCHAN-
// TABILITY or FITNESS FOR A PARTICULAR PURPOSE. See  the  Lesser  GNU
// General Public License for more details.
//
// You should have received a copy of the Lesser  GNU  General  Public
// License along with Hurricane. If not, see
//                                     <http://www.gnu.org/licenses/>.
//
// +-----------------------------------------------------------------+
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                       Remy Escassut              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./DBo.cpp"                                     |
// +-----------------------------------------------------------------+


#include "hurricane/Initializer.h"
#include "hurricane/Timer.h"
#include "hurricane/DBo.h"
#include "hurricane/Entity.h"
#include "hurricane/Property.h"
#include "hurricane/Quark.h"
#include "hurricane/Error.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "Hurricane::DBo".


  unsigned int   DBo::_memoryLimit    =  0;
  unsigned long  DBo::_flags          =  0;
  unsigned int   DBo::_nextId         =  0;
  unsigned int   DBo::_idCounterLimit =  0;
  unsigned int   DBo::_idCounter      =  1;


  void  DBo::setIdCounterLimit ( unsigned int limit )
  { _idCounterLimit = limit; }


  void  DBo::setMemoryLimit ( unsigned int limit )
  { _memoryLimit = limit; }


  unsigned int  DBo::getIdCounter ()
  { return _idCounter; }


  bool  DBo::inForcedIdMode ()
  { return _flags & ForcedIdMode; }


  void  DBo::enableForcedIdMode ()
  {
    if (_flags & ForcedIdMode) return;
    if (_idCounter != 1) {
      throw Error( "DBo::enableForcedIdMode(): DataBase must be reset before forcind ids." );
    }
    _flags |= ForcedIdMode;
  }


  void  DBo::disableForcedIdMode ()
  {
    if (not (_flags & ForcedIdMode)) return;
    _flags &= ~ForcedIdMode;
  }


  void  DBo::setNextId ( unsigned int nid )
  {
    if (not (_flags & ForcedIdMode)) {
      cerr << Error("DBo::setNextId(): Not in forced id mode, ignored.") << endl;
      return;
    }
    _nextId = nid;
    if (nid > _idCounter) _idCounter = nid;
    _flags |= NextIdSet;
  }


  unsigned int  DBo::getNextId ()
  {
    if (_flags & ForcedIdMode) {
      if (_flags & NextIdSet) {
        _flags &= ~NextIdSet;
        cdebug_log(18,0) << demangle(typeid(*this).name())
                   << "::getNextId(): Consuming the preset id:" << _nextId << endl;
        return _nextId;
      } else {
        throw Error("DBo::getNextId(): Next id is not set, while in forced id mode.");
      }
    }

    return ++_idCounter;
  }



  DBo::DBo ()
    : _id         (getNextId())
    , _propertySet()
  {
    if (_idCounterLimit and (_id > _idCounterLimit)) {
      throw Error( "DBo::DBo(): Identifier counter has reached user's limit (%d)."
                 , _idCounterLimit );
    }
    if (_idCounter == std::numeric_limits<unsigned int>::max()) {
      throw Error( "DBo::DBo(): Identifier counter has reached type limit (%d bits)."
                 , std::numeric_limits<unsigned int>::digits );
    }

    size_t memorySize = Timer::getMemorySize();
    if (_memoryLimit and ((memorySize >> 20) > _memoryLimit)) {
      throw Error( "DBo::DBo(): Program has reached maximum allowed limit of %dMb."
                 , _memoryLimit );
    }

    // if (_id % 10000 == 0) {
    //   cerr << "Reached id:" << _id << " " << Timer::getStringMemory(memorySize) << endl;
    // }
    // if (_id == 75060)
    //   cerr << "DBo::DBo() " << this << endl;
  }


  DBo::~DBo () throw(Error)
  { }


  void  DBo::_postCreate ()
  {
    cdebug_log(0,0) << "DBo::_postCreate() " << this << endl;
  }


  void  DBo::_preDestroy ()
  {
    clearProperties ();
  }


  void DBo::destroy ()
  {
    cdebug_log(0,1) << "DBo::destroy() " << this << endl;
    _preDestroy();
    cdebug_tabw(0,-1);
    delete this;
  }


  Property* DBo::getProperty ( const Name& name ) const
  {
    set<Property*>::const_iterator iterator = _propertySet.begin();
    while ( iterator != _propertySet.end() ) {
      Property* property = *iterator;
      if (property->getName() == name) return property;
      ++iterator;
    }
    return NULL;
  }


  Properties  DBo::getProperties () const
  {
    return getCollection(_propertySet);
  }


  void  DBo::setId ( unsigned int id )
  {
    if (_flags & ForcedIdMode) {
      _id = id;
      if (_id > _idCounter) _idCounter = _id;
    } else {
      throw Error("DBo::setId(): Attempt to set id while not in forced id mode.");
    }
  }


  void  DBo::put ( Property* property )
  {
    if ( !property )
      throw Error("DBo::put(): Can't put property : NULL property.");

    Property* oldProperty = getProperty ( property->getName() );
    if ( property != oldProperty ) {
      if ( oldProperty ) {
        _propertySet.erase ( oldProperty );
        oldProperty->onReleasedBy ( this );
      }
      _propertySet.insert ( property );
      property->onCapturedBy ( this );
    }
  }


  void  DBo::remove ( Property* property )
  {
    if ( !property )
      throw Error("DBo::remove(): Can't remove property : NULL property.");

    if ( _propertySet.find(property) != _propertySet.end() ) {
      _propertySet.erase ( property );
      property->onReleasedBy ( this );
      if ( dynamic_cast<Quark*>(this) && _propertySet.empty() )
        destroy();
    }
  }


  void  DBo::removeProperty ( const Name& name )
  {
    Property* property = getProperty ( name );
    if ( property ) {
      _propertySet.erase ( property );
      property->onReleasedBy ( this );
      if ( dynamic_cast<Quark*>(this) && _propertySet.empty() )
        destroy();
    }
  }


  void  DBo::_onDestroyed ( Property* property )
  {
    if ( property && ( _propertySet.find(property) != _propertySet.end() ) ) {
      _propertySet.erase ( property );
      if ( dynamic_cast<Quark*>(this) && _propertySet.empty() )
        destroy();
    }
  }


  void  DBo::clearProperties ()
  {
    while ( !_propertySet.empty() ) {
      Property* property = *_propertySet.begin();
      _propertySet.erase ( property );
      property->onReleasedBy ( this );
    }
  }


  void  DBo::_toJson ( JsonWriter* writer ) const
  { }


  void  DBo::_toJsonSignature ( JsonWriter* writer ) const
  { _toJson( writer ); }


  void  DBo::_toJsonCollections ( JsonWriter* writer ) const
  {
    writer->key( "+propertySet" );
    writer->startArray();
    for ( Property* property : getProperties() ) {
      if (property->hasJson()) property->toJson( writer, this );
    }
    writer->endArray();
  }


  string  DBo::_getTypeName () const
  {
    return "DBo";
  }


  string  DBo::_getString () const
  {
    return "<id:" + getString(_id) + " " + _getTypeName() + ">";
  }


  Record* DBo::_getRecord () const
  {
    Record* record = new Record ( getString(this) );
    record->add( getSlot("_id"         , _id          ) );
    record->add( getSlot("_propertySet", &_propertySet) );
    return record;
  }


  void  DBo::toJsonSignature ( JsonWriter* w ) const
  {
    w->startObject();
    std::string tname = "Signature." + _getTypeName();
    jsonWrite( w, "@typename", tname );
    _toJsonSignature( w );
    w->endObject();
  }


  void  DBo::toJson ( JsonWriter* w ) const
  {
    w->startObject();
    const Entity* entity = dynamic_cast<const Entity*>( this );
    if (w->issetFlags(JsonWriter::UseEntityReference) and entity) {
      jsonWrite( w, "@typename", "&Entity"       );
      jsonWrite( w, "_id"      , entity->getId() );
    } else {
      std::string tname = _getTypeName();
      if (w->issetFlags(JsonWriter::UsePlugReference) and (tname == "Plug")) {
        tname.insert( 0, "&" );
      }
      jsonWrite( w, "@typename", tname );
      _toJson( w );
      _toJsonCollections( w );
    }
    w->endObject();
  }


// -------------------------------------------------------------------
// Class  :  "Hurricane::JsonDBo".

  JsonDBo::JsonDBo ( unsigned int flags )
    : JsonObject(flags | JsonWriter::DBoObject)
  { }


} // End of Hurricane namespace.
