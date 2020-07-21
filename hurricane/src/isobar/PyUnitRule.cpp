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
// |  C++ Module  :  "./PyUnitRule.cpp"                              |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyUnitRule.h"


namespace  Isobar {

  using namespace Hurricane;

  extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT            _baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define METHOD_HEAD(function)    GENERIC_METHOD_HEAD(UnitRule,rule,function)


// +=================================================================+
// |                 "PyUnitRule" Python Module Code Part            |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  DirectDestroyAttribute(PyUnitRule_destroy, PyUnitRule)

  DirectGetDoubleAttribute(PyUnitRule_getValue, getValue, PyUnitRule, UnitRule)


  PyMethodDef PyUnitRule_Methods[] =
    { { "getValue"          , (PyCFunction)PyUnitRule_getValue, METH_NOARGS
                            , "Returns the value (double)." }
    , { "destroy"           , (PyCFunction)PyUnitRule_destroy , METH_NOARGS
                            , "Destroy associated hurricane object, the python object remains." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |                  "PyUnitRule" Object Methods                |
  // +-------------------------------------------------------------+


  PythonOnlyDeleteMethod(UnitRule)
  PyTypeObjectLinkPyType(UnitRule)


#else  // End of Python Module Code Part.


// +=================================================================+
// |                "PyUnitRule" Shared Library Code Part            |
// +=================================================================+

  
  PyTypeInheritedObjectDefinitions(UnitRule,Rule)
  LinkCreateMethod(UnitRule)


#endif  // End of Shared Library Code Part.

  }  // extern "C".

}  // Isobar namespace.
