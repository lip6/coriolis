
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2022-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |              F o e h n  -  DAG Toolbox                          |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyFoehn.cpp"                            |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/isobar/PyCell.h"
#include "foehn/PyFoehnEngine.h"
//#include "foehn/PyGraphicFoehnEngine.h"
#include "foehn/PyDagExtension.h"
#include "foehn/PyDag.h"


namespace Foehn {

  using std::cerr;
  using std::endl;
  using Hurricane::tab;
  using Isobar::getPyHash;
  using Isobar::__cs;
  using CRL::PyTypeToolEngine;
//using CRL::PyTypeGraphicTool;


#if !defined(__PYTHON_MODULE__)

// +=================================================================+
// |               "PyFoehn" Shared Library Code Part                |
// +=================================================================+


# else // End of PyHurricane Shared Library Code Part.


// +=================================================================+
// |                "PyFoehn" Python Module Code Part                |
// +=================================================================+


extern "C" {


  static PyMethodDef PyFoehn_Methods[] =
    { {NULL, NULL, 0, NULL}     /* sentinel */
    };


  static PyModuleDef  PyFoehn_ModuleDef =
    { PyModuleDef_HEAD_INIT
    , .m_name    = "Foehn"
    , .m_doc     = "A DAG Toolbox."
    , .m_size    = -1
    , .m_methods = PyFoehn_Methods
    };




  // ---------------------------------------------------------------
  // Module Initialization  :  "PyInit_Foehn()"

  PyMODINIT_FUNC PyInit_Foehn ( void )
  {
    cdebug_log(40,0) << "PyInit_Foehn()" << endl;

 // PyFoehnFlags_LinkPyType();
    PyFoehnEngine_LinkPyType();
 // PyGraphicFoehnEngine_LinkPyType();
    PyDagExtension_LinkPyType();
    PyDag_LinkPyType();

 // PYTYPE_READY    ( FoehnFlags );
    PYTYPE_READY_SUB( FoehnEngine       , ToolEngine  );
 // PYTYPE_READY_SUB( GraphicFoehnEngine, GraphicTool );
    PYTYPE_READY    ( DagExtension );
    PYTYPE_READY    ( Dag );


    PyObject* module = PyModule_Create( &PyFoehn_ModuleDef );
    if (module == NULL) {
      cerr << "[ERROR]\n"
           << "  Failed to initialize Foehn module." << endl;
      return NULL;
    }

    Py_INCREF( &PyTypeFoehnEngine );
    PyModule_AddObject( module, "FoehnEngine", (PyObject*)&PyTypeFoehnEngine );
 // Py_INCREF( &PyTypeGraphicFoehnEngine );
 // PyModule_AddObject( module, "GraphicFoehnEngine", (PyObject*)&PyTypeGraphicFoehnEngine );
 // Py_INCREF( &PyTypeFoehnFlags );
 // PyModule_AddObject( module, "Flags", (PyObject*)&PyTypeFoehnFlags );
    Py_INCREF( &PyTypeDagExtension );
    PyModule_AddObject( module, "DagExtension", (PyObject*)&PyTypeDagExtension );
    Py_INCREF( &PyTypeDag );
    PyModule_AddObject( module, "Dag", (PyObject*)&PyTypeDag );

    PyFoehnEngine_postModuleInit();
    PyDagExtension_postModuleInit();
    return module;
  }

  
} // extern "C".


#endif // Python Module Code Part.


}  // Foehn namespace.
