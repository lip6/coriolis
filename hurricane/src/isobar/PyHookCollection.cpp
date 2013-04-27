
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2013-2013, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyHookCollection.cpp"                        |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyHookCollection.h"
#include "hurricane/isobar/PyHook.h"


namespace  Isobar {

using namespace Hurricane;


extern "C" {


#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |            "PyHookCollection" Python Module Code Part           |
// +=================================================================+


  DirectDeleteMethod(PyHookCollection_DeAlloc, PyHookCollection)

  NolinkLocatorNextMethod(Hook)
  CollectionMethods(Hook)


#else  // End of Python Module Code Part.

// +=================================================================+
// |           "PyHookCollection" Shared Library Code Part           |
// +=================================================================+


  PyTypeCollectionObjectDefinitions(HookCollection)
  PyTypeCollectionObjectDefinitions(HookCollectionLocator)

#endif  // End of Shared Library Code Part.


}  // End of extern "C".

}  // End of Isobar namespace.
