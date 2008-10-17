#ifndef __PYINSTANCECOLLECTION__
#define __PYINSTANCECOLLECTION__


#include "hurricane/isobar/PyHurricane.h"

#include "hurricane/Instance.h"
#include "hurricane/Instances.h"


namespace Isobar {


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyInstanceCollection".

  typedef struct {
      PyObject_HEAD
      Hurricane::Instances* _object;
  } PyInstanceCollection;

  typedef struct {
      PyObject_HEAD
      Hurricane::Locator<Hurricane::Instance*>* _object;
      PyInstanceCollection* _collection;
  } PyInstanceCollectionLocator;

// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

  extern PyTypeObject PyTypeInstanceCollection;
  extern PyTypeObject PyTypeInstanceCollectionLocator;

  extern void PyInstanceCollection_LinkPyType();
  extern void PyInstanceCollectionLocator_LinkPyType();

}  // End of extern "C".

}  // End of Isobar namespace.
 
#endif
