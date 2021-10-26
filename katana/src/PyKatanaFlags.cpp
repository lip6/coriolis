// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2017-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyKatanaFlags.cpp"                           |
// +-----------------------------------------------------------------+


#include "katana/PyKatanaFlags.h"


namespace  Katana {

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
  using Isobar::getPyHash;
  using Isobar::ParseOneArg;
  using Isobar::ParseTwoArg;


extern "C" {


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(Flags,flags,function)


// +=================================================================+
// |            "PyKatanaFlags" Python Module Code Part              |
// +=================================================================+

#if defined(__PYTHON_MODULE__)

  
  // Standart Accessors (Attributes).


  // Standart Destroy (Attribute).
  // DBoDestroyAttribute(PyKatanaFlags_destroy,PyKatanaFlags)


  PyMethodDef PyKatanaFlags_Methods[] =
    { {NULL, NULL, 0, NULL}           /* sentinel */
    };


  PythonOnlyDeleteMethod(KatanaFlags)
  DirectReprMethod      (PyKatanaFlags_Repr, PyKatanaFlags,   Katana::Flags)
  DirectStrMethod       (PyKatanaFlags_Str,  PyKatanaFlags,   Katana::Flags)
  DirectCmpByValueMethod(PyKatanaFlags_Cmp,  IsPyKatanaFlags, PyKatanaFlags)
  DirectHashMethod      (PyKatanaFlags_Hash, KatanaFlags)

  extern void  PyKatanaFlags_LinkPyType() {
    cdebug_log(20,0) << "PyKatanaFlags_LinkType()" << endl;
    PyTypeKatanaFlags.tp_dealloc     = (destructor) PyKatanaFlags_DeAlloc;
    PyTypeKatanaFlags.tp_richcompare = (richcmpfunc)PyKatanaFlags_Cmp;
    PyTypeKatanaFlags.tp_repr        = (reprfunc)   PyKatanaFlags_Repr;
    PyTypeKatanaFlags.tp_str         = (reprfunc)   PyKatanaFlags_Str;
    PyTypeKatanaFlags.tp_hash        = (hashfunc)   PyKatanaFlags_Hash;
    PyTypeKatanaFlags.tp_methods     = PyKatanaFlags_Methods;
  }


#else  // End of Python Module Code Part.


// +=================================================================+
// |             "PyKatanaFlags" Shared Library Code Part            |
// +=================================================================+

  // Link/Creation Method.
  PyTypeObjectDefinitions(KatanaFlags)


  extern  void  PyKatanaFlags_postModuleInit ()
  {
    PyObject* constant;

    LoadObjectConstant(PyTypeKatanaFlags.tp_dict,(uint64_t)Katana::Flags::NoFlags             ,"NoFlags"             );
    LoadObjectConstant(PyTypeKatanaFlags.tp_dict,(uint64_t)Katana::Flags::SlowMotion          ,"SlowMotion"          );
    LoadObjectConstant(PyTypeKatanaFlags.tp_dict,(uint64_t)Katana::Flags::PreRoutedStage      ,"PreRoutedStage"      );
    LoadObjectConstant(PyTypeKatanaFlags.tp_dict,(uint64_t)Katana::Flags::PairSymmetrics      ,"PairSymmetrics"      );
    LoadObjectConstant(PyTypeKatanaFlags.tp_dict,(uint64_t)Katana::Flags::ShowFailedNets      ,"ShowFailedNets"      );
    LoadObjectConstant(PyTypeKatanaFlags.tp_dict,(uint64_t)Katana::Flags::ShowFailedGSegments ,"ShowFailedGSegments" );
    LoadObjectConstant(PyTypeKatanaFlags.tp_dict,(uint64_t)Katana::Flags::ShowOverloadedGCells,"ShowOverloadedGCells");
    LoadObjectConstant(PyTypeKatanaFlags.tp_dict,(uint64_t)Katana::Flags::ShowOverloadedEdges ,"ShowOverloadedEdges" );
    LoadObjectConstant(PyTypeKatanaFlags.tp_dict,(uint64_t)Katana::Flags::ShowBloatedInstances,"ShowBloatedInstances");
  }


#endif  // Shared Library Code Part.

}  // extern "C".

}  // Katana namespace.
 
