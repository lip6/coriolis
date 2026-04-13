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
#include <istream>
#include <map>
#include <string>
#include <string_view>
#include <vector>

namespace Liberty {

  class Library;

  /**
    * A group is a statement which contains others statements.
    * Depending on the number of expressions in between parentheses, it can be a
    * SimpleGroup, ComplexGroup or AnonymousGroup.
    * ```
    * group_name (...)
    * {
    *   ... statements ...
    * }
    * ```
    * */
  class Group : public Statement {
    public:
      Group(Group *parent);
      ~Group();

      inline        bool      isGroup           ()                      const override;
      inline        void      addStatement      (Statement *statement)                ;
                    Library  *getLibrary        ()                                    ;
      inline        void      clear_statements  ()                                    ;

      void setName (const std::string      &name) override;
      void setName (      std::string_view &name) override;

      inline const  std::vector<Statement*> &getStatements() const;

      Group      *getGroup    ( const std::string &group_name     ) const;
      Attribute  *getAttribute( const std::string &attribute_name ) const;
    protected:
      // group_name of group is in the parent class Statement.
      std::vector< Statement* >           _statements;
      std::map<std::string, Attribute *>  _attributes;
    private:
      // NEVER access directly this pointer, use getLibrary !!!
      Library                  *_library;
  };

  inline bool Group::isGroup() const
  {
    return true;
  }

  inline void Group::addStatement(Statement *statement)
  {
    _statements.push_back(statement);
    if (statement->isAttribute())
      _attributes.at(statement->getName()) = statement->getAsAttribute();
  }

  inline const std::vector<Statement*> &Group::getStatements() const
  {
    return _statements;
  }

  inline void Group::clear_statements() {
    _statements.clear();
  }

}
