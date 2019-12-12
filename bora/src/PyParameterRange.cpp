// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |  B o r a  -  A n a l o g   S l i c i n g   T r e e              |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./bora/PyParameterRange.cpp"                   |
// +-----------------------------------------------------------------+


#include "bora/PyParameterRange.h"
#include "bora/PyStepParameterRange.h"


namespace Bora {

  using namespace Hurricane;
  using namespace Isobar;


extern "C" {


#define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(ParameterRange,range,function)


// +=================================================================+
// |           "PyParameterRange" Python Module Code Part            |
// +=================================================================+

#if defined(__PYTHON_MODULE__)

  // Standart destroy (Attribute).
  //DBoDestroyAttribute(PyParameterRange_destroy ,PyParameterRange)

  // Standart Accessors (Attributes).
  DirectGetBoolAttribute(PyParameterRange_isValid ,isValid ,PyParameterRange,ParameterRange)
  DirectGetBoolAttribute(PyParameterRange_progress,progress,PyParameterRange,ParameterRange)
  DirectGetUIntAttribute(PyParameterRange_getIndex,getIndex,PyParameterRange,ParameterRange)
  DirectGetUIntAttribute(PyParameterRange_getSize ,getSize ,PyParameterRange,ParameterRange)
  DirectSetIntAttribute (PyParameterRange_setIndex,setIndex,PyParameterRange,ParameterRange)

  DirectVoidMethod(ParameterRange,range,reset)


  // ---------------------------------------------------------------
  // PyParameterRange Attribute Method table.

  PyMethodDef PyParameterRange_Methods[] =
    { { "isValid"              , (PyCFunction)PyParameterRange_isValid             , METH_NOARGS , "To be documented." }
    , { "getIndex"             , (PyCFunction)PyParameterRange_getIndex            , METH_NOARGS , "To be documented." }
    , { "getSize"              , (PyCFunction)PyParameterRange_getSize             , METH_NOARGS , "To be documented." }
    , { "progress"             , (PyCFunction)PyParameterRange_progress            , METH_NOARGS , "To be documented." }
    , { "setIndex"             , (PyCFunction)PyParameterRange_setIndex            , METH_VARARGS, "To be documented." }
    , { "reset"                , (PyCFunction)PyParameterRange_reset               , METH_NOARGS , "To be documented." }
  //, { "destroy"              , (PyCFunction)PyParameterRange_destroy             , METH_NOARGS
  //                           , "Destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}    /* sentinel */
    };


  PythonOnlyDeleteMethod(ParameterRange)
  PyTypeObjectLinkPyType(ParameterRange) 


#else  // End of Python Module Code Part.


// +=================================================================+
// |          "PyParameterRange" Shared Library Code Part            |
// +=================================================================+


  // ---------------------------------------------------------------
  // Allocator Method  :  "PyParameterRange_NEW ()"
  //
  // No PyParameterRange should ever be created, it's not a terminal object
  // of the class hierarchy. Instead create the real underlying PyObject.

  PyObject* PyParameterRange_NEW ( ParameterRange* range ) {
    if (not range) {
      PyErr_SetString ( HurricaneError, "PyParameterRange_NEW(): NULL range argument." );
      return NULL;
    }
    
    StepParameterRange* stepRange = dynamic_cast<StepParameterRange*>(range);
    if (stepRange) return PyStepParameterRange_Link( stepRange );

    Py_RETURN_NONE;
  }


  PyTypeRootObjectDefinitions(ParameterRange)


#endif  // Shared Library Code Part.

}  // extern "C".


// +=================================================================+
// |          "PyParameterRange" Shared Library Code Part            |
// +=================================================================+


# if !defined(__PYTHON_MODULE__)

ParameterRange* ParameterRangeCast ( PyObject* derivedObject ) {
  if (IsPyStepParameterRange(derivedObject))  return PYSTEPPARAMETERRANGE_O(derivedObject);

  return NULL;
}

#endif

}  // Bora namespace.
