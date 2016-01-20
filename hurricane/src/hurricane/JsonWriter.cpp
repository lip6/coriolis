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
// |  C++ Module  :  "./JsonWriter.cpp"                              |
// +-----------------------------------------------------------------+


//#include "rapidjson/filewritestream.h"
#include "rapidjson/prettywriter.h"
#include "hurricane/Commons.h"
#include "hurricane/FileWriteGzStream.h"


//namespace Hurricane {

//typedef  rapidjson::PrettyWriter<rapidjson::FileWriteStream>  JsonOfstream;
  typedef  rapidjson::PrettyWriter<Hurricane::FileWriteGzStream>  JsonOfstream;

#define  _WRITER  reinterpret_cast<JsonOfstream*>(_writer)


// -------------------------------------------------------------------
// Class  :  "JsonWriter".

  JsonWriter::JsonWriter ( std::string fileName )
    : _flags     (0)
    , _bufferSize(65536)
    , _buffer    (new char [_bufferSize])
    , _file      (NULL)
    , _stream    (NULL)
    , _writer    (NULL)
  {
    _file   = fopen( fileName.c_str(), "w" );
  //_stream = new rapidjson::FileWriteGzStream ( _file, _buffer, _bufferSize );
    _stream = new Hurricane::FileWriteGzStream ( _file, _buffer, _bufferSize );
    _writer = new JsonOfstream ( *_stream );
    _WRITER->SetIndent( ' ', 2 );
  }


  JsonWriter::~JsonWriter ()
  { close(); }


  void  JsonWriter::close ()
  {
    if (_writer) { delete _WRITER; _writer = NULL; }
    if (_stream) { delete _stream; _stream = NULL; }
    if (_file  ) { fclose(_file);  _file   = NULL; }
  }


  void           JsonWriter::key        ( const char* k )            { _WRITER->Key(k); };
  void           JsonWriter::key        ( std::string k )            { _WRITER->Key(k.c_str()); };
  void           JsonWriter::startObject()                           { _WRITER->StartObject(); };
  void           JsonWriter::startArray ()                           { _WRITER->StartArray(); };
  void           JsonWriter::endObject  ()                           { _WRITER->EndObject(); };
  void           JsonWriter::endArray   ()                           { _WRITER->EndArray(); };
  void           JsonWriter::write      ()                           { _WRITER->Null(); };
  void           JsonWriter::write      ( const char* s )            { _WRITER->String(s); };
  void           JsonWriter::write      ( const std::string* s )     { _WRITER->String(s->c_str()); };
  void           JsonWriter::write      (       std::string  s )     { _WRITER->String(s.c_str()); };
  void           JsonWriter::write      ( const bool* v )            { _WRITER->Bool  (*v); }
  void           JsonWriter::write      (       bool  v )            { _WRITER->Bool  ( v); }
  void           JsonWriter::write      ( const int* v )             { _WRITER->Int   (*v); }
  void           JsonWriter::write      (       int  v )             { _WRITER->Int   ( v); }
  void           JsonWriter::write      ( const long* v )            { _WRITER->Int64 (*v); }
  void           JsonWriter::write      (       long  v )            { _WRITER->Int64 ( v); }
  void           JsonWriter::write      ( const unsigned int* v )    { _WRITER->Uint  (*v); }
  void           JsonWriter::write      (       unsigned int  v )    { _WRITER->Uint  ( v); }
  void           JsonWriter::write      ( const unsigned long* v )   { _WRITER->Uint64(*v); }
  void           JsonWriter::write      (       unsigned long  v )   { _WRITER->Uint64( v); }
  JsonWriter*    JsonWriter::setFlags   ( unsigned long mask ) { _flags |= mask; return this; }
  JsonWriter*    JsonWriter::resetFlags ( unsigned long mask ) { _flags &= ~mask; return this; }
  bool           JsonWriter::issetFlags ( unsigned long mask ) const { return _flags & mask; }
  unsigned long  JsonWriter::getFlags   () const { return _flags; }



//}  // Hurricane namespace.
