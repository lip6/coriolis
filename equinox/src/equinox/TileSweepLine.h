
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
// | =============================================================== |
// |  C++ Header  :       "./TileSweepLine.h"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x

#ifndef _EQUINOX_TILESWEEPLINE_H
#define _EQUINOX_TILESWEEPLINE_H

#include <equinox/SweepLine.h>

namespace Equinox {

  class Tile;
  class EquinoxEngine;
  template<typename ITEM,typename ENGINE>
  class SweepLine;

  typedef  SweepLine<Tile*,EquinoxEngine*> TileSweepLine;

}



#endif  // _EQUINOX_TILESWEEPLINE_H
