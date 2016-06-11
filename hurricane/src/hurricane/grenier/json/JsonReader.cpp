// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2015-2016, All Rights Reserved
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
// |  C++ Module  :  "./JsonReader.cpp"                              |
// +-----------------------------------------------------------------+


#include "rapidjson/filereadstream.h"
#include "rapidjson/reader.h"
// Needed for registering. May be deleted later.
#include "hurricane/DebugSession.h"
#include "hurricane/Warning.h"
#include "hurricane/JsonReader.h"
#include "hurricane/Cell.h"
#include "hurricane/Net.h"
#include "hurricane/Instance.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Contact.h"
#include "hurricane/Vertical.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Pad.h"


namespace {
  
  using namespace std;
  using namespace rapidjson;
  using namespace Hurricane;


// -------------------------------------------------------------------
// Struct  :  "HurricaneHandler".

  class HurricaneHandler {
    public:
      HurricaneHandler ( JsonStack& );
    public:
      bool  Null         ();
      bool  Bool         ( bool );
      bool  Int          ( int );
      bool  Int64        ( int64_t );
      bool  Uint         ( unsigned int );
      bool  Uint64       ( uint64_t );
      bool  Double       ( double );
      bool  String       ( const char*, SizeType, bool copy );
      bool  Key          ( const char*, SizeType, bool copy );
      bool  StartObject  ();
      bool  EndObject    ( SizeType );
      bool  StartArray   ();
      bool  EndArray     ( SizeType );
      bool  doCallToData () const;
    private:
      enum Flags { TypenameKey  = (1<<0)
                 , ArrayMode    = (1<<2)
                 , SkipObject   = (1<<3)
                 , SkipArray    = (1<<4)
                 , SkipMask     = SkipObject | SkipArray
                 };
    private:
      unsigned long       _state;
      string              _key;
      string              _objectName;
      vector<JsonObject*> _objects;
      JsonStack&          _stack;
  };


  HurricaneHandler::HurricaneHandler ( JsonStack& stack )
    : _state     (0)
    , _key       ()
    , _objectName()
    , _objects   ()
    , _stack     (stack)
  { }


  bool  HurricaneHandler::doCallToData () const
  { return not _objects.empty() and _objects.back() and not _objects.back()->isCreated(); }


  bool  HurricaneHandler::Null ()
  {
  //if (not _objects.empty()) ltrace(59) << "null  _objects.back(): " << _objects.back() << endl;

    if (_state & SkipMask) return true;
    _stack.push_back<void*>( _key, NULL );
    return true;
  }


  bool  HurricaneHandler::Bool ( bool v )
  {
    if (_state & SkipMask) return true;
    _stack.push_back<bool>( _key, v );
    return true;
  }


  bool  HurricaneHandler::Int ( int v )
  {
    if (_state & SkipMask) return true;
    _stack.push_back<int64_t>( _key, v );
    return true;
  }


  bool  HurricaneHandler::Int64 ( int64_t v )
  {
    if (_state & SkipMask) return true;
    _stack.push_back<int64_t>( _key, v );
    return true;
  }


  bool  HurricaneHandler::Uint ( unsigned int v )
  {
    if (_state & SkipMask) return true;
    _stack.push_back<int64_t>( _key, v );
    return true;
  }


  bool  HurricaneHandler::Uint64 ( uint64_t v )
  {
    if (_state & SkipMask) return true;
    _stack.push_back<uint64_t>( _key, v );
    return true;
  }


  bool  HurricaneHandler::Double ( double v )
  {
    if (_state & SkipMask) return true;
    _stack.push_back<double>( _key, v );
    return true;
  }


  bool  HurricaneHandler::String ( const char* value, SizeType, bool copy )
  {
    if (_state & SkipMask) return true;
    if (_state & TypenameKey) {
      _state &= ~TypenameKey;

      JsonObject* object = JsonTypes::find( value );
      _objects[_objects.size()-1] = object->clone();

      if (not object) {
        cerr << Warning("JsonReader::parse(): Do not know how to parse type %s.", value ) << endl;
        _state |= SkipObject;
      } else {
        _objects[_objects.size()-1]->setName( _objectName );
      }

      cdebug_log(19,0) << "HurricaneHandler::String() [key/typename] " << value << endl;
      return true;
    }

    _stack.push_back<string>( _key, value );
    return true;
  }


