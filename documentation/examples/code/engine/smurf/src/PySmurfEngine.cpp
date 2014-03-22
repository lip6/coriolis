
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2014-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |   S m u r f  -  A  D e m o   T o o l E n g i n e                |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PySmurfEngine.cpp"                           |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyCell.h"
#include "hurricane/Cell.h"
#include "smurf/PySmurfEngine.h"

# undef   ACCESS_OBJECT
# undef   ACCESS_CLASS
# define  ACCESS_OBJECT            _baseObject._object
# define  ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define   METHOD_HEAD(function)    GENERIC_METHOD_HEAD(SmurfEngine,smurf,function)


namespace  Smurf {

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
// |          "PySmurfEngine" Python Module Code Part              |
// +=================================================================+


  static PyObject* PySmurfEngine_get ( PyObject*, PyObject* args )
  {
    trace << "PySmurfEngine_get()" << endl;

    SmurfEngine* smurf = NULL;
    
    HTRY
    PyObject* arg0;

    if (not ParseOneArg("Smurf.get", args, CELL_ARG, &arg0)) return NULL;
    smurf = SmurfEngine::get(PYCELL_O(arg0));
    HCATCH

    return PySmurfEngine_Link(smurf);
  }


  static PyObject* PySmurfEngine_create ( PyObject*, PyObject* args )
  {
    trace << "PySmurfEngine_create()" << endl;

    SmurfEngine* smurf = NULL;
    
    HTRY
    PyObject* arg0;

    if (not ParseOneArg("Smurf.get", args, CELL_ARG, &arg0)) return NULL;

    Cell* cell = PYCELL_O(arg0);
    smurf = SmurfEngine::get(cell);

    if (smurf == NULL) {
      smurf = SmurfEngine::create(cell);
    //if (cmess1.enabled())
    //  smurf->getSmurfConfiguration()->print(cell);
    } else
      cerr << Warning("%s already has a Smurf engine.",getString(cell).c_str()) << endl;
    HCATCH

    return PySmurfEngine_Link(smurf);
  }


  PyObject* PySmurfEngine_place ( PySmurfEngine* self, PyObject* args )
  {
    trace << "PySmurfEngine_place()" << endl;

    HTRY
    METHOD_HEAD("SmurfEngine.place()")
    unsigned int flags = 0;
    if (PyArg_ParseTuple(args,"I:SmurfEngine.place", &flags)) {
      smurf->place(/*flags*/);
    } else {
      PyErr_SetString(ConstructorError, "SmurfEngine.place(): Invalid number/bad type of parameter.");
      return NULL;
    }
    HCATCH

    Py_RETURN_NONE;
  }


  // Standart Accessors (Attributes).
  // DirectVoidMethod(SmurfEngine,smurf,runNegociate)
  // DirectVoidMethod(SmurfEngine,smurf,printConfiguration)
  // DirectVoidMethod(SmurfEngine,smurf,saveGlobalSolution)
  // DirectVoidMethod(SmurfEngine,smurf,finalizeLayout)
  // DirectVoidMethod(SmurfEngine,smurf,dumpMeasures)
  // DirectGetBoolAttribute(PySmurfEngine_getToolSuccess,getToolSuccess,PySmurfEngine,SmurfEngine)

  // Standart Destroy (Attribute).
  DBoDestroyAttribute(PySmurfEngine_destroy,PySmurfEngine)


  PyMethodDef PySmurfEngine_Methods[] =
    { { "get"               , (PyCFunction)PySmurfEngine_get               , METH_VARARGS|METH_STATIC
                            , "Returns the Smurf engine attached to the Cell, None if there isnt't." }
    , { "create"            , (PyCFunction)PySmurfEngine_create            , METH_VARARGS|METH_STATIC
                            , "Create a Smurf engine on this cell." }
 // , { "printConfiguration", (PyCFunction)PySmurfEngine_printConfiguration, METH_NOARGS
 //                         , "Display on the console the configuration of Smurf." }
 // , { "saveGlobalSolution", (PyCFunction)PySmurfEngine_saveGlobalSolution, METH_NOARGS
 //                         , "Save the global routing solution on disk." }
 // , { "getToolSuccess"    , (PyCFunction)PySmurfEngine_getToolSuccess    , METH_NOARGS
 //                         , "Returns True if the detailed routing has been successful." }
 // , { "loadGlobalRouting" , (PyCFunction)PySmurfEngine_loadGlobalRouting , METH_VARARGS
 //                         , "Read/load the global routing and build topologies for Smurf." }
    , { "place"             , (PyCFunction)PySmurfEngine_place             , METH_VARARGS
                            , "Run the global router (Knik)." }
 // , { "layerAssign"       , (PyCFunction)PySmurfEngine_layerAssign       , METH_VARARGS
 //                         , "Run the layer assigment stage." }
 // , { "runNegociate"      , (PyCFunction)PySmurfEngine_runNegociate      , METH_NOARGS
 //                         , "Run the negociation stage of the detailed router." }
 // , { "finalizeLayout"    , (PyCFunction)PySmurfEngine_finalizeLayout    , METH_NOARGS
 //                         , "Revert to a pure Hurricane database, remove router's additionnal data structures." }
 // , { "dumpMeasures"      , (PyCFunction)PySmurfEngine_dumpMeasures      , METH_NOARGS
 //                         , "Dump to disk lots of statistical informations about the routing." }
    , { "destroy"           , (PyCFunction)PySmurfEngine_destroy           , METH_NOARGS
                            , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL} /* sentinel */
    };


  DBoDeleteMethod(SmurfEngine)
  PyTypeObjectLinkPyType(SmurfEngine)


#else  // End of Python Module Code Part.


// +=================================================================+
// |           "PySmurfEngine" Shared Library Code Part            |
// +=================================================================+


  // Link/Creation Method.
  PyTypeInheritedObjectDefinitions(SmurfEngine,PyToolEngine)
  DBoLinkCreateMethod(SmurfEngine)


#endif  // Shared Library Code Part.

}  // extern "C".

}  // Smurf namespace.
 
