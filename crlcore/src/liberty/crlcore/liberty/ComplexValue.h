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
// |  C++ Module  :  "./ComplexValue.h"                              |
// +-----------------------------------------------------------------+

#pragma once
#include "Value.h"
#include "ValueString.h"
#include <string>
#include <vector>

namespace Liberty {

  /**
    * A complex value is a list of values, separated by comas. They are
    * found in complex attributes.
    * ```
    * attribute_name (parameter1, [parameter2, parameter3]);
    * ```
    * */
  class ComplexValue : public Value {
    public:
      ComplexValue();
      ~ComplexValue();

      std::string getAsString ()                          const override;
      void        set         (const std::string &value)        override;
      void        set         (const std::string_view &value)           ;
      void        clear       ()                                        ;
    private:
      std::vector<ValueString*> _values;
  };

}
