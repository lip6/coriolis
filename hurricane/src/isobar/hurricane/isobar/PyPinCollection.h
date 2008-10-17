#ifndef __PYPINCOLLECTION__
#define __PYPINCOLLECTION__


#include "hurricane/isobar/PyHurricane.h"

#include "hurricane/Pin.h"
#include "hurricane/Pins.h"


namespace Isobar {


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyPinCollection".

  typedef struct {
      PyObject_HEAD
      Hurricane::Pins* _object;
  } PyPinCollection;

  typedef struct {
      PyObject_HEAD
      Hurricane::Locator<Hurricane::Pin*>* _object;
      PyPinCollection* _collection;
  } PyPinCollectionLocator;

// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

  extern PyTypeObject PyTypePinCollection;
  extern PyTypeObject PyTypePinCollectionLocator;

  extern void PyPinCollection_LinkPyType();
  extern void PyPinCollectionLocator_LinkPyType();

}  // End of extern "C".

}  // End of Isobar namespace.
 
#endif
