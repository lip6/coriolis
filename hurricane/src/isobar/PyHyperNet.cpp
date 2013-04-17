
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2007-2013, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyHyperNet.cpp"                         |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyHyperNet.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/isobar/PyOccurrence.h"
#include "hurricane/isobar/PyOccurrenceCollection.h" 

using namespace Hurricane;


namespace  Isobar {

extern "C" {

#define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(HyperNet,hyperNet,function)


// +=================================================================+
// |              "PyHyperNet" Python Module Code Part               |
// +=================================================================+

#if defined(__PYTHON_MODULE__)

  
  // Standart Predicates (Attributes).
  DirectGetBoolAttribute(PyHyperNet_isValid  ,isValid  ,PyHyperNet,HyperNet)

  // Standart Delete (Attribute).
  DirectDestroyAttribute(PyHyperNet_destroy, PyHyperNet)


  static PyObject* PyHyperNet_getNetOccurrences(PyHyperNet *self)
  {
    trace << "PyHyperNet_getNetOccurrences()" << endl;

    METHOD_HEAD ( "HyperNet.getNetOccurrences()" )

    PyOccurrenceCollection* pyOccurrenceCollection = NULL;

    HTRY
    Occurrences* occurrences = new Occurrences(hyperNet->getNetOccurrences());

    pyOccurrenceCollection = PyObject_NEW(PyOccurrenceCollection, &PyTypeOccurrenceCollection);
    if (pyOccurrenceCollection == NULL) { 
        return NULL;
    }

    pyOccurrenceCollection->_object = occurrences;
    HCATCH
    
    return (PyObject*)pyOccurrenceCollection;
  }


  static PyObject* PyHyperNet_getLeafPlugOccurrences(PyHyperNet *self)
  {
    trace << "PyHyperNet_getLeafPlugOccurrences()" << endl;

    METHOD_HEAD ( "HyperNet.getLeafPlugOccurrences()" )

    PyOccurrenceCollection* pyOccurrenceCollection = NULL;

    HTRY
    Occurrences* occurrences = new Occurrences(hyperNet->getLeafPlugOccurrences());

    pyOccurrenceCollection = PyObject_NEW(PyOccurrenceCollection, &PyTypeOccurrenceCollection);
    if (pyOccurrenceCollection == NULL) { 
        return NULL;
    }

    pyOccurrenceCollection->_object = occurrences;
    HCATCH
    
    return (PyObject*)pyOccurrenceCollection;
  }

    
  static PyObject* PyHyperNet_getCell ( PyHyperNet *self )
  {
    trace << "PyHyperNet_getCell ()" << endl;

    Cell* cell = NULL;
    
    HTRY
    METHOD_HEAD ( "HyperNet.getCell()" )
    cell = hyperNet->getCell ();
    HCATCH

    return PyCell_Link ( cell );
  }


  PyMethodDef PyHyperNet_Methods[] =
    { { "getCell"               , (PyCFunction)PyHyperNet_getCell               , METH_NOARGS , "Returns the hyperNet cell." }
    , { "isValid"               , (PyCFunction)PyHyperNet_isValid               , METH_NOARGS , "Returns trus if the HyperNet isValid." }
    , { "getNetOccurrences"     , (PyCFunction)PyHyperNet_getNetOccurrences     , METH_NOARGS 
                                , "Returns the collection of Net occurrences" }
    , { "getLeafPlugOccurrences", (PyCFunction)PyHyperNet_getLeafPlugOccurrences, METH_NOARGS 
                                , "Returns the collection of leaf occurrences" }
    , { "destroy"               , (PyCFunction)PyHyperNet_destroy               , METH_NOARGS
                                , "Destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}     /* sentinel */
    };


  DirectDeleteMethod(PyHyperNet_DeAlloc,PyHyperNet)
  PyTypeObjectLinkPyType(HyperNet)


#else  // End of Python Module Code Part.


// +=================================================================+
// |             "PyHyperNet" Shared Library Code Part               |
// +=================================================================+


  PyObject* PyHyperNet_create ( PyObject *module, PyObject *args ) {
    trace << "PyHyperNet_create()" << endl;

    HyperNet* hyperNet = NULL;
    PyObject* arg0;
    
    if (! ParseOneArg ( "HyperNet.create()", args, ":occur", &arg0 )) {
        PyErr_SetString(ConstructorError, "invalid number of parameters for HyperNet constructor." );
        return ( NULL );
    }

    hyperNet =  new HyperNet ( *PYOCCURRENCE_O(arg0) );

    PyHyperNet* pyHyperNet;
    pyHyperNet = PyObject_NEW(PyHyperNet, &PyTypeHyperNet);
    if (pyHyperNet == NULL) return NULL;

    HTRY
    pyHyperNet->_object = hyperNet;
    HCATCH

    return ( (PyObject*)pyHyperNet );
  }


  PyTypeObjectDefinitions(HyperNet)


#endif  // End of Shared Library Code Part.


}  // extern "C".

}  // Isobar namespace.
