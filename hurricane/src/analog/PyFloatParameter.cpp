// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2019-2019, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyFloatParameter.cpp"                        |
// +-----------------------------------------------------------------+


#include "hurricane/analog/PyFloatParameter.h"


namespace  Isobar {

using namespace Hurricane;
using namespace Analog;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define METHOD_HEAD(function)   GENERIC_METHOD_HEAD(FloatParameter,stepParameter,function)

#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |            "PyFloatParameter" Python Module Code Part           |
// +=================================================================+


  DirectGetDoubleAttribute(PyFloatParameter_getValue, getValue, PyFloatParameter, FloatParameter)
  DirectSetDoubleAttribute(PyFloatParameter_setValue, setValue, PyFloatParameter, FloatParameter)


  // ---------------------------------------------------------------
  // PyFloatParameter Attribute Method table.

  PyMethodDef PyFloatParameter_Methods[] =
    { { "getValue"          , (PyCFunction)PyFloatParameter_getValue, METH_NOARGS
                            , "Self explanatory." }
    , { "setValue"          , (PyCFunction)PyFloatParameter_setValue, METH_VARARGS
                            , "Self explanatory." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |             "PyFloatParameter" Object Methods               |
  // +-------------------------------------------------------------+


  PythonOnlyDeleteMethod(FloatParameter)
  PyTypeObjectLinkPyType(FloatParameter)


#else  // End of Python Module Code Part.

// +=================================================================+
// |           "PyFloatParameter" Shared Library Code Part           |
// +=================================================================+
  

  LinkCreateMethod(FloatParameter)
  PyTypeInheritedObjectDefinitions(FloatParameter, Parameter)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
