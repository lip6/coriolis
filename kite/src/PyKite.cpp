
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2012-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyKite.cpp"                             |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/isobar/PyCell.h"
#include "kite/PyKiteEngine.h"
#include "kite/PyGraphicKiteEngine.h"


namespace Kite {

  using std::cerr;
  using std::endl;
  using Hurricane::tab;
  using Isobar::__cs;
  using Isobar::getPyHash;
  using CRL::PyTypeToolEngine;
  using CRL::PyTypeGraphicTool;


#if !defined(__PYTHON_MODULE__)

// x=================================================================x
// |                "PyKite" Shared Library Code Part                |
// x=================================================================x


# else // End of PyHurricane Shared Library Code Part.


// x=================================================================x
// |                 "PyKite" Python Module Code Part                |
// x=================================================================x


extern "C" {


  // x-------------------------------------------------------------x
  // |                  "PyKite" Module Methods                    |
  // x-------------------------------------------------------------x


  static PyMethodDef PyKite_Methods[] =
    { {NULL, NULL, 0, NULL}     /* sentinel */
    };


  static PyModuleDef  PyKite_ModuleDef =
    { PyModuleDef_HEAD_INIT
    , .m_name    = "Kite"
    , .m_doc     = "Detailed router."
    , .m_size    = -1
    , .m_methods = PyKite_Methods
    };


  // ---------------------------------------------------------------
  // Module Initialization  :  "PyInit_Kite ()"

  PyMODINIT_FUNC PyInit_Kite ( void )
  {
    cdebug_log(40,0) << "PyInit_Kite()" << endl;

    PyKiteEngine_LinkPyType();
    PyGraphicKiteEngine_LinkPyType();

    PYTYPE_READY_SUB( KiteEngine       , ToolEngine  );
    PYTYPE_READY_SUB( GraphicKiteEngine, GraphicTool );

    PyObject* module = PyModule_Create( &PyKite_ModuleDef );
    if (module == NULL) {
      cerr << "[ERROR]\n"
           << "  Failed to initialize Kite module." << endl;
      return NULL;
    }

    Py_INCREF( &PyTypeKiteEngine );
    PyModule_AddObject( module, "KiteEngine", (PyObject*)&PyTypeKiteEngine );
    Py_INCREF( &PyTypeGraphicKiteEngine );
    PyModule_AddObject( module, "GraphicKiteEngine", (PyObject*)&PyTypeGraphicKiteEngine );

    PyObject* dictionnary = PyModule_GetDict( module );
    PyObject* constant;

    LoadObjectConstant( dictionnary, KtBuildGlobalRouting, "KtBuildGlobalRouting" );
    LoadObjectConstant( dictionnary, KtLoadGlobalRouting , "KtLoadGlobalRouting"  );

    return module;
  }

  
} // End of extern "C".


#endif // End of Python Module Code Part.


}  // End of Kite namespace.
