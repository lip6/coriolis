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
// |  C++ Module  :  "./PyMCheckBoxParameter.cpp"                    |
// +-----------------------------------------------------------------+


#include "hurricane/analog/PyMCheckBoxParameter.h"


namespace  Isobar {

using namespace Hurricane;
using namespace Analog;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define METHOD_HEAD(function)   GENERIC_METHOD_HEAD(MCheckBoxParameter,mCheckboxParameter,function)

#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |          "PyMCheckBoxParameter" Python Module Code Part         |
// +=================================================================+


  DirectGetUIntAttribute(PyMCheckBoxParameter_getValue, getValue, PyMCheckBoxParameter, MCheckBoxParameter)
  DirectSetIntAttribute (PyMCheckBoxParameter_setValue, setValue, PyMCheckBoxParameter, MCheckBoxParameter)

  // ---------------------------------------------------------------
  // PyMCheckBoxParameter Attribute Method table.

  PyMethodDef PyMCheckBoxParameter_Methods[] =
    { { "getValue"          , (PyCFunction)PyMCheckBoxParameter_getValue, METH_NOARGS
                            , "Self explanatory." }
    , { "setValue"          , (PyCFunction)PyMCheckBoxParameter_setValue, METH_VARARGS
                            , "Self explanatory." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |           "PyMCheckBoxParameter" Object Methods             |
  // +-------------------------------------------------------------+


  PythonOnlyDeleteMethod(MCheckBoxParameter)
  PyTypeObjectLinkPyType(MCheckBoxParameter)


#else  // End of Python Module Code Part.

// +=================================================================+
// |         "PyMCheckBoxParameter" Shared Library Code Part         |
// +=================================================================+
  

  LinkCreateMethod(MCheckBoxParameter)
  PyTypeInheritedObjectDefinitions(MCheckBoxParameter, Parameter)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
