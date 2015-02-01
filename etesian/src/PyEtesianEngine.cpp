
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2014-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |   E t e s i a n  -  A n a l y t i c   P l a c e r               |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyEtesianEngine.cpp"                         |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyCell.h"
#include "hurricane/Cell.h"
#include "etesian/PyEtesianEngine.h"

# undef   ACCESS_OBJECT
# undef   ACCESS_CLASS
# define  ACCESS_OBJECT            _baseObject._object
# define  ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define   METHOD_HEAD(function)    GENERIC_METHOD_HEAD(EtesianEngine,etesian,function)


namespace  Etesian {

  using std::cerr;
  using std::endl;
  using std::hex;
  using std::ostringstream;
  using Hurricane::tab;
  using Hurricane::in_trace;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Isobar::ProxyProperty;
  using Isobar::ProxyError;
  using Isobar::ConstructorError;
  using Isobar::HurricaneError;
  using Isobar::HurricaneWarning;
  using Isobar::ParseOneArg;
  using Isobar::ParseTwoArg;
  using Isobar::PyCell;
  using Isobar::PyCell_Link;
  using CRL::PyToolEngine;


extern "C" {

#if defined(__PYTHON_MODULE__)


// +=================================================================+
// |          "PyEtesianEngine" Python Module Code Part              |
// +=================================================================+


  static PyObject* PyEtesianEngine_get ( PyObject*, PyObject* args )
  {
    trace << "PyEtesianEngine_get()" << endl;

    EtesianEngine* etesian = NULL;
    
    HTRY
    PyObject* arg0;

    if (not ParseOneArg("Etesian.get", args, CELL_ARG, &arg0)) return NULL;
    etesian = EtesianEngine::get(PYCELL_O(arg0));
    HCATCH

    return PyEtesianEngine_Link(etesian);
  }


  static PyObject* PyEtesianEngine_create ( PyObject*, PyObject* args )
  {
    trace << "PyEtesianEngine_create()" << endl;

    EtesianEngine* etesian = NULL;
    
    HTRY
    PyObject* arg0;

    if (not ParseOneArg("Etesian.get", args, CELL_ARG, &arg0)) return NULL;

    Cell* cell = PYCELL_O(arg0);
    etesian = EtesianEngine::get(cell);

    if (etesian == NULL) {
      etesian = EtesianEngine::create(cell);
    //if (cmess1.enabled())
    //  etesian->getEtesianConfiguration()->print(cell);
    } else
      cerr << Warning("%s already has a Etesian engine.",getString(cell).c_str()) << endl;
    HCATCH

    return PyEtesianEngine_Link(etesian);
  }


  PyObject* PyEtesianEngine_place ( PyEtesianEngine* self, PyObject* args )
  {
    trace << "PyEtesianEngine_place()" << endl;

    HTRY
    METHOD_HEAD("EtesianEngine.place()")
    unsigned int flags = 0;
    if (PyArg_ParseTuple(args,"I:EtesianEngine.place", &flags)) {
      etesian->place( flags );
    } else {
      PyErr_SetString(ConstructorError, "EtesianEngine.place(): Invalid number/bad type of parameter.");
      return NULL;
    }
    HCATCH

    Py_RETURN_NONE;
  }


  // Standart Accessors (Attributes).
  // DirectVoidMethod(EtesianEngine,etesian,runNegociate)
  // DirectVoidMethod(EtesianEngine,etesian,printConfiguration)
  // DirectVoidMethod(EtesianEngine,etesian,saveGlobalSolution)
  // DirectVoidMethod(EtesianEngine,etesian,finalizeLayout)
  // DirectVoidMethod(EtesianEngine,etesian,dumpMeasures)
  // DirectGetBoolAttribute(PyEtesianEngine_getToolSuccess,getToolSuccess,PyEtesianEngine,EtesianEngine)

  // Standart Destroy (Attribute).
  DBoDestroyAttribute(PyEtesianEngine_destroy,PyEtesianEngine)


  PyMethodDef PyEtesianEngine_Methods[] =
    { { "get"               , (PyCFunction)PyEtesianEngine_get               , METH_VARARGS|METH_STATIC
                            , "Returns the Etesian engine attached to the Cell, None if there isnt't." }
    , { "create"            , (PyCFunction)PyEtesianEngine_create            , METH_VARARGS|METH_STATIC
                            , "Create a Etesian engine on this cell." }
    , { "place"             , (PyCFunction)PyEtesianEngine_place             , METH_VARARGS
                            , "Run the global router (Knik)." }
    , { "destroy"           , (PyCFunction)PyEtesianEngine_destroy           , METH_NOARGS
                            , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL} /* sentinel */
    };


  DBoDeleteMethod(EtesianEngine)
  PyTypeObjectLinkPyType(EtesianEngine)


#else  // End of Python Module Code Part.


// +=================================================================+
// |           "PyEtesianEngine" Shared Library Code Part            |
// +=================================================================+


  // Link/Creation Method.
  PyTypeInheritedObjectDefinitions(EtesianEngine,PyToolEngine)
  DBoLinkCreateMethod(EtesianEngine)


  extern void  PyEtesianEngine_postModuleInit ()
  {
    PyObject* constant;

    LoadObjectConstant(PyTypeEtesianEngine.tp_dict,EtesianEngine::SlowMotion,"SlowMotion");
  }


#endif  // Shared Library Code Part.

}  // extern "C".

}  // Etesian namespace.
 