  bool  HurricaneHandler::Key ( const char* key, SizeType, bool copy )
  {
    _key = key;
    if (_state & SkipMask) {
      _key.clear();
      return true;
    }

    if (_state & TypenameKey) {
      if (_key != "@typename") {
        _state |= SkipObject;
        _key.clear();
      }
      return true;
    }

  //cdebug_log(19,0) << "HurricaneHandler::Key() key:" << _key << " _objects.size():" << _objects.size() << endl;

    if (_objects.back()) {
      if ( doCallToData() and not _key.empty() and (_key[0] != '_') ) {
      // The key is no longer a simple attribute of the object.
      // Triggers it's creation in the Json stack.
        cdebug_log(19,0) << "HurricaneHandler::key() Calling "
                   << _objects.back()->getTypeName() << "::toData(JsonStack&)." << endl;
        _objects.back()->toData( _stack );
      }
    }

    return true;
  }


  bool  HurricaneHandler::StartObject ()
  {
    cdebug_log(19,0) << "Hurricane::StartObject()" << endl;
    ltracein(50);

    _state |= TypenameKey;
    // if (doCallToData()) {
    //   cdebug_log(19,0) << "Calling " << _objects.back()->getTypeName() << "::toData(JsonStack&)." << endl;
    //   _objects.back()->toData( _stack );
    // }
    _objectName = (_key == ".Array") ? "" : _key;
    _objects.push_back( NULL );
    cdebug_log(19,0) << "_objects.push_back(NULL), size():" << _objects.size() << "." << endl;

    ltracein(50);
    return true;
  }


  bool  HurricaneHandler::EndObject ( SizeType )
  {
    ltraceout(50,2);
    cdebug_log(19,0) << "HurricaneHandler::EndObject()" << endl;
    ltracein(50);

    _objectName.clear();
    if (_state & SkipObject) {
      _state &= ~SkipObject;
    } else {
      if (doCallToData()) {
        cdebug_log(19,0) << "Calling " << _objects.back()->getTypeName() << "::toData(JsonStack&)." << endl;
        _objects.back()->toData( _stack );
      }
      if (_objects.size() > 1) {
        cdebug_log(19,0) << "_objects.pop_back(), size():" << _objects.size() << "." << endl;
        delete _objects.back();
        _objects.pop_back();
      }
      if (_stack.size() > 1) {
        if (_stack[-1].first[0] != '_') _stack.pop_back();
      }
    }
    cdebug_tabw(19,-1);
    return true;
  }


  bool  HurricaneHandler::StartArray()
  {
    cdebug_log(19,0) << "HurricaneHandler::StartArray() key:\"" << _key << "\"." << endl;
    ltracein(50);

    _objectName.clear();
    if (_key.empty()) { _state |= SkipArray; return true; }
    if (_key[0] != '+') {
      cerr << Warning("JsonReader::parse(): Array attributes must start by \'+\' %s.", _key.c_str() ) << endl;
      _state |= SkipArray;
      return true;
    }

    _state |= ArrayMode;
    _key = ".Array";

    return true;
  }

  bool  HurricaneHandler::EndArray ( SizeType )
  {
    cdebug_tabw(19,-1);
    cdebug_log(19,0) << "HurricaneHandler::EndArray()" << endl;
    ltracein(50);

    _state &= ~(ArrayMode | SkipArray);
    _key.clear();

    cdebug_tabw(19,-1);
    return true;
  }


// -------------------------------------------------------------------
// Class  :  "JsonReader".

