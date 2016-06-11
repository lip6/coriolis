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


#include "hurricane/DebugSession.h"
#include "hurricane/Error.h"
#include "hurricane/Warning.h"
#include "hurricane/Signature.h"
#include "hurricane/JsonReader.h"
#include "hurricane/Entity.h"
#include "hurricane/Property.h"


namespace Hurricane {
  
  using namespace std;
  using namespace rapidjson;


// -------------------------------------------------------------------
// Class  :  "HurricaneHandler" (declaration).

  class HurricaneHandler {
    public:
                                  HurricaneHandler ( JsonReader& );
    public:                       
             bool                 Null             ();
             bool                 Bool             ( bool );
             bool                 Int              ( int );
             bool                 Int64            ( int64_t );
             bool                 Uint             ( unsigned int );
             bool                 Uint64           ( uint64_t );
             bool                 Double           ( double );
             bool                 RawNumber        (  const char*, SizeType, bool copy );
             bool                 String           ( const char*, SizeType, bool copy );
             bool                 Key              ( const char*, SizeType, bool copy );
             bool                 StartObject      ();
             bool                 EndObject        ( SizeType );
             bool                 StartArray       ();
             bool                 EndArray         ( SizeType );
      inline bool                 isDummy          () const;
      inline bool                 doCallToData     () const;
      inline JsonStack&           stack            ();
      inline vector<JsonObject*>& objects          ();
      inline vector<JsonObject*>& objects          () const;
      inline unsigned long        flags            ();
    private:
      enum Flags { TypenameKey  = (1<<0)
                 };
    private:
      unsigned long       _state;
      string              _key;
      string              _objectName;
      JsonReader&         _reader;
  };


// -------------------------------------------------------------------
// Class  :  "HurricaneHandler" (definition).

  HurricaneHandler::HurricaneHandler ( JsonReader& reader )
    : _state     (0)
    , _key       ()
    , _objectName()
    , _reader    (reader)
  { }


  inline JsonStack&           HurricaneHandler::stack   () { return _reader.getStack(); }
  inline vector<JsonObject*>& HurricaneHandler::objects () { return _reader.getStack().jobjects(); }
  inline vector<JsonObject*>& HurricaneHandler::objects () const { return _reader.getStack().jobjects(); }
  inline unsigned long        HurricaneHandler::flags   () { return _reader.getStack().getFlags(); }


  inline bool  HurricaneHandler::isDummy () const
  { return objects().empty() or objects().back()->isDummy(); }


  inline bool  HurricaneHandler::doCallToData () const
  { return not objects().empty() and objects().back() and not objects().back()->isBound(); }


  bool  HurricaneHandler::Null ()
  {
    if (isDummy()) return true;
    stack().push_back<DBo*>( _key, (DBo*)NULL );
    _key.clear();
    return true;
  }


  bool  HurricaneHandler::Bool ( bool v )
  {
    if (isDummy()) return true;
    stack().push_back<bool>( _key, v );
    _key.clear();
    return true;
  }


  bool  HurricaneHandler::Int ( int v )
  {
    if (isDummy()) return true;
    stack().push_back<int64_t>( _key, v );
    _key.clear();
    return true;
  }


  bool  HurricaneHandler::Int64 ( int64_t v )
  {
    if (isDummy()) return true;
    stack().push_back<int64_t>( _key, v );
    _key.clear();
    return true;
  }


  bool  HurricaneHandler::Uint ( unsigned int v )
  {
    if (isDummy()) return true;
    stack().push_back<int64_t>( _key, v );
    _key.clear();
    return true;
  }


  bool  HurricaneHandler::Uint64 ( uint64_t v )
  {
    if (isDummy()) return true;
    stack().push_back<int64_t>( _key, v );
    _key.clear();
    return true;
  }


  bool  HurricaneHandler::Double ( double v )
  {
    if (isDummy()) return true;
    stack().push_back<double>( _key, v );
    _key.clear();
    return true;
  }


  bool  HurricaneHandler::RawNumber ( const char* value, SizeType, bool copy )
  {
    cerr << Warning( "HurricaneHandler::RawNumber(): Unmanaged (value=\"%s\").", value ) << endl;
    return true;
  }

  bool  HurricaneHandler::String ( const char* value, SizeType, bool copy )
  {
    if (isDummy() and not (_state & TypenameKey)) return true;
    if (_state & TypenameKey) {
      _state &= ~TypenameKey;

      if (_key != "@typename") {
        cerr << Warning("JsonReader::parse(): First field is not @typename, skipping object." ) << endl;
        _key.clear();
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
        dynamic_cast<JsonDummy*>(objects().back())->setTypeName( value );
      } else {
      // Replace the dummy object on top of the stack.
        delete objects().back();
        objects()[objects().size()-1] = object->clone( flags() );
        objects().back()->setName( _objectName );

        signature = dynamic_cast<JsonSignature*>( objects().back() );
        if (signature) {
          size_t dot = svalue.find('.');
          signature->setSubType( svalue.substr(dot+1) );
        }
      }

      cdebug_log(19,0) << "HurricaneHandler::String() [key/typename] \"" << value << "\"." << endl;
      _key.clear();
      return true;
    }

    stack().push_back<string>( _key, value );
    _key.clear();
    return true;
  }


