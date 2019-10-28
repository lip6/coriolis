// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2019-2019, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PySystem.cpp"                                |
// +-----------------------------------------------------------------+


#include "crlcore/PySystem.h"


namespace  CRL {

  using std::cerr;
  using std::endl;
  using std::hex;
  using std::ostringstream;
  using Hurricane::tab;
  using Hurricane::Exception;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::DataBase;
  using Isobar::__cs;
  using Isobar::Converter;
  using Isobar::ProxyProperty;
  using Isobar::ProxyError;
  using Isobar::ConstructorError;
  using Isobar::HurricaneError;
  using Isobar::HurricaneWarning;
  using Isobar::PyAny_AsLong;
  using Isobar::ParseOneArg;
  using Isobar::ParseTwoArg;


extern "C" {


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(System,sys,function)


// +=================================================================+
// |               "PySystem" Python Module Code Part                |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  static PyObject* PySystem_get ( PyObject* args )
  {
    cdebug_log(30,0) << "PySystem_get()" << endl;

    System*   sys   = NULL;
    PySystem* pySys = NULL;
    
    HTRY
      sys = System::get();
      pySys = PyObject_NEW( PySystem, &PyTypeSystem );
      if (pySys == NULL) return NULL;
      pySys->_object = sys;
    HCATCH

    return (PyObject*)pySys;
  }

  
  // Standart Accessors (Attributes).


  // Standart Destroy (Attribute).
  // DBoDestroyAttribute(PySystem_destroy,PySystem)


  PyMethodDef PySystem_Methods[] =
    { { "get"                  , (PyCFunction)PySystem_get                  , METH_NOARGS|METH_STATIC
                               , "Gets the Coriolis System singleton." }                      
  //, { "destroy"              , (PyCFunction)PySystem_destroy              , METH_NOARGS
  //                           , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL}    /* sentinel */
    };


  PythonOnlyDeleteMethod(System)
  PyTypeObjectLinkPyType(System)


#else  // Python Module Code Part.


// x=================================================================x
// |                "PySystem" Shared Library Code Part              |
// x=================================================================x


  // Link/Creation Method.
  PyTypeObjectDefinitions(System)


#endif  // Shared Library Code Part.


}  // extern "C".

}  // CRL namespace.
 
