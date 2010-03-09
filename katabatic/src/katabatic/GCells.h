
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
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./GCells.h"                               |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#ifndef  __KATABATIC_GCELLS__
#define  __KATABATIC_GCELLS__

#include  "hurricane/Collections.h"


namespace Katabatic {


  using  Hurricane::Locator;
  using  Hurricane::Collection;
  using  Hurricane::GenericLocator;
  using  Hurricane::GenericCollection;
  using  Hurricane::GenericFilter;


// -------------------------------------------------------------------
// Forward declarations.


  class GCell;


// -------------------------------------------------------------------
// Collections.


  typedef GenericCollection<GCell*> GCells;
  typedef GenericLocator<GCell*>    GCellLocator;
  typedef GenericFilter<GCell*>     GCellFilter;


} // End of Katabatic namespace.


#endif  // __KATABATIC_GCELLS__
