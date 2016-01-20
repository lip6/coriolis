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
// |  C++ Header  :  "./hurricane/JsonWriter.h"                      |
// +-----------------------------------------------------------------+


#ifndef HURRICANE_SLOT_H
#error "JsonWriter.h must be included through Commons.h"
#endif

#ifndef  HURRICANE_JSON_WRITER_H
#define  HURRICANE_JSON_WRITER_H

#include <stdio.h>
#include <string>

namespace rapidjson {
  class FileWriteStream;
}

namespace Hurricane {
  class FileWriteGzStream;
}
//namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "JsonWriter".

  class JsonWriter {
    public:
      enum Mode { UsePlugReference = (1<<0)
                , DesignBlobMode   = (1<<1)
                , CellMode         = (1<<2)
                , RegisterMode     = (1<<3)
                , RegisterStatus   = (1<<4)
                , RegisterType     = (1<<5)
                , UnregisterType   = (1<<6)
                , CellObject       = (1<<7)
                , DBoObject        = (1<<8)
                };
    public:
                    JsonWriter  ( std::string fileName );
                   ~JsonWriter  ();
      void          key         ( const char* );
      void          key         ( std::string );
      void          startObject ();
      void          startArray  ();
      void          endObject   ();
      void          endArray    ();
      void          write       ();
      void          write       ( const char* );
      void          write       (       std::string );
      void          write       ( const std::string* );
      void          write       (       bool );
      void          write       ( const bool* );
      void          write       (       int );
      void          write       ( const int* );
      void          write       (       long );
      void          write       ( const long* );
      void          write       (       unsigned int );
      void          write       ( const unsigned int* );
      void          write       (       unsigned long );
      void          write       ( const unsigned long* );
      void          close       ();
      JsonWriter*   setFlags    ( unsigned long mask );
      JsonWriter*   resetFlags  ( unsigned long mask );
      bool          issetFlags  ( unsigned long mask ) const;
      unsigned long getFlags    () const;
    private:                  
                  JsonWriter  ( const JsonWriter& );
      JsonWriter& operator=   ( const JsonWriter& ) const;
    private:
      unsigned long                 _flags;
      size_t                        _bufferSize;
      char*                         _buffer;
      FILE*                         _file;
    //rapidjson::FileWriteStream*   _stream;
      Hurricane::FileWriteGzStream* _stream;
      void*                         _writer;
  };


//}  // Hurricane namespace.


// All jsonWrite() overload are put in the top level namespace,
// in order to facilitate additions from other tools.

inline void  jsonWrite ( JsonWriter* w )                         { w->write( ); }
inline void  jsonWrite ( JsonWriter* w, const char* s )          { w->write(s); }
inline void  jsonWrite ( JsonWriter* w, const std::string* s )   { w->write(s); }
inline void  jsonWrite ( JsonWriter* w,       std::string  s )   { w->write(s); }
inline void  jsonWrite ( JsonWriter* w, const bool* v )          { w->write(v); }
inline void  jsonWrite ( JsonWriter* w,       bool  v )          { w->write(v); }
inline void  jsonWrite ( JsonWriter* w, const int* v )           { w->write(v); }
inline void  jsonWrite ( JsonWriter* w,       int  v )           { w->write(v); }
inline void  jsonWrite ( JsonWriter* w, const long* v )          { w->write(v); }
inline void  jsonWrite ( JsonWriter* w,       long  v )          { w->write(v); }
inline void  jsonWrite ( JsonWriter* w, const unsigned int* v )  { w->write(v); }
inline void  jsonWrite ( JsonWriter* w,       unsigned int  v )  { w->write(v); }
inline void  jsonWrite ( JsonWriter* w, const unsigned long* v ) { w->write(v); }
inline void  jsonWrite ( JsonWriter* w,       unsigned long  v ) { w->write(v); }


//template<typename Type>
//inline void  jsonWrite ( JsonWriter* w, const std::string& key, const Type value )
//{ w->key( key ); w->write( getString(value).c_str() ); }


inline void  jsonWrite ( JsonWriter* w, const std::string& key, const bool* value )
{ w->key( key ); w->write( value ); }


inline void  jsonWrite ( JsonWriter* w, const std::string& key, bool value )
{ w->key( key ); w->write( value ); }


inline void  jsonWrite ( JsonWriter* w, const std::string& key, const int* value )
{ w->key( key ); w->write( value ); }


inline void  jsonWrite ( JsonWriter* w, const std::string& key, int value )
{ w->key( key ); w->write( value ); }


inline void  jsonWrite ( JsonWriter* w, const std::string& key, const long* value )
{ w->key( key ); w->write( value ); }


inline void  jsonWrite ( JsonWriter* w, const std::string& key, long value )
{ w->key( key ); w->write( value ); }


inline void  jsonWrite ( JsonWriter* w, const std::string& key, const unsigned int* value )
{ w->key( key ); w->write( value ); }


inline void  jsonWrite ( JsonWriter* w, const std::string& key, unsigned int value )
{ w->key( key ); w->write( value ); }


inline void  jsonWrite ( JsonWriter* w, const std::string& key, const unsigned long* value )
{ w->key( key ); w->write( value ); }


inline void  jsonWrite ( JsonWriter* w, const std::string& key, unsigned long value )
{ w->key( key ); w->write( value ); }


inline void  jsonWrite ( JsonWriter* w, const std::string& key, const char* value )
{ w->key( key ); w->write( value ); }


inline void  jsonWrite ( JsonWriter* w, const std::string& key, const std::string& value )
{ w->key( key ); w->write( value ); }


template<typename C>
inline void  jsonWrite ( JsonWriter* w, const std::string& key, const C& )
{
  w->key( key );
  std::string message = "Unsupported type " + Hurricane::demangle(typeid(C).name());
  w->write( message.c_str() );
}


template<typename C>
inline void  jsonWrite ( JsonWriter* w, const C* object )
{
  if (Hurricane::inltrace(50))
    std::cerr << Hurricane::tab
              << "jsonWrite<" << Hurricane::demangle(typeid(C).name()) << "*>(w,object)" << std::endl;
  Hurricane::ltracein(50);

  if (object) object->toJson( w );
  else        jsonWrite(w);

  Hurricane::ltraceout(50);
}


template<typename C>
inline void  jsonWrite ( JsonWriter* w, const std::string& key, C* object )
{
  jsonWrite( w, key, const_cast<const C*>(object) );
}


template<typename C>
inline void  jsonWrite ( JsonWriter* w, const std::string& key, const C* object )
{
  if (Hurricane::inltrace(50))
    std::cerr << Hurricane::tab
              << "jsonWrite<" << Hurricane::demangle(typeid(C).name()) << "*>(w,key,object)"
              << " key:\"" << key << "\"" << std::endl;
  Hurricane::ltracein(50);

  w->key( key );
  if (object) object->toJson( w );
  else        jsonWrite(w);

  Hurricane::ltraceout(50);
}


#endif  // HURRICANE_JSON_WRITER_H
