#ifndef __PYREFERENCECOLLECTION__
#define __PYREFERENCECOLLECTION__


#include "hurricane/isobar/PyHurricane.h"

#include "hurricane/Reference.h"
#include "hurricane/References.h"


namespace Isobar {


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyReferenceCollection".

  typedef struct {
      PyObject_HEAD
      Hurricane::References* _object;
  } PyReferenceCollection;

  typedef struct {
      PyObject_HEAD
      Hurricane::Locator<Hurricane::Reference*>* _object;
      PyReferenceCollection* _collection;
  } PyReferenceCollectionLocator;

// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

  extern PyTypeObject PyTypeReferenceCollection;
  extern PyTypeObject PyTypeReferenceCollectionLocator;

  extern void PyReferenceCollection_LinkPyType();
  extern void PyReferenceCollectionLocator_LinkPyType();

}  // End of extern "C".

}  // End of Isobar namespace.
 
#endif
