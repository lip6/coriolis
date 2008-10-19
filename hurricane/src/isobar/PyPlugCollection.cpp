#include "hurricane/isobar/PyPlugCollection.h"
#include "hurricane/isobar/PyPlug.h"

namespace  Isobar {

using namespace Hurricane;


extern "C" {


// x=================================================================x
// |             "PyPlugCollection" Python Module Code Part           |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |                 "PyPlugCollection" Attribute Methods         |
  // x-------------------------------------------------------------x


  DirectDeleteMethod(PyPlugCollection_DeAlloc, PyPlugCollection)


  LocatorNextMethod(Plug)
  CollectionMethods(Plug)




#else  // End of Python Module Code Part.


// x=================================================================x
// |            "PyPlugCollection" Shared Library Code Part           |
// x=================================================================x


  // ---------------------------------------------------------------
  // PyPlugCollection Object Definitions.
  
  PyTypeCollectionObjectDefinitions(PlugCollection)
  PyTypeCollectionObjectDefinitions(PlugCollectionLocator)

#endif  // End of Shared Library Code Part.


}  // End of extern "C".

}  // End of Isobar namespace.
