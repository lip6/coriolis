// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2014-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyPlacementStatus.h"                         |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyPlacementStatus.h"


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


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(PlacementStatus,status,function)


// +=================================================================+
// |          "PyPlacementStatus" Python Module Code Part            |
// +=================================================================+

#if defined(__PYTHON_MODULE__)

  
  // Standart Accessors (Attributes).


  // Standart Destroy (Attribute).
  // DBoDestroyAttribute(PyPlacementStatus_destroy,PyPlacementStatus)


  PyMethodDef PyPlacementStatus_Methods[] =
    { {NULL, NULL, 0, NULL}           /* sentinel */
    };


//PythonOnlyDeleteMethod(Transformation::PlacementStatus)
//PyTypeObjectLinkPyType(Transformation::PlacementStatus)


  PythonOnlyDeleteMethod(PlacementStatus)

  DirectReprMethod(PyPlacementStatus_Repr, PyPlacementStatus,   Instance::PlacementStatus)
  DirectStrMethod (PyPlacementStatus_Str,  PyPlacementStatus,   Instance::PlacementStatus)
  DirectCmpMethod (PyPlacementStatus_Cmp,  IsPyPlacementStatus, PyPlacementStatus)
  DirectHashMethod(PyPlacementStatus_Hash, PyPlacementStatus)

  extern void  PyPlacementStatus_LinkPyType() {
    cdebug_log(20,0) << "PyPlacementStatus_LinkType()" << endl;
    PyTypePlacementStatus.tp_dealloc = (destructor) PyPlacementStatus_DeAlloc;
    PyTypePlacementStatus.tp_compare = (cmpfunc)    PyPlacementStatus_Cmp;
    PyTypePlacementStatus.tp_repr    = (reprfunc)   PyPlacementStatus_Repr;
    PyTypePlacementStatus.tp_str     = (reprfunc)   PyPlacementStatus_Str;
    PyTypePlacementStatus.tp_hash    = (hashfunc)   PyPlacementStatus_Hash;
    PyTypePlacementStatus.tp_methods = PyPlacementStatus_Methods;
  }


#else  // End of Python Module Code Part.


// +=================================================================+
// |           "PyPlacementStatus" Shared Library Code Part          |
// +=================================================================+

  // Link/Creation Method.
  PyTypeObjectDefinitions(PlacementStatus)


  extern  void  PyPlacementStatus_postModuleInit ()
  {
    PyObject* constant;

    LoadObjectConstant(PyTypePlacementStatus.tp_dict,Instance::PlacementStatus::UNPLACED,"UNPLACED");
    LoadObjectConstant(PyTypePlacementStatus.tp_dict,Instance::PlacementStatus::PLACED  ,"PLACED");
    LoadObjectConstant(PyTypePlacementStatus.tp_dict,Instance::PlacementStatus::FIXED   ,"FIXED");
  }


#endif  // Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
 
