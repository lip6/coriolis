#ifndef __PYSEGMENTCOLLECTION__
#define __PYSEGMENTCOLLECTION__


#include "hurricane/isobar/PyHurricane.h"

#include "hurricane/Segment.h"
#include "hurricane/Segments.h"


namespace Isobar {


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PySegmentCollection".

  typedef struct {
      PyObject_HEAD
      Hurricane::Segments* _object;
  } PySegmentCollection;

  typedef struct {
      PyObject_HEAD
      Hurricane::Locator<Hurricane::Segment*>* _object;
      PySegmentCollection* _collection;
  } PySegmentCollectionLocator;

// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

  extern PyTypeObject PyTypeSegmentCollection;
  extern PyTypeObject PyTypeSegmentCollectionLocator;

  extern void PySegmentCollection_LinkPyType();
  extern void PySegmentCollectionLocator_LinkPyType();

}  // End of extern "C".

}  // End of Isobar namespace.
 
#endif
