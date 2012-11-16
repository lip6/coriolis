
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2010-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |              M a u k a  -  P l a c e r                          |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyMaukaEngine.cpp"                      |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyCell.h"
#include "nimbus/NimbusEngine.h"
#include "mauka/PyMaukaEngine.h"

# undef   ACCESS_OBJECT
# undef   ACCESS_CLASS
# define  ACCESS_OBJECT            _baseObject._object
# define  ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define   METHOD_HEAD(function)    GENERIC_METHOD_HEAD(MaukaEngine,mauka,function)


namespace  Mauka {

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
  using Nimbus::NimbusEngine;


extern "C" {

#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |            "PyMaukaEngine" Python Module Code Part              |
// +=================================================================+


  static PyObject* PyMaukaEngine_get ( PyObject*, PyObject* args )
  {
    trace << "PyMaukaEngine_get()" << endl;

    MaukaEngine*   mauka   = NULL;
    
    HTRY
    PyObject* arg0;

    if (not ParseOneArg( "Mauka.get", args, CELL_ARG, &arg0)) return NULL;
    mauka = MaukaEngine::get(PYCELL_O(arg0));
    HCATCH

    return PyMaukaEngine_Link(mauka);
  }


  static PyObject* PyMaukaEngine_create ( PyObject*, PyObject* args )
  {
    trace << "PyMaukaEngine_create()" << endl;

    MaukaEngine*   mauka   = NULL;
    
    HTRY
    PyObject* arg0;

    if (not ParseOneArg( "Mauka.get", args, CELL_ARG, &arg0)) return NULL;

    Cell* cell = PYCELL_O(arg0);
    mauka = MaukaEngine::get(cell);

    if ( mauka == NULL ) {
      NimbusEngine* nimbus = NimbusEngine::get(cell);
      if (nimbus == NULL)
        nimbus = NimbusEngine::create(cell);

      mauka = MaukaEngine::create(cell);
    } else
      cerr << Warning("%s already has a Mauka engine.",getString(cell).c_str()) << endl;
    HCATCH

    return PyMaukaEngine_Link(mauka);
  }


  PyObject* PyMaukaEngine_regroupOverloadedGCells ( PyMaukaEngine*, PyObject* args )
  {
    trace << "PyMaukaEngine_regroupOverloadedGCells()" << endl;

    HTRY
    PyObject* arg0;

    if (not ParseOneArg("Mauka.regroupOverloadedGCells", args, CELL_ARG, &arg0)) return NULL;
    MaukaEngine::regroupOverloadedGCells(PYCELL_O(arg0));
    HCATCH

    Py_RETURN_NONE;
  }


  // Standart Accessors (Attributes).
  DirectVoidMethod(MaukaEngine,mauka,Run)
  DirectVoidMethod(MaukaEngine,mauka,printConfiguration)

  // Standart Destroy (Attribute).
  DBoDestroyAttribute(PyMaukaEngine_destroy,PyMaukaEngine)


  PyMethodDef PyMaukaEngine_Methods[] =
    { { "get"                    , (PyCFunction)PyMaukaEngine_get                    , METH_VARARGS|METH_STATIC
                                 , "Return the Mauka engine associated to the Cell, None if there isn't." }
    , { "create"                 , (PyCFunction)PyMaukaEngine_create                 , METH_VARARGS|METH_STATIC
                                 , "Create a Mauka engine on this Cell." }
    , { "printConfiguration"     , (PyCFunction)PyMaukaEngine_printConfiguration     , METH_NOARGS
                                 , "Display on the console the configuration of Mauka." }
    , { "regroupOverloadedGCells", (PyCFunction)PyMaukaEngine_regroupOverloadedGCells, METH_VARARGS|METH_STATIC
                                 , "Destroy hierarchical levels that cannot satisfy the placement." }
    , { "run"                    , (PyCFunction)PyMaukaEngine_Run                    , METH_NOARGS
                                 , "Runs Mauka on it's Cell." }
    , { "destroy"                , (PyCFunction)PyMaukaEngine_destroy                , METH_NOARGS
                                 , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL}      /* sentinel */
    };


  DBoDeleteMethod(MaukaEngine)
  PyTypeObjectLinkPyType(MaukaEngine)


#else  // End of Python Module Code Part.


// +=================================================================+
// |             "PyMaukaEngine" Shared Library Code Part            |
// +=================================================================+


  // Link/Creation Method.
  PyTypeInheritedObjectDefinitions(MaukaEngine,PyToolEngine)
  DBoLinkCreateMethod(MaukaEngine)


#endif  // End of Shared Library Code Part.


}  // End of extern "C".


}  // End of Mauka namespace.
 
