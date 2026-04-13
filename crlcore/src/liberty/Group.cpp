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
// |  C++ Module  :  "./Group.cpp"                                   |
// +-----------------------------------------------------------------+

#include "crlcore/liberty/Attribute.h"
#include "crlcore/liberty/Group.h"
#include "crlcore/liberty/Library.h"
#include "crlcore/liberty/Statement.h"
#include <algorithm>
#include <boost/process/group.hpp>
#include <iostream>
#include <regex>
#include <string>
#include <string_view>
#include <vector>

namespace Liberty {

  Group::Group(Group *parent) : Statement(parent)
  {
    if (parent and parent != this)
      _library = parent->getLibrary();
    else
      _library = static_cast<Library*>(this);
    // this is a static_cast because Library is a child of Group
    // dynamic_case would not work.
  }

  Group::~Group()
  {
    for (auto statement : _statements)
      delete statement;
  }

  void Group::addStatement(Statement *statement)
  {
    if (statement->isAttribute()) {
      auto it = _attributes.find(statement->getName());
      if (it != _attributes.end()) {
        std::cout << (getParent() ? "[WARNING]" : "[INFO]") << " Attribute " << statement->getName()
          << " twice in group " << getName()
          << (getParent() ? ", getAttribute() will return incoherent results.":".") << std::endl;
        it->second = statement->getAsAttribute();
      } else
        _attributes.insert({statement->getName(), statement->getAsAttribute()});
    }
    if (statement == this)
      return; // not adding library to its own statements.
    _statements.push_back(statement);
  }

  Group *Group::getGroup(const std::string &group_name) const
  {
    auto ret = std::find_if(_statements.begin(), _statements.end(), [&](Statement *item){
      if (not item->isGroup())
        return false;
      Group *g = item->getAsGroup();
      return g->getGroupName() == group_name;
    });
    if (ret == _statements.end())
        return nullptr;
    return (*ret)->getAsGroup();
  }

  std::vector<Group *> Group::getGroups(const std::string &group_name_regex) const
  {
    std::regex rgx(group_name_regex);
    std::vector<Group *> ret;
    for (Statement *s:_statements) {
      if (not s->isGroup())
        continue;
      Group *g = s->getAsGroup();
      if (std::regex_match(g->getGroupName(), rgx))
        ret.push_back(g);
    }
    return ret;
  }

  Attribute *Group::getAttribute(const std::string &attribute_name) const
  {
    auto it = _attributes.find(attribute_name);
    if (it != _attributes.end()) {
      return it->second;
    }
    return nullptr;
  }

  Library *Group::getLibrary()
  {
    return _library;
  }

  void Group::setName(const std::string &name) {
    _name = name;
  }

  void Group::setName(std::string_view &name) {
    _name = name;
  }

}
