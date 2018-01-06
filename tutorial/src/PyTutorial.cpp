
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2017-2018, All Rights Reserved
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


  // ---------------------------------------------------------------
  // Module Initialization  :  "initTutorial ()"

  DL_EXPORT(void) initTutorial () {
    cdebug_log(40,0) << "initTutorial()" << endl;

    PyTutorialEngine_LinkPyType();
    PyGraphicTutorialEngine_LinkPyType();

    PYTYPE_READY_SUB( TutorialEngine       , ToolEngine  );
    PYTYPE_READY_SUB( GraphicTutorialEngine, GraphicTool );

    PyObject* module = Py_InitModule( "Tutorial", PyTutorial_Methods );
    if (module == NULL) {
      cerr << "[ERROR]\n"
           << "  Failed to initialize Tutorial module." << endl;
      return;
    }

    Py_INCREF( &PyTypeTutorialEngine );
    PyModule_AddObject( module, "TutorialEngine", (PyObject*)&PyTypeTutorialEngine );
    Py_INCREF( &PyTypeGraphicTutorialEngine );
    PyModule_AddObject( module, "GraphicTutorialEngine", (PyObject*)&PyTypeGraphicTutorialEngine );
  }

  
} // End of extern "C".

#endif // Python Module Code Part.

}  // Tutorial namespace.