  class JsonReader {
    public:
      enum Mode { CellMode = (1<<0) };
    public:
                        JsonReader  ();
                       ~JsonReader  ();
            JsonReader* setFlags    ( unsigned long mask );
            JsonReader* resetFlags  ( unsigned long mask );
            bool        issetFlags  ( unsigned long mask ) const;
      const JsonStack&  getStack    () const;
            void        parse       ( std::string fileName, unsigned int flags );
            void        close       ();
    private:                  
                        JsonReader  ( const JsonReader& );
            JsonReader& operator=   ( const JsonReader& ) const;
    private:
      unsigned long              _flags;
      size_t                     _bufferSize;
      char*                      _buffer;
      FILE*                      _file;
      FileReadStream*            _stream;
      JsonStack                  _stack;
      Reader                     _reader;
      HurricaneHandler           _handler;
  };


  JsonReader::JsonReader ()
    : _flags     (0)
    , _bufferSize(65536)
    , _buffer    (new char [_bufferSize])
    , _file      (NULL)
    , _stream    (NULL)
    , _stack     ()
    , _reader    ()
    , _handler   (_stack)
  {
  }


  JsonReader::~JsonReader ()
  {
    close();
    delete _buffer;
  }


  void  JsonReader::close ()
  {
    if (_stream) { delete _stream; _stream = NULL; }
    if (_file  ) { fclose(_file);  _file   = NULL; }
  }


  const JsonStack&  JsonReader::getStack () const
  { return _stack; }


  void  JsonReader::parse ( string fileName, unsigned int flags )
  {
    close();

    DebugSession::open( 50 );

    fileName += ".json";
    _file     = fopen( fileName.c_str(), "r" );
    cerr << "_file:" << _file << ", _buffer:" << (void*)_buffer << endl;
    _stream = new FileReadStream ( _file, _buffer, _bufferSize );

    _reader.Parse( *_stream, _handler );
    _stack.print( cerr );

    DebugSession::close();
    close();
  }


}  // local namespace.


namespace Hurricane {

  using namespace std;


// -------------------------------------------------------------------
// Class  :  "JsonAttribute".

  JsonAttribute::~JsonAttribute ()
  { }


// -------------------------------------------------------------------
// Class  :  "JsonStacked".

  JsonStacked::~JsonStacked ()
  { }


  type_index  JsonStacked::tid () const
  { return typeid(JsonStacked); }


  void  JsonStacked::toJson ( JsonWriter*, boost::any object ) const
  {
    cerr << Error("JsonStacked::toJson() is a dummy method that should never be called.") << endl;
  }


  JsonStacked* JsonStacked::clone () const
  { return new JsonStacked( *this ); }


// -------------------------------------------------------------------
// Class  :  "JsonObject".

  JsonObject::JsonObject ()
    : _name       ()
    , _stackeds   ()
    , _attributes ()
    , _collections()
    , _object     ()
  { }


  JsonObject::JsonObject ( const JsonObject& other )
    : _name       (other._name)
    , _stackeds   ()
    , _attributes ()
    , _collections()
    , _object     ()
  {
    for ( JsonAttribute* attribute : other._stackeds    ) _stackeds.push_back( attribute->clone() );
    for ( JsonAttribute* attribute : other._attributes  ) _stackeds.push_back( attribute->clone() );
    for ( JsonAttribute* attribute : other._collections ) _stackeds.push_back( attribute->clone() );
  }


  JsonObject::~JsonObject ()
  { clear(); }


  bool  JsonObject::isJsonObject () const
  { return _flags & IsJsonObject; }


  void  JsonObject::remove ( const std::string& key )
  {
    if (key.empty()) {
      cerr << Error( "JsonObject::remove(): Attempt to remove attribute with an empty name, ignored." ) << endl;
      return;
    }

    switch ( key[0] ) {
      case '.':
        for ( auto it = _stackeds.begin() ; it != _stackeds.end() ; ++it )
          if (key == (*it)->key()) { delete (*it); _stackeds.erase(it); break; }
        break;
      case '_':
        for ( auto it = _attributes.begin() ; it != _attributes.end() ; ++it )
          if (key == (*it)->key()) { delete (*it); _attributes.erase(it); break; }
        break;
      case '+':
        for ( auto it = _collections.begin() ; it != _collections.end() ; ++it )
          if (key == (*it)->key()) { delete (*it); _collections.erase(it); break; }
        break;
    }
  }


