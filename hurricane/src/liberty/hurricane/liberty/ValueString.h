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
#include "Value.h"

namespace Liberty {

  class ValueString : public Value {
    public:
      ValueString();
      ~ValueString();

      inline const  std::string  &getAsString ()                          const override;
                    void          set         (const std::string &value)        override;
      inline        void          setHadQuotes(bool had_quotes = true)                  ;
      inline        bool          hadQuotes   ()                          const         ;
    private:
      std::string _value      ;
      bool        _had_quotes ; // default false
  };

  inline const  std::string  &ValueString::getAsString  ()                const { return _value;            }
  inline        bool          ValueString::hadQuotes    ()                const { return _had_quotes;       }
  inline        void          ValueString::setHadQuotes (bool had_quotes)       { _had_quotes = had_quotes; }

}
