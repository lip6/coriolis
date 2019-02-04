// -*- C++ -*-
//
// This file is part of the Chams Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyRule.cpp"                                  |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyRule.h"
#include "hurricane/isobar/PyUnitRule.h"
#include "hurricane/isobar/PyPhysicalRule.h"
#include "hurricane/isobar/PyTwoLayersPhysicalRule.h"


namespace  Isobar {

  using namespace Hurricane;

  extern "C" {


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(Rule,rule,function)


// +=================================================================+
// |                   "PyRule" Python Module Code Part              |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  DirectDestroyAttribute(PyRule_destroy, PyRule)

  GetNameMethod(Rule,rule)

  DirectGetStringAttribute(PyRule_getReference, getReference, PyRule, Rule)


  PyMethodDef PyRule_Methods[] =
    { { "getName"           , (PyCFunction)PyRule_getName     , METH_NOARGS
                            , "Returns the name of the rule." }
    , { "getReference"      , (PyCFunction)PyRule_getReference, METH_NOARGS
                            , "Returns the reference (founder documentation) of the rule." }
    , { "destroy"           , (PyCFunction)PyRule_destroy     , METH_NOARGS
                            , "Destroy associated hurricane object, the python object remains." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |                    "PyRule" Object Methods                  |
  // +-------------------------------------------------------------+


  DirectDeleteMethod(PyRule_DeAlloc,PyRule)
  PyTypeObjectLinkPyType(Rule)


#else  // End of Python Module Code Part.


// +=================================================================+
// |                  "PyRule" Shared Library Code Part              |
// +=================================================================+

  
PyTypeRootObjectDefinitions(Rule)
LinkCreateMethod(Rule)


  extern PyObject* PyRule_LinkDerived ( Rule* object )
  {
    if (object == NULL) Py_RETURN_NONE;

    UnitRule* unitRule = dynamic_cast<UnitRule*>(object);
    if (unitRule) return PyUnitRule_Link(unitRule);

    TwoLayersPhysicalRule* twoLayersPhysicalRule = dynamic_cast<TwoLayersPhysicalRule*>(object);
    if (twoLayersPhysicalRule) return PyTwoLayersPhysicalRule_Link(twoLayersPhysicalRule);

    PhysicalRule* physicalRule = dynamic_cast<PhysicalRule*>(object);
    if (physicalRule) return PyPhysicalRule_Link(physicalRule);

    Py_RETURN_NONE;
  }


#endif  // Shared Library Code Part.

  }  // extern "C".

}  // Isobar namespace.
