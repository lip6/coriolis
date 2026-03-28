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
// |  C++ Module  :  "./Library.cpp"                                 |
// +-----------------------------------------------------------------+


#include <filesystem>
#include <iostream>
#include "hurricane/liberty/Library.h"
#include "hurricane/liberty/Statement.h"

namespace Liberty {

  Library::Library(const std::string &filepath):  Group(nullptr)
                                                , _path(filepath)
                                                , _cells()
  {}

  Library::~Library() {}

  void Library::load() {} // TODO: parsing here

  Group *Library::getCellGroup(const std::string &cell_name) const
  {
    return _cells.at(cell_name);
  }

  void Library::_include(const std::string &filename) {
    std::cerr << "[CRITICAL WARNING] Trying to parse include directive for " << filename << " "
      << "but that feature is not yet available. "
      << "Parts of your library are probably not parsed and will be missing." << std::endl;
  }

}
