// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2019-2019, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyCapacitiesParameter.cpp"                   |
// +-----------------------------------------------------------------+


#include "hurricane/analog/PyCapacitiesParameter.h"


namespace  Isobar {

using namespace Hurricane;
using namespace Analog;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define METHOD_HEAD(function)   GENERIC_METHOD_HEAD(CapacitiesParameter,capacitiesParameter,function)

#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |         "PyCapacitiesParameter" Python Module Code Part         |
// +=================================================================+


  DirectGetUIntAttribute(PyCapacitiesParameter_getCount, getCount, PyCapacitiesParameter, CapacitiesParameter)


  static PyObject* PyCapacitiesParameter_getValue ( PyCapacitiesParameter *self, PyObject* args )
  {
    cdebug_log(20,0) << "PyCapacitiesParameter.getValue()" << endl;
   
    double value = 0.0;

    HTRY
      METHOD_HEAD ( "CapacitiesParameter.getValue()" )
      unsigned int  index = 0;
      if (PyArg_ParseTuple(args,"I:CapacitiesParameter.getValue",&index)) {
        value = capacitiesParameter->getValue( (size_t)index );
      } else {
        PyErr_SetString( ConstructorError, "CapacitiesParameter.getValue(): Invalid number/bad type of parameters." );
        return NULL;
      }
    HCATCH

    return Py_BuildValue("d",value);
  }


  static PyObject* PyCapacitiesParameter_setValue ( PyCapacitiesParameter *self, PyObject* args )
  {
    cdebug_log(20,0) << "PyCapacitiesParameter.setValue()" << endl;
   
    HTRY
      METHOD_HEAD ( "CapacitiesParameter.setValue()" )
      unsigned int  count  = 0;
      double        value  = 0.0;
      if (PyArg_ParseTuple(args,"Id:CapacitiesParameter.setValue",&count,&value)) {
        capacitiesParameter->setValue( (size_t)count, value );
      } else {
        PyErr_SetString( ConstructorError, "CapacitiesParameter.setValue(): Invalid number/bad type of parameters." );
        return NULL;
      }
    HCATCH

    Py_RETURN_NONE;
  }


  // ---------------------------------------------------------------
  // PyCapacitiesParameter Attribute Method table.

  PyMethodDef PyCapacitiesParameter_Methods[] =
    { { "getCount"          , (PyCFunction)PyCapacitiesParameter_getCount  , METH_NOARGS
                            , "Self explanatory." }
    , { "getValue"          , (PyCFunction)PyCapacitiesParameter_getValue  , METH_VARARGS
                            , "Self explanatory." }
    , { "setValue"          , (PyCFunction)PyCapacitiesParameter_setValue  , METH_VARARGS
                            , "Self explanatory." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |          "PyCapacitiesParameter" Object Methods             |
  // +-------------------------------------------------------------+


  PythonOnlyDeleteMethod(CapacitiesParameter)
  PyTypeObjectLinkPyType(CapacitiesParameter)


#else  // End of Python Module Code Part.

// +=================================================================+
// |        "PyCapacitiesParameter" Shared Library Code Part         |
// +=================================================================+
  

  LinkCreateMethod(CapacitiesParameter)
  PyTypeInheritedObjectDefinitions(CapacitiesParameter, Parameter)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
