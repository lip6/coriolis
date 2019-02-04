// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyPhysicalRule.cpp"                          |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyPhysicalRule.h"


namespace  Isobar {

  using namespace Hurricane;

  extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT            _baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define METHOD_HEAD(function)    GENERIC_METHOD_HEAD(PhysicalRule,rule,function)


// +=================================================================+
// |                 "PyPhysicalRule" Python Module Code Part        |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  DirectDestroyAttribute(PyPhysicalRule_destroy, PyPhysicalRule)

  DirectGetLongAttribute(PyPhysicalRule_getValue, getValue, PyPhysicalRule, PhysicalRule)


  PyMethodDef PyPhysicalRule_Methods[] =
    { { "getValue"          , (PyCFunction)PyPhysicalRule_getValue, METH_NOARGS
                            , "Returns the value (DbU::Unit)." }
    , { "destroy"           , (PyCFunction)PyPhysicalRule_destroy , METH_NOARGS
                            , "Destroy associated hurricane object, the python object remains." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |                  "PyPhysicalRule" Object Methods            |
  // +-------------------------------------------------------------+


  DirectDeleteMethod(PyPhysicalRule_DeAlloc,PyPhysicalRule)
  PyTypeObjectLinkPyType(PhysicalRule)


#else  // End of Python Module Code Part.


// +=================================================================+
// |                "PyPhysicalRule" Shared Library Code Part        |
// +=================================================================+

  
  PyTypeInheritedObjectDefinitions(PhysicalRule,Rule)
  LinkCreateMethod(PhysicalRule)


# endif  // End of Shared Library Code Part.

  }  // extern "C".

}  // Isobar namespace.
