// -*- C++ -*-
//
// Copyright (c) BULL S.A. 2015-2015, All Rights Reserved
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
#include "hurricane/Library.h"
#include "hurricane/Cell.h"
#include "hurricane/Net.h"
#include "hurricane/DeepNet.h"
#include "hurricane/Instance.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/Contact.h"
#include "hurricane/Vertical.h"
#include "hurricane/Horizontal.h"
#include "hurricane/Pad.h"
#include "hurricane/UpdateSession.h"
#include "hurricane/DesignBlob.h"


namespace {
  
  using namespace std;
  using namespace rapidjson;
  using namespace Hurricane;

  class JsonReader;


// -------------------------------------------------------------------
// Class  :  "HurricaneHandler" (declaration).

  class HurricaneHandler {
    public:
                            HurricaneHandler ( JsonReader& );
    public:                 
             bool           Null             ();
             bool           Bool             ( bool );
             bool           Int              ( int );
             bool           Int64            ( int64_t );
             bool           Uint             ( unsigned int );
             bool           Uint64           ( uint64_t );
             bool           Double           ( double );
             bool           String           ( const char*, SizeType, bool copy );
             bool           Key              ( const char*, SizeType, bool copy );
             bool           StartObject      ();
             bool           EndObject        ( SizeType );
             bool           StartArray       ();
             bool           EndArray         ( SizeType );
      inline bool           isDummy          () const;
      inline bool           doCallToData     () const;
      inline JsonStack&     stack            ();
      inline unsigned long  flags            ();
    private:
      enum Flags { TypenameKey  = (1<<0)
                 };
    private:
      unsigned long       _state;
      string              _key;
      string              _objectName;
      vector<JsonObject*> _objects;
      JsonReader&         _reader;
  };


// -------------------------------------------------------------------
// Class  :  "JsonReader" (declaration).

  class JsonReader {
    public:
                            JsonReader  ( unsigned long flags );
                           ~JsonReader  ();
      inline JsonReader*    setFlags    ( unsigned long mask );
      inline JsonReader*    resetFlags  ( unsigned long mask );
      inline bool           issetFlags  ( unsigned long mask ) const;
      inline unsigned long  getFlags    () const;
      inline JsonStack&     getStack    ();
             void           parse       ( std::string fileName );
             void           close       ();
    private:               
                            JsonReader  ( const JsonReader& );
             JsonReader&    operator=   ( const JsonReader& ) const;
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


// -------------------------------------------------------------------
// Class  :  "HurricaneHandler" (definition).

  HurricaneHandler::HurricaneHandler ( JsonReader& reader )
    : _state     (0)
    , _key       ()
    , _objectName()
    , _objects   ()
    , _reader    (reader)
  { }


  inline JsonStack&     HurricaneHandler::stack () { return _reader.getStack(); }
  inline unsigned long  HurricaneHandler::flags () { return _reader.getFlags(); }


  inline bool  HurricaneHandler::isDummy () const
  { return _objects.empty() or _objects.back()->isDummy(); }


  inline bool  HurricaneHandler::doCallToData () const
  { return not _objects.empty() and _objects.back() and not _objects.back()->isBound(); }


  bool  HurricaneHandler::Null ()
  {
    if (isDummy()) return true;
    stack().push_back<void*>( _key, NULL );
    return true;
  }


  bool  HurricaneHandler::Bool ( bool v )
  {
    if (isDummy()) return true;
    stack().push_back<bool>( _key, v );
    return true;
  }


  bool  HurricaneHandler::Int ( int v )
  {
    if (isDummy()) return true;
    stack().push_back<int64_t>( _key, v );
    return true;
  }


  bool  HurricaneHandler::Int64 ( int64_t v )
  {
    if (isDummy()) return true;
    stack().push_back<int64_t>( _key, v );
    return true;
  }


  bool  HurricaneHandler::Uint ( unsigned int v )
  {
    if (isDummy()) return true;
    stack().push_back<int64_t>( _key, v );
    return true;
  }


