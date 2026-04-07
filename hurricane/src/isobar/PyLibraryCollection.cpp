#include "hurricane/isobar/PyLibraryCollection.h"
#include "hurricane/isobar/PyLibrary.h"

namespace  Isobar {

using namespace Hurricane;


extern "C" {


// x=================================================================x
// |             "PyLibraryCollection" Python Module Code Part           |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |                 "PyLibraryCollection" Attribute Methods         |
  // x-------------------------------------------------------------x


  DirectDeleteMethod(PyLibraryCollection_DeAlloc, PyLibraryCollection)


  LocatorNextMethod(Library)
  CollectionMethods(Library)




#else  // End of Python Module Code Part.


// x=================================================================x
// |            "PyLibraryCollection" Shared Library Code Part           |
// x=================================================================x


  // ---------------------------------------------------------------
  // PyLibraryCollection Object Definitions.
  
  PyTypeCollectionObjectDefinitions(LibraryCollection)
  PyTypeCollectionObjectDefinitions(LibraryCollectionLocator)

#endif  // End of Shared Library Code Part.


}  // End of extern "C".

}  // End of Isobar namespace.
