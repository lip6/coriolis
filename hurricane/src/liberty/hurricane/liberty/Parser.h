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
// |  C++ Module  :  "./Parser.h"                                    |
// +-----------------------------------------------------------------+

#include <string>
#include "Library.h"
#include "Tokenizer.h"

namespace Liberty {

  class Parser {
    public:
      Parser(const std::string &filepath);
      ~Parser();

      bool parse(Library *lib);

    private:
      enum State {
        Default,
        DefineState,
        First,
        Paren,
        ParenEnd,
        SimpleAttribute,
        SimpleAttributeEnd,
      };
    private:
      bool _print_error(const Token &t);
    private:
      Tokenizer _tokenizer;
      std::string _filepath;
  };

}
