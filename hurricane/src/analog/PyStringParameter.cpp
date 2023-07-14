// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2020-2020, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyStringParameter.cpp"                       |
// +-----------------------------------------------------------------+


#include "hurricane/analog/PyStringParameter.h"


namespace  Isobar {

using namespace Hurricane;
using namespace Analog;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define METHOD_HEAD(function)   GENERIC_METHOD_HEAD(StringParameter,stringParameter,function)

#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |            "PyStringParameter" Python Module Code Part          |
// +=================================================================+


  DirectGetStringAttribute( PyStringParameter_getValue, getValue , PyStringParameter, StringParameter)
  DirectSetCStringAttribute(PyStringParameter_setValue, setValue , PyStringParameter, StringParameter)


  // ---------------------------------------------------------------
  // PyStringParameter Attribute Method table.

  PyMethodDef PyStringParameter_Methods[] =
    { { "getValue"          , (PyCFunction)PyStringParameter_getValue , METH_NOARGS
                            , "Self explanatory." }
    , { "setValue"          , (PyCFunction)PyStringParameter_setValue , METH_VARARGS
                            , "Self explanatory." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |             "PyStringParameter" Object Methods              |
  // +-------------------------------------------------------------+


  PythonOnlyDeleteMethod(StringParameter)
  PyTypeObjectLinkPyType(StringParameter)


#else  // End of Python Module Code Part.

// +=================================================================+
// |           "PyStringParameter" Shared Library Code Part          |
// +=================================================================+
  

  LinkCreateMethod(StringParameter)
  PyTypeInheritedObjectDefinitions(StringParameter, Parameter)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