  bool  HurricaneHandler::Uint64 ( uint64_t v )
  {
    if (isDummy()) return true;
    stack().push_back<int64_t>( _key, v );
    return true;
  }


  bool  HurricaneHandler::Double ( double v )
  {
    if (isDummy()) return true;
    stack().push_back<double>( _key, v );
    return true;
  }


  bool  HurricaneHandler::String ( const char* value, SizeType, bool copy )
  {
    if (isDummy() and not (_state & TypenameKey)) return true;
    if (_state & TypenameKey) {
      _state &= ~TypenameKey;

      if (_key != "@typename") {
        cerr << Warning("JsonReader::parse(): First field is not @typename, skipping object." ) << endl;
        return true;
      }

      string         svalue    = value;
      JsonObject*    object    = NULL;
      JsonSignature* signature = NULL;
      if (svalue.compare(0,10,"Signature.") == 0) object = JsonTypes::find( "Signature" );
      else                                        object = JsonTypes::find( value );

      if (not object) {
      // Keep the dummy object on top of the stack.
        cerr << Warning( "JsonReader::parse(): Do not know how to parse type \"%s\" (ignored)."
                       , value ) << endl;
        dynamic_cast<JsonDummy*>(_objects.back())->setTypeName( value );
      } else {
      // Replace the dummy object on top of the stack.
        delete _objects.back();
        _objects[_objects.size()-1] = object->clone( flags() );
        _objects.back()->setName( _objectName );

        signature = dynamic_cast<JsonSignature*>( _objects.back() );
        if (signature) {
          size_t dot = svalue.find('.');
          signature->setSubType( svalue.substr(dot+1) );
        }
      }

      ltrace(51) << "HurricaneHandler::String() [key/typename] \"" << value << "\"." << endl;
      return true;
    }

    stack().push_back<string>( _key, value );
    return true;
  }


  bool  HurricaneHandler::Key ( const char* key, SizeType, bool copy )
  {
    if (isDummy() and not (_state & TypenameKey)) return true;

    _key = key;
    if (_state & TypenameKey) return true;

  //ltrace(51) << "HurricaneHandler::Key() key:" << _key << " _objects.size():" << _objects.size() << endl;

    if (_objects.back()) {
      if ( doCallToData() and not _key.empty() and (_key[0] != '_') ) {
      // The key is no longer a simple attribute of the object.
      // Triggers it's creation in the Json stack.
        ltrace(51) << "HurricaneHandler::key() Calling "
                   << _objects.back()->getTypeName() << "::toData(JsonStack&)." << endl;
        _objects.back()->toData( stack() );
      }
    }

    return true;
  }


  bool  HurricaneHandler::StartObject ()
  {
    ltrace(50) << "Hurricane::StartObject()" << endl;
    ltracein(50);

    _state |= TypenameKey;
    _objectName = (_key == ".Array") ? "" : _key;
    _objects.push_back( new JsonDummy() );
    ltrace(51) << "_objects.push_back(NULL), size():" << _objects.size() << "." << endl;

    ltracein(50);
    return true;
  }


  bool  HurricaneHandler::EndObject ( SizeType )
  {
    ltraceout(50,2);
    ltrace(50) << "HurricaneHandler::EndObject()" << endl;
    ltracein(50);

    _objectName.clear();
    if (not isDummy()) {
      if (doCallToData()) {
        ltrace(51) << "Calling " << _objects.back()->getTypeName() << "::toData(JsonStack&)." << endl;
        _objects.back()->toData( stack() );
      }
      if (stack().size() > 1) {
        if (stack()[-1].first[0] != '_') stack().pop_back();
      }
    }

  //if (_objects.size() > 1) {
      ltrace(51) << "_objects.pop_back(), size():" << _objects.size() << "." << endl;
      delete _objects.back();
      _objects.pop_back();
    //}

    ltraceout(50);
    return true;
  }


  bool  HurricaneHandler::StartArray()
  {
    ltrace(50) << "HurricaneHandler::StartArray() key:\"" << _key << "\"." << endl;
    ltracein(50);

    _objectName.clear();
    if (_key[0] != '+') {
      cerr << Warning("JsonReader::parse(): Array attributes must start by \'+\' %s.", _key.c_str() ) << endl;
      return true;
    }

    _key = ".Array";
    return true;
  }

