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
// |  C++ Module  :  "./FileReadGzStream.h"                          |
// +-----------------------------------------------------------------+


#include <bzlib.h>
#include "hurricane/Error.h"
#include "hurricane/FileReadGzStream.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "FileReadGzStream".

  FileReadGzStream::FileReadGzStream ( std::FILE* fp
                                     , char*      buffer
                                     , size_t     bufferSize
                                     )
    : fp_        (fp)
    , bzfp_      (NULL)
    , buffer_    (buffer)
    , bufferSize_(bufferSize)
    , bufferLast_(0)
    , current_   (buffer_)
    , readCount_ (0)
    , count_     (0)
    , eof_       (false)
    , bzerror_   (BZ_OK)
  { 
    assert(fp_        != 0);
    assert(bufferSize >= 4);

    bzfp_ = BZ2_bzReadOpen( &bzerror_, fp_, 0, 0, NULL, 0 );
    if (bzerror_ != BZ_OK) {
      cerr << Error( "FileReadGzStream::FileReadGzstream(): Unable to initialize bzip2 stream." ) << endl;
      eof_ = true;
    }

    Read();
  }


  void  FileReadGzStream::Read ()
  {
    if (current_ < bufferLast_)
      ++current_;
    else if (not eof_) {
      count_     += readCount_;
      readCount_  = BZ2_bzRead( &bzerror_, bzfp_, buffer_, bufferSize_ );
      bufferLast_ = buffer_ + readCount_ - 1;
      current_    = buffer_;
      
      if (readCount_ < bufferSize_) {
        buffer_[readCount_] = '\0';
        ++bufferLast_;
        eof_ = true;
      }

      if (bzerror_ != BZ_OK) {
        eof_ = true;
        if (bzerror_ != BZ_STREAM_END) {
          cerr << Error( "FileReadGzStream::Read(): Unexpected termination of bzip2 stream." ) << endl;
        }
        BZ2_bzReadClose ( &bzerror_, bzfp_ );
      }
    }
  }


}  // Hurricane namespace.
