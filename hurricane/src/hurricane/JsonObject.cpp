// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2015-2018, All Rights Reserved
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
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./JsonObject.cpp"                              |
// +-----------------------------------------------------------------+


// Needed for registering. May be deleted later.
#include "hurricane/Initializer.h"
#include "hurricane/DebugSession.h"
#include "hurricane/Warning.h"
#include "hurricane/Library.h"
#include "hurricane/Cell.h"


namespace Hurricane {

  using namespace std;


// -------------------------------------------------------------------
// Class  :  "JsonObject".

  JsonObject::JsonObject ( unsigned long flags )
    : _flags      (flags)
    , _name       ()
    , _stackeds   ()
    , _attributes ()
    , _collections()
    , _object     ()
  { }


  JsonObject::~JsonObject ()
  { }


  bool  JsonObject::isDummy () const
  { return false; }


  void  JsonObject::add ( const string& key, type_index tid )
  {
    if (key.empty()) {
      cerr << "[ERROR] JsonObject::add(): Attempt to add attribute with an empty name, ignored."
           << endl;
      return;
    }
    if (has(key)) {
      cerr << "[ERROR] JsonObject::add(): Attempt to add attribute \"" << key << "\" twice, cancelled."
           << endl;
      return;
    }

    switch ( key[0] ) {
      case '.': _stackeds   .push_back( JsonAttribute(key,tid) ); return;
      case '_': _attributes .push_back( JsonAttribute(key,tid) ); return;
      case '+': _collections.push_back( JsonAttribute(key,tid) ); return;
      default: break;
    }

    cerr << "[ERROR] JsonObject::add(): Key name \"" << key
         << "\" do not follow naming convention, cancelled." << endl;
  }


  void  JsonObject::remove ( const std::string& key )
  {
    if (key.empty()) {
      cerr << Error( "JsonObject::remove(): Attempt to remove attribute with an empty name, ignored." ) << endl;
      return;
    }

    switch ( key[0] ) {
      case '.':
        for ( auto it = _stackeds.begin() ; it != _stackeds.end() ; ++it )
          if (key == (*it).key()) { _stackeds.erase(it); break; }
        break;
      case '_':
        for ( auto it = _attributes.begin() ; it != _attributes.end() ; ++it )
          if (key == (*it).key()) { _attributes.erase(it); break; }
        break;
      case '+':
        for ( auto it = _collections.begin() ; it != _collections.end() ; ++it )
          if (key == (*it).key()) { _collections.erase(it); break; }
        break;
    }
  }


  bool  JsonObject::has ( const std::string& key ) const
  {
    if (key.empty()) return false;
    switch ( key[0] ) {
      case '.':
        for ( size_t i=0 ; i<_stackeds.size() ; ++i )
          if (key == _stackeds[i].key()) return true;
        break;
      case '_':
        for ( size_t i=0 ; i<_attributes.size() ; ++i )
          if (key == _attributes[i].key()) return true;
        break;
      case '+':
        for ( size_t i=0 ; i<_collections.size() ; ++i )
          if (key == _collections[i].key()) return true;
        break;
    }
    return false;
  }


  bool  JsonObject::check ( JsonStack& stack, string fname ) const
  {
    for ( size_t i=0 ; i<_stackeds.size() ; ++i ) {
      if (not stack.rhas(_stackeds[i].key())) {
        cerr << Error( "%s(): Stack is missing context element with key \"%s\""
                     , fname.c_str(), _stackeds[i].key().c_str() ) << endl;
        return false;
      }
    }
    for ( size_t i=0 ; i<_attributes.size() ; ++i ) {
      if (not stack.rhas(_attributes[i].key())) {
        cerr << Error( "%s(): Stack is missing attribute element with key \"%s\""
                     , fname.c_str(), _attributes[i].key().c_str() ) << endl;
        return false;
      }
    }
    return true;
  }


  void  JsonObject::toData ( JsonStack& )
  { }


