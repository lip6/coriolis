
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2012-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |      N i m b u s  -  Global Routing Framework                   |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyNimbus.cpp"                           |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/isobar/PyCell.h"
#include "nimbus/PyNimbusEngine.h"


namespace Nimbus {

  using std::cerr;
  using std::endl;
  using Hurricane::tab;
  using Hurricane::in_trace;
  using Isobar::__cs;
  using CRL::PyTypeToolEngine;


#if !defined(__PYTHON_MODULE__)

// x=================================================================x
// |               "PyNimbus" Shared Library Code Part               |
// x=================================================================x


# else // End of PyHurricane Shared Library Code Part.


// x=================================================================x
// |                "PyNimbus" Python Module Code Part               |
// x=================================================================x


extern "C" {


  // x-------------------------------------------------------------x
  // |                 "PyNimbus" Module Methods                   |
  // x-------------------------------------------------------------x


  static PyMethodDef PyNimbus_Methods[] =
    { {NULL, NULL, 0, NULL}     /* sentinel */
    };




  // ---------------------------------------------------------------
  // Module Initialization  :  "initNimbus ()"

  DL_EXPORT(void) initNimbus () {
    trace << "initNimbus()" << endl;

    PyNimbusEngine_LinkPyType ();

    PYTYPE_READY_SUB ( NimbusEngine, ToolEngine );


    PyObject* module = Py_InitModule ( "Nimbus", PyNimbus_Methods );
    if ( module == NULL ) {
      cerr << "[ERROR]\n"
           << "  Failed to initialize Nimbus module." << endl;
      return;
    }

    Py_INCREF ( &PyTypeNimbusEngine );
    PyModule_AddObject ( module, "NimbusEngine", (PyObject*)&PyTypeNimbusEngine );
  }

  
} // End of extern "C".

#endif // End of Python Module Code Part.

}  // End of Nimbus namespace.
