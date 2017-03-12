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


  const uint64_t  Flags::AllowDoglegReuse = (1 << 20);
  const uint64_t  Flags::DataSelf         = (1 << 21);
  const uint64_t  Flags::Nearest          = (1 << 22);
  const uint64_t  Flags::Force            = (1 << 23);
  const uint64_t  Flags::ResetCount       = (1 << 24);
  const uint64_t  Flags::WithConstraints  = (1 << 25);
  const uint64_t  Flags::MoveToLeft       = (1 << 26);
  const uint64_t  Flags::MoveToRight      = (1 << 27);
  const uint64_t  Flags::LoadingStage     = (1 << 28);
  const uint64_t  Flags::SlowMotion       = (1 << 29);
  const uint64_t  Flags::PreRoutedStage   = (1 << 30);
  const uint64_t  Flags::SymmetricStage   = (1 << 31);


}  // Anabatic namespace.
