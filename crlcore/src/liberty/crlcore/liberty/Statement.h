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
// |  C++ Module  :  "./Statement.h"                                 |
// +-----------------------------------------------------------------+

#pragma once
#include <string>

namespace Liberty {

  class Attribute;
  class Define;
  class Group;
  class Library;

  /**
   *  A statement is a base building block of a Liberty Library.
   * */
  class Statement {
    public:
      Statement(Group *parent);
      virtual            ~Statement() = 0;

      inline const    std::string  &getName       () const ;
                      Library      *getLibrary    ()       ;
      inline          Group        *getParent     () const ;
      inline virtual  bool          isGroup       () const ;
      inline virtual  bool          isAttribute   () const ;
      inline virtual  bool          isDefine      () const ;
                      Group        *getAsGroup    ()       ;
                      Attribute    *getAsAttribute()       ;
                      Define       *getAsDefine   ()       ;

      inline virtual void setName(const std::string      &name);
      inline virtual void setName(      std::string_view &name);

    protected:
      Group      *_parent;
      std::string _name;
  };

  inline const  std::string  &Statement::getName      ()  const { return _name;   }
  inline        Group        *Statement::getParent    ()  const { return _parent; }
  inline        bool          Statement::isGroup      ()  const { return false;   }
  inline        bool          Statement::isAttribute  ()  const { return false;   }
  inline        bool          Statement::isDefine     ()  const { return false;   }

  inline void Statement::setName( const std::string &name ) {
    _name = name;
  }

  inline void Statement::setName( std::string_view &name ) {
    _name = name;
  }

}
