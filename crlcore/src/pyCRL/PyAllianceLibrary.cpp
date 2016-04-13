// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@lip6.fr                   |
// | =============================================================== |
// |  C++ Module  :  "./PyAllianceLibrary.cpp"                       |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyLibrary.h"
#include "crlcore/PyAllianceLibrary.h"
#include <string>
#include <sstream>


namespace  CRL {

  using std::cerr;
  using std::endl;
  using std::hex;
  using std::string;
  using std::ostringstream;
  using Hurricane::tab;
  using Hurricane::in_trace;
  using Hurricane::Exception;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::Library;
  using Isobar::ProxyProperty;
  using Isobar::ProxyError;
  using Isobar::ConstructorError;
  using Isobar::HurricaneError;
  using Isobar::HurricaneWarning;
  using Isobar::ParseOneArg;
  using Isobar::ParseTwoArg;
  using Isobar::__cs;
  using Isobar::PyLibrary_Link;


extern "C" {


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(AllianceLibrary,alib,function)


#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |          "PyAllianceLibrary" Python Module Code Part            |
// +=================================================================+


  static PyObject* PyAllianceLibrary_getLibrary ( PyAllianceLibrary* self, PyObject* args )
  {
    trace << "PyAllianceLibrary_getLibrary()" << endl;

    Library* lib = NULL;

    HTRY
    METHOD_HEAD("AllianceLibrary.getLibrary()")
    lib = alib->getLibrary();
    HCATCH

    return PyLibrary_Link(lib);
  }


  static PyObject* PyAllianceLibrary_getPath ( PyAllianceLibrary* self, PyObject* args )
  {
    trace << "PyAllianceLibrary_getPath()" << endl;

    HTRY
    METHOD_HEAD("AllianceLibrary.getPath()")
    return PyString_FromString( getString(alib->getPath()).c_str() );
    HCATCH

    return NULL;
  }


  static PyObject* PyAllianceLibrary_Repr ( PyAllianceLibrary* self )
  {
    if ( self->ACCESS_OBJECT == NULL )
      return PyString_FromString("<PyObject unbound>");

    string s = getString( self->ACCESS_OBJECT );
    return PyString_FromString( s.c_str() );
  }


  // Standart Destroy (Attribute).
  // DBoDestroyAttribute(PyAllianceLibrary_destroy,PyAllianceLibrary)


  PyMethodDef PyAllianceLibrary_Methods[] =
    { { "getPath"         , (PyCFunction)PyAllianceLibrary_getPath       , METH_NOARGS
                          , "Return the complete path of the library." }
    , { "getLibrary"      , (PyCFunction)PyAllianceLibrary_getLibrary    , METH_NOARGS
                          , "Returns the associated Hurricane library." }
  //, { "destroy"         , (PyCFunction)PyAllianceLibrary_destroy       , METH_VARARGS
  //                      , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  PythonOnlyDeleteMethod(AllianceLibrary)
  DirectHashMethod(PyAllianceLibrary_Hash, PyAllianceLibrary)

  extern void  PyAllianceLibrary_LinkPyType() {
    trace << "PyAllianceLibrary_LinkType()" << endl;

    PyTypeAllianceLibrary.tp_dealloc = (destructor) PyAllianceLibrary_DeAlloc;
    PyTypeAllianceLibrary.tp_repr    = (reprfunc)   PyAllianceLibrary_Repr;
    PyTypeAllianceLibrary.tp_str     = (reprfunc)   PyAllianceLibrary_Repr;
    PyTypeAllianceLibrary.tp_hash    = (hashfunc)   PyAllianceLibrary_Hash;
    PyTypeAllianceLibrary.tp_methods = PyAllianceLibrary_Methods;
  }


#else  // End of Python Module Code Part.

// +=================================================================+
// |           "PyAllianceLibrary" Shared Library Code Part          |
// +=================================================================+

  // Type Definition.
  PyTypeObjectDefinitionsOfModule(CRL,AllianceLibrary)

  // Link/Creation Method.
  LinkCreateMethod(AllianceLibrary)


#endif  // End of Shared Library Code Part.


}  // extern "C".

}  // CRL namespace.
