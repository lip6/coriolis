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
// |  C++ Header  :       "./BrickSweepLine.h"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x

#ifndef _SOLSTICE_BRICKSWEEPLINE_H
#define _SOLSTICE_BRICKSWEEPLINE_H

#include "equinox/SweepLine.h"
namespace Equinox {

  template<typename ITEM,typename ENGINE>
  class SweepLine;

}

namespace Solstice {

  class Brick;
  class SolsticeEngine;


  typedef  Equinox::SweepLine<Brick*,SolsticeEngine*> BrickSweepLine;

}

#endif  //_SOLSTICE_BRICKSWEEPLINE_H
