#include "hurricane/isobar/PyInstanceCollection.h"
#include "hurricane/isobar/PyInstance.h"

namespace  Isobar {

using namespace Hurricane;


extern "C" {


// x=================================================================x
// |             "PyInstanceCollection" Python Module Code Part           |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |                 "PyInstanceCollection" Attribute Methods         |
  // x-------------------------------------------------------------x


  DirectDeleteMethod(PyInstanceCollection_DeAlloc, PyInstanceCollection)


  LocatorNextMethod(Instance)
  CollectionMethods(Instance)




#else  // End of Python Module Code Part.


// x=================================================================x
// |            "PyInstanceCollection" Shared Library Code Part           |
// x=================================================================x


  // ---------------------------------------------------------------
  // PyInstanceCollection Object Definitions.
  
  PyTypeCollectionObjectDefinitions(InstanceCollection)
  PyTypeCollectionObjectDefinitions(InstanceCollectionLocator)

#endif  // End of Shared Library Code Part.


}  // End of extern "C".

}  // End of Isobar namespace.
