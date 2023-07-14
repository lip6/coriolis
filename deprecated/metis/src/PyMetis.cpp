
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
// |  C++ Module  :       "./PyMetis.cpp"                           |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/isobar/PyCell.h"
#include "metis/PyMetisEngine.h"


namespace Metis {

  using std::cerr;
  using std::endl;
  using Hurricane::tab;
  using Hurricane::in_trace;
  using Isobar::__cs;
  using CRL::PyTypeToolEngine;


#if !defined(__PYTHON_MODULE__)

// x=================================================================x
// |               "PyMetis" Shared Library Code Part                |
// x=================================================================x


# else // End of PyHurricane Shared Library Code Part.


extern "C" {

// x=================================================================x
// |                "PyMetis" Python Module Code Part                |
// x=================================================================x


  static PyMethodDef PyMetis_Methods[] =
    { {NULL, NULL, 0, NULL}     /* sentinel */
    };




  // ---------------------------------------------------------------
  // Module Initialization  :  "initMetis ()"

  DL_EXPORT(void) initMetis () {
    trace << "initMetis()" << endl;

    PyMetisEngine_LinkPyType ();

    PYTYPE_READY_SUB ( MetisEngine, ToolEngine );


    PyObject* module = Py_InitModule ( "Metis", PyMetis_Methods );
    if ( module == NULL ) {
      cerr << "[ERROR]\n"
           << "  Failed to initialize Metis module." << endl;
      return;
    }

    Py_INCREF ( &PyTypeMetisEngine );
    PyModule_AddObject ( module, "MetisEngine", (PyObject*)&PyTypeMetisEngine );
  }

  
} // End of extern "C".

#endif // End of Python Module Code Part.

}  // End of Metis namespace.
