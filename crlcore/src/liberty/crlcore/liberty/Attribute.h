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

namespace Liberty {

  class Library;

  /**
    * An attribute is a statement used to define characteristics
    * of objects in the library.
    * */
  class Attribute : public Statement {
    public:
      Attribute(Group *parent);
      ~Attribute();

      inline  bool      isAttribute ()              const override;
      inline  Value    *getValue    ()              const         ;
              void      setValue    (Value *value)                ;
    private:
      Value *_value;
  };

  inline  bool    Attribute::isAttribute()  const { return true;    }
  inline  Value  *Attribute::getValue()     const { return _value;  }

}
