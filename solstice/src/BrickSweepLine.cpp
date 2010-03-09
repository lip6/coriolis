// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2009, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                      C O R I O L I S                            |
// |          S O L S T I C E   -   C o m p a r a t o r              |
// |                                                                 |
// |  Author      :                             Wu Yife              |
// |  E-mail      :                    Wu.Yifei@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./BrickSweepLine.cpp"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x

#include <hurricane/ExtensionGo.h>
#include <hurricane/DataBase.h>
#include <hurricane/Technology.h>
#include <hurricane/Occurrence.h>

#include <hurricane/Component.h>

#include <equinox/Strategy.h>
#include <equinox/IntervalTree.h>
#include <equinox/Tiles.h>
#include <equinox/SweepLine.h>
#include <equinox/TileSweepLine.h>
#include <solstice/RoutingError.h>
#include "solstice/SolsticeEngine.h"
#include "solstice/Brick.h"
namespace Equinox {

  using Solstice::SolsticeEngine;
  using Solstice::Brick;

 template class  SweepLine<Brick*,SolsticeEngine*>;
}
