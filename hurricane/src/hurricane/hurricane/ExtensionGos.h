
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
// |  C++ Header  :       "./ExtensionGos.h"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef __HURRICANE_EXTENSION_GOS__
#define __HURRICANE_EXTENSION_GOS__


#include  "hurricane/Collection.h"


namespace Hurricane {


  class ExtensionGo;


  typedef GenericCollection<ExtensionGo*> ExtensionGos;
  typedef GenericLocator<ExtensionGo*>    ExtensionGoLocator;
  typedef GenericFilter<ExtensionGo*>     ExtensionGoFilter;


} // End of Hurricane namespace.


#endif  // __HURRICANE_EXTENSION_GOS__
