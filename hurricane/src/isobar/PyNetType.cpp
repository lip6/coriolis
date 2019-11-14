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
// |  C++ Module  :  "./PyNetType.h"                                 |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyNetType.h"


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


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(NetType,catalog,function)


// +=================================================================+
// |              "PyNetType" Python Module Code Part                |
// +=================================================================+

#if defined(__PYTHON_MODULE__)

  
  // Standart Accessors (Attributes).


  // Standart Destroy (Attribute).
  // DBoDestroyAttribute(PyNetType_destroy,PyNetType)


  PyMethodDef PyNetType_Methods[] =
    { {NULL, NULL, 0, NULL}           /* sentinel */
    };


  PythonOnlyDeleteMethod(NetType)
  DirectReprMethod      (PyNetType_Repr, PyNetType,   Net::Type)
  DirectStrMethod       (PyNetType_Str,  PyNetType,   Net::Type)
  DirectCmpByValueMethod(PyNetType_Cmp,  IsPyNetType, PyNetType)
  DirectHashMethod      (PyNetType_Hash, NetType)

  extern void  PyNetType_LinkPyType() {
    cdebug_log(20,0) << "PyNetType_LinkType()" << endl;
    PyTypeNetType.tp_dealloc = (destructor) PyNetType_DeAlloc;
    PyTypeNetType.tp_compare = (cmpfunc)    PyNetType_Cmp;
    PyTypeNetType.tp_repr    = (reprfunc)   PyNetType_Repr;
    PyTypeNetType.tp_str     = (reprfunc)   PyNetType_Str;
    PyTypeNetType.tp_hash    = (hashfunc)   PyNetType_Hash;
    PyTypeNetType.tp_methods = PyNetType_Methods;
  }


#else  // End of Python Module Code Part.


// +=================================================================+
// |               "PyNetType" Shared Library Code Part              |
// +=================================================================+

  // Link/Creation Method.
  PyTypeObjectDefinitions(NetType)


  extern  void  PyNetType_postModuleInit ()
  {
    PyObject* constant;

    LoadObjectConstant(PyTypeNetType.tp_dict,Net::Type::UNDEFINED,"UNDEFINED");
    LoadObjectConstant(PyTypeNetType.tp_dict,Net::Type::LOGICAL  ,"LOGICAL"  );
    LoadObjectConstant(PyTypeNetType.tp_dict,Net::Type::CLOCK    ,"CLOCK"    );
    LoadObjectConstant(PyTypeNetType.tp_dict,Net::Type::POWER    ,"POWER"    );
    LoadObjectConstant(PyTypeNetType.tp_dict,Net::Type::GROUND   ,"GROUND"   );
    LoadObjectConstant(PyTypeNetType.tp_dict,Net::Type::BLOCKAGE ,"BLOCKAGE" );
    LoadObjectConstant(PyTypeNetType.tp_dict,Net::Type::FUSED    ,"FUSED"    );
  }


#endif  // Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
 