  bool  HurricaneHandler::EndArray ( SizeType )
  {
    ltraceout(50);
    ltrace(50) << "HurricaneHandler::EndArray()" << endl;
    ltracein(50);

    _key.clear();

    ltraceout(50);
    return true;
  }


// -------------------------------------------------------------------
// Class  :  "JsonReader" (definition).

  JsonReader::JsonReader ( unsigned long flags )
    : _flags     (flags)
    , _bufferSize(65536)
    , _buffer    (new char [_bufferSize])
    , _file      (NULL)
    , _stream    (NULL)
    , _stack     ()
    , _reader    ()
    , _handler   (*this)
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


  inline JsonReader*    JsonReader::setFlags   ( unsigned long mask ) { _flags |=  mask; return this; }
  inline JsonReader*    JsonReader::resetFlags ( unsigned long mask ) { _flags &= ~mask; return this; }
  inline bool           JsonReader::issetFlags ( unsigned long mask ) const { return _flags & mask; }
  inline unsigned long  JsonReader::getFlags   () const { return _flags; }
  inline JsonStack&     JsonReader::getStack   () { return _stack; }


  void  JsonReader::parse ( string fileName )
  {
    close();

    DebugSession::open( 50 );

    fileName += ".json";
    _file     = fopen( fileName.c_str(), "r" );
    ltrace(50) << "_file:" << _file << ", _buffer:" << (void*)_buffer << endl;

    if (not _file) {
      throw Error( "JsonReader::parse(): Cannot open file \"%s\"."
                 , fileName.c_str() );
    }

    _stream = new FileReadStream ( _file, _buffer, _bufferSize );

    if (issetFlags(JsonWriter::DesignBlobMode))
      Entity::enableForcedIdMode();
    _reader.Parse( *_stream, _handler );
    _stack.print( cerr );
    if (issetFlags(JsonWriter::DesignBlobMode))
      Entity::disableForcedIdMode();

    DebugSession::close();
    close();
  }


}  // local namespace.


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
    _jsonTypes->_registerType( new JsonPoint         (JsonWriter::RegisterMode) );
    _jsonTypes->_registerType( new JsonBox           (JsonWriter::RegisterMode) );
    _jsonTypes->_registerType( new JsonTransformation(JsonWriter::RegisterMode) );
    _jsonTypes->_registerType( new JsonLibrary       (JsonWriter::RegisterMode) );
    _jsonTypes->_registerType( new JsonCell          (JsonWriter::RegisterMode) );
    _jsonTypes->_registerType( new JsonNet           (JsonWriter::RegisterMode) );
    _jsonTypes->_registerType( new JsonDeepNet       (JsonWriter::RegisterMode) );
    _jsonTypes->_registerType( new JsonPlugRef       (JsonWriter::RegisterMode) );
    _jsonTypes->_registerType( new JsonRoutingPad    (JsonWriter::RegisterMode) );
    _jsonTypes->_registerType( new JsonContact       (JsonWriter::RegisterMode) );
    _jsonTypes->_registerType( new JsonVertical      (JsonWriter::RegisterMode) );
    _jsonTypes->_registerType( new JsonHorizontal    (JsonWriter::RegisterMode) );
    _jsonTypes->_registerType( new JsonPad           (JsonWriter::RegisterMode) );
    _jsonTypes->_registerType( new JsonInstance      (JsonWriter::RegisterMode) );
    _jsonTypes->_registerType( new JsonPlug          (JsonWriter::RegisterMode) );
    _jsonTypes->_registerType( new JsonOccurrence    (JsonWriter::RegisterMode) );
    _jsonTypes->_registerType( new JsonSignature     (JsonWriter::RegisterMode) );
    _jsonTypes->_registerType( new JsonDesignBlob    (JsonWriter::RegisterMode) );
  }


