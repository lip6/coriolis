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


#include "crlcore/liberty/Library.h"
#include "crlcore/liberty/LibertyLibProperty.h"
#include "crlcore/liberty/LibertyProperty.h"
#include "crlcore/liberty/Parser.h"
#include "crlcore/liberty/SimpleGroup.h"
#include "crlcore/liberty/Statement.h"
#include "hurricane/Cells.h"
#include "hurricane/Library.h"
#include "hurricane/DataBase.h"
#include <cstddef>
#include <filesystem>
#include <iostream>
#include <string>

namespace Liberty {

  Library::Library(const std::string &filepath):  SimpleGroup(nullptr)
                                                , _path(filepath)
                                                , _cells()
                                                , _parsed(false)
  {}

  Library::~Library() {}

  bool Library::load() {
    if (_parsed) {
      std::cerr << "[INFO] " << _path << " was already loaded." << std::endl;
      return _parsed;
    }
    Parser parser(_path.string());
    _parsed = parser.parse(this);
    return _parsed;
  }

  Group *Library::getCellGroup(const std::string &cell_name) const
  {
    auto it = _cells.find(cell_name);
    if (it != _cells.end()) {
      return it->second;
    }
    return nullptr;
  }

  void Library::_include(const std::string &filename) {
    std::cerr << "[CRITICAL WARNING] Trying to parse include directive for " << filename << " "
      << "but that feature is not yet available. "
      << "Parts of your library are probably not parsed and will be missing." << std::endl;
  }

  void Library::addCellGroup(const std::string &cell_name, Group *group) {
    auto it = _cells.find(cell_name);
    if (it != _cells.end()) {
      if (it->second)
        delete it->second;
      it->second = group;
      return;
    }
    _cells.insert({cell_name, group});
  }

  void Library::mapLibertyToDb(Hurricane::DataBase *db) const
  {
    using Hurricane::Cell;

    Hurricane::Library* root = db->getRootLibrary();
    size_t count = 0;

    for (Hurricane::Library * lib : root->getLibraries()) {
      for (Cell *cell : lib->getCells()) {
        Group *liberty_cell = getCellGroup(cell->getName()._getString());
        if (!liberty_cell) {
          std::cerr << "[WARNING] Cell " << cell->getName()._getString()<< " is not found in Liberty library "
            << _path.filename()<< std::endl;
          continue;
        }
        LibertyProperty *property = new LibertyProperty(liberty_cell);
        cell->put(property);
        count++;
      }
    }
    std::cout << "[INFO] Matched " << count << " cells of library. Added Liberty infos." << std::endl;

    // Make db own Liberty::Library to free it properly (at this point python won't do it).
    LibertyLibProperty *library_prop = new LibertyLibProperty(this);
    db->put(library_prop);
  }
}
