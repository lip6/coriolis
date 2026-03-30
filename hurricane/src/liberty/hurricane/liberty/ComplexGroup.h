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
// |  C++ Module  :  "./ComplexGroup.h"                              |
// +-----------------------------------------------------------------+

#pragma once
#include "Group.h"
#include <string>
#include <vector>

namespace Liberty {

  class ComplexGroup: public Group {
    public:
      ComplexGroup(Group *parent);
      ~ComplexGroup();

      inline        void                      addVariables(const std::string &variable)             ;
      inline        void                      addVariables(const std::string_view &variable)        ;
      inline const  std::vector<std::string> &getVariables()                                  const ;

    private:
      std::vector<std::string> _variables;
  };

  inline void ComplexGroup::addVariables(const std::string &variable)
  {
    _variables.push_back(variable);
  }

  inline void ComplexGroup::addVariables(const std::string_view &variable)
  {
    _variables.push_back(std::string(variable));
  }

  inline const std::vector<std::string> &ComplexGroup::getVariables() const
  {
    return _variables;
  }

}
