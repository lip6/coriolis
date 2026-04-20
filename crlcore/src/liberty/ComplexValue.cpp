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
// |  C++ Module  :  "./ComplexValue.cpp"                            |
// +-----------------------------------------------------------------+

#include "crlcore/liberty/ComplexValue.h"
#include "crlcore/liberty/Value.h"
#include "crlcore/liberty/ValueString.h"
#include <string>

namespace Liberty {

  ComplexValue::ComplexValue(): Value() {}

  ComplexValue::~ComplexValue() {
    clear();
  }

  void ComplexValue::set(std::string value)
  {
    _values.push_back(new ValueString());
    _values.back()->set(value);
  }

  void ComplexValue::clear()
  {
    for (auto value : _values)
      delete value;
    _values.clear();
  }

  std::string ComplexValue::getAsString() const {
    std::string ret;
    for (auto value: _values) {
      if (not ret.empty())
        ret += ", ";
      ret += value->getAsString();
    }
    return ret;
  }

}
