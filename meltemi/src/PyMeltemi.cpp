
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2014-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |   E t e s i a n  -  A n a l y t i c   P l a c e r               |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyMeltemi.cpp"                          |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/isobar/PyCell.h"
#include "meltemi/PyMeltemiEngine.h"


namespace Meltemi {

  using std::cerr;
  using std::endl;
  using Hurricane::tab;
  using Isobar::__cs;
  using Isobar::getPyHash;
  using CRL::PyTypeToolEngine;


#if !defined(__PYTHON_MODULE__)

// +=================================================================+
// |              "PyMeltemi" Shared Library Code Part               |
// +=================================================================+


# else // End of PyMeltemi Shared Library Code Part.


// +=================================================================+
// |               "PyMeltemi" Python Module Code Part               |
// +=================================================================+


extern "C" {


  // +-------------------------------------------------------------+
  // |                "PyMeltemi" Module Methods                   |
  // +-------------------------------------------------------------+


  static PyMethodDef PyMeltemi_Methods[] =
    { {NULL, NULL, 0, NULL}     /* sentinel */
    };


  static PyModuleDef  PyMeltemi_ModuleDef =
    { PyModuleDef_HEAD_INIT
    , "Meltemi"           /* m_name     */
    , "Standard cell analytical placer with routing-driven capability."
                          /* m_doc      */
    , -1                  /* m_size     */
    , PyMeltemi_Methods   /* m_methods  */
    , NULL                /* m_reload   */
    , NULL                /* m_traverse */
    , NULL                /* m_clear    */
    , NULL                /* m_free     */
    };


  // ---------------------------------------------------------------
  // Module Initialization  :  "initMeltemi ()"

  PyMODINIT_FUNC PyInit_Meltemi ( void )
  {
    cdebug_log(34,0) << "PyInit_Meltemi()" << endl;

    PyMeltemiEngine_LinkPyType();

    PYTYPE_READY_SUB( MeltemiEngine       , ToolEngine  );


    PyObject* module = PyModule_Create( &PyMeltemi_ModuleDef );
    if (module == NULL) {
      cerr << "[ERROR]\n"
           << "  Failed to initialize Meltemi module." << endl;
      return NULL;
    }

    Py_INCREF( &PyTypeMeltemiEngine );
    PyModule_AddObject( module, "MeltemiEngine", (PyObject*)&PyTypeMeltemiEngine );

    PyMeltemiEngine_postModuleInit();
    return module;
  }

  
} // End of extern "C".


#endif // End of Python Module Code Part.


}  // End of Meltemi namespace.
