
// -*- C++ -*-
//
// This file is part of the Hurricane Software.
// Copyright (c) UPMC/LIP6 2008-2008, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x
// |                                                                 |
// |                  H U R R I C A N E                              |
// |     V L S I   B a c k e n d   D a t a - B a s e                 |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./ExtensionSlices.h"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef __HURRICANE_EXTENSION_SLICES__
#define __HURRICANE_EXTENSION_SLICES__


#include  "hurricane/Collection.h"


namespace Hurricane {


  class ExtensionSlice;


  typedef GenericCollection<ExtensionSlice*> ExtensionSlices;
  typedef GenericLocator<ExtensionSlice*>    ExtensionSliceLocator;
  typedef GenericFilter<ExtensionSlice*>     ExtensionSliceFilter;


} // End of Hurricane namespace.


#endif  // __HURRICANE_EXTENSION_SLICES__
