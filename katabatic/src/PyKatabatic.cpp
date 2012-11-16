
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2012-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
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


# else // End of PyHurricane Shared Library Code Part.


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

    LoadObjectConstant(dictionnary,LoadGrByNet        ,"LoadGrByNet"        );
    LoadObjectConstant(dictionnary,LoadGrByGCell      ,"LoadGrByGCell"      );
    LoadObjectConstant(dictionnary,LayerAssignByLength,"LayerAssignByLength");
    LoadObjectConstant(dictionnary,LayerAssignByTrunk ,"LayerAssignByTrunk" );
    LoadObjectConstant(dictionnary,NoNetLayerAssign   ,"NoNetLayerAssign"   );
  }

  
} // extern "C".

#endif // End of Python Module Code Part.

}  // Katabatic namespace.
