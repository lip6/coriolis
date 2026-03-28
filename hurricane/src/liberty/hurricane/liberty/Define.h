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

namespace Liberty {

  class Library;

  class Define : public Statement {
    public:
      Define(Group *parent);
      ~Define();

      inline        bool          isDefine          ()                            const override;
      inline        Group        *getGroup          ()                            const         ;
      inline const  std::string  &getAttributeName  ()                            const         ;
      inline const  std::string  &getAttributeType  ()                            const         ;
      inline        void          setAttributeName  ( const std::string  &name  )               ;
      inline        void          setAttributeType  ( const std::string  &type  )               ;
      inline        void          setGroup          (       Group        *group )               ;
    private:
      Group        *_group;
      std::string   _attribute_name;
      std::string   _attribute_type; // should be specific type
  };

  inline  bool                Define::isDefine        ()  const { return true;            }
  inline  Group              *Define::getGroup        ()  const { return _group;          }
  inline const  std::string  &Define::getAttributeName()  const { return _attribute_name; }
  inline const  std::string  &Define::getAttributeType()  const { return _attribute_type; }

  inline  void  Define::setAttributeName(const std::string &name) { _attribute_name = name;   }
  inline  void  Define::setAttributeType(const std::string &type) { _attribute_type = type;   }
  inline  void  Define::setGroup        (Group *group           ) { _group          = group;  }

}
