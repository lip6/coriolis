#include "hurricane/isobar/PySegmentCollection.h"
#include "hurricane/isobar/PySegment.h"

namespace  Isobar {

using namespace Hurricane;


extern "C" {


// x=================================================================x
// |             "PySegmentCollection" Python Module Code Part           |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |                 "PySegmentCollection" Attribute Methods         |
  // x-------------------------------------------------------------x


  DirectDeleteMethod(PySegmentCollection_DeAlloc, PySegmentCollection)


  EntityLocatorNextMethod(Segment)
  CollectionMethods(Segment)




#else  // End of Python Module Code Part.


// x=================================================================x
// |            "PySegmentCollection" Shared Library Code Part           |
// x=================================================================x


  // ---------------------------------------------------------------
  // PySegmentCollection Object Definitions.
  
  PyTypeCollectionObjectDefinitions(SegmentCollection)
  PyTypeCollectionObjectDefinitions(SegmentCollectionLocator)

#endif  // End of Shared Library Code Part.


}  // End of extern "C".

}  // End of Isobar namespace.
