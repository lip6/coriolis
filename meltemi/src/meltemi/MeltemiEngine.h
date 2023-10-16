// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU 2014-2023, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |   E t e s i a n  -  A n a l y t i c   P l a c e r               |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./meltemi/MeltemiEngine.h"                     |
// +-----------------------------------------------------------------+

#pragma once

#include "etesian/EtesianEngine.h"
#include "katana/KatanaEngine.h"


namespace Meltemi {
using Hurricane::Cell;
using Hurricane::Name;

class MeltemiEngine : public Etesian::EtesianEngine {
 public:
  typedef Etesian::EtesianEngine Super;

  static const Name &staticGetName();
  static MeltemiEngine *create(Cell *);
  static MeltemiEngine *get(const Cell *);

 private:
  MeltemiEngine(Cell *);

 protected:
  virtual void _coloquinteCallback(coloquinte::PlacementStep step) override;

 private:
  static Name _toolName;
  Katana::KatanaEngine *_katana;
};
}  // namespace Meltemi