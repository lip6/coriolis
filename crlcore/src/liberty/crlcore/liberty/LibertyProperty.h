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
// |  C++ Module  :  "./LibertyProperty.h"                           |
// +-----------------------------------------------------------------+

#pragma once
#include "Group.h"
#include "hurricane/Name.h"
#include "hurricane/Property.h"
#include <string>

namespace Liberty {

  using Hurricane::PrivateProperty;

  class LibertyProperty: public PrivateProperty {
    public:
      LibertyProperty(Group *cell);
      ~LibertyProperty();

      inline Group *getLibCell() const;

      inline Hurricane::Name  getName()       const override;
      inline std::string      _getTypeName()  const override;

    protected:
      Group *_cell;
  };

  inline Group *LibertyProperty::getLibCell() const
  {
    return _cell;
  }

  inline Hurricane::Name LibertyProperty::getName() const
  {
    return Hurricane::Name("Liberty Cell Informations");
  }

  inline std::string LibertyProperty::_getTypeName() const
  {
    return "LibertyProperty";
  }
}
