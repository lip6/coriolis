#ifndef __PYNETCOLLECTION__
#define __PYNETCOLLECTION__


#include "hurricane/isobar/PyHurricane.h"

#include "hurricane/Net.h"
#include "hurricane/Nets.h"


namespace Isobar {


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyNetCollection".

  typedef struct {
      PyObject_HEAD
      Hurricane::Nets* _object;
  } PyNetCollection;

  typedef struct {
      PyObject_HEAD
      Hurricane::Locator<Hurricane::Net*>* _object;
      PyNetCollection* _collection;
  } PyNetCollectionLocator;

// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

  extern PyTypeObject PyTypeNetCollection;
  extern PyTypeObject PyTypeNetCollectionLocator;

  extern void PyNetCollection_LinkPyType();
  extern void PyNetCollectionLocator_LinkPyType();

}  // End of extern "C".

}  // End of Isobar namespace.
 
#endif
