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
// |  C++ Module  :  "./PyStepParameter.cpp"                         |
// +-----------------------------------------------------------------+


#include "hurricane/analog/PyStepParameter.h"


namespace  Isobar {

using namespace Hurricane;
using namespace Analog;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define METHOD_HEAD(function)   GENERIC_METHOD_HEAD(StepParameter,stepParameter,function)

#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |             "PyStepParameter" Python Module Code Part           |
// +=================================================================+


  DirectGetLongAttribute(PyStepParameter_getMin  , getMin  , PyStepParameter, StepParameter)
  DirectGetLongAttribute(PyStepParameter_getMax  , getMax  , PyStepParameter, StepParameter)
  DirectGetLongAttribute(PyStepParameter_getStep , getStep , PyStepParameter, StepParameter)
  DirectGetLongAttribute(PyStepParameter_getValue, getValue, PyStepParameter, StepParameter)
  DirectSetLongAttribute(PyStepParameter_setValue, setValue, PyStepParameter, StepParameter)


  // ---------------------------------------------------------------
  // PyStepParameter Attribute Method table.

  PyMethodDef PyStepParameter_Methods[] =
    { { "getMin"            , (PyCFunction)PyStepParameter_getMin  , METH_NOARGS
                            , "Self explanatory." }
    , { "getMax"            , (PyCFunction)PyStepParameter_getMax  , METH_NOARGS
                            , "Self explanatory." }
    , { "getStep"           , (PyCFunction)PyStepParameter_getStep , METH_NOARGS
                            , "Self explanatory." }
    , { "getValue"          , (PyCFunction)PyStepParameter_getValue, METH_NOARGS
                            , "Self explanatory." }
    , { "setValue"          , (PyCFunction)PyStepParameter_setValue, METH_VARARGS
                            , "Self explanatory." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |              "PyStepParameter" Object Methods               |
  // +-------------------------------------------------------------+


  PythonOnlyDeleteMethod(StepParameter)
  PyTypeObjectLinkPyType(StepParameter)


#else  // End of Python Module Code Part.

// +=================================================================+
// |            "PyStepParameter" Shared Library Code Part           |
// +=================================================================+
  

  LinkCreateMethod(StepParameter)
  PyTypeInheritedObjectDefinitions(StepParameter, Parameter)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
