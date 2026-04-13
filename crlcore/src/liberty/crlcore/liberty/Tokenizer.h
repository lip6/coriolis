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
// |  C++ Module  :  "./Tokenizer.h"                                 |
// +-----------------------------------------------------------------+

#pragma once
#include <string>
#include <string_view>
#include "Reader.h"

namespace Liberty {

  enum TokenType {
    Error = -1,
    CommentLine,
    CommentBlock,
    Expression,
    QuotedExpression,
    BracketOpen,
    BracketClose,
    ParenOpen,
    ParenClose,
    CurlyOpen,
    CurlyClose,
    Semi,
    Colon,
    Coma,
    End,
    DefineStatement,
  };

  inline const std::string TokenTypeToStr(TokenType type);

  /**
    * A token used to parse Liberty file.
    * */
  struct Token {
      TokenType         type;
      std::string_view  str;
      size_t            line_count;
      size_t            char_count;
      size_t            token_count;

      operator bool() const {
        return !(type == Error || type == End);
      }
  };

  /**
    * Simple tokenizer to build token for Liberty files.
    * */
  class Tokenizer {
    public:
      Tokenizer (const std::string &filepath);
      ~Tokenizer ();

            bool   init ();
      const Token& next ();
    private:
      enum State {
        Error = -1,
        Default,
        CommentBegin,
        CommentLine,
        CommentBlock,
        CommentBlockEnd,
        Expression,
        QuotedExpression,
        Define,
      };
    private:
      const   Token      &_buildToken (char* begin, size_t end, TokenType type);
      const   Token      &_buildTokenNext (char* begin, size_t end, TokenType type);
      inline  TokenType   _getTokenType (char c);
    private:
      const std::string _filepath;
      Token             _token;
      Token             _next_token;
      bool              _is_next;
      Reader            _reader;
      State             _state;
      size_t            _line_count;
      size_t            _token_count;
      bool              _inc_line;
      char             *_line_begin;
  };

  inline TokenType Tokenizer::_getTokenType (char c)
  {
    switch (c) {
    case '{':
      return TokenType::CurlyOpen;
    case '}':
      return TokenType::CurlyClose;
    case '(':
      return TokenType::ParenOpen;
    case ')':
      return TokenType::ParenClose;
    case '[':
      return TokenType::BracketOpen;
    case ']':
      return TokenType::BracketClose;
    case ':':
      return TokenType::Colon;
    case ';':
      return TokenType::Semi;
    case ',':
      return TokenType::Coma;
    }
    return TokenType::Expression;
  }

  inline const std::string TokenTypeToStr(TokenType type) {
    switch (type) {
      case Error:
        return "Error";
      case CommentLine:
        return "CommentLine";
      case CommentBlock:
        return "CommentBlock";
      case Expression:
        return "Expression";
      case BracketOpen:
        return "BracketOpen";
      case BracketClose:
        return "BracketClose";
      case ParenOpen:
        return "ParenOpen";
      case ParenClose:
        return "ParenClose";
      case CurlyOpen:
        return "CurlyOpen";
      case CurlyClose:
        return "CurlyClose";
      case Semi:
        return "Semi";
      case Colon:
        return "Colon";
      case Coma:
        return "Coma";
      case End:
        return "End";
      case DefineStatement:
        return "DefineStatement";
      case QuotedExpression:
        return "QuotedExpression";
    }
    return std::string();
  }

}
