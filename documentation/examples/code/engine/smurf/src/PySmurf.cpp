
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
// |  C++ Module  :       "./PySmurf.cpp"                            |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/isobar/PyCell.h"
#include "smurf/PySmurfEngine.h"
#include "smurf/PyGraphicSmurfEngine.h"


namespace Smurf {

  using std::cerr;
  using std::endl;
  using Hurricane::tab;
  using Hurricane::in_trace;
  using Isobar::__cs;
  using CRL::PyTypeToolEngine;
  using CRL::PyTypeGraphicTool;


#if !defined(__PYTHON_MODULE__)

// +=================================================================+
// |              "PySmurf" Shared Library Code Part               |
// +=================================================================+


# else // End of PyHurricane Shared Library Code Part.


// +=================================================================+
// |               "PySmurf" Python Module Code Part               |
// +=================================================================+


extern "C" {


  // +-------------------------------------------------------------+
  // |                "PySmurf" Module Methods                   |
  // +-------------------------------------------------------------+


  static PyMethodDef PySmurf_Methods[] =
    { {NULL, NULL, 0, NULL}     /* sentinel */
    };




  // ---------------------------------------------------------------
  // Module Initialization  :  "initSmurf ()"

  DL_EXPORT(void) initSmurf () {
    trace << "initSmurf()" << endl;

    PySmurfEngine_LinkPyType();
    PyGraphicSmurfEngine_LinkPyType();

    PYTYPE_READY_SUB( SmurfEngine       , ToolEngine  );
    PYTYPE_READY_SUB( GraphicSmurfEngine, GraphicTool );


    PyObject* module = Py_InitModule( "Smurf", PySmurf_Methods );
    if (module == NULL) {
      cerr << "[ERROR]\n"
           << "  Failed to initialize Smurf module." << endl;
      return;
    }

    Py_INCREF( &PyTypeSmurfEngine );
    PyModule_AddObject( module, "SmurfEngine", (PyObject*)&PyTypeSmurfEngine );
    Py_INCREF( &PyTypeGraphicSmurfEngine );
    PyModule_AddObject( module, "GraphicSmurfEngine", (PyObject*)&PyTypeGraphicSmurfEngine );

  //PyObject* dictionnary = PyModule_GetDict( module );
  //PyObject* constant;

  //LoadObjectConstant( dictionnary, KtBuildGlobalRouting, "KtBuildGlobalRouting" );
  //LoadObjectConstant( dictionnary, KtLoadGlobalRouting , "KtLoadGlobalRouting"  );
  }

  
} // End of extern "C".


#endif // End of Python Module Code Part.


}  // End of Smurf namespace.
