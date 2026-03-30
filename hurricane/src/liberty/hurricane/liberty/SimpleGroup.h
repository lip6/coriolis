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

namespace Liberty {

  class SimpleGroup: public Group {
    public:
      SimpleGroup(Group *parent);
      ~SimpleGroup();

      inline        void          setGroupIdentifier(const std::string &group_id)             ;
      inline        void          setGroupIdentifier(const std::string_view &group_id)        ;
      inline const  std::string  &getGroupIdentifier()                                  const ;

    private:
      std::string _group_identifier;
  };

  inline void SimpleGroup::setGroupIdentifier(const std::string &group_id)
  {
    _group_identifier = group_id;
  }

  inline void SimpleGroup::setGroupIdentifier(const std::string_view &group_id)
  {
    _group_identifier = group_id;
  }

  inline const std::string &SimpleGroup::getGroupIdentifier() const
  {
    return _group_identifier;
  }

}
