
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
// |            E q u i n o x   -   E x t r a c t o r                |
// |                                                                 |
// |  Author      :                             Wu Yife              |
// |  E-mail      :                    Wu.Yifei@lip6.fr              |
// |                                                                 |
// |  Updater     :                         Bodin bruno              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./TileSweepLine.cpp"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x

#include <hurricane/ExtensionGo.h>
#include <hurricane/DataBase.h>
#include <hurricane/Technology.h>

#include <crlcore/Utilities.h>
#include <crlcore/ToolEngine.h>

#include <equinox/Strategy.h>
#include <equinox/IntervalTree.h>
#include <equinox/Tile.h>
#include <equinox/Tiles.h>
#include <equinox/SweepLine.h>
#include <equinox/EquinoxEngine.h>
#include <equinox/TileSweepLine.h>

namespace Equinox {

  template class  SweepLine<Tile*,EquinoxEngine*>;

}
