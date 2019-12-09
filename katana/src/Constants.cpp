// -*- mode: C++; explicit-buffer-name: "Constants.cpp<katana>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |     A n a b a t i c  -  Global Routing Toolbox                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./Constants.cpp"                               |
// +-----------------------------------------------------------------+


#include <string>
#include "katana/Constants.h"


namespace Katana {

// Flags for functions arguments only.           
  const Hurricane::BaseFlags  Flags::AllowDoglegReuse     = (1L << 20);
  const Hurricane::BaseFlags  Flags::DataSelf             = (1L << 21);
  const Hurricane::BaseFlags  Flags::Nearest              = (1L << 22);
  const Hurricane::BaseFlags  Flags::Force                = (1L << 23);
  const Hurricane::BaseFlags  Flags::ResetCount           = (1L << 24);
  const Hurricane::BaseFlags  Flags::WithConstraints      = (1L << 25);
  const Hurricane::BaseFlags  Flags::MoveToLeft           = (1L << 26);
  const Hurricane::BaseFlags  Flags::MoveToRight          = (1L << 27);
  const Hurricane::BaseFlags  Flags::ShortDogleg          = (1L << 28);
  const Hurricane::BaseFlags  Flags::LoadingStage         = (1L << 29);
  const Hurricane::BaseFlags  Flags::SlowMotion           = (1L << 30);
  const Hurricane::BaseFlags  Flags::PreRoutedStage       = (1L << 31);
  const Hurricane::BaseFlags  Flags::PairSymmetrics       = (1L << 32);
  const Hurricane::BaseFlags  Flags::ShowFailedGSegments  = (1L << 33);
  const Hurricane::BaseFlags  Flags::ShowOverloadedGCells = (1L << 34);
  const Hurricane::BaseFlags  Flags::ShowBloatedInstances = (1L << 35);


}  // Anabatic namespace.
