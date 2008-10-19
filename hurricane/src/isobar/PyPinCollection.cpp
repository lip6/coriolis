#include "hurricane/isobar/PyPinCollection.h"
#include "hurricane/isobar/PyPin.h"

namespace  Isobar {

using namespace Hurricane;


extern "C" {


// x=================================================================x
// |             "PyPinCollection" Python Module Code Part           |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |                 "PyPinCollection" Attribute Methods         |
  // x-------------------------------------------------------------x


  DirectDeleteMethod(PyPinCollection_DeAlloc, PyPinCollection)


  LocatorNextMethod(Pin)
  CollectionMethods(Pin)




#else  // End of Python Module Code Part.


// x=================================================================x
// |            "PyPinCollection" Shared Library Code Part           |
// x=================================================================x


  // ---------------------------------------------------------------
  // PyPinCollection Object Definitions.
  
  PyTypeCollectionObjectDefinitions(PinCollection)
  PyTypeCollectionObjectDefinitions(PinCollectionLocator)

#endif  // End of Shared Library Code Part.


}  // End of extern "C".

}  // End of Isobar namespace.
