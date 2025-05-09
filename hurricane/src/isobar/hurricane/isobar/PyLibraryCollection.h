
#pragma  once
#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/Library.h"
#include "hurricane/Libraries.h"


namespace Isobar {


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyLibraryCollection".

  typedef struct {
      PyObject_HEAD
      Hurricane::Libraries* _object;
  } PyLibraryCollection;

  typedef struct {
      PyObject_HEAD
      Hurricane::Locator<Hurricane::Library*>* _object;
      PyLibraryCollection* _collection;
  } PyLibraryCollectionLocator;

// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

  extern PyTypeObject PyTypeLibraryCollection;
  extern PyTypeObject PyTypeLibraryCollectionLocator;

  extern void PyLibraryCollection_LinkPyType();
  extern void PyLibraryCollectionLocator_LinkPyType();

}  // End of extern "C".

}  // End of Isobar namespace.