  bool  HurricaneHandler::Key ( const char* key, SizeType, bool copy )
  {
    if (isDummy() and not (_state & TypenameKey)) return true;

    _key = key;
    if (_state & TypenameKey) return true;

  //cdebug_log(19,0) << "HurricaneHandler::Key() key:" << _key << " objects().size():" << objects().size() << endl;

    if (objects().back()) {
      if ( doCallToData() and not _key.empty() and (_key[0] != '_') ) {
      // The key is no longer a simple attribute of the object.
      // Triggers it's creation in the Json stack.
        cdebug_log(19,0) << "HurricaneHandler::key() Calling "
                       << objects().back()->getTypeName() << "::toData(JsonStack&)." << endl;
        objects().back()->toData( stack() );
      }
    }

    return true;
  }


  bool  HurricaneHandler::StartObject ()
  {
    cdebug_log(19,1) << "Hurricane::StartObject()" << endl;

    _state |= TypenameKey;
    _objectName = _key;
    objects().push_back( new JsonDummy() );
    _key.clear();
    cdebug_log(19,0) << "objects().push_back(NULL), size():" << objects().size() << "." << endl;
    cdebug_tabw(19,1);
    return true;
  }


  bool  HurricaneHandler::EndObject ( SizeType )
  {
    cdebug_tabw(19,-2);
    cdebug_log(19,0) << "HurricaneHandler::EndObject()" << endl;
    cdebug_tabw(19,1);

    _objectName.clear();
    if (not isDummy()) {
      if (doCallToData()) {
        cdebug_log(19,0) << "Calling " << objects().back()->getTypeName() << "::toData(JsonStack&)." << endl;
        objects().back()->toData( stack() );
      }
      if (stack().size() > 1) {
        if (stack()[-1].first[0] != '_') stack().pop_back();
      }
    }

    cdebug_log(19,0) << "objects().pop_back(), size():" << objects().size() << "." << endl;
    if (objects().back()->issetFlags(JsonWriter::DBoObject))
       stack().pop_back_dbo();

    delete objects().back();
    objects().pop_back();

    cdebug_tabw(19,-1);
    return true;
  }


  bool  HurricaneHandler::StartArray()
  {
    cdebug_log(19,1) << "HurricaneHandler::StartArray() key:\"" << _key << "\"." << endl;

    _objectName.clear();
    if (_key[0] != '+') {
      cerr << Warning("JsonReader::parse(): Array attributes must start by \'+\' %s.", _key.c_str() ) << endl;
      return true;
    }
    _key.clear();

    return true;
  }


  bool  HurricaneHandler::EndArray ( SizeType )
  {
    cdebug_tabw(19,-1);
    cdebug_log(19,0) << "HurricaneHandler::EndArray()" << endl;
    return true;
  }


// -------------------------------------------------------------------
// Class  :  "JsonReader" (definition).

  JsonReader::JsonReader ( unsigned long flags )
    : _bufferSize(65536)
    , _buffer    (new char [_bufferSize])
    , _file      (NULL)
    , _stream    (NULL)
    , _stack     ()
    , _reader    ()
    , _handler   (new HurricaneHandler(*this))
  {
    _stack.setFlags( flags );
  }


  JsonReader::~JsonReader ()
  {
    close();
    delete [] _buffer;
    delete    _handler;
  }


  void  JsonReader::close ()
  {
    if (_stream) { delete _stream; _stream = NULL; }
    if (_file  ) { fclose(_file);  _file   = NULL; }
  }


  void  JsonReader::parse ( string fileName )
  {
    close();

  //DebugSession::open( 19, 20 );

    fileName += ".json.bz2";
    _file     = fopen( fileName.c_str(), "r" );
    cdebug_log(19,0) << "_file:" << _file << ", _buffer:" << (void*)_buffer << endl;

    if (not _file) {
      throw Error( "JsonReader::parse(): Cannot open file \"%s\"."
                 , fileName.c_str() );
    }

    _stream = new FileReadGzStream ( _file, _buffer, _bufferSize );

    if (issetFlags(JsonWriter::DesignBlobMode))
      Entity::enableForcedIdMode();
    _reader.Parse( *_stream, *_handler );
    if (_stack.size() != 1) {
      cerr << Error( "JsonReader::parse(): Stack must contain exatly one element upon completion." ) << endl;
      _stack.print( cerr );
    }
    if (issetFlags(JsonWriter::DesignBlobMode))
      Entity::disableForcedIdMode();

  //DebugSession::close();
    close();

    SharedProperty::clearOrphaneds();
  }


}  // Hurricane namespace.
