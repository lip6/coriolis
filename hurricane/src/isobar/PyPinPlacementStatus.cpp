// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2014-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyPinPlacementStatus.h"                      |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyPinPlacementStatus.h"


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


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(PinPlacementStatus,netPlacementStatus,function)


// +=================================================================+
// |         "PyPinPlacementStatus" Python Module Code Part          |
// +=================================================================+

#if defined(__PYTHON_MODULE__)

  
  // Standart Accessors (Attributes).


  // Standart Destroy (Attribute).
  // DBoDestroyAttribute(PyPinPlacementStatus_destroy,PyPinPlacementStatus)


  PyMethodDef PyPinPlacementStatus_Methods[] =
    { {NULL, NULL, 0, NULL}           /* sentinel */
    };
//PyTypeObjectLinkPyType(Transformation::PinPlacementStatus)


  PythonOnlyDeleteMethod(PinPlacementStatus)
  DirectReprMethod(PyPinPlacementStatus_Repr, PyPinPlacementStatus,   Pin::PlacementStatus)
  DirectStrMethod (PyPinPlacementStatus_Str,  PyPinPlacementStatus,   Pin::PlacementStatus)
  DirectCmpMethod (PyPinPlacementStatus_Cmp,  IsPyPinPlacementStatus, PyPinPlacementStatus)
  DirectHashMethod(PyPinPlacementStatus_Hash, PyPinPlacementStatus)

  extern void  PyPinPlacementStatus_LinkPyType() {
    trace << "PyPinPlacementStatus_LinkType()" << endl;
    PyTypePinPlacementStatus.tp_dealloc = (destructor) PyPinPlacementStatus_DeAlloc;
    PyTypePinPlacementStatus.tp_compare = (cmpfunc)    PyPinPlacementStatus_Cmp;
    PyTypePinPlacementStatus.tp_repr    = (reprfunc)   PyPinPlacementStatus_Repr;
    PyTypePinPlacementStatus.tp_str     = (reprfunc)   PyPinPlacementStatus_Str;
    PyTypePinPlacementStatus.tp_hash    = (hashfunc)   PyPinPlacementStatus_Hash;
    PyTypePinPlacementStatus.tp_methods = PyPinPlacementStatus_Methods;
  }


#else  // End of Python Module Code Part.


// +=================================================================+
// |          "PyPinPlacementStatus" Shared Library Code Part        |
// +=================================================================+

  // Link/Creation Method.
  PyTypeObjectDefinitions(PinPlacementStatus)


  extern  void  PyPinPlacementStatus_postModuleInit ()
  {
    PyObject* constant;

    LoadObjectConstant(PyTypePinPlacementStatus.tp_dict,Pin::PlacementStatus::UNPLACED,"UNPLACED");
    LoadObjectConstant(PyTypePinPlacementStatus.tp_dict,Pin::PlacementStatus::PLACED  ,"PLACED");
    LoadObjectConstant(PyTypePinPlacementStatus.tp_dict,Pin::PlacementStatus::FIXED   ,"FIXED");
  }


#endif  // Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
 
