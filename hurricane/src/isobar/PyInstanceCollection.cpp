#include "hurricane/isobar/PyCellCollection.h"
#include "hurricane/isobar/PyCell.h"

namespace  Isobar {

using namespace Hurricane;


extern "C" {


// x=================================================================x
// |             "PyCellCollection" Python Module Code Part           |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |                 "PyCellCollection" Attribute Methods         |
  // x-------------------------------------------------------------x


  DirectDeleteMethod(PyCellCollection_DeAlloc, PyCellCollection)


  LocatorNextMethod(Cell)
  CollectionMethods(Cell)




#else  // End of Python Module Code Part.


// x=================================================================x
// |            "PyCellCollection" Shared Library Code Part           |
// x=================================================================x


  // ---------------------------------------------------------------
  // PyCellCollection Object Definitions.
  
  PyTypeCollectionObjectDefinitions(CellCollection)
  PyTypeCollectionObjectDefinitions(CellCollectionLocator)

#endif  // End of Shared Library Code Part.


}  // End of extern "C".

}  // End of Isobar namespace.