  bool  JsonObject::has ( const std::string& key ) const
  {
    if (key.empty()) return false;
    switch ( key[0] ) {
      case '.':
        for ( size_t i=0 ; i<_stackeds.size() ; ++i )
          if (key == _stackeds[i]->key()) return true;
        break;
      case '_':
        for ( size_t i=0 ; i<_attributes.size() ; ++i )
          if (key == _attributes[i]->key()) return true;
        break;
      case '+':
        for ( size_t i=0 ; i<_collections.size() ; ++i )
          if (key == _collections[i]->key()) return true;
        break;
    }
    return false;
  }


  bool  JsonObject::check ( JsonStack& stack, string fname ) const
  {
    for ( size_t i=0 ; i<_stackeds.size() ; ++i ) {
      if (not stack.rhas(_stackeds[i]->key())) {
        cerr << Error( "%s(): Stack is missing context element with key \"%s\""
                     , fname.c_str(), _stackeds[i]->key().c_str() ) << endl;
        return false;
      }
    }
    for ( size_t i=0 ; i<_attributes.size() ; ++i ) {
      if (not stack.rhas(_attributes[i]->key())) {
        cerr << Error( "%s(): Stack is missing attribute element with key \"%s\""
                     , fname.c_str(), _attributes[i]->key().c_str() ) << endl;
        return false;
      }
    }
    return true;
  }


  void  JsonObject::toData ( JsonStack& )
  { }


  void  JsonObject::print ( ostream& o ) const
  {
    o << tab << "JsonObject for type: " << getTypeName() << endl;
    for ( size_t i=0 ; i<_stackeds.size() ; ++i )
      o << tab << "key:" << left << setw(20) << _stackeds[i]->key()
        << " type:" << _stackeds[i]->tid().name() << endl;

    for ( size_t i=0 ; i<_attributes.size() ; ++i )
      o << tab << "key:" << left << setw(20) << _attributes[i]->key()
        << " type:" << _attributes[i]->tid().name() << endl;

    for ( size_t i=0 ; i<_collections.size() ; ++i )
      o << tab << "key:" << left << setw(20) << _collections[i]->key()
        << " type:" << _collections[i]->tid().name() << endl;
  }


  JsonKey::JsonKey ( const string& key )
    : JsonObject()
    , _key      (key)
  { }


  string  JsonKey::getTypeName () const
  { return _key; }


  JsonKey* JsonKey::clone () const
  { return new JsonKey ( *this ); }


// -------------------------------------------------------------------
// Class  :  "JsonTypes".

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
  {
    if (_jsonTypes) return;

    _jsonTypes = new JsonTypes ();
    _jsonTypes->_registerType( new JsonPoint         (0) );
    _jsonTypes->_registerType( new JsonBox           (0) );
    _jsonTypes->_registerType( new JsonTransformation(0) );
    _jsonTypes->_registerType( new JsonCell          (0) );
    _jsonTypes->_registerType( new JsonNet           (0) );
    _jsonTypes->_registerType( new JsonPlugRef       (0) );
    _jsonTypes->_registerType( new JsonRoutingPad    (0) );
    _jsonTypes->_registerType( new JsonContact       (0) );
    _jsonTypes->_registerType( new JsonVertical      (0) );
    _jsonTypes->_registerType( new JsonHorizontal    (0) );
    _jsonTypes->_registerType( new JsonPad           (0) );
    _jsonTypes->_registerType( new JsonInstance      (0) );
    _jsonTypes->_registerType( new JsonPlug          (0) );
  }


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


// -------------------------------------------------------------------
// Function  :  Json Cell parser.

  Cell* jsonCellParse ( string filename )
  {
    JsonReader reader;
    reader.parse( filename, JsonReader::CellMode );

    const JsonStack& stack = reader.getStack();
    if (stack.rhas(".Cell")) return stack.as<Cell*>(".Cell");
    return NULL;
  }


}  // Hurricane namespace.
