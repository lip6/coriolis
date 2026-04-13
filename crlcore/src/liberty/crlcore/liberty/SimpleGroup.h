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
// |  C++ Module  :  "./SimpleGroup.h"                               |
// +-----------------------------------------------------------------+

#pragma once
#include "Group.h"
#include <string>
#include <string_view>

namespace Liberty {

  /**
    * A simple group is a group with one name in between the parentheses.
    * It is used to represent most objects in a library.
    * ```
    * group_name(name) {
    *   ... statements ...
    * }
    * ```
    * */
  class SimpleGroup: public Group {
    public:
      SimpleGroup(Group *parent);
      ~SimpleGroup();

                    void          setGroupIdentifier(const std::string &group_id)                     ;
                    void          setGroupIdentifier(const std::string_view &group_id)                ;
      inline const  std::string  &getGroupIdentifier()                                  const         ;
      inline        std::string   getGroupName()                                        const override;

    private:
      std::string _group_identifier;
  };

  inline const std::string &SimpleGroup::getGroupIdentifier() const
  {
    return _group_identifier;
  }

  inline std::string SimpleGroup::getGroupName() const
  {
    return _name + "(" + _group_identifier + ")";
  }

}