  unsigned int  JsonObject::presetId ( JsonStack& stack )
  {
    unsigned int jsonId = get<int64_t>( stack, "_id" );
    if (issetFlags(JsonWriter::DesignBlobMode)) {
      Entity::setNextId( jsonId );
    }
    return jsonId;
  }


  void  JsonObject::print ( ostream& o ) const
  {
    o << tab << "JsonObject for type: " << getTypeName() << endl;
    for ( size_t i=0 ; i<_stackeds.size() ; ++i )
      o << tab << "key:" << left << setw(20) << _stackeds[i].key()
        << " type:" << _stackeds[i].tid().name() << endl;

    for ( size_t i=0 ; i<_attributes.size() ; ++i )
      o << tab << "key:" << left << setw(20) << _attributes[i].key()
        << " type:" << _attributes[i].tid().name() << endl;

    for ( size_t i=0 ; i<_collections.size() ; ++i )
      o << tab << "key:" << left << setw(20) << _collections[i].key()
        << " type:" << _collections[i].tid().name() << endl;
  }


// -------------------------------------------------------------------
// Class  :  "JsonKey".

  JsonKey::JsonKey ( const string& key )
    : JsonObject(0)
    , _key      (key)
  { }


  string   JsonKey::getTypeName () const                { return _key; }
  JsonKey* JsonKey::clone       ( unsigned long ) const { return new JsonKey ( *this ); }


// -------------------------------------------------------------------
// Class  :  "JsonDummy".

  JsonDummy::JsonDummy ()
    : JsonObject(0)
    , _typename ("dummy")
  { }


  bool       JsonDummy::isDummy     () const                { return true; }
  string     JsonDummy::getTypeName () const                { return _typename; }
  void       JsonDummy::setTypeName ( const string& name )  { _typename=name; }
  JsonDummy* JsonDummy::clone       ( unsigned long ) const { return new JsonDummy ( *this ); }


// -------------------------------------------------------------------
// Class  :  "JsonTypes".

  Initializer<JsonTypes>  jsonTypesInitialize ( 10 );

  JsonTypes* JsonTypes::_jsonTypes = NULL;


  JsonTypes::JsonTypes ()
    : _jsonObjects()
  { }


  JsonTypes::~JsonTypes ()
  {
    for ( JsonObject* object : _jsonObjects ) delete object;
  }


  void  JsonTypes::_registerType ( JsonObject* object )
  {
    if (_find(object->getTypeName())) {
      throw Error( "JsonTypes::_registerType(): Attempt to register <%s> twice.", object->getTypeName().c_str() );
    }
    _jsonObjects.insert( object );
  }


  JsonObject* JsonTypes::_find ( const string& tname )
  {
    JsonKey key( tname );
    set<JsonObject*>::iterator it = _jsonObjects.find( &key );
    if (it != _jsonObjects.end()) return (*it);
    return NULL;
  }


  void  JsonTypes::registerType ( JsonObject* object )
  {
    if (not _jsonTypes) initialize();
    _jsonTypes->_registerType( object );
  }


  JsonObject* JsonTypes::find ( const string& tname )
  {
    if (not _jsonTypes) initialize();
    return _jsonTypes->_find( tname );
  }


  void  JsonTypes::initialize ()
  { if (not _jsonTypes) _jsonTypes = new JsonTypes (); }


// -------------------------------------------------------------------
// Class  :  "JsonStack".

  void  JsonStack::addEntity ( unsigned int jsonId, Entity* entity )
  { _entities.insert( std::make_pair(jsonId,entity) ); }


  void  JsonStack::print ( ostream& o ) const
  {
    o << tab << "JsonStack::print() Stack contains " << _stack.size() << " elements." << endl;
    for ( size_t i=0 ; i<_stack.size() ; ++i ) {
      o << "[" << right << setw(2) << i << "] key: \"" << left << setw(20) << _stack[i].first
        << "\", type: \"" << demangle(_stack[i].second.type()) << "\"." << endl;
    }
  }


}  // Hurricane namespace.
