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
// |  C++ Module  :  "./Attribute.h"                                 |
// +-----------------------------------------------------------------+

#pragma once
#include "Statement.h"
#include "Value.h"
#include <string>

namespace Liberty {

  class Library;

  class Attribute : public Statement {
    public:
      Attribute(Group *parent);
      ~Attribute();

      inline  bool      isAttribute ()                          const override;
      inline  Value    *getValue    ()                          const         ;
              void      setValue    (const std::string &value)                ;
    private:
      Value *_value;
  };

  inline  bool    Attribute::isAttribute()  const { return true;    }
  inline  Value  *Attribute::getValue()     const { return _value;  }

}
