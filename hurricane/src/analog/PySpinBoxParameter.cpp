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
// |  C++ Module  :  "./PySpinBoxParameter.cpp"                      |
// +-----------------------------------------------------------------+


#include "hurricane/analog/PySpinBoxParameter.h"


namespace  Isobar {

using namespace Hurricane;
using namespace Analog;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define METHOD_HEAD(function)   GENERIC_METHOD_HEAD(SpinBoxParameter,stepParameter,function)

#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |           "PySpinBoxParameter" Python Module Code Part          |
// +=================================================================+


  DirectGetLongAttribute(PySpinBoxParameter_getMin  , getMin  , PySpinBoxParameter, SpinBoxParameter)
  DirectGetLongAttribute(PySpinBoxParameter_getMax  , getMax  , PySpinBoxParameter, SpinBoxParameter)
  DirectGetLongAttribute(PySpinBoxParameter_getValue, getValue, PySpinBoxParameter, SpinBoxParameter)
  DirectSetLongAttribute(PySpinBoxParameter_setValue, setValue, PySpinBoxParameter, SpinBoxParameter)


  // ---------------------------------------------------------------
  // PySpinBoxParameter Attribute Method table.

  PyMethodDef PySpinBoxParameter_Methods[] =
    { { "getMin"            , (PyCFunction)PySpinBoxParameter_getMin  , METH_NOARGS
                            , "Self explanatory." }
    , { "getMax"            , (PyCFunction)PySpinBoxParameter_getMax  , METH_NOARGS
                            , "Self explanatory." }
    , { "getValue"          , (PyCFunction)PySpinBoxParameter_getValue, METH_NOARGS
                            , "Self explanatory." }
    , { "setValue"          , (PyCFunction)PySpinBoxParameter_setValue, METH_VARARGS
                            , "Self explanatory." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |            "PySpinBoxParameter" Object Methods              |
  // +-------------------------------------------------------------+


  PythonOnlyDeleteMethod(SpinBoxParameter)
  PyTypeObjectLinkPyType(SpinBoxParameter)


#else  // End of Python Module Code Part.

// +=================================================================+
// |          "PySpinBoxParameter" Shared Library Code Part          |
// +=================================================================+
  

  LinkCreateMethod(SpinBoxParameter)
  PyTypeInheritedObjectDefinitions(SpinBoxParameter, Parameter)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
