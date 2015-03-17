// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2014-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyPinDirection.h"                            |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyPinDirection.h"


namespace  Isobar {

  using std::cerr;
  using std::endl;
  using std::hex;
  using std::ostringstream;
  using Hurricane::tab;
  using Hurricane::in_trace;
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


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(PinDirection,netDirection,function)


// +=================================================================+
// |            "PyPinDirection" Python Module Code Part             |
// +=================================================================+

#if defined(__PYTHON_MODULE__)

  
  // Standart Accessors (Attributes).


  // Standart Destroy (Attribute).
  // DBoDestroyAttribute(PyPinDirection_destroy,PyPinDirection)


  PyMethodDef PyPinDirection_Methods[] =
    { {NULL, NULL, 0, NULL}           /* sentinel */
    };
//PyTypeObjectLinkPyType(Transformation::PinDirection)


  PythonOnlyDeleteMethod(PinDirection)
  DirectReprMethod(PyPinDirection_Repr, PyPinDirection,   Pin::AccessDirection)
  DirectStrMethod (PyPinDirection_Str,  PyPinDirection,   Pin::AccessDirection)
  DirectCmpMethod (PyPinDirection_Cmp,  IsPyPinDirection, PyPinDirection)
  DirectHashMethod(PyPinDirection_Hash, PyPinDirection)

  extern void  PyPinDirection_LinkPyType() {
    trace << "PyPinDirection_LinkType()" << endl;
    PyTypePinDirection.tp_dealloc = (destructor) PyPinDirection_DeAlloc;
    PyTypePinDirection.tp_compare = (cmpfunc)    PyPinDirection_Cmp;
    PyTypePinDirection.tp_repr    = (reprfunc)   PyPinDirection_Repr;
    PyTypePinDirection.tp_str     = (reprfunc)   PyPinDirection_Str;
    PyTypePinDirection.tp_hash    = (hashfunc)   PyPinDirection_Hash;
    PyTypePinDirection.tp_methods = PyPinDirection_Methods;
  }


#else  // End of Python Module Code Part.


// +=================================================================+
// |             "PyPinDirection" Shared Library Code Part            |
// +=================================================================+

  // Link/Creation Method.
  PyTypeObjectDefinitions(PinDirection)


  extern  void  PyPinDirection_postModuleInit ()
  {
    PyObject* constant;

    LoadObjectConstant(PyTypePinDirection.tp_dict,Pin::AccessDirection::UNDEFINED,"UNDEFINED");
    LoadObjectConstant(PyTypePinDirection.tp_dict,Pin::AccessDirection::NORTH    ,"NORTH");
    LoadObjectConstant(PyTypePinDirection.tp_dict,Pin::AccessDirection::SOUTH    ,"SOUTH");
    LoadObjectConstant(PyTypePinDirection.tp_dict,Pin::AccessDirection::EAST     ,"EAST");
    LoadObjectConstant(PyTypePinDirection.tp_dict,Pin::AccessDirection::WEST     ,"WEST");
  }


#endif  // Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
 