// -------------------------------------------------------------------
// Class  :  "JsonStack".

  void  JsonStack::addEntity ( unsigned int jsonId, Entity* entity )
  { _entities.insert( std::make_pair(jsonId,entity) ); }


  void  JsonStack::addHookLink ( Hook* hook, unsigned int jsonId, const string& jsonNext )
  {
    if (jsonNext.empty()) return;

    unsigned int id      = jsonId;
    string       tname   = hook->_getTypeName();

    auto ielement = _hooks.find( HookKey(id,tname) );
    if (ielement == _hooks.end()) {
      auto r = _hooks.insert( make_pair( HookKey(id,tname), HookElement(hook) ) );
      ielement = r.first;
      (*ielement).second.setFlags( HookElement::OpenRingStart );
    }
    HookElement* current = &((*ielement).second);
    if (not current->hook()) current->setHook( hook );

    hookFromString( jsonNext, id, tname );
    ielement = _hooks.find( HookKey(id,tname) );
    if (ielement == _hooks.end()) {
      auto r = _hooks.insert( make_pair( HookKey(id,tname), HookElement(NULL) ) );
      ielement = r.first;
    } else {
      (*ielement).second.resetFlags( HookElement::OpenRingStart );
    }
    current->setNext( &((*ielement).second) );
  }


  Hook* JsonStack::getHook ( unsigned int jsonId, const std::string& tname ) const
  {
    auto ihook = _hooks.find( HookKey(jsonId,tname) );
    if (ihook == _hooks.end()) return NULL;

    return (*ihook).second.hook();
  }


  bool  JsonStack::hookFromString ( std::string s, unsigned int& id, std::string& tname )
  {
    size_t dot = s.rfind('.');
    if (dot == string::npos) return false;

    tname = s.substr( 0, dot );
    id    = stoul( s.substr(dot+1) );
    return true;
  }


  bool  JsonStack::checkRings () const
  {
    bool status = true;

    for ( auto kv : _hooks ) {
      HookElement* ringStart = &(kv.second);
      if (ringStart->issetFlags(HookElement::ClosedRing)) continue;

      if (ringStart->issetFlags(HookElement::OpenRingStart)) {
        cerr << Error( "JsonStack::checkRing(): Open ring found, starting with %s.\n"
                     "        Closing the ring..."
                     , getString(ringStart->hook()).c_str() ) << endl;

        status = false;
        HookElement* element = ringStart;
        while ( true ) {
          if (not element->next()) {
          // The ring is open: close it (loop on ringStart).
            element->setNext( ringStart );
            element->setFlags( HookElement::ClosedRing );

            cerr << Error( "Simple open ring." ) << endl;
            break;
          }
          if (element->next()->issetFlags(HookElement::ClosedRing)) {
          // The ring is half merged with itself, or another ring.
          // (i.e. *multiple* hooks pointing the *same* next element)
            element->setNext( ringStart );
            element->setFlags( HookElement::ClosedRing );

            cerr << Error( "Complex fault: ring partially merged (convergent)." ) << endl;
            break;
          }
          element = element->next();
        }
      }
    }

    return status;
  }


  void  JsonStack::buildRings () const
  {
    for ( auto kv : _hooks ) {
      kv.second.hook()->_setNextHook( kv.second.next()->hook() );
    }
  }


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
    UpdateSession::open();

    JsonReader reader ( JsonWriter::CellMode );
    reader.parse( filename );

    UpdateSession::close();

    const JsonStack& stack = reader.getStack();
    if (stack.rhas(".Cell")) return stack.as<Cell*>(".Cell");

    return NULL;
  }


// -------------------------------------------------------------------
// Function  :  Json Blob parser.

  Cell* jsonBlobParse ( string filename )
  {
    UpdateSession::open();

    JsonReader reader ( JsonWriter::DesignBlobMode );
    reader.parse( filename );

    UpdateSession::close();

    const JsonStack& stack = reader.getStack();
    if (stack.rhas(".DesignBlob")) {
      DesignBlob* blob = stack.as<DesignBlob*>(".DesignBlob");
      Cell*       cell = blob->getTopCell();
      delete blob;
      return cell;
    }

    return NULL;
  }


}  // Hurricane namespace.
