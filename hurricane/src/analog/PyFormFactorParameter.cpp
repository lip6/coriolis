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
// |  C++ Module  :  "./PyFormFactorParameter.cpp"                   |
// +-----------------------------------------------------------------+


#include "hurricane/analog/PyFormFactorParameter.h"


namespace  Isobar {

using namespace Hurricane;
using namespace Analog;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define METHOD_HEAD(function)   GENERIC_METHOD_HEAD(FormFactorParameter,ffParameter,function)

#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |          "PyFormFactorParameter" Python Module Code Part        |
// +=================================================================+


  DirectGetLongAttribute(PyFormFactorParameter_getMin  , getMin  , PyFormFactorParameter, FormFactorParameter)
  DirectGetLongAttribute(PyFormFactorParameter_getMax  , getMax  , PyFormFactorParameter, FormFactorParameter)
  DirectGetLongAttribute(PyFormFactorParameter_getValue, getValue, PyFormFactorParameter, FormFactorParameter)
  DirectSetLongAttribute(PyFormFactorParameter_setMax  , setMax  , PyFormFactorParameter, FormFactorParameter)
  DirectSetLongAttribute(PyFormFactorParameter_setValue, setValue, PyFormFactorParameter, FormFactorParameter)


  // ---------------------------------------------------------------
  // PyFormFactorParameter Attribute Method table.

  PyMethodDef PyFormFactorParameter_Methods[] =
    { { "getMin"            , (PyCFunction)PyFormFactorParameter_getMin  , METH_NOARGS
                            , "Self explanatory." }
    , { "getMax"            , (PyCFunction)PyFormFactorParameter_getMax  , METH_NOARGS
                            , "Self explanatory." }
    , { "getValue"          , (PyCFunction)PyFormFactorParameter_getValue, METH_NOARGS
                            , "Self explanatory." }
    , { "setMax"            , (PyCFunction)PyFormFactorParameter_setMax  , METH_NOARGS
                            , "Self explanatory." }
    , { "setValue"          , (PyCFunction)PyFormFactorParameter_setValue, METH_VARARGS
                            , "Self explanatory." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |           "PyFormFactorParameter" Object Methods            |
  // +-------------------------------------------------------------+


  PythonOnlyDeleteMethod(FormFactorParameter)
  PyTypeObjectLinkPyType(FormFactorParameter)


#else  // End of Python Module Code Part.

// +=================================================================+
// |         "PyFormFactorParameter" Shared Library Code Part        |
// +=================================================================+
  

  LinkCreateMethod(FormFactorParameter)
  PyTypeInheritedObjectDefinitions(FormFactorParameter, Parameter)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
