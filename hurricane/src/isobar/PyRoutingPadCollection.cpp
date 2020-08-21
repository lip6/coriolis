
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU/LIP6 2020-2020, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                   Jean-Paul Chaput               |
// |  E-mail      :           Jean-Paul.Chaput@lip6.fr               |
// | =============================================================== |
// |  C++ Module  :       "./PyRoutingPadCollection.h"               |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyRoutingPadCollection.h"
#include "hurricane/isobar/PyRoutingPad.h"

namespace  Isobar {

  using namespace Hurricane;


  extern "C" {



#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |         "PyRoutingPadCollection" Python Module Code Part        |
// +=================================================================+

    DirectDeleteMethod(PyRoutingPadCollection_DeAlloc, PyRoutingPadCollection)
    LocatorNextMethod(RoutingPad)
    CollectionMethods(RoutingPad)


#else  // End of Python Module Code Part.

// +=================================================================+
// |        "PyRoutingPadCollection" Shared Library Code Part        |
// +=================================================================+

    PyTypeCollectionObjectDefinitions(RoutingPadCollection)
    PyTypeCollectionObjectDefinitions(RoutingPadCollectionLocator)


#endif  // End of Shared Library Code Part.

      
  }  // End of extern "C".

}  // Isobar namespace.
