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
// |  C++ Module  :  "./Parser.cpp"                                  |
// +-----------------------------------------------------------------+

#include "hurricane/liberty/Parser.h"
#include "hurricane/liberty/AnonymousGroup.h"
#include "hurricane/liberty/ComplexGroup.h"
#include "hurricane/liberty/ComplexValue.h"
#include "hurricane/liberty/Define.h"
#include "hurricane/liberty/SimpleGroup.h"
#include "hurricane/liberty/Statement.h"
#include "hurricane/liberty/Attribute.h"
#include "hurricane/liberty/ValueString.h"
#include <iostream>
#include <queue>
#include <string>
#include <string_view>

namespace Liberty {

  Parser::Parser(const std::string &filepath): _tokenizer(filepath), _filepath(filepath) {}

  Parser::~Parser() {}

  bool Parser::parse(Library *lib)
  {
    if (not _tokenizer.init())
      return false;
    State state = Default;
    Group *current = dynamic_cast<Group*>(lib);
    std::queue<Token> waiting;

    while (1) {
      const Token &t = _tokenizer.next();
      switch (state) {
        case Default:
          switch (t.type) {
            case CommentBlock:
            case CommentLine:
            case Semi:
              break;
            case Expression:
            case QuotedExpression:
              waiting.push(t);
              state = First;
              break;
            case DefineStatement:
              state = DefineState;
              break;
            case CurlyClose:
              current = current->getParent();
              if (not current) // Out of library group.
                return true;
              break;
            case End:
              return true;
            default:
              return _print_error(t);
          }
          break;

        case First:
          switch (t.type) {
            case ParenOpen:
              state = ParenBegin; // could be a group or a complex statement
              break;
            case Colon:
              state = SimpleAttribute;
              break;
            default:
              return _print_error(t);
          }
          break;

        case SimpleAttribute:
          switch (t.type) {
            Attribute *to_add;
            ValueString *value_str;
            case Expression:
            case QuotedExpression:
              to_add = new Attribute(current);
              value_str = new ValueString;
              value_str->set(t.str);
              to_add->setName(waiting.front().str);
              waiting.pop();
              to_add->setValue(value_str);
              current->addStatement(to_add);
              state = SimpleAttributeEnd;
              break;
            default:
              return _print_error(t);
          }
          break;

          case SimpleAttributeEnd:
            switch (t.type) {
              case Semi:
                state = Default;
                break;
              default:
                return _print_error(t);
            }
            break;

          case ParenBegin:
            switch (t.type) {
              case Expression:
              case QuotedExpression:
                waiting.push(t);
                [[fallthrough]];
              case Coma:
                state = Paren;
                break;
              case ParenClose:
                // waiting.push(_copy_empty(t));
                state = ParenEnd;
                break;
              default:
                return _print_error(t);
            }
            break;

          case Paren:
            switch (t.type) {
              case Expression:
              case QuotedExpression:
                waiting.push(t);
                break;
              case Coma:
                break;
              case ParenClose:
                state = ParenEnd;
                break;
              default:
                return _print_error(t);
            }
            break;

          case DefineState:
            switch(t.type) {
              Define *define;
              case Expression:
              case QuotedExpression:
                waiting.push(t);
                break;
              case Coma:
                break;
              case ParenClose:
                if (waiting.size() != 3)
                  return _print_error(t);
                define = new Define(current);
                define->setAttributeName(waiting.front().str);
                waiting.pop();
                define->setGroupName(waiting.front().str);
                waiting.pop();
                define->setAttributeType(waiting.front().str);
                waiting.pop();
                break;
              default:
                return _print_error(t);
            }
            break;

          case ParenEnd:
            switch (t.type) {
              Attribute *attribute;
              ComplexValue *complex_value;
              case CurlyOpen: // this is a group
                // if (waiting.size() != 2  or waiting.front().type == Coma)
                //   return _print_error(t);
                if (waiting.size() == 2) {
                  SimpleGroup *sgroup = new SimpleGroup(current);
                  sgroup->setName(waiting.front().str);
                  waiting.pop();
                  sgroup->setGroupIdentifier(waiting.front().str);
                  current->addStatement(sgroup);
                  current = sgroup;
                  waiting.pop();
                } else if (waiting.size() > 2) {
                  ComplexGroup *cgroup = new ComplexGroup(current);
                  cgroup->setName(waiting.front().str);
                  waiting.pop();
                  while (not waiting.empty()) {
                    cgroup->addVariables(waiting.front().str);
                    waiting.pop();
                  }
                  current->addStatement(cgroup);
                  current = cgroup;
                } else if (waiting.size() == 1) {
                  AnonymousGroup *agroup = new AnonymousGroup(current);
                  agroup->setName(waiting.front().str);
                  current->addStatement(agroup);
                  current = agroup;
                  waiting.pop();
                } else {
                  return _print_error(t);
                }
                state = Default;
                break;
              case Semi: // this was a complex statement
                attribute = new Attribute(current);
                complex_value = new ComplexValue;
                attribute->setValue(complex_value);
                attribute->setName(waiting.front().str);
                waiting.pop();
                while (not waiting.empty()) {
                  complex_value->set(waiting.front().str);
                  waiting.pop();
                }
                current->addStatement(attribute);
                state = Default;
                break;
              default:
                return _print_error(t);
            }
            break;

      }
    }
    return true;
  }

  bool Parser::_print_error(const Token &t) {
    std::cerr << "[ERROR] While parsing file " << _filepath << " :" << std::endl
      << "unexpected token " << t.token_count << " " << TokenTypeToStr(t.type)
      << " '" << t.str << "' at " << t.line_count << ":" << t.char_count << std::endl;
    return false;
  }

  // Token Parser::_copy_empty(const Token &t) {
  //   Token empty = {TokenType::Expression, std::string_view(""), 0, 0, 0};
  //   empty.char_count = t.char_count;
  //   empty.line_count = t.line_count;
  //   // no token count as token does not exist
  //   return empty;
  // }

}
