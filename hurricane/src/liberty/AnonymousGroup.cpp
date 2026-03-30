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
// |  C++ Module  :  "./AnonymousGroup.cpp"                          |
// +-----------------------------------------------------------------+

#include "hurricane/liberty/AnonymousGroup.h"
#include "hurricane/liberty/Group.h"

namespace Liberty {

  AnonymousGroup::AnonymousGroup(Group *parent): Group(parent) {}

  AnonymousGroup::~AnonymousGroup() {}

}
