// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |     A n a b a t i c  -  Global Routing Toolbox                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyAnabatic.cpp"                         |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/isobar/PyCell.h"
#include "anabatic/PyAnabaticEngine.h"
#include "anabatic/PyGraphicAnabaticEngine.h"


namespace Anabatic {

  using std::cerr;
  using std::endl;
  using Hurricane::tab;
  using Isobar::__cs;
  using CRL::PyTypeToolEngine;
  using CRL::PyTypeGraphicTool;


#if !defined(__PYTHON_MODULE__)

// +=================================================================+
// |              "PyAnabatic" Shared Library Code Part              |
// +=================================================================+


# else // End of PyHurricane Shared Library Code Part.


// +=================================================================+
// |               "PyAnabatic" Python Module Code Part              |
// +=================================================================+


extern "C" {


  // +-------------------------------------------------------------+
  // |                "PyAnabatic" Module Methods                  |
  // +-------------------------------------------------------------+


  static PyMethodDef PyAnabatic_Methods[] =
    { {NULL, NULL, 0, NULL}     /* sentinel */
    };


  // ---------------------------------------------------------------
  // Module Initialization  :  "initAnabatic ()"

  DL_EXPORT(void) initAnabatic () {
    cdebug_log(32,0) << "initAnabatic()" << endl;

    PyAnabaticEngine_LinkPyType();
    PyGraphicAnabaticEngine_LinkPyType();

    PYTYPE_READY_SUB( AnabaticEngine       , ToolEngine  );
    PYTYPE_READY_SUB( GraphicAnabaticEngine, GraphicTool );


    PyObject* module = Py_InitModule( "Anabatic", PyAnabatic_Methods );
    if (module == NULL) {
      cerr << "[ERROR]\n"
           << "  Failed to initialize Anabatic module." << endl;
      return;
    }

    Py_INCREF( &PyTypeAnabaticEngine );
    PyModule_AddObject( module, "AnabaticEngine", (PyObject*)&PyTypeAnabaticEngine );
    Py_INCREF( &PyTypeGraphicAnabaticEngine );
    PyModule_AddObject( module, "GraphicAnabaticEngine", (PyObject*)&PyTypeGraphicAnabaticEngine );
  }

  
} // End of extern "C".


#endif // End of Python Module Code Part.


}  // End of Anabatic namespace.
