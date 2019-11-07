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
// |  C++ Module  :  "./bora/PyStepParameterRange.cpp"               |
// +-----------------------------------------------------------------+


#include "hurricane/analog/PyDevice.h"
#include "crlcore/PyRoutingGauge.h"
#include "bora/PyStepParameterRange.h"


namespace  Bora {

  using namespace Hurricane;
  using namespace Isobar;


extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT            _baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define METHOD_HEAD(function)    GENERIC_METHOD_HEAD(StepParameterRange,node,function)


// +=================================================================+
// |           "PyStepParameterRange" Python Module Code Part        |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  static PyObject* PyStepParameterRange_NEW ( PyObject* , PyObject* args )
  {
    double              start = 0.0;
    double              step  = 0.0;
    double              count = 0.0;
    StepParameterRange* range = NULL;

    HTRY
      if (not PyArg_ParseTuple( args, "ddd:StepParameterRange_NEW", &start, &step, &count ) ) {
        PyErr_SetString ( ConstructorError, "StepParameterRange.create(): Invalid/bad number of parameters ." );
        return NULL;
      }

      range = new StepParameterRange( start, step, count );
    HCATCH

    return PyStepParameterRange_Link(range);
  }


  static int  PyStepParameterRange_Init ( PyStepParameterRange* self, PyObject* args, PyObject* kwargs )
  {
    cdebug_log(20,0) << "PyStepParameterRange_Init(): " << (void*)self << endl;
    return 0;
  }


  static void  PyStepParameterRange_DeAlloc ( PyStepParameterRange* self )
  {
    cdebug_log(20,0) << "PyStepParameterRange_DeAlloc(" << (void*)self << ") "
          << (void*)(self->ACCESS_OBJECT) << ":" << self->ACCESS_OBJECT << endl;

    if (self->ACCESS_OBJECT and not self->ACCESS_OBJECT->isNested()) {
      cdebug_log(20,0) << "C++ object := " << &(self->ACCESS_OBJECT) << endl;
      delete self->ACCESS_OBJECT;
    }
    PyObject_DEL( self );
  }


  DirectGetDoubleAttribute(PyStepParameterRange_getValue,getValue,PyStepParameterRange,StepParameterRange)
  // Standart Destroy (Attribute).


  // ---------------------------------------------------------------
  // PyStepParameterRange Attribute Method table.

  PyMethodDef PyStepParameterRange_Methods[] =
    { { "getValue"          , (PyCFunction)PyStepParameterRange_getValue , METH_NOARGS , "To be documented." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |            "PyStepParameterRange" Object Methods            |
  // +-------------------------------------------------------------+


  PyTypeObjectLinkPyTypeNewInit(StepParameterRange)


#else  // End of Python Module Code Part.

// +=================================================================+
// |          "PyStepParameterRange" Shared Library Code Part        |
// +=================================================================+


  extern LinkCreateMethod(StepParameterRange);
  

  PyTypeInheritedObjectDefinitions(StepParameterRange, ParameterRange)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Bora namespace.
