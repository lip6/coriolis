
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2010-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |              M a u k a  -  P l a c e r                          |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyMaukaEngine.cpp"                      |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include "hurricane/isobar/PyCell.h"
#include "nimbus/NimbusEngine.h"
#include "mauka/PyMaukaEngine.h"


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
  using Nimbus::NimbusEngine;


extern "C" {


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(MaukaEngine,mauka,function)


// x=================================================================x
// |            "PyMaukaEngine" Python Module Code Part              |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // Standart Accessors (Attributes).
  DirectVoidMethod(MaukaEngine,mauka,Run)

  // Standart Destroy (Attribute).
  DBoDestroyAttribute(PyMaukaEngine_destroy,PyMaukaEngine)


  PyMethodDef PyMaukaEngine_Methods[] =
    { { "run"          , (PyCFunction)PyMaukaEngine_Run    , METH_NOARGS
                       , "Runs Mauka on it's Cell." }
    , { "destroy"      , (PyCFunction)PyMaukaEngine_destroy, METH_NOARGS
                       , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };


  PythonOnlyDeleteMethod(MaukaEngine)
  PyTypeObjectLinkPyType(MaukaEngine)


#else  // End of Python Module Code Part.


// x=================================================================x
// |             "PyMaukaEngine" Shared Library Code Part            |
// x=================================================================x


  extern PyObject* PyMaukaEngine_get ( PyObject *module, PyObject* args )
  {
    trace << "PyMaukaEngine_get()" << endl;

    MaukaEngine*   mauka   = NULL;
    PyMaukaEngine* pyMauka = NULL;
    
    HTRY
    PyObject* arg0;

    if ( not ParseOneArg ( "Mauka.get", args, CELL_ARG, &arg0) ) return NULL;
    mauka = MaukaEngine::get ( PYCELL_O(arg0) );

    pyMauka = PyObject_NEW ( PyMaukaEngine, &PyTypeMaukaEngine );
    if ( pyMauka == NULL ) return NULL;

    pyMauka->_object = mauka;
    HCATCH

    return (PyObject*)pyMauka;
  }


  extern PyObject* PyMaukaEngine_create ( PyObject *module, PyObject* args )
  {
    trace << "PyMaukaEngine_create()" << endl;

    MaukaEngine*   mauka   = NULL;
    PyMaukaEngine* pyMauka = NULL;
    
    HTRY
    PyObject* arg0;

    if ( not ParseOneArg ( "Mauka.get", args, CELL_ARG, &arg0) ) return NULL;

    Cell* cell = PYCELL_O(arg0);
    mauka = MaukaEngine::get ( cell );

    if ( mauka == NULL ) {
      NimbusEngine* nimbus = NimbusEngine::get ( cell );
      if ( nimbus == NULL )
        NimbusEngine::create ( cell );

      mauka = MaukaEngine::create ( cell );
      if ( cmess1.enabled() )
        mauka->getConfiguration()->print( cell );
    } else
      cerr << Warning("%s already has a Mauka engine.",getString(cell).c_str()) << endl;

    pyMauka = PyObject_NEW ( PyMaukaEngine, &PyTypeMaukaEngine );
    if ( pyMauka == NULL ) return NULL;

    pyMauka->_object = mauka;
    HCATCH

    return (PyObject*)pyMauka;
  }


  // Link/Creation Method.
  PyTypeObjectDefinitions(MaukaEngine)


#endif  // End of Shared Library Code Part.


}  // End of extern "C".


}  // End of Mauka namespace.
 
