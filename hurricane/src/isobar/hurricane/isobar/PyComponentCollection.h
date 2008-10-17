#ifndef __PYCOMPONENTCOLLECTION__
#define __PYCOMPONENTCOLLECTION__


#include "hurricane/isobar/PyHurricane.h"

#include "hurricane/Component.h"
#include "hurricane/Components.h"


namespace Isobar {


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyComponentCollection".

  typedef struct {
      PyObject_HEAD
      Hurricane::Components* _object;
  } PyComponentCollection;

  typedef struct {
      PyObject_HEAD
      Hurricane::Locator<Hurricane::Component*>* _object;
      PyComponentCollection* _collection;
  } PyComponentCollectionLocator;

// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

  extern PyTypeObject PyTypeComponentCollection;
  extern PyTypeObject PyTypeComponentCollectionLocator;

  extern void PyComponentCollection_LinkPyType();
  extern void PyComponentCollectionLocator_LinkPyType();

}  // End of extern "C".

}  // End of Isobar namespace.
 
#endif
