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

#include <algorithm>
#include "hurricane/liberty/Group.h"
#include "hurricane/liberty/Attribute.h"

namespace Liberty {

  Group::Group(Group *parent) : Statement(parent) {}

  Group::~Group()
  {
    for (auto statement : _statements)
      delete statement;
  }

  Group *Group::getGroup(const std::string &group_name, const std::string &name) const
  {
    Statement *ret;
    ret = *std::find_if(_statements.begin(), _statements.end(), [&](Group *item){
      if (item->isGroup())
        return false;
      Group *g = item->getAsGroup();
      return g->getName() == name && g->getGroupName() == group_name;
    });
    return ret->getAsGroup();
  }

  Attribute *Group::getAttribute(const std::string &attribute_name) const
  {
    Statement *ret;
    ret = *std::find_if(_statements.begin(), _statements.end(), [&](Group *item){
      if (item->isAttribute())
        return false;
      Attribute *a = item->getAsAttribute();
      return a->getName() == attribute_name;
    });
    return ret->getAsAttribute();
  }

}
