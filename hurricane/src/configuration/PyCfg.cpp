// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU/LIP6 2010-2020, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyCfg.cpp"                                   |
// +-----------------------------------------------------------------+


#include "hurricane/configuration/PyHurricane2.h"
#include "hurricane/configuration/PyConfiguration.h"


namespace Cfg2 {

  using std::cerr;
  using std::endl;
  using std::string;
  using namespace Isobar2;


#if !defined(__PYTHON_MODULE__)

// +=================================================================+
// |                "PyCfg" Shared Library Code Part                 |
// +=================================================================+


# else // PyCfg Shared Library Code Part.

// +=================================================================+
// |                 "PyCfg" Python Module Code Part                 |
// +=================================================================+


extern "C" {

  
  PyObject* ConstructorError = NULL;
  PyObject* ProxyError       = NULL;
  PyObject* HurricaneError   = NULL;
  PyObject* HurricaneWarning = NULL;


  static PyObject* PyCfg_get ( PyObject* args )
  {
    return staticMethod( "Cfg.get()", new PyFunctionWrapperAny<Configuration>(&Configuration::get), args );
  }


  static PyMethodDef PyCfg_Methods[] =
    { { "get"             , (PyCFunction)PyCfg_get, METH_VARARGS
                          , "Return the Configuration database singleton." }
    , {NULL, NULL, 0, NULL}  /* sentinel */
    };


  DL_EXPORT(void) initCfg2 ()
  {
    cdebug_log(20,0) << "initCfg2()" << endl;
    
    PyObject* module = Py_InitModule( "Cfg2", PyCfg_Methods );
    if (not module) {
      cerr << "[ERROR]\n"
           << "  Failed to initialize Cfg2 module." << endl;
      return;
    }
    PyObject* dictionnary = PyModule_GetDict( module );

    uint64_t flags = PyTypeManager::NoFlags;
    PyTypeManager::add<Configuration>( module, (PyMethodDef*)PyConfiguration_Methods, flags );
    
  //PyParameter_postModuleInit();

    ConstructorError = PyErr_NewException ( (char*)"hurricane.ConstructorError", NULL, NULL );
    ProxyError       = PyErr_NewException ( (char*)"hurricane.ProxyError"      , NULL, NULL );
    HurricaneError   = PyErr_NewException ( (char*)"hurricane.HurricaneError"  , NULL, NULL );
    HurricaneWarning = PyErr_NewException ( (char*)"hurricane.HurricaneWarning", PyExc_Warning, NULL );

    PyDict_SetItemString ( dictionnary, "ConstructorError", ConstructorError );
    PyDict_SetItemString ( dictionnary, "ProxyError"      , ProxyError );
    PyDict_SetItemString ( dictionnary, "HurricaneError"  , HurricaneError );

    cdebug_log(20,0) << "Cfg.so loaded " << (void*)&typeid(string) << endl;
  }

  
} // End of extern "C".


#endif // PyCfg Module Code Part.

}  // Cfg namespace.
