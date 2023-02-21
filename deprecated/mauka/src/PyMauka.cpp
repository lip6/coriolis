
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
// |  C++ Module  :       "./PyMauka.cpp"                            |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/isobar/PyCell.h"
#include "mauka/PyMaukaEngine.h"
#include "mauka/PyGraphicMaukaEngine.h"


namespace Mauka {

  using std::cerr;
  using std::endl;
  using Hurricane::tab;
  using Hurricane::in_trace;
  using Isobar::__cs;
  using CRL::PyTypeToolEngine;
  using CRL::PyTypeGraphicTool;


#if !defined(__PYTHON_MODULE__)

// x=================================================================x
// |               "PyMauka" Shared Library Code Part                |
// x=================================================================x


# else // End of PyHurricane Shared Library Code Part.


// x=================================================================x
// |                "PyMauka" Python Module Code Part                |
// x=================================================================x


extern "C" {


  // x-------------------------------------------------------------x
  // |                 "PyMauka" Module Methods                    |
  // x-------------------------------------------------------------x


  static PyMethodDef PyMauka_Methods[] =
    { {NULL, NULL, 0, NULL}     /* sentinel */
    };




  // ---------------------------------------------------------------
  // Module Initialization  :  "initMauka ()"

  DL_EXPORT(void) initMauka () {
    trace << "initMauka()" << endl;

    PyMaukaEngine_LinkPyType ();
    PyGraphicMaukaEngine_LinkPyType ();

    PYTYPE_READY_SUB ( MaukaEngine       , ToolEngine );
    PYTYPE_READY_SUB ( GraphicMaukaEngine, GraphicTool );
   
    // Identifier string can take up to 10 characters.
    __cs.addType ( "mauka", &PyTypeMaukaEngine, "<MaukaEngine>", false );


    PyObject* module = Py_InitModule ( "Mauka", PyMauka_Methods );
    if ( module == NULL ) {
      cerr << "[ERROR]\n"
           << "  Failed to initialize Mauka module." << endl;
      return;
    }

    Py_INCREF ( &PyTypeMaukaEngine );
    PyModule_AddObject ( module, "MaukaEngine", (PyObject*)&PyTypeMaukaEngine );
    Py_INCREF ( &PyTypeGraphicMaukaEngine );
    PyModule_AddObject ( module, "GraphicMaukaEngine", (PyObject*)&PyTypeGraphicMaukaEngine );
  }

  
} // End of extern "C".


#endif // End of Python Module Code Part.


}  // End of Mauka namespace.
