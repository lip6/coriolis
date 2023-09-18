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


#include "hurricane/configuration/PyTypeManager.h"
#include "hurricane/configuration/PyConfiguration.h"
#include "hurricane/configuration/PyParameter.h"
#include "hurricane/configuration/PyLayoutDescription.h"


namespace Cfg {

  using std::cerr;
  using std::endl;
  using std::string;
  using namespace Isobar3;


extern "C" {


  Parameter* getParamString1     ( std::string id ) { return getParamString(id); }
  Parameter* getParamBool1       ( std::string id ) { return getParamBool(id); }
  Parameter* getParamInt1        ( std::string id ) { return getParamInt(id); }
  Parameter* getParamEnumerate1  ( std::string id ) { return getParamEnumerate(id); }
  Parameter* getParamDouble1     ( std::string id ) { return getParamDouble(id); }
  Parameter* getParamPercentage1 ( std::string id ) { return getParamPercentage(id); }

  Parameter* getParamString2     ( std::string id, std::string value ) { return getParamString(id,value); }
  Parameter* getParamInt2        ( std::string id, int         value ) { return getParamInt(id,value); }
  Parameter* getParamEnumerate2  ( std::string id, int         value ) { return getParamEnumerate(id,value); }
  Parameter* getParamBool2       ( std::string id, bool        value ) { return getParamBool(id,value); }
  Parameter* getParamDouble2     ( std::string id, double      value ) { return getParamDouble(id,value); }
  Parameter* getParamPercentage2 ( std::string id, double      value ) { return getParamPercentage(id,value); }


  static PyObject* PyCfg_hasParameter ( PyObject* module, PyObject* args )
  { return callFunction("hasParameter",&hasParameter,args); }


  static PyObject* PyCfg_getParamString ( PyObject* self, PyObject* args )
  {
    PyObject*       rvalue = callFunction("getParamString",&getParamString2,args);
    if (not rvalue) rvalue = callFunction("getParamString",&getParamString1,args);
    return rvalue;
  }


  static PyObject* PyCfg_getParamBool ( PyObject* self, PyObject* args )
  {
    PyObject*       rvalue = callFunction("getParamBool",&getParamBool2,args);
    if (not rvalue) rvalue = callFunction("getParamBool",&getParamBool1,args);
    return rvalue;
  }


  static PyObject* PyCfg_getParamInt ( PyObject* self, PyObject* args )
  {
    PyObject*       rvalue = callFunction("getParamInt",&getParamInt2,args);
    if (not rvalue) rvalue = callFunction("getParamInt",&getParamInt1,args);
    return rvalue;
  }


  static PyObject* PyCfg_getParamEnumerate ( PyObject* self, PyObject* args )
  {
    PyObject*       rvalue = callFunction("getParamEnumerate",&getParamEnumerate2,args);
    if (not rvalue) rvalue = callFunction("getParamEnumerate",&getParamEnumerate1,args);
    return rvalue;
  }


  static PyObject* PyCfg_getParamDouble ( PyObject* self, PyObject* args )
  {
    PyObject*       rvalue = callFunction("getParamDouble",&getParamDouble2,args);
    if (not rvalue) rvalue = callFunction("getParamDouble",&getParamDouble1,args);
    return rvalue;
  }


  static PyObject* PyCfg_getParamPercentage ( PyObject* self, PyObject* args )
  {
    PyObject*       rvalue = callFunction("getParamPercentage",&getParamPercentage2,args);
    if (not rvalue) rvalue = callFunction("getParamPercentage",&getParamPercentage1,args);
    return rvalue;
  }


  static PyMethodDef PyCfg_Methods[] =
    { { "hasParameter"      , (PyCFunction)PyCfg_hasParameter      , METH_VARARGS
                            , "Return the Configuration database singleton." }
    , { "getParamString"    , (PyCFunction)PyCfg_getParamString    , METH_VARARGS
                            , "Return the named string parameter (if it exists)." }
    , { "getParamBool"      , (PyCFunction)PyCfg_getParamBool      , METH_VARARGS
                            , "Return the named string parameter (if it exists)." }
    , { "getParamInt"       , (PyCFunction)PyCfg_getParamInt       , METH_VARARGS
                            , "Return the named string parameter (if it exists)." }
    , { "getParamEnumerate" , (PyCFunction)PyCfg_getParamEnumerate , METH_VARARGS
                            , "Return the named string parameter (if it exists)." }
    , { "getParamDouble"    , (PyCFunction)PyCfg_getParamDouble    , METH_VARARGS
                            , "Return the named string parameter (if it exists)." }
    , { "getParamPercentage", (PyCFunction)PyCfg_getParamPercentage, METH_VARARGS
                            , "Return the named string parameter (if it exists)." }
    , {NULL, NULL, 0, NULL}  /* sentinel */
    };


  static PyModuleDef  PyCfg_ModuleDef =
    { PyModuleDef_HEAD_INIT
    , .m_name    = "Cfg"
    , .m_doc     = "Coriolis configuration database."
    , .m_size    = -1
    , .m_methods = PyCfg_Methods
    };


  PyMODINIT_FUNC PyInit_Cfg ( void )
  {
    cdebug_log(20,0) << "PyInit_Cfg() (Python3+template)" << endl;
    cerr << "PyInit_Cfg() (Python3+template)" << endl;
    
    PyObject* module = PyModule_Create( &PyCfg_ModuleDef );
    if (not module) {
      cerr << "[ERROR]\n"
           << "  Failed to initialize Cfg module." << endl;
      return NULL;
    }
    PyObject* dictionnary = PyModule_GetDict( module );

    PyTypeManagerNonDBo<Configuration    >::create( module
                                                  , PyConfiguration_Methods
                                                  , PyConfiguration_Getsets
                                                  , PyTypeManager::NoCppDelete );
    PyTypeManagerNonDBo<Parameter        >::create( module
                                                  , PyParameter_Methods
                                                  , PyParameter_Getsets
                                                  , PyTypeManager::NoCppDelete );
    //PyTypeManagerVector<const std::string>::create( module, PyTypeManager::NoCppDelete );
    PyTypeManagerVector<std::string      >::create( module, PyTypeManager::NoCppDelete );
    PyTypeManagerMap   <const std::string,Parameter*>::create( module, PyTypeManager::NoCppDelete );
    //PyTypeManagerMap   <std::string,Parameter*>::create( module, PyTypeManager::NoCppDelete );
    PyTypeManagerNonDBo<LayoutDescription>::create( module
                                                  , PyLayoutDescription_Methods
                                                  , NULL
                                                  , PyTypeManager::NoCppDelete );

    ConstructorError = PyErr_NewException( (char*)"hurricane.ConstructorError", NULL, NULL );
    ProxyError       = PyErr_NewException( (char*)"hurricane.ProxyError"      , NULL, NULL );
    HurricaneError   = PyErr_NewException( (char*)"hurricane.HurricaneError"  , NULL, NULL );
    HurricaneWarning = PyErr_NewException( (char*)"hurricane.HurricaneWarning", PyExc_Warning, NULL );

    PyDict_SetItemString( dictionnary, "ConstructorError", ConstructorError );
    PyDict_SetItemString( dictionnary, "ProxyError"      , ProxyError );
    PyDict_SetItemString( dictionnary, "HurricaneError"  , HurricaneError );

    cdebug_log(20,0)
      << "Cfg.so loaded " << (void*)&typeid(string)
      << " (" << demangle(typeid(string).name()) << ")" << endl;
    cerr
      << "Cfg.so loaded " << (void*)&typeid(string)
      << " (" << demangle(typeid(string).name()) << ")" << endl;

    return module;
  }

  
} // extern "C".

}  // Cfg namespace.
