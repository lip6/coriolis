
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2012-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          U n i c o r n  -  M a i n   G U I                      |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyUnicorn.cpp"                               |
// +-----------------------------------------------------------------+


#include "unicorn/PyUnicornGui.h"


namespace Unicorn {

  using std::cerr;
  using std::endl;
  using Hurricane::tab;
  using Hurricane::CellViewer;
  using Hurricane::PyTypeCellViewer;
  using Isobar::__cs;


#if !defined(__PYTHON_MODULE__)

// +=================================================================+
// |              "PyUnicorn" Shared Library Code Part               |
// +=================================================================+


# else // End of PyHurricane Shared Library Code Part.


// +=================================================================+
// |               "PyUnicorn" Python Module Code Part               |
// +=================================================================+


extern "C" {


  static PyMethodDef PyUnicorn_Methods[] =
    { {NULL, NULL, 0, NULL}     /* sentinel */
    };


  static PyModuleDef  PyUnicorn_ModuleDef =
    { PyModuleDef_HEAD_INIT
    , .m_name    = "Unicorn"
    , .m_doc     = "Coriolis Visualizer & Debugger."
    , .m_size    = -1
    , .m_methods = PyUnicorn_Methods
    };


  // ---------------------------------------------------------------
  // Module Initialization  :  "PyInit_Unicorn ()"

  PyMODINIT_FUNC PyInit_Unicorn ( void )
  {
    cdebug_log(46,0) << "PyInit_Unicorn()" << endl;

    PyUnicornGui_LinkPyType ();

    PYTYPE_READY_SUB ( UnicornGui, CellViewer );

    PyObject* module = PyModule_Create( &PyUnicorn_ModuleDef );
    if ( module == NULL ) {
      cerr << "[ERROR]\n"
           << "  Failed to initialize Unicorn module." << endl;
      return NULL;
    }

    Py_INCREF ( &PyTypeUnicornGui );
    PyModule_AddObject ( module, "UnicornGui", (PyObject*)&PyTypeUnicornGui );

    return module;
  }

  
} // End of extern "C".

#endif // Python Module Code Part.

}  // Unicorn namespace.
