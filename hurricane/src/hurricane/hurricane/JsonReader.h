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
// |  C++ Header  :  "./hurricane/JsonReader.h"                      |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_JSON_READER_H
#define  HURRICANE_JSON_READER_H

#include <stdio.h>
#include <string>
#include "rapidjson/filereadstream.h"
#include "rapidjson/reader.h"
#include "hurricane/Commons.h"
#include "hurricane/FileReadGzStream.h"


namespace Hurricane {

  using rapidjson::Reader;
  class HurricaneHandler;


// -------------------------------------------------------------------
// Class  :  "JsonReader".

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
      size_t            _bufferSize;
      char*             _buffer;
      FILE*             _file;
      FileReadGzStream* _stream;
      JsonStack         _stack;
      Reader            _reader;
      HurricaneHandler* _handler;
  };


  inline JsonReader*    JsonReader::setFlags   ( unsigned long mask ) { _stack.setFlags(mask); return this; }
  inline JsonReader*    JsonReader::resetFlags ( unsigned long mask ) { _stack.resetFlags(mask); return this; }
  inline bool           JsonReader::issetFlags ( unsigned long mask ) const { return _stack.issetFlags(mask); }
  inline unsigned long  JsonReader::getFlags   () const { return _stack.getFlags(); }
  inline JsonStack&     JsonReader::getStack   () { return _stack; }


}  // Hurricane namespace.

#endif  // HURRICANE_JSON_READER_H
