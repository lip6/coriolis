#ifndef __PYCELLCOLLECTION__
#define __PYCELLCOLLECTION__


#include "hurricane/isobar/PyHurricane.h"

#include "hurricane/Cell.h"
#include "hurricane/Cells.h"


namespace Isobar {


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyCellCollection".

  typedef struct {
      PyObject_HEAD
      Hurricane::Cells* _object;
  } PyCellCollection;

  typedef struct {
      PyObject_HEAD
      Hurricane::Locator<Hurricane::Cell*>* _object;
      PyCellCollection* _collection;
  } PyCellCollectionLocator;

// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

  extern PyTypeObject PyTypeCellCollection;
  extern PyTypeObject PyTypeCellCollectionLocator;

  extern void PyCellCollection_LinkPyType();
  extern void PyCellCollectionLocator_LinkPyType();

}  // End of extern "C".

}  // End of Isobar namespace.
 
#endif
