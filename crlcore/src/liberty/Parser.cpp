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

#include "crlcore/liberty/AnonymousGroup.h"
#include "crlcore/liberty/Attribute.h"
#include "crlcore/liberty/ComplexGroup.h"
#include "crlcore/liberty/ComplexValue.h"
#include "crlcore/liberty/Define.h"
#include "crlcore/liberty/Parser.h"
#include "crlcore/liberty/SimpleGroup.h"
#include "crlcore/liberty/Statement.h"
#include "crlcore/liberty/Tokenizer.h"
#include "crlcore/liberty/ValueString.h"
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
    bool CurrentIsLibrary = true;
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
                  SimpleGroup *sgroup;
                  if (CurrentIsLibrary) {
                    sgroup = dynamic_cast<SimpleGroup*>(current);
                    CurrentIsLibrary = false;
                  } else
                    sgroup = new SimpleGroup(current);
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

}

// // g++ -Wimplicit-fallthrough=3 -Wall -Wextra -g3 *.cpp
// // main to test
//
// #include <cstddef>
//
// using namespace Liberty;
//
// void print_group(Group *g, size_t indent = 0) {
//   std::string ind(indent, ' ');
//   if (dynamic_cast<SimpleGroup*>(g)) {
//     SimpleGroup *s = dynamic_cast<SimpleGroup*>(g);
//     std::cout << ind << s->getName() << " (" << s->getGroupIdentifier() << ") {" << std::endl;
//   } else if (dynamic_cast<ComplexGroup*>(g)) {
//     ComplexGroup *c = dynamic_cast<ComplexGroup*>(g);
//     std::cout << ind << c->getName() << " (";
//     for (auto variable: c->getVariables())
//       std::cout << variable << ", ";
//     std::cout << ") {" << std::endl;
//   } else if (dynamic_cast<AnonymousGroup*>(g)) {
//     std::cout << ind << g->getName() << " () {" << std::endl;
//   }
//   indent += 2;
//   ind = std::string(indent, ' ');
//   for (Statement *s: g->getStatements()) {
//     if (s->isGroup()) {
//       print_group(s->getAsGroup(), indent);
//     }
//     else if (s->isDefine()) {
//       Define *d = s->getAsDefine();
//       std::cout << ind << "define("<< d->getAttributeName() << ", " << d->getGroupName() << ", "
//         << d->getAttributeType() << ");" << std::endl;
//     } else if (s->isAttribute()) {
//       Attribute *a = s->getAsAttribute();
//       std::cout << ind << a->getName();
//       if (dynamic_cast<ValueString*>(a->getValue())) {
//         ValueString *v = dynamic_cast<ValueString*>(a->getValue());
//         std::cout << " : " << v->getAsString() << " ;"<< std::endl;
//       } else {
//         ComplexValue *v = dynamic_cast<ComplexValue*>(a->getValue());
//         if (not v)
//           std::cout << "ERROR STUFF" << std::endl;
//         else {
//           std::cout << "(" << v->getAsString() << ");" << std::endl;
//         }
//       }
//     }
//   }
//   indent -= 2;
//   ind = std::string(indent, ' ');
// }
//
// int main (int ac, char **av)
// {
//   if (ac < 2) {
//     std::cout << "Usage: " << av[0] << " /path/to.lib" << std::endl;
//     return 1;
//   }
//
//   Library lib(av[1]);
//   bool ret = lib.load();
//   std::cout << "Parsing returned : " << (ret ? "TRUE":"FALSE") << std::endl;
//
//   if (ret)
//     print_group(&lib);
//   return ret ? 0 : 1;
// }
