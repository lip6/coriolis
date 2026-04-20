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
// |  C++ Module  :  "./Define.h"                                    |
// +-----------------------------------------------------------------+

#pragma once
#include "Statement.h"
#include <string>

namespace Liberty {

  class Library;

  /**
    * A define statement is used to add custom attributes to a group.
    * ```
    * define (attribute_name, group_name, attribute_type);
    * ```
    * */
  class Define : public Statement {
    public:
      Define(Group *parent);
      ~Define();

      inline        bool          isDefine          () const override;
      inline const  std::string  &getAttributeName  () const         ;
      inline const  std::string  &getGroupName      () const         ;
      inline const  std::string  &getAttributeType  () const         ;

      inline        void          setAttributeName  (std::string name      );
      inline        void          setAttributeType  (std::string type      );
      inline        void          setGroupName      (std::string group_name);
    private:
      std::string   _group_name     ;
      std::string   _attribute_type ; // should be specific type
  };

  inline  bool Define::isDefine() const { return true; }

  inline const  std::string  &Define::getGroupName    ()  const { return _group_name;     }
  inline const  std::string  &Define::getAttributeName()  const { return _name;           }
  inline const  std::string  &Define::getAttributeType()  const { return _attribute_type; }

  inline  void  Define::setAttributeName(std::string name       ) { _name           = name      ;}
  inline  void  Define::setAttributeType(std::string type       ) { _attribute_type = type      ;}
  inline  void  Define::setGroupName    (std::string group_name ) { _group_name     = group_name;}

}
