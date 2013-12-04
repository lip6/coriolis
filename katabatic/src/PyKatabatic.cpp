
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2012-2013, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |        K a t a b a t i c  -  Routing Toolbox                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyKatabatic.cpp"                             |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyHurricane.h"
#include "katabatic/KatabaticEngine.h"


namespace Katabatic {

  using std::cerr;
  using std::endl;
  using Hurricane::tab;
  using Hurricane::in_trace;
  using Isobar::__cs;


#if !defined(__PYTHON_MODULE__)

// x=================================================================x
// |             "PyKatabatic" Shared Library Code Part              |
// x=================================================================x


# else // PyHurricane Shared Library Code Part.


// x=================================================================x
// |              "PyKatabatic" Python Module Code Part              |
// x=================================================================x

extern "C" {


  static PyMethodDef PyKatabatic_Methods[] =
    { {NULL, NULL, 0, NULL}     /* sentinel */
    };




  // ---------------------------------------------------------------
  // Module Initialization  :  "initKatabatic ()"

  DL_EXPORT(void) initKatabatic () {
    trace << "initKatabatic()" << endl;

    PyObject* module = Py_InitModule ( "Katabatic", PyKatabatic_Methods );
    if ( module == NULL ) {
      cerr << "[ERROR]\n"
           << "  Failed to initialize Katabatic module." << endl;
      return;
    }

    PyObject* dictionnary = PyModule_GetDict(module);
    PyObject* constant;

    LoadObjectConstant(dictionnary,EngineLoadGrByNet        ,"EngineLoadGrByNet"        );
    LoadObjectConstant(dictionnary,EngineLoadGrByGCell      ,"EngineLoadGrByGCell"      );
    LoadObjectConstant(dictionnary,EngineLayerAssignByLength,"EngineLayerAssignByLength");
    LoadObjectConstant(dictionnary,EngineLayerAssignByTrunk ,"EngineLayerAssignByTrunk" );
    LoadObjectConstant(dictionnary,EngineNoNetLayerAssign   ,"EngineNoNetLayerAssign"   );
  }

  
} // extern "C".

#endif // Python Module Code Part.

}  // Katabatic namespace.
