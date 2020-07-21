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


  DirectGetBoolAttribute  (PyPhysicalRule_isDouble      ,isDouble      ,PyPhysicalRule,PhysicalRule)
  DirectGetBoolAttribute  (PyPhysicalRule_isDbU         ,isDbU         ,PyPhysicalRule,PhysicalRule)
  DirectGetBoolAttribute  (PyPhysicalRule_isSymmetric   ,isSymmetric   ,PyPhysicalRule,PhysicalRule)
  DirectGetBoolAttribute  (PyPhysicalRule_hasSteps      ,hasSteps      ,PyPhysicalRule,PhysicalRule)
  DirectGetDoubleAttribute(PyPhysicalRule_getDoubleValue,getDoubleValue,PyPhysicalRule,PhysicalRule)
  DirectSetBoolAttribute  (PyPhysicalRule_setSymmetric  ,setSymmetric  ,PyPhysicalRule,PhysicalRule)
  DirectDestroyAttribute  (PyPhysicalRule_destroy, PyPhysicalRule)


  static PyObject* PyPhysicalRule_getValue ( PyPhysicalRule *self, PyObject* args )
  {
    cdebug.log(49) << "PyPhysicalRule_getValue()" << endl;
    METHOD_HEAD("PhysicalRule.getValue()")

    DbU::Unit value = 0;

    HTRY
      long      length = 0;
      PyObject* hDir   = NULL;
      if (PyArg_ParseTuple(args,"|lO:PhysicalRule.getValue", &length, &hDir)) {
        if      (hDir)   value = rule->getValue( length, PyObject_IsTrue(hDir) );
        else if (length) value = rule->getValue( length );
        else             value = rule->getValue();
      } else {
        PyErr_SetString( ProxyError
                       , "invalid number or types of parameters for PhysicalRule.getValue()." );
        return NULL;
      }
    HCATCH

    return Py_BuildValue("I",value);
  }


  static PyObject* PyPhysicalRule_addValue ( PyPhysicalRule* self, PyObject *args )
  {
    cdebug.log(49) << "PyPhysicalRule_addValue()" << endl;
    METHOD_HEAD("PhysicalRule.addValue()")

    PyObject*     arg0 = NULL;
    PyObject*     arg1 = NULL;
    PyObject*     arg2 = NULL;

    HTRY
      __cs.init ("PhysicalRule.addValue");
      if (not PyArg_ParseTuple(args,"O&|O&O&:PhysicalRule.addValue"
                              ,Converter,&arg0
                              ,Converter,&arg1
                              ,Converter,&arg2
                              )) {
        PyErr_SetString( ConstructorError, "PhysicalRule.addValue(): invalid number of parameters." );
        return NULL;
      }
      if (__cs.getObjectIds() == ":float")
        rule->addValue( PyFloat_AsDouble(arg0) );
      else if (__cs.getObjectIds() == ":int:int")
        rule->addValue( PyAny_AsLong(arg0), PyAny_AsLong(arg1) );
      else if (__cs.getObjectIds() == ":int:int:int")
        rule->addValue( PyAny_AsLong(arg0), PyAny_AsLong(arg1), PyAny_AsLong(arg2) );
      else {
        PyErr_SetString( ConstructorError, "PhysicalRule.addValue(): Invalid number or bad type of parameters." );
        return NULL;
      }
    HCATCH

    Py_RETURN_NONE;
  }


  PyMethodDef PyPhysicalRule_Methods[] =
    { { "isDouble"          , (PyCFunction)PyPhysicalRule_isDouble, METH_NOARGS
                            , "Is the rule stored as a double." }
    , { "isDbU"             , (PyCFunction)PyPhysicalRule_isDbU, METH_NOARGS
                            , "Is the rule stored as a DbU::Unit." }
    , { "isSymmetric"       , (PyCFunction)PyPhysicalRule_isSymmetric, METH_NOARGS
                            , "Is the rule symmetric (for two layers rules)." }
    , { "hasSteps"          , (PyCFunction)PyPhysicalRule_hasSteps, METH_NOARGS
                            , "Does the rule have multiple length steps." }
    , { "getDoubleValue"    , (PyCFunction)PyPhysicalRule_getDoubleValue, METH_VARARGS
                            , "Returns the double value of the rule." }
    , { "getValue"          , (PyCFunction)PyPhysicalRule_getValue, METH_VARARGS
                            , "Returns the value (DbU::Unit)." }
    , { "addValue"          , (PyCFunction)PyPhysicalRule_addValue, METH_VARARGS
                            , "Add any kind of value to the rule." }
    , { "setSymmetric"      , (PyCFunction)PyPhysicalRule_setSymmetric, METH_VARARGS
                            , "Set/reset the symmetric state of the rule." }
    , { "destroy"           , (PyCFunction)PyPhysicalRule_destroy , METH_NOARGS
                            , "Destroy associated hurricane object, the python object remains." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |                  "PyPhysicalRule" Object Methods            |
  // +-------------------------------------------------------------+


  PythonOnlyDeleteMethod(PhysicalRule)
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
