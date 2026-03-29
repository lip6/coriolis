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
// |  C++ Module  :  "./Attribute.cpp"                               |
// +-----------------------------------------------------------------+

#include "hurricane/liberty/ValueString.h"
#include "hurricane/liberty/Attribute.h"

namespace Liberty {

  Attribute::Attribute(Group *parent): Statement(parent), _value(nullptr) {}

  Attribute::~Attribute()
  {
    if (_value)
      delete _value;
  }

  void Attribute::setValue(Value *value)
  {
    // logic to set value type should be added here
    // maybe add a type argument with default value to string
    if (_value)
      delete _value;
    _value = value;
  }

}
