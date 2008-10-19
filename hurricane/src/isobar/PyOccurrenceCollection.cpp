#include "hurricane/isobar/PyOccurrenceCollection.h"
#include "hurricane/isobar/PyOccurrence.h"

namespace  Isobar {

using namespace Hurricane;


extern "C" {


// x=================================================================x
// |             "PyOccurrenceCollection" Python Module Code Part           |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |                 "PyOccurrenceCollection" Attribute Methods         |
  // x-------------------------------------------------------------x


  DirectDeleteMethod(PyOccurrenceCollection_DeAlloc, PyOccurrenceCollection)

  static PyObject* PyOccurrenceLocatorNext(PyOccurrenceCollectionLocator* pyLocator) { 
      Locator<Occurrence>* locator = pyLocator->_object;

      HTRY
      if (locator->isValid()) {
          Occurrence occurrence = locator->getElement();
          PyOccurrence* pyOccurrence = PyObject_NEW(PyOccurrence, &PyTypeOccurrence);
          if (pyOccurrence == NULL) {
              return NULL;
          }
          pyOccurrence->_object = new Occurrence(occurrence);

          locator->progress();
          return (PyObject*)pyOccurrence;
      }
      HCATCH
      return NULL;
  }

  CollectionMethods(Occurrence)




#else  // End of Python Module Code Part.


// x=================================================================x
// |            "PyOccurrenceCollection" Shared Library Code Part           |
// x=================================================================x


  // ---------------------------------------------------------------
  // PyOccurrenceCollection Object Definitions.
  
  PyTypeCollectionObjectDefinitions(OccurrenceCollection)
  PyTypeCollectionObjectDefinitions(OccurrenceCollectionLocator)

#endif  // End of Shared Library Code Part.


}  // End of extern "C".

}  // End of Isobar namespace.
