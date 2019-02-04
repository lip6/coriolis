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
// |  C++ Module  :  "./PyChoiceParameter.cpp"                       |
// +-----------------------------------------------------------------+


#include "hurricane/analog/PyChoiceParameter.h"


namespace  Isobar {

using namespace Hurricane;
using namespace Analog;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define METHOD_HEAD(function)   GENERIC_METHOD_HEAD(ChoiceParameter,choiceParameter,function)

#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |            "PyChoiceParameter" Python Module Code Part          |
// +=================================================================+


  DirectGetStringAttribute (PyChoiceParameter_getValue, getValue, PyChoiceParameter, ChoiceParameter)
  DirectSetCStringAttribute(PyChoiceParameter_setValue, setValue, PyChoiceParameter, ChoiceParameter)


  // ---------------------------------------------------------------
  // PyChoiceParameter Attribute Method table.

  PyMethodDef PyChoiceParameter_Methods[] =
    { { "getValue"          , (PyCFunction)PyChoiceParameter_getValue, METH_NOARGS
                            , "Self explanatory." }
    , { "setValue"          , (PyCFunction)PyChoiceParameter_setValue, METH_VARARGS
                            , "Self explanatory." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |             "PyChoiceParameter" Object Methods              |
  // +-------------------------------------------------------------+


  PythonOnlyDeleteMethod(ChoiceParameter)
  PyTypeObjectLinkPyType(ChoiceParameter)


#else  // End of Python Module Code Part.

// +=================================================================+
// |           "PyChoiceParameter" Shared Library Code Part          |
// +=================================================================+
  

  LinkCreateMethod(ChoiceParameter)
  PyTypeInheritedObjectDefinitions(ChoiceParameter, Parameter)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
