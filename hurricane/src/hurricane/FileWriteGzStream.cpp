//  -*- mode: C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./FileWriteGzStream.h"                         |
// +-----------------------------------------------------------------+


#include <bzlib.h>
#include "hurricane/Error.h"
#include "hurricane/FileWriteGzStream.h"


namespace Hurricane {


// -------------------------------------------------------------------
// Class  :  "FileWriteGzStream".

  FileWriteGzStream::FileWriteGzStream ( std::FILE* fp
                                       , char*      buffer
                                       , size_t     bufferSize
                                       )
    : fp_       (fp)
    , bzfp_     (NULL)
    , buffer_   (buffer)
    , bufferEnd_(buffer + bufferSize), current_(buffer_)
    , bzerror_  (BZ_OK)
  { 
    assert(fp_ != 0);

    bzfp_ = BZ2_bzWriteOpen( &bzerror_, fp_, 9, 0, 0 );
    if (bzerror_ != BZ_OK) {
      cerr << Error( "FileWriteGzStream::FileWriteGzstream(): Unable to initialize bzip2 stream." ) << endl;
    }
  }


  FileWriteGzStream::~FileWriteGzStream ()
  {
    if (not bzfp_) return;

    unsigned int  bytesin  = 0;
    unsigned int  bytesout = 0;
    
    BZ2_bzWriteClose( &bzerror_, bzfp_, 0, &bytesin, &bytesout );
    if (bzerror_ != BZ_OK) {
      cerr << Error( "FileWriteGzStream::~FileWritegzstream(): I/O error while closing stream." ) << endl;
    }
  }


  void FileWriteGzStream::Flush ()
  {
    if (current_ != buffer_) {
      BZ2_bzWrite( &bzerror_,bzfp_, buffer_, static_cast<int>(current_ - buffer_) );
      if (bzerror_ != BZ_OK) {
        unsigned int  bytesin  = 0;
        unsigned int  bytesout = 0;

        BZ2_bzWriteClose( &bzerror_, bzfp_, 0, &bytesin, &bytesout );
        if (bzerror_ != BZ_OK) {
          cerr << Error( "FileWriteGzStream::Write(): I/O error in bzip2 stream." ) << endl;
        }
      }
      current_ = buffer_;
    }
  }


}  // Hurricane namespace.
