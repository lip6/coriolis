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
// |  C++ Module  :  "./AnonymousGroup.h"                            |
// +-----------------------------------------------------------------+

#pragma once
#include "Group.h"
#include <string>

namespace Liberty {

  /**
    * An anonymous group is a group without a specific group name.
    * Such as :
    * ```
    *   name () {
    *     ... statements ...
    *   }
    * ```
    * */
  class AnonymousGroup: public Group {
    public:
      AnonymousGroup(Group *parent);
      ~AnonymousGroup();

      inline std::string getGroupName() const override;
  };

  inline std::string AnonymousGroup::getGroupName() const
  {
    return _name + "()";
  }

}
