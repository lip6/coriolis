
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2008, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./Hierarchy.cpp"                          |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef __CRL_HIERARCHY__
#define __CRL_HIERARCHY__

#include  "hurricane/Occurrences.h"

namespace Hurricane {
  class Cell;
}


namespace CRL {

  using Hurricane::Cell;
  using Hurricane::Occurrences;


  Occurrences  getAllUniqueCellOccurrencesFrom     ( Cell* cell );
    
  void         createRoutingPadsRing               ( Cell* cell );
  void         createRoutingPadsAndPinsRing        ( Cell* cell );
  void         createPlacedRoutingPadsAndPinsRing  ( Cell* cell );


} // End of CRL namespace.


#endif  // __CRL_HIERARCHY__

