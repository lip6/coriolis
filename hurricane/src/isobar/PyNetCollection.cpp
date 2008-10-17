#include "hurricane/isobar/PyNetCollection.h"
#include "hurricane/isobar/PyNet.h"

namespace  Isobar {

using namespace Hurricane;


extern "C" {


// x=================================================================x
// |             "PyNetCollection" Python Module Code Part           |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |                 "PyNetCollection" Attribute Methods         |
  // x-------------------------------------------------------------x


  DirectDeleteMethod(PyNetCollection_DeAlloc, PyNetCollection)


  LocatorNextMethod(Net)
  CollectionMethods(Net)




#else  // End of Python Module Code Part.


// x=================================================================x
// |            "PyNetCollection" Shared Library Code Part           |
// x=================================================================x


  // ---------------------------------------------------------------
  // PyNetCollection Object Definitions.
  
  PyTypeCollectionObjectDefinitions(NetCollection)
  PyTypeCollectionObjectDefinitions(NetCollectionLocator)

#endif  // End of Shared Library Code Part.


}  // End of extern "C".

}  // End of Isobar namespace.
