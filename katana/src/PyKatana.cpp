
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2012-2013, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyKatana.cpp"                           |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/isobar/PyCell.h"
#include "katana/PyKatanaFlags.h"
#include "katana/PyKatanaEngine.h"
#include "katana/PyGraphicKatanaEngine.h"


namespace Katana {

  using std::cerr;
  using std::endl;
  using Hurricane::tab;
  using Isobar::getPyHash;
  using Isobar::__cs;
  using CRL::PyTypeToolEngine;
  using CRL::PyTypeGraphicTool;


#if !defined(__PYTHON_MODULE__)

// x=================================================================x
// |               "PyKatana" Shared Library Code Part               |
// x=================================================================x


# else // End of PyHurricane Shared Library Code Part.


// x=================================================================x
// |                "PyKatana" Python Module Code Part               |
// x=================================================================x


extern "C" {


  // x-------------------------------------------------------------x
  // |                 "PyKatana" Module Methods                   |
  // x-------------------------------------------------------------x


  static PyMethodDef PyKatana_Methods[] =
    { {NULL, NULL, 0, NULL}     /* sentinel */
    };




  // ---------------------------------------------------------------
  // Module Initialization  :  "initKatana ()"

  DL_EXPORT(void) initKatana () {
    cdebug_log(40,0) << "initKatana()" << endl;

    PyKatanaFlags_LinkPyType();
    PyKatanaEngine_LinkPyType();
    PyGraphicKatanaEngine_LinkPyType();

    PYTYPE_READY    ( KatanaFlags );
    PYTYPE_READY_SUB( KatanaEngine       , ToolEngine  );
    PYTYPE_READY_SUB( GraphicKatanaEngine, GraphicTool );


    PyObject* module = Py_InitModule( "Katana", PyKatana_Methods );
    if (module == NULL) {
      cerr << "[ERROR]\n"
           << "  Failed to initialize Katana module." << endl;
      return;
    }

    Py_INCREF( &PyTypeKatanaEngine );
    PyModule_AddObject( module, "KatanaEngine", (PyObject*)&PyTypeKatanaEngine );
    Py_INCREF( &PyTypeGraphicKatanaEngine );
    PyModule_AddObject( module, "GraphicKatanaEngine", (PyObject*)&PyTypeGraphicKatanaEngine );
    Py_INCREF( &PyTypeKatanaFlags );
    PyModule_AddObject( module, "Flags", (PyObject*)&PyTypeKatanaFlags );
    // PyObject* dictionnary = PyModule_GetDict( module );
    // PyObject* constant;

    // LoadObjectConstant( dictionnary, KtBuildGlobalRouting, "KtBuildGlobalRouting" );
    // LoadObjectConstant( dictionnary, KtLoadGlobalRouting , "KtLoadGlobalRouting"  );

    PyKatanaEngine_postModuleInit();
  }

  
} // End of extern "C".


#endif // End of Python Module Code Part.


}  // End of Katana namespace.
