
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2017-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |        T o o l E n g i n e   T u t o r i a l                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyTutorial.cpp"                         |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/isobar/PyCell.h"
#include "tutorial/PyTutorialEngine.h"
#include "tutorial/PyGraphicTutorialEngine.h"


namespace Tutorial {

  using std::cerr;
  using std::endl;
  using Hurricane::tab;
  using Isobar::__cs;
  using Isobar::getPyHash;
  using CRL::PyTypeToolEngine;
  using CRL::PyTypeGraphicTool;


#if !defined(__PYTHON_MODULE__)

// +=================================================================+
// |              "PyTutorial" Shared Library Code Part              |
// +=================================================================+


# else // End of PyHurricane Shared Library Code Part.


// +=================================================================+
// |               "PyTutorial" Python Module Code Part              |
// +=================================================================+


extern "C" {

  // +-------------------------------------------------------------+
  // |                "PyTutorial" Module Methods                  |
  // +-------------------------------------------------------------+


  static PyMethodDef PyTutorial_Methods[] =
    { {NULL, NULL, 0, NULL}     /* sentinel */
    };

  
  static PyModuleDef  PyTutorial_ModuleDef =
    { PyModuleDef_HEAD_INIT
    , .m_name    = "Tutorial"
    , .m_doc     = "Show how to interface Coriolis/C++ to Python."
    , .m_size    = -1
    , .m_methods = PyTutorial_Methods
    };


  // ---------------------------------------------------------------
  // Module Initialization  :  "PyInit_Tutorial ()"

  PyMODINIT_FUNC PyInit_Tutorial ( void )
  {
    cdebug_log(40,0) << "PyInit_Tutorial()" << endl;

    PyTutorialEngine_LinkPyType();
    PyGraphicTutorialEngine_LinkPyType();

    PYTYPE_READY_SUB( TutorialEngine       , ToolEngine  );
    PYTYPE_READY_SUB( GraphicTutorialEngine, GraphicTool );

    PyObject* module = PyModule_Create( &PyTutorial_ModuleDef );
    if (module == NULL) {
      cerr << "[ERROR]\n"
           << "  Failed to initialize Tutorial module." << endl;
      return NULL;
    }

    Py_INCREF( &PyTypeTutorialEngine );
    PyModule_AddObject( module, "TutorialEngine", (PyObject*)&PyTypeTutorialEngine );
    Py_INCREF( &PyTypeGraphicTutorialEngine );
    PyModule_AddObject( module, "GraphicTutorialEngine", (PyObject*)&PyTypeGraphicTutorialEngine );

    return module;
  }

  
} // End of extern "C".

#endif // Python Module Code Part.

}  // Tutorial namespace.
