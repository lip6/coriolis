// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Uiversité 2013-2026, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                              Hippolyte MELICA    |
// |  E-mail      :   hippolyte.melica@etu.sorbonne-universite.fr    |
// | =============================================================== |
// |  C++ Module  :  "./Reader.h"                                    |
// +-----------------------------------------------------------------+

#pragma once
#include <cstddef>

namespace Liberty {

  /**
   *  Basic character by character file reader. Uses mmap to handle large files rapidly.
   * */
  class Reader {
    public:
      Reader();
      ~Reader();

             bool open(const  char  *filename);
      inline bool next(       char *&c       );

    private:
      int     _fd;
      size_t  _size;
      char   *_current;
      char   *_end;
      void   *_data;
  };

  inline bool Reader::next(char *&c)
  {
    if (_current < _end) {
      c = _current++;
      return true;
    }
    return false;
  }

}
