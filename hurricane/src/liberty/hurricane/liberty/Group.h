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
// |  C++ Module  :  "./Group.h"                                     |
// +-----------------------------------------------------------------+

#pragma once
#include "Statement.h"
#include <string>
#include <string_view>
#include <vector>

namespace Liberty {

  class Library;

  class Group : public Statement {
    public:
      Group(Group *parent);
      ~Group();

      inline        bool          isGroup     ()                              const override;
      inline        void          addStatement(Statement *statement)                        ;
      inline        void          setGroupName(const std::string &group_name)               ;
      inline        void          setGroupName(const std::string_view &group_name)          ;
      inline const  std::string  &getGroupName()                              const         ;
                    Library      *getLibrary  ()                                            ;

      Group     *getGroup     ( const std::string &group_name
                               ,const std::string &name           )  const;
      Attribute *getAttribute ( const std::string &attribute_name )  const;
    protected:
      // name of group is in the parent class Statement.
      std::string               _group_name;
      std::vector< Statement* > _statements;
      Library                  *_library;
  };

  inline bool Group::isGroup() const
  {
    return true;
  }

  inline void Group::addStatement(Statement *statement)
  {
    _statements.push_back(statement);
  }

  inline void Group::setGroupName(const std::string &group_name)
  {
    _group_name = group_name;
  }

  inline void Group::setGroupName(const std::string_view &group_name)
  {
    _group_name = group_name;
  }

  inline const std::string &Group::getGroupName() const
  {
    return _group_name;
  }

}
