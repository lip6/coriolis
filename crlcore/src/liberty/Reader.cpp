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
// |  C++ Module  :  "./Reader.cpp"                                  |
// +-----------------------------------------------------------------+

#include "crlcore/liberty/Reader.h"
#include <cstddef>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

namespace Liberty {

  Reader::Reader(): _fd(-1), _size(0), _data(MAP_FAILED) {}

  Reader::~Reader()
  {
    if (_data != MAP_FAILED)
      munmap(_data, _size);
    if (_fd != -1)
      close(_fd);
  }

  bool Reader::open(const char *filename)
  {
    _fd = ::open(filename, O_RDONLY);
    if (_fd == -1) {
      std::cerr << "[ERROR] Could not open file " << filename << std::endl;
      return false;
    }

    struct stat sb;
    if (fstat(_fd, &sb) == -1) {
      close(_fd);
      std::cerr << "[ERROR] Could not get size of " << filename << std::endl;
      _fd = -1;
      return false;
    }
    _size = static_cast<size_t>(sb.st_size);

    _data = mmap(nullptr, _size, PROT_READ, MAP_PRIVATE, _fd, 0);
    if (_data == MAP_FAILED) {
      close(_fd);
      std::cerr << "[ERROR] Could not map " << filename << std::endl;
      _fd   = -1;
      _size =  0;
      return false;
    }
    _current  = static_cast<char *>(_data);
    _end      = _current + _size;
    return true;
  }

}
