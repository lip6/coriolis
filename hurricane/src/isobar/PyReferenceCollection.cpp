#include "hurricane/isobar/PyReferenceCollection.h"
#include "hurricane/isobar/PyReference.h"

namespace  Isobar {

using namespace Hurricane;


extern "C" {


// x=================================================================x
// |             "PyReferenceCollection" Python Module Code Part           |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |                 "PyReferenceCollection" Attribute Methods         |
  // x-------------------------------------------------------------x


  DirectDeleteMethod(PyReferenceCollection_DeAlloc, PyReferenceCollection)


  LocatorNextMethod(Reference)
  CollectionMethods(Reference)




#else  // End of Python Module Code Part.


// x=================================================================x
// |            "PyReferenceCollection" Shared Library Code Part           |
// x=================================================================x


  // ---------------------------------------------------------------
  // PyReferenceCollection Object Definitions.
  
  PyTypeCollectionObjectDefinitions(ReferenceCollection)
  PyTypeCollectionObjectDefinitions(ReferenceCollectionLocator)

#endif  // End of Shared Library Code Part.


}  // End of extern "C".

}  // End of Isobar namespace.
