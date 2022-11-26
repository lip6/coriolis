// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2022-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |     A n a b a t i c  -  Global Routing Toolbox                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyStyleFlags.h"                              |
// +-----------------------------------------------------------------+


#include "anabatic/PyStyleFlags.h"


namespace  Anabatic {

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
  using Isobar::getPyHash;


extern "C" {


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(StyleFlags,status,function)


// +=================================================================+
// |             "PyStyleFlags" Python Module Code Part              |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  PyMethodDef PyStyleFlags_Methods[] =
    { {NULL, NULL, 0, NULL}  /* sentinel */
    };


  PythonOnlyDeleteMethod(StyleFlags)

  DirectReprMethod      (PyStyleFlags_Repr, PyStyleFlags,   StyleFlags)
  DirectStrMethod       (PyStyleFlags_Str,  PyStyleFlags,   StyleFlags)
  DirectCmpByValueMethod(PyStyleFlags_Cmp,  IsPyStyleFlags, PyStyleFlags)
  DirectHashMethod      (PyStyleFlags_Hash, StyleFlags)

  extern void  PyStyleFlags_LinkPyType() {
    cdebug_log(20,0) << "PyStyleFlags_LinkType()" << endl;
    PyTypeStyleFlags.tp_dealloc     = (destructor) PyStyleFlags_DeAlloc;
    PyTypeStyleFlags.tp_richcompare = (richcmpfunc)PyStyleFlags_Cmp;
    PyTypeStyleFlags.tp_repr        = (reprfunc)   PyStyleFlags_Repr;
    PyTypeStyleFlags.tp_str         = (reprfunc)   PyStyleFlags_Str;
    PyTypeStyleFlags.tp_hash        = (hashfunc)   PyStyleFlags_Hash;
    PyTypeStyleFlags.tp_methods     = PyStyleFlags_Methods;
  }


#else  // End of Python Module Code Part.


// +=================================================================+
// |              "PyStyleFlags" Shared Library Code Part            |
// +=================================================================+

  // Link/Creation Method.
  PyTypeObjectDefinitions(StyleFlags)


  extern  void  PyStyleFlags_postModuleInit ()
  {
    PyObject* constant = NULL;
    LoadObjectConstant( PyTypeStyleFlags.tp_dict, (uint64_t)StyleFlags::NoStyle, "NoStyle" );
    LoadObjectConstant( PyTypeStyleFlags.tp_dict, (uint64_t)StyleFlags::HV     , "HV" );
    LoadObjectConstant( PyTypeStyleFlags.tp_dict, (uint64_t)StyleFlags::VH     , "VH" );
    LoadObjectConstant( PyTypeStyleFlags.tp_dict, (uint64_t)StyleFlags::OTH    , "OTH" );
    LoadObjectConstant( PyTypeStyleFlags.tp_dict, (uint64_t)StyleFlags::Channel, "Channel" );
    LoadObjectConstant( PyTypeStyleFlags.tp_dict, (uint64_t)StyleFlags::Hybrid , "Hybrid" );
  }


#endif  // Shared Library Code Part.

}  // extern "C".
  

#if !defined(__PYTHON_MODULE__)


  extern StyleFlags  PyInt_AsStyleFlags ( PyObject* object )
  {
    uint64_t  value = (uint64_t)Isobar::PyAny_AsLong( object );
    if (  (value == (uint64_t)StyleFlags::NoStyle)
       or (value == (uint64_t)StyleFlags::HV)
       or (value == (uint64_t)StyleFlags::VH)
       or (value == (uint64_t)StyleFlags::OTH)
       or (value == (uint64_t)StyleFlags::Channel)
       or (value == (uint64_t)StyleFlags::Hybrid))
      return value;
    return StyleFlags::NoStyle;
  }


#endif

}  // Isobar namespace.
 
