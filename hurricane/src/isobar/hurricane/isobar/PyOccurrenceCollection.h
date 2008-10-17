#ifndef __PYOCCURRENCECOLLECTION__
#define __PYOCCURRENCECOLLECTION__


#include "hurricane/isobar/PyHurricane.h"

#include "hurricane/Occurrence.h"
#include "hurricane/Occurrences.h"


namespace Isobar {


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyOccurrenceCollection".

  typedef struct {
      PyObject_HEAD
      Hurricane::Occurrences* _object;
  } PyOccurrenceCollection;

  typedef struct {
      PyObject_HEAD
      Hurricane::Locator<Hurricane::Occurrence>* _object;
      PyOccurrenceCollection* _collection;
  } PyOccurrenceCollectionLocator;

// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

  extern PyTypeObject PyTypeOccurrenceCollection;
  extern PyTypeObject PyTypeOccurrenceCollectionLocator;

  extern void PyOccurrenceCollection_LinkPyType();
  extern void PyOccurrenceCollectionLocator_LinkPyType();

}  // End of extern "C".

}  // End of Isobar namespace.
 
#endif
