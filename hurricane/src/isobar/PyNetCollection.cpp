#include "hurricane/isobar/PyNetCollection.h"
#include "hurricane/isobar/PyNet.h"

namespace  Isobar {

using namespace Hurricane;


extern "C" {


// x=================================================================x
// |             "PyNetCollection" Python Module Code Part           |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |                 "PyNetCollection" Attribute Methods         |
  // x-------------------------------------------------------------x


  DirectDeleteMethod(PyNetCollection_DeAlloc, PyNetCollection)

  static void PyNetCollectionLocator_DeAlloc(PyNetCollectionLocator* pyLocator) {
      Py_XDECREF(pyLocator->_collection);
      if (pyLocator->_object) {
          delete pyLocator->_object;
      }
      PyObject_Del(pyLocator);
  }

  GetLocatorMethod(Net)

  static PyObject* NetLocatorNext(PyNetCollectionLocator* pyLocator) {
      Locator<Net*>* locator = pyLocator->_object;

      if (locator->isValid()) {
          Net* net = locator->getElement();
          locator->progress();
          return PyNet_Link(net); 
      }
      return NULL;
  }

  extern void PyNetCollection_LinkPyType () {
    trace << "PyNetCollection_LinkType()" << endl;
    PyTypeNetCollection.tp_iter = (getiterfunc)GetLocator;      /* tp_iter */
    PyTypeNetCollection.tp_dealloc = (destructor)PyNetCollection_DeAlloc;
    PyTypeNetCollectionLocator.tp_dealloc = (destructor)PyNetCollectionLocator_DeAlloc;
    PyTypeNetCollectionLocator.tp_iter = PyObject_SelfIter;
    PyTypeNetCollectionLocator.tp_iternext = (iternextfunc)NetLocatorNext;
    PyTypeNetCollection.tp_dealloc = (destructor)PyNetCollection_DeAlloc;
  }


#else  // End of Python Module Code Part.


// x=================================================================x
// |            "PyNetCollection" Shared Library Code Part           |
// x=================================================================x


  // ---------------------------------------------------------------
  // PyNetCollection Object Definitions.
  
  PyTypeCollectionObjectDefinitions(NetCollection)
  PyTypeCollectionObjectDefinitions(NetCollectionLocator)

#endif  // End of Shared Library Code Part.


}  // End of extern "C".

}  // End of Isobar namespace.
