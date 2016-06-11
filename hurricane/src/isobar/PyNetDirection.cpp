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
// |  C++ Module  :  "./PyNetDirection.h"                            |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyNetDirection.h"


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


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(NetDirection,netDirection,function)


// +=================================================================+
// |            "PyNetDirection" Python Module Code Part             |
// +=================================================================+

#if defined(__PYTHON_MODULE__)

  
  // Standart Accessors (Attributes).


  // Standart Destroy (Attribute).
  // DBoDestroyAttribute(PyNetDirection_destroy,PyNetDirection)


  PyMethodDef PyNetDirection_Methods[] =
    { {NULL, NULL, 0, NULL}           /* sentinel */
    };
//PyTypeObjectLinkPyType(Transformation::NetDirection)


  PythonOnlyDeleteMethod(NetDirection)
  DirectReprMethod(PyNetDirection_Repr, PyNetDirection,   Net::Direction)
  DirectStrMethod (PyNetDirection_Str,  PyNetDirection,   Net::Direction)
  DirectCmpMethod (PyNetDirection_Cmp,  IsPyNetDirection, PyNetDirection)
  DirectHashMethod(PyNetDirection_Hash, PyNetDirection)

  extern void  PyNetDirection_LinkPyType() {
    cdebug_log(20,0) << "PyNetDirection_LinkType()" << endl;
    PyTypeNetDirection.tp_dealloc = (destructor) PyNetDirection_DeAlloc;
    PyTypeNetDirection.tp_compare = (cmpfunc)    PyNetDirection_Cmp;
    PyTypeNetDirection.tp_repr    = (reprfunc)   PyNetDirection_Repr;
    PyTypeNetDirection.tp_str     = (reprfunc)   PyNetDirection_Str;
    PyTypeNetDirection.tp_hash    = (hashfunc)   PyNetDirection_Hash;
    PyTypeNetDirection.tp_methods = PyNetDirection_Methods;
  }


#else  // End of Python Module Code Part.


// +=================================================================+
// |             "PyNetDirection" Shared Library Code Part            |
// +=================================================================+

  // Link/Creation Method.
  PyTypeObjectDefinitions(NetDirection)


  extern  void  PyNetDirection_postModuleInit ()
  {
    PyObject* constant;

    LoadObjectConstant(PyTypeNetDirection.tp_dict,Net::Direction::UNDEFINED,"UNDEFINED");
    LoadObjectConstant(PyTypeNetDirection.tp_dict,Net::Direction::IN       ,"IN");
    LoadObjectConstant(PyTypeNetDirection.tp_dict,Net::Direction::OUT      ,"OUT");
    LoadObjectConstant(PyTypeNetDirection.tp_dict,Net::Direction::INOUT    ,"INOUT");
    LoadObjectConstant(PyTypeNetDirection.tp_dict,Net::Direction::TRISTATE ,"TRISTATE");
  }


#endif  // Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
 
