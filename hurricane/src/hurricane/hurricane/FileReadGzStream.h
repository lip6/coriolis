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
// |  C++ Header  :  "./hurricane/FileReadGzStream.h"                |
// +-----------------------------------------------------------------+


#ifndef HURRICANE_FILEREADGZSTREAM_H
#define HURRICANE_FILEREADGZSTREAM_H


namespace Hurricane {

  typedef  void  BZFILE;  // Must be identical to the typedef in <bzlib.h>.


// -------------------------------------------------------------------
// Class  :  "FileReadGzStream".

  class FileReadGzStream {
    public:
      typedef char Ch;
    public:
                       FileReadGzStream ( std::FILE* fp
                                        , char*      buffer
                                        , size_t     bufferSize
                                        );
      inline Ch        Peek             () const;
      inline Ch        Take             ();
      inline size_t    Tell             () const;
      inline void      Put              ( Ch );
      inline void      Flush            ();
      inline Ch*       PutBegin         ();
      inline size_t    PutEnd           ( Ch* );
      inline const Ch* Peek4            () const;
    private:                            
             void      Read             (); 
    private:
      std::FILE* fp_;
      BZFILE*    bzfp_;
      Ch*        buffer_;
      size_t     bufferSize_;
      Ch*        bufferLast_;
      Ch*        current_;
      size_t     readCount_;
      size_t     count_;
      bool       eof_;
      int        bzerror_;
  };


  inline FileReadGzStream::Ch        FileReadGzStream::Peek     () const { return *current_; }
  inline FileReadGzStream::Ch        FileReadGzStream::Take     ()       { Ch c = *current_; Read(); return c; }
  inline size_t                      FileReadGzStream::Tell     () const { return count_ + static_cast<size_t>(current_ - buffer_); }
  inline void                        FileReadGzStream::Put      ( Ch )   { assert(false); }
  inline void                        FileReadGzStream::Flush    ()       { assert(false); } 
  inline FileReadGzStream::Ch*       FileReadGzStream::PutBegin ()       { assert(false); return 0; }
  inline size_t                      FileReadGzStream::PutEnd   ( Ch* )  { assert(false); return 0; }
  inline const FileReadGzStream::Ch* FileReadGzStream::Peek4    () const { return (current_ + 4 <= bufferLast_) ? current_ : 0; } 


}  // Hurricane namespace.

#endif  // HURRICANE_FILEREADGZSTREAM_H
