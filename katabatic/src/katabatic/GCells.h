// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katabatic/GCells.h"                          |
// +-----------------------------------------------------------------+


#ifndef  KATABATIC_GCELLS_H
#define  KATABATIC_GCELLS_H

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


} // Katabatic namespace.

#endif  // KATABATIC_GCELLS_H
