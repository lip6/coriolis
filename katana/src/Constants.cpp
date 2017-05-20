// -*- mode: C++; explicit-buffer-name: "Constants.cpp<katana>" -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2016, All Rights Reserved
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


  const Hurricane::BaseFlags  Flags::AllowDoglegReuse = (1 << 20);
  const Hurricane::BaseFlags  Flags::DataSelf         = (1 << 21);
  const Hurricane::BaseFlags  Flags::Nearest          = (1 << 22);
  const Hurricane::BaseFlags  Flags::Force            = (1 << 23);
  const Hurricane::BaseFlags  Flags::ResetCount       = (1 << 24);
  const Hurricane::BaseFlags  Flags::WithConstraints  = (1 << 25);
  const Hurricane::BaseFlags  Flags::MoveToLeft       = (1 << 26);
  const Hurricane::BaseFlags  Flags::MoveToRight      = (1 << 27);
  const Hurricane::BaseFlags  Flags::LoadingStage     = (1 << 28);
  const Hurricane::BaseFlags  Flags::SlowMotion       = (1 << 29);
  const Hurricane::BaseFlags  Flags::PreRoutedStage   = (1 << 30);
  const Hurricane::BaseFlags  Flags::SymmetricStage   = (1 << 31);


}  // Anabatic namespace.
