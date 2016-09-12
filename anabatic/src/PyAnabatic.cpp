// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2016, All Rights Reserved
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


  // ---------------------------------------------------------------
  // Module Initialization  :  "initAnabatic ()"

  DL_EXPORT(void) initAnabatic () {
    cdebug_log(32,0) << "initAnabatic()" << endl;

    PyObject* module = Py_InitModule( "Anabatic", PyAnabatic_Methods );
    if (module == NULL) {
      cerr << "[ERROR]\n"
           << "  Failed to initialize Anabatic module." << endl;
      return;
    }

    PyObject* dictionnary = PyModule_GetDict(module);
    PyObject* constant;

    LoadObjectConstant( dictionnary,EngineLoadGrByNet        ,"EngineLoadGrByNet"         );
    LoadObjectConstant( dictionnary,EngineLoadGrByGCell      ,"EngineLoadGrByGCell"       );
    LoadObjectConstant( dictionnary,EngineLayerAssignByLength,"EngineLayerAssignByLength" );
    LoadObjectConstant( dictionnary,EngineLayerAssignByTrunk ,"EngineLayerAssignByTrunk"  );
    LoadObjectConstant( dictionnary,EngineNoNetLayerAssign   ,"EngineNoNetLayerAssign"    );
  }

  
} // End of extern "C".


#endif // End of Python Module Code Part.


}  // End of Anabatic namespace.
