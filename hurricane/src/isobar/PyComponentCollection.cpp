#include "hurricane/isobar/PyComponentCollection.h"
#include "hurricane/isobar/PyComponent.h"

namespace  Isobar {

using namespace Hurricane;


extern "C" {


// x=================================================================x
// |             "PyComponentCollection" Python Module Code Part           |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |                 "PyComponentCollection" Attribute Methods         |
  // x-------------------------------------------------------------x


  DirectDeleteMethod(PyComponentCollection_DeAlloc, PyComponentCollection)


  EntityLocatorNextMethod(Component)
  CollectionMethods(Component)




#else  // End of Python Module Code Part.


// x=================================================================x
// |            "PyComponentCollection" Shared Library Code Part           |
// x=================================================================x


  // ---------------------------------------------------------------
  // PyComponentCollection Object Definitions.
  
  PyTypeCollectionObjectDefinitions(ComponentCollection)
  PyTypeCollectionObjectDefinitions(ComponentCollectionLocator)

#endif  // End of Shared Library Code Part.


}  // End of extern "C".

}  // End of Isobar namespace.
