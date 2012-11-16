
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                       Damien DUPUIS              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyToolEngineCollection.h"                    |
// +-----------------------------------------------------------------+


#include "crlcore/PyToolEngineCollection.h"
#include "crlcore/PyToolEngine.h"


namespace  CRL {

using namespace Hurricane;
using namespace Isobar;


extern "C" {


// +=================================================================+
// |         "PyToolEngineCollection" Python Module Code Part        |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  // +-------------------------------------------------------------+
  // |             "PyToolEngineCollection" Attribute Methods      |
  // +-------------------------------------------------------------+


  DirectDeleteMethod(PyToolEngineCollection_DeAlloc, PyToolEngineCollection)
  LocatorNextMethod(ToolEngine)
  CollectionMethods(ToolEngine)


#else  // End of Python Module Code Part.


// +=================================================================+
// |        "PyToolEngineCollection" Shared Library Code Part        |
// +=================================================================+

  PyTypeCollectionObjectDefinitions(ToolEngineCollection)
  PyTypeCollectionObjectDefinitions(ToolEngineCollectionLocator)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // CRL namespace.
