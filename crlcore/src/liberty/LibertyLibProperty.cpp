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
// |  C++ Module  :  "./LibertyLibProperty.cpp"                      |
// +-----------------------------------------------------------------+

#include "crlcore/liberty/LibertyLibProperty.h"
#include "crlcore/liberty/Group.h"
#include "hurricane/Property.h"

namespace Liberty {

  using Hurricane::PrivateProperty;

  LibertyLibProperty::LibertyLibProperty(Library *library) : PrivateProperty(), _library(library) {}

  LibertyLibProperty::LibertyLibProperty(const Library *library) : PrivateProperty(), _library(library) {}

  LibertyLibProperty::~LibertyLibProperty() {
    delete _library;
  }

}
