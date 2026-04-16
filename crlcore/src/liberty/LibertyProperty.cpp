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
// |  C++ Module  :  "./LibertyProperty.cpp"                         |
// +-----------------------------------------------------------------+

#include "crlcore/liberty/LibertyProperty.h"
#include "crlcore/liberty/Group.h"
#include "hurricane/Property.h"

namespace Liberty {

  using Hurricane::PrivateProperty;

  LibertyProperty::LibertyProperty(Group *cell) : PrivateProperty(), _cell(cell) {}

  LibertyProperty::~LibertyProperty() {}

}
