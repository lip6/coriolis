#include "hurricane/isobar/PyNetCollection.h"
#include "hurricane/isobar/PyNet.h"


namespace  Isobar {

using namespace Hurricane;


extern "C" {


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(Collection<Net*>,collection,function)


// x=================================================================x
// |             "PyNetCollection" Python Module Code Part           |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |                 "PyNetCollection" Attribute Methods         |
  // x-------------------------------------------------------------x


#if 0
  // Standart Predicates (Attributes).
  DirectGetBoolAttribute(PyNetLocator_isValid,isValid,PyNetLocator,Locator<Net*>)


  // Standart Locator Accessors (Attributes).
  LocatorProgressAttribute(Net)
  LocatorGetElementAttribute(Net)
  LocatorGetCloneAttribute(Net)


  // Standart destroy (Attribute).
  DirectDestroyAttribute(PyNetLocator_destroy, PyNetLocator)



  // ---------------------------------------------------------------
  // PyNetLocator Attribute Method table.

  PyMethodDef PyNetLocator_Methods[] =
    { { "isValid"        , (PyCFunction)PyNetLocator_isValid    , METH_NOARGS , "Returns true while the walk has not exhausted the set of elements, else false." }
    , { "progress"       , (PyCFunction)PyNetLocator_progress   , METH_NOARGS , "Moves forward the locator to the following element." }
    , { "getElement"     , (PyCFunction)PyNetLocator_getElement , METH_NOARGS , "Returns the current element (or the value Type() when the locator is not or no longer valid)." }
    , { "getClone"       , (PyCFunction)PyNetLocator_getClone   , METH_NOARGS , "This function allocates and returns a new locator that will have the same visiting course than the remaining one of the locator being cloned." }
    , { "destroy"        , (PyCFunction)PyNetLocator_destroy    , METH_NOARGS
                         , "Destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };


  // x-------------------------------------------------------------x
  // |                   "PyNetLocator" Object Methods             |
  // x-------------------------------------------------------------x


  DirectDeleteMethod(PyNetLocator_DeAlloc,PyNetLocator)
  LocatorPyTypeObjectLinkPyType(Net, Net*)



#endif
  DirectDeleteMethod(PyNetCollection_DeAlloc, PyNetCollection)
  DirectDeleteMethod(PyNetCollectionLocator_DeAlloc, PyNetCollectionLocator)

  static PyObject* GetNetLocator(PyNetCollection *collection) {
      PyNetCollectionLocator* nl = PyObject_New(PyNetCollectionLocator, &PyTypeNetCollectionLocator);
      if (nl == NULL) {
          return NULL;
      }

      nl->_object = collection->_object->getLocator();
      return (PyObject *)nl;
  }

  static PyObject* NetLocatorNext(PyNetCollectionLocator* pyLocator) {
      Locator<Net*>* locator = pyLocator->_object;

      if (locator->isValid()) {
          cerr << "akecoucou" << endl;
          Net* net = locator->getElement();
          locator->progress();
          return PyNet_Link(net); 
      }
      cerr << "end" << endl;

      return NULL;
  }

  extern void PyNetCollection_LinkPyType () {
    trace << "PyNetCollection_LinkType()" << endl;
    PyTypeNetCollection.tp_iter = (getiterfunc)GetNetLocator;      /* tp_iter */
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
  
  PyTypeObject PyTypeNetCollection =                        
    { PyObject_HEAD_INIT(NULL)                               
      0                               /* ob_size.          */
    , "Hurricane.NetCollection"       /* tp_name.          */
    , sizeof(PyNetCollection)         /* tp_basicsize.     */
    , 0                               /* tp_itemsize.      */
    /* methods. */                                           
    , 0                               /* tp_dealloc.       */
    , 0                               /* tp_print.         */
    , 0                               /* tp_getattr.       */
    , 0                               /* tp_setattr.       */
    , 0                               /* tp_compare.       */
    , 0                               /* tp_repr.          */
    , 0                               /* tp_as_number.     */
    , 0                               /* tp_as_sequence.   */
    , 0                               /* tp_as_mapping.    */
    , 0                               /* tp_hash.          */
    , 0                               /* tp_call.          */
    , 0                               /* tp_str            */
    , 0                               /* tp_getattro.      */
    , 0                               /* tp_setattro.      */
    , 0                               /* tp_as_buffer.     */
    , Py_TPFLAGS_DEFAULT              /* tp_flags.         */
    , "NetCollection objects"         /* tp_doc.           */
    , 0                               /* tp_traverse */
    , 0                               /* tp_clear */
    , 0                               /* tp_richcompare */
    , 0                               /* tp_weaklistoffset */
    , 0                               /* tp_iter */
    , 0                               /* tp_iternext */
    , 0                               /* tp_methods */
    , 0
  };

  PyTypeObject PyTypeNetCollectionLocator =                        
    { PyObject_HEAD_INIT(NULL)                               
      0                               /* ob_size.          */
    , "Hurricane.NetLocator"          /* tp_name.          */
    , sizeof(PyNetCollectionLocator)  /* tp_basicsize.     */
    , 0                               /* tp_itemsize.      */
    /* methods. */                                           
    , 0                               /* tp_dealloc.       */
    , 0                               /* tp_print.         */
    , 0                               /* tp_getattr.       */
    , 0                               /* tp_setattr.       */
    , 0                               /* tp_compare.       */
    , 0                               /* tp_repr.          */
    , 0                               /* tp_as_number.     */
    , 0                               /* tp_as_sequence.   */
    , 0                               /* tp_as_mapping.    */
    , 0                               /* tp_hash.          */
    , 0                               /* tp_call.          */
    , 0                               /* tp_str            */
    , 0                               /* tp_getattro.      */
    , 0                               /* tp_setattro.      */
    , 0                               /* tp_as_buffer.     */
    , Py_TPFLAGS_DEFAULT              /* tp_flags.         */
    , "NetLocator objects"            /* tp_doc.           */
    , 0                               /* tp_traverse */
    , 0                               /* tp_clear */
    , 0                               /* tp_richcompare */
    , 0                               /* tp_weaklistoffset */
    , 0                               /* tp_iter */
    , 0   /* tp_iternext */
    , 0                               /* tp_methods */
    , 0
  };



#endif  // End of Shared Library Code Part.


}  // End of extern "C".




}  // End of Isobar namespace.
