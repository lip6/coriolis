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
// |  C++ Module  :  "./LibertyLibProperty.h"                        |
// +-----------------------------------------------------------------+

#pragma once
#include "Group.h"
#include "crlcore/liberty/Library.h"
#include "hurricane/Name.h"
#include "hurricane/Property.h"
#include <string>

namespace Liberty {

  using Hurricane::PrivateProperty;

  class LibertyLibProperty: public PrivateProperty {
    public:
      LibertyLibProperty(       Library *library);
      LibertyLibProperty(const  Library *library);
      ~LibertyLibProperty();

      inline Hurricane::Name  getName()       const override;
      inline std::string      _getTypeName()  const override;

    protected:
      const Library *_library;
  };

  inline Hurricane::Name LibertyLibProperty::getName() const
  {
    return Hurricane::Name("Liberty Cell Library");
  }

  inline std::string LibertyLibProperty::_getTypeName() const
  {
    return "LibertyLibProperty";
  }
}
