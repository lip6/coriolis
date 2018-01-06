//  -*- mode: C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/FileWriteGzStream.h"               |
// +-----------------------------------------------------------------+


#ifndef HURRICANE_FILEWRITEGZSTREAM_H
#define HURRICANE_FILEWRITEGZSTREAM_H

#include "rapidjson/rapidjson.h"
#include "rapidjson/filewritestream.h"


namespace Hurricane {

  typedef  void  BZFILE;  // Must be identical to the typedef in <bzlib.h>.


  class FileWriteGzStream {
    public:
      typedef  char  Ch;
    public:
                                FileWriteGzStream ( std::FILE* fp, char* buffer, size_t bufferSize );
                               ~FileWriteGzStream ();
      inline void               Put               ( char c );
      inline void               PutN              ( char c, size_t n );
             void               Flush             ();
      inline char               Peek              () const;
      inline char               Take              ();
      inline size_t             Tell              () const;
      inline char*              PutBegin          ();
      inline size_t             PutEnd            ( char* );
    private:
                                FileWriteGzStream ( const FileWriteGzStream& );
             FileWriteGzStream& operator=         ( const FileWriteGzStream& );
    private:
      std::FILE* fp_;
      BZFILE*    bzfp_;
      char*      buffer_;
      char*      bufferEnd_;
      char*      current_;
      int        bzerror_;
  };


  inline  void FileWriteGzStream::Put ( char c )
  { 
    if (current_ >= bufferEnd_) Flush();
    *current_++ = c;
  }


  inline void FileWriteGzStream::PutN ( char c, size_t n )
  {
    size_t avail = static_cast<size_t>( bufferEnd_ - current_ );
    while (n > avail) {
      std::memset( current_, c, avail );
      current_ += avail;
      Flush();
      n -= avail;
      avail = static_cast<size_t>( bufferEnd_ - current_ );
    }

    if (n > 0) {
      std::memset( current_, c, n );
      current_ += n;
    }
  }


// Not implemented
  inline char   FileWriteGzStream::Peek     () const  { assert(false); return 0; }
  inline char   FileWriteGzStream::Take     ()        { assert(false); return 0; }
  inline size_t FileWriteGzStream::Tell     () const  { assert(false); return 0; }
  inline char*  FileWriteGzStream::PutBegin ()        { assert(false); return 0; }
  inline size_t FileWriteGzStream::PutEnd   ( char* ) { assert(false); return 0; }


}  // Hurricane namespace.
  

namespace rapidjson {


  template<>
  inline void  PutN ( Hurricane::FileWriteGzStream& stream, char c, size_t n ) { stream.PutN(c,n); }


}  // rapidjson namespace.

#endif  // HURRICANE_FILEWRITEGZSTREAM_H
