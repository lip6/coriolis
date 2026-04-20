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
// |  C++ Module  :  "./Value.h"                                     |
// +-----------------------------------------------------------------+

#pragma once
#include <string>

namespace Liberty {

  /**
    * A value is stored in every attribute.
    * This is the generic class, that can be used to expand parsed types.
    * */
  class Value {
    public:
      Value();
      virtual ~Value() = 0;

      virtual std::string getAsString()          const = 0;
      virtual void        set(std::string value)       = 0;
    private:
  };

}
