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
// |  C++ Module  :  "./Tokenizer.cpp"                               |
// +-----------------------------------------------------------------+

#include "hurricane/liberty/Tokenizer.h"
#include <iostream>
#include <string_view>

namespace Liberty {

  Tokenizer::Tokenizer(const std::string &filepath): _filepath(filepath), _token(), _reader(), _state(Default), _is_next(false) {}

  Tokenizer::~Tokenizer() {}

  bool Tokenizer::init() {
    return _reader.open(_filepath.c_str());
  }

  const Token &Tokenizer::_buildToken(char *begin, size_t end, TokenType type) {
    _token.str = std::string_view(begin, end);
    _token.type = type;
    return _token;
  }

  const Token &Tokenizer::_buildTokenNext(char *begin, size_t end, TokenType type) {
    _next_token.str = std::string_view(begin, end);
    _next_token.type = type;
    _is_next = true;
    return _next_token;
  }

  const Token &Tokenizer::next()
  {
    if (_is_next) {
      _is_next = false;
      return _next_token;
    }
    static const Token error {TokenType::Error, std::string_view("ERROR")};
    static const Token end {TokenType::End, std::string_view("END")};

    char  *c;
    char  *begin = 0;
    size_t size = 0;
    while (_reader.next(c)) {
      if (begin == 0)
        begin = c;
      switch (_state) {
        case Error:
          return error;
        case Default:
          switch (*c) {
            case '/':
              _state = CommentBegin;
              size++;
              break;
            case ' ':
            case '\r':
            case '\n':
            case '\t':
            case '\\':
              begin = 0;
              break;
            case '{':
            case '}':
            case '(':
            case ')':
            case '[':
            case ']':
            case ':':
            case ';':
            case ',':
              return _buildToken(c, 1, _getTokenType(*c));
            case '"':
              _state = QuotedExpression;
              begin = 0;
              break;
            default:
              _state = Expression;
              size++;
          }
          break;

        case CommentBegin:
          switch (*c) {
            case '/':
              _state = CommentLine;
              size++;
              break;
            case '*':
              _state = CommentBlock;
              size++;
              break;
            default:
              std::cerr << "[ERROR] Parsing (CommentBegin) with unexpected char " << *c << std::endl;
              return error;
          }
            break;
          case CommentLine:
            switch (*c) {
              case '\n':
                return _buildToken(begin, size, TokenType::CommentLine);
              case '\r':
                break;
              default:
                size++;
            }
            break;
          case CommentBlock:
            switch (*c) {
              case '*':
                _state = CommentBlockEnd;
                [[fallthrough]];
              default:
                size++;
                break;
            }
            break;
          case CommentBlockEnd:
            switch (*c) {
              case '/':
                _state = Default;
                return _buildToken(begin, ++size, TokenType::CommentBlock);
              default:
                _state = CommentBlock;
                size++;
            }
            break;
          case Expression:
            switch (*c) {
              case '/':
                _state = CommentBegin;
                return _buildToken(begin, size++, TokenType::Expression);
              case '{':
              case '}':
              case '(':
              case ')':
              case '[':
              case ']':
              case ':':
              case ';':
              case ',':
                _buildTokenNext(c, 1, _getTokenType(*c));
              case ' ':
              case '\r':
              case '\n':
              case '\t':
                _state = Default;
                return _buildToken(begin, size, TokenType::Expression);
              default:
                  size++;
            }
            break;
          case QuotedExpression:
            switch (*c) {
              case '"':
                _state = Default;
                return _buildToken(begin, size, TokenType::QuotedExpression);
              default:
                  size++;
            }

      }
    }
    return end;
  }


}

// // g++ -g3 main.cpp Tokenizer.cpp Reader.cpp
//
// int main (int ac, char **av)
// {
//   if (ac < 1) {
//     std::cout << "Usage: " << av[0] << " /path/to.lib" << std::endl;
//     return 1;
//   }
//   Tokenizer t(av[1]);
//   std::cout << "init: " << t.init() << std::endl;
//   Token tok;
//   do {
//     tok = t.next();
//     std::cout << "[" << TokenTypeToStr(tok.type) << "] '" << tok.str << "'" << std::endl;
//   } while (tok);
// }
