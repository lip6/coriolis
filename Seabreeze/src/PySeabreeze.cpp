
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
// |  C++ Module  :       "./PySeabreeze.cpp"                         |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/isobar/PyCell.h"
#include "Seabreeze/PySeabreezeEngine.h"
//#include "Seabreeze/PyGraphicSeabreezeEngine.h"


namespace Seabreeze {

  using std::cerr;
  using std::endl;
  using Hurricane::tab;
  using Isobar::__cs;
  using Isobar::getPyHash;
  using CRL::PyTypeToolEngine;
//  using CRL::PyTypeGraphicTool;


#if !defined(__PYTHON_MODULE__)

// +=================================================================+
// |              "PySeabreeze" Shared Library Code Part              |
// +=================================================================+


# else // End of PyHurricane Shared Library Code Part.


// +=================================================================+
// |               "PySeabreeze" Python Module Code Part              |
// +=================================================================+


extern "C" {

  // +-------------------------------------------------------------+
  // |                "PySeabreeze" Module Methods                  |
  // +-------------------------------------------------------------+


  static PyMethodDef PySeabreeze_Methods[] =
    { {NULL, NULL, 0, NULL}     /* sentinel */
    };

  
  static PyModuleDef  PySeabreeze_ModuleDef =
    { PyModuleDef_HEAD_INIT
    , .m_name    = "Seabreeze"
    , .m_doc     = "Show how to interface Coriolis/C++ to Python."
    , .m_size    = -1
    , .m_methods = PySeabreeze_Methods
    };


  // ---------------------------------------------------------------
  // Module Initialization  :  "PyInit_Seabreeze ()"

  PyMODINIT_FUNC PyInit_Seabreeze ( void )
  {
    cdebug_log(40,0) << "PyInit_Seabreeze()" << endl;

    PySeabreezeEngine_LinkPyType();
  //PyGraphicSeabreezeEngine_LinkPyType();

    PYTYPE_READY_SUB( SeabreezeEngine       , ToolEngine  );
  //PYTYPE_READY_SUB( GraphicSeabreezeEngine, GraphicTool );

    PyObject* module = PyModule_Create( &PySeabreeze_ModuleDef );
    if (module == NULL) {
      cerr << "[ERROR]\n"
           << "  Failed to initialize Seabreeze module." << endl;
      return NULL;
    }

    Py_INCREF( &PyTypeSeabreezeEngine );
    PyModule_AddObject( module, "SeabreezeEngine", (PyObject*)&PyTypeSeabreezeEngine );
  //Py_INCREF( &PyTypeGraphicSeabreezeEngine );
  //PyModule_AddObject( module, "GraphicSeabreezeEngine", (PyObject*)&PyTypeGraphicSeabreezeEngine );

    return module;
  }

  
} // End of extern "C".

#endif // Python Module Code Part.

}  // Seabreeze namespace.
