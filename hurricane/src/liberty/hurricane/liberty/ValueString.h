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
// |  C++ Module  :  "./ValueString.h"                               |
// +-----------------------------------------------------------------+

#pragma once
#include <string>
#include <string_view>
#include "Value.h"

namespace Liberty {

  class ValueString : public Value {
    public:
      ValueString();
      ~ValueString();

      inline  std::string getAsString ()                          const override;
      inline  void        set         (const std::string &value)        override;
      inline  void        set         (const std::string_view &value);
    private:
      std::string _value;
  };

  inline  std::string ValueString::getAsString  ()                              const { return _value;  }
  inline  void        ValueString::set          (const std::string &value)            { _value = value; }
  inline  void        ValueString::set          (const std::string_view &value)       { _value = value; }

}
