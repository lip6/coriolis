// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyCapacitorParameter.cpp"                    |
// +-----------------------------------------------------------------+


#include "hurricane/analog/PyCapacitorParameter.h"


namespace  Isobar {

using namespace Hurricane;
using namespace Analog;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define METHOD_HEAD(function)   GENERIC_METHOD_HEAD(CapacitorParameter,capacitorParameter,function)

#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |          "PyCapacitorParameter" Python Module Code Part         |
// +=================================================================+


  DirectGetDoubleAttribute(PyCapacitorParameter_getValue, getValue, PyCapacitorParameter, CapacitorParameter)
  DirectSetDoubleAttribute(PyCapacitorParameter_setValue, setValue, PyCapacitorParameter, CapacitorParameter)

  // ---------------------------------------------------------------
  // PyCapacitorParameter Attribute Method table.

  PyMethodDef PyCapacitorParameter_Methods[] =
    { { "getValue"          , (PyCFunction)PyCapacitorParameter_getValue, METH_NOARGS
                            , "Self explanatory." }
    , { "setValue"          , (PyCFunction)PyCapacitorParameter_setValue, METH_VARARGS
                            , "Self explanatory." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |           "PyCapacitorParameter" Object Methods             |
  // +-------------------------------------------------------------+


  PythonOnlyDeleteMethod(CapacitorParameter)
  PyTypeObjectLinkPyType(CapacitorParameter)


#else  // End of Python Module Code Part.

// +=================================================================+
// |         "PyCapacitorParameter" Shared Library Code Part         |
// +=================================================================+
  

  LinkCreateMethod(CapacitorParameter)
  PyTypeInheritedObjectDefinitions(CapacitorParameter, Parameter)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
