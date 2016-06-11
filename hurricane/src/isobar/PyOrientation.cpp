// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2014-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyOrientation.h"                             |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyOrientation.h"


namespace  Isobar {

  using std::cerr;
  using std::endl;
  using std::hex;
  using std::ostringstream;
  using Hurricane::tab;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Isobar::ProxyProperty;
  using Isobar::ProxyError;
  using Isobar::ConstructorError;
  using Isobar::HurricaneError;
  using Isobar::HurricaneWarning;
  using Isobar::ParseOneArg;
  using Isobar::ParseTwoArg;


extern "C" {


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(Orientation,catalog,function)


// +=================================================================+
// |            "PyOrientation" Python Module Code Part              |
// +=================================================================+

#if defined(__PYTHON_MODULE__)

  
  // Standart Accessors (Attributes).


  // Standart Destroy (Attribute).
  // DBoDestroyAttribute(PyOrientation_destroy,PyOrientation)


  PyMethodDef PyOrientation_Methods[] =
    { {NULL, NULL, 0, NULL}           /* sentinel */
    };


//PythonOnlyDeleteMethod(Transformation::Orientation)
//PyTypeObjectLinkPyType(Transformation::Orientation)


  PythonOnlyDeleteMethod(Orientation)

  DirectReprMethod(PyOrientation_Repr, PyOrientation,   Transformation::Orientation)
  DirectStrMethod (PyOrientation_Str,  PyOrientation,   Transformation::Orientation)
  DirectCmpMethod (PyOrientation_Cmp,  IsPyOrientation, PyOrientation)
  DirectHashMethod(PyOrientation_Hash, PyOrientation)

  extern void  PyOrientation_LinkPyType() {
    cdebug_log(20,0) << "PyOrientation_LinkType()" << endl;
    PyTypeOrientation.tp_dealloc = (destructor) PyOrientation_DeAlloc;
    PyTypeOrientation.tp_compare = (cmpfunc)    PyOrientation_Cmp;
    PyTypeOrientation.tp_repr    = (reprfunc)   PyOrientation_Repr;
    PyTypeOrientation.tp_str     = (reprfunc)   PyOrientation_Str;
    PyTypeOrientation.tp_hash    = (hashfunc)   PyOrientation_Hash;
    PyTypeOrientation.tp_methods = PyOrientation_Methods;
  }


#else  // End of Python Module Code Part.


// +=================================================================+
// |             "PyOrientation" Shared Library Code Part            |
// +=================================================================+

  // Link/Creation Method.
  PyTypeObjectDefinitions(Orientation)


  extern  void  PyOrientation_postModuleInit ()
  {
    PyObject* constant;

    LoadObjectConstant(PyTypeOrientation.tp_dict,Transformation::Orientation::ID,"ID");
    LoadObjectConstant(PyTypeOrientation.tp_dict,Transformation::Orientation::R1,"R1");
    LoadObjectConstant(PyTypeOrientation.tp_dict,Transformation::Orientation::R2,"R2");
    LoadObjectConstant(PyTypeOrientation.tp_dict,Transformation::Orientation::R3,"R3");
    LoadObjectConstant(PyTypeOrientation.tp_dict,Transformation::Orientation::MX,"MX");
    LoadObjectConstant(PyTypeOrientation.tp_dict,Transformation::Orientation::XR,"XR");
    LoadObjectConstant(PyTypeOrientation.tp_dict,Transformation::Orientation::MY,"MY");
    LoadObjectConstant(PyTypeOrientation.tp_dict,Transformation::Orientation::YR,"YR");
  }


#endif  // Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
 
