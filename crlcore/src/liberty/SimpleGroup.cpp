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
// |  C++ Module  :  "./SimpleGroup.cpp"                             |
// +-----------------------------------------------------------------+

#include "crlcore/liberty/Group.h"
#include "crlcore/liberty/Library.h"
#include "crlcore/liberty/SimpleGroup.h"
#include <string>

namespace Liberty {

  SimpleGroup::SimpleGroup(Group *parent): Group(parent) {}

  SimpleGroup::~SimpleGroup() {}

  void SimpleGroup::setGroupIdentifier(std::string group_id)
  {
    _group_identifier = group_id;
    if (getName() == "cell")
      getLibrary()->addCellGroup(_group_identifier, this);
  }


}
