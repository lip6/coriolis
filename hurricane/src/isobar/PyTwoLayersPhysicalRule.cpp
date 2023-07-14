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
// |  C++ Module  :  "./PyTwoLayersPhysicalRule.cpp"                 |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyTwoLayersPhysicalRule.h"


namespace  Isobar {

  using namespace Hurricane;

  extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT            _baseObject._baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject._baseObject)
#define METHOD_HEAD(function)    GENERIC_METHOD_HEAD(TwoLayersPhysicalRule,rule,function)


// +=================================================================+
// |          "TwoLayersPyPhysicalRule" Python Module Code Part      |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  DirectDestroyAttribute(PyTwoLayersPhysicalRule_destroy, PyTwoLayersPhysicalRule)

  DirectGetBoolAttribute(PyTwoLayersPhysicalRule_isSymetric, isSymetric, PyTwoLayersPhysicalRule, TwoLayersPhysicalRule)


  PyMethodDef PyTwoLayersPhysicalRule_Methods[] =
    { { "isSymetric"        , (PyCFunction)PyTwoLayersPhysicalRule_isSymetric, METH_NOARGS
                            , "Is the rule symetric regarding the two involved layers." }
    , { "destroy"           , (PyCFunction)PyTwoLayersPhysicalRule_destroy   , METH_NOARGS
                            , "Destroy associated hurricane object, the python object remains." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |                  "PyPhysicalRule" Object Methods            |
  // +-------------------------------------------------------------+


  PythonOnlyDeleteMethod(TwoLayersPhysicalRule)
  PyTypeObjectLinkPyType(TwoLayersPhysicalRule)


#else  // End of Python Module Code Part.


// +=================================================================+
// |                "PyPhysicalRule" Shared Library Code Part        |
// +=================================================================+

  
  PyTypeInheritedObjectDefinitions(TwoLayersPhysicalRule,PhysicalRule)
  LinkCreateMethod(TwoLayersPhysicalRule)


# endif  // End of Shared Library Code Part.

  }  // extern "C".

}  // Isobar namespace.
