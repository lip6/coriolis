// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |     A n a b a t i c  -  Global Routing Toolbox                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyAnabatic.cpp"                         |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/isobar/PyCell.h"
#include "anabatic/Constants.h"


namespace Anabatic {

  using std::cerr;
  using std::endl;
  using Hurricane::tab;
  using Isobar::__cs;


#if !defined(__PYTHON_MODULE__)

// +=================================================================+
// |              "PyAnabatic" Shared Library Code Part              |
// +=================================================================+


# else // End of PyHurricane Shared Library Code Part.


// +=================================================================+
// |               "PyAnabatic" Python Module Code Part              |
// +=================================================================+


extern "C" {


  // +-------------------------------------------------------------+
  // |                "PyAnabatic" Module Methods                  |
  // +-------------------------------------------------------------+


  static PyMethodDef PyAnabatic_Methods[] =
    { {NULL, NULL, 0, NULL}     /* sentinel */
    };


  static PyModuleDef  PyAnabatic_ModuleDef =
    { PyModuleDef_HEAD_INIT
    , "Anabatic"          /* m_name     */
    , "Low level database for global & detailed routing."
                          /* m_doc      */
    , -1                  /* m_size     */
    , PyAnabatic_Methods  /* m_methods  */
    , NULL                /* m_reload   */
    , NULL                /* m_traverse */
    , NULL                /* m_clear    */
    , NULL                /* m_free     */
    };


  // ---------------------------------------------------------------
  // Module Initialization  :  "initAnabatic ()"

  PyMODINIT_FUNC PyInit_Anabatic ( void )
  {
    cdebug_log(32,0) << "PyInit_Anabatic()" << endl;

    PyObject* module = PyModule_Create( &PyAnabatic_ModuleDef );
    if (module == NULL) {
      cerr << "[ERROR]\n"
           << "  Failed to initialize Anabatic module." << endl;
      return NULL;
    }

    PyObject* dictionnary = PyModule_GetDict(module);
    PyObject* constant;

    LoadObjectConstant( dictionnary,EngineLoadGrByNet           ,"EngineLoadGrByNet"            );
    LoadObjectConstant( dictionnary,EngineLoadGrByGCell         ,"EngineLoadGrByGCell"          );
    LoadObjectConstant( dictionnary,EngineLayerAssignByLength   ,"EngineLayerAssignByLength"    );
    LoadObjectConstant( dictionnary,EngineLayerAssignByTrunk    ,"EngineLayerAssignByTrunk"     );
    LoadObjectConstant( dictionnary,EngineLayerAssignNoGlobalM2V,"EngineLayerAssignNoGlobalM2V" );
    LoadObjectConstant( dictionnary,EngineNoNetLayerAssign      ,"EngineNoNetLayerAssign"       );

    return module;
  }

  
} // End of extern "C".


#endif // End of Python Module Code Part.


}  // End of Anabatic namespace.
