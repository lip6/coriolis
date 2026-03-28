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
// |  C++ Module  :  "./Statement.cpp"                               |
// +-----------------------------------------------------------------+

#include <iostream>
#include "hurricane/liberty/Attribute.h"
#include "hurricane/liberty/Define.h"
#include "hurricane/liberty/Group.h"
#include "hurricane/liberty/Statement.h"
#include "hurricane/liberty/Library.h"

namespace Liberty {

  Group  *Statement::getAsGroup()
  {
    if (!isGroup()) {
      std::cerr << "[ERROR] Statement " << getName()
        << " is not a group, and therefor can not be returned as one." << std::endl;
      return nullptr;
    }
    return dynamic_cast<Group*>(this);
  }

  Attribute  *Statement::getAsAttribute()
  {
    if (!isAttribute()) {
      std::cerr << "[ERROR] Statement " << getName()
        << " is not an attribute, and therefor can not be returned as one." << std::endl;
      return nullptr;
    }
    return dynamic_cast<Attribute*>(this);
  }

  Define  *Statement::getAsDefine()
  {
    if (!isDefine()) {
      std::cerr << "[ERROR] Statement " << getName()
        << " is not a define, and therefor can not be returned as one." << std::endl;
      return nullptr;
    }
    return dynamic_cast<Define*>(this);
  }

  Library *Statement::getLibrary() {
    if (_parent == nullptr)
      return dynamic_cast<Library*>(this);
    Group *g = _parent;
    while (g->getParent())
      g = g->getParent();
    return dynamic_cast<Library *>(g);
  }

}
