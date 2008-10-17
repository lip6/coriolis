#ifndef __PYPLUGCOLLECTION__
#define __PYPLUGCOLLECTION__


#include "hurricane/isobar/PyHurricane.h"

#include "hurricane/Plug.h"
#include "hurricane/Plugs.h"


namespace Isobar {


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyPlugCollection".

  typedef struct {
      PyObject_HEAD
      Hurricane::Plugs* _object;
  } PyPlugCollection;

  typedef struct {
      PyObject_HEAD
      Hurricane::Locator<Hurricane::Plug*>* _object;
      PyPlugCollection* _collection;
  } PyPlugCollectionLocator;

// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

  extern PyTypeObject PyTypePlugCollection;
  extern PyTypeObject PyTypePlugCollectionLocator;

  extern void PyPlugCollection_LinkPyType();
  extern void PyPlugCollectionLocator_LinkPyType();

}  // End of extern "C".

}  // End of Isobar namespace.
 
#endif
