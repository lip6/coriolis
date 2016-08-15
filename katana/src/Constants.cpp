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


  const unsigned int  Flags::AllowDoglegReuse = (1 << 20);
  const unsigned int  Flags::DataSelf         = (1 << 21);
  const unsigned int  Flags::Nearest          = (1 << 22);
  const unsigned int  Flags::Force            = (1 << 23);
  const unsigned int  Flags::ResetCount       = (1 << 24);
  const unsigned int  Flags::WithConstraints  = (1 << 25);
  const unsigned int  Flags::MoveToLeft       = (1 << 26);
  const unsigned int  Flags::MoveToRight      = (1 << 27);
  const unsigned int  Flags::LoadingStage     = (1 << 28);
  const unsigned int  Flags::SlowMotion       = (1 << 29);
  const unsigned int  Flags::PreRoutedStage   = (1 << 30);


}  // Anabatic namespace.
