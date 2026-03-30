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


#include "hurricane/liberty/Library.h"
#include "hurricane/liberty/Parser.h"
#include "hurricane/liberty/SimpleGroup.h"
#include "hurricane/liberty/Statement.h"
#include <filesystem>
#include <iostream>
#include <string>

namespace Liberty {

  Library::Library(const std::string &filepath):  SimpleGroup(nullptr)
                                                , _path(filepath)
                                                , _cells()
  {}

  Library::~Library() {}

  bool Library::load() {
    Parser parser(_path.string());
    if (not parser.parse(this))
      return false;
    // we need to make library top level.
    SimpleGroup *actual = dynamic_cast<SimpleGroup*>(this->_statements.front()->getAsGroup());
    if (!actual)
      return false;
    _statements.clear();
    _statements = actual->getStatements();
    this->setName(actual->getName());
    this->setGroupIdentifier(actual->getGroupIdentifier());
    actual->clear_statements();
    delete actual;
    return true;
  }

  Group *Library::getCellGroup(const std::string &cell_name) const
  {
    if (_cells.find(cell_name) == _cells.end())
      return nullptr;
    return _cells.at(cell_name);
  }

  void Library::_include(const std::string &filename) {
    std::cerr << "[CRITICAL WARNING] Trying to parse include directive for " << filename << " "
      << "but that feature is not yet available. "
      << "Parts of your library are probably not parsed and will be missing." << std::endl;
  }

  void Library::addCellGroup(const std::string &cell_name, Group *group) {
    if (_cells.find(cell_name) != _cells.end()) {
      delete _cells.at(cell_name);
    }
    _cells[cell_name] = group;
  }

}
