// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2007-2023, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |       T r a m o n t a n a  -  Extractor & LVX                   |
// |                                                                 |
// |  Algorithm    :                   Christian MASSON              |
// |  First impl.  :                           Yifei WU              |
// |  Second impl. :                   Jean-Paul CHAPUT              |
// |  E-mail       :           Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module   :  "./PyTramontana.cpp"                           |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/isobar/PyCell.h"
#include "tramontana/PyTramontanaEngine.h"
#include "tramontana/PyGraphicTramontanaEngine.h"


namespace Tramontana {

  using std::cerr;
  using std::endl;
  using Hurricane::tab;
  using Isobar::getPyHash;
  using Isobar::__cs;
  using CRL::PyTypeToolEngine;
  using CRL::PyTypeGraphicTool;


#if !defined(__PYTHON_MODULE__)

// +=================================================================+
// |             "PyTramontana" Shared Library Code Part             |
// +=================================================================+


# else // End of PyHurricane Shared Library Code Part.


// +=================================================================+
// |              "PyTramontana" Python Module Code Part             |
// +=================================================================+


extern "C" {


  static PyMethodDef PyTramontana_Methods[] =
    { {NULL, NULL, 0, NULL}     /* sentinel */
    };


  static PyModuleDef  PyTramontana_ModuleDef =
    { PyModuleDef_HEAD_INIT
    , .m_name    = "Tramontana"
    , .m_doc     = "Layout extractor & LVX."
    , .m_size    = -1
    , .m_methods = PyTramontana_Methods
    };




  // ---------------------------------------------------------------
  // Module Initialization  :  "PyInit_Tramontana ()"

  PyMODINIT_FUNC PyInit_Tramontana ( void )
  {
    cdebug_log(40,0) << "PyInit_Tramontana()" << endl;

    PyTramontanaEngine_LinkPyType();
    PyGraphicTramontanaEngine_LinkPyType();

    PYTYPE_READY_SUB( TramontanaEngine       , ToolEngine  );
    PYTYPE_READY_SUB( GraphicTramontanaEngine, GraphicTool );


    PyObject* module = PyModule_Create( &PyTramontana_ModuleDef );
    if (module == NULL) {
      cerr << "[ERROR]\n"
           << "  Failed to initialize Tramontana module." << endl;
      return NULL;
    }

    Py_INCREF( &PyTypeTramontanaEngine );
    PyModule_AddObject( module, "TramontanaEngine", (PyObject*)&PyTypeTramontanaEngine );
    Py_INCREF( &PyTypeGraphicTramontanaEngine );
    PyModule_AddObject( module, "GraphicTramontanaEngine", (PyObject*)&PyTypeGraphicTramontanaEngine );

  //PyTramontanaEngine_postModuleInit();
    return module;
  }

  
} // End of extern "C".


#endif // End of Python Module Code Part.


}  // End of Tramontana namespace.
