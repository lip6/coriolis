// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2022-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyErrorWidget.cpp"                      |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyErrorWidget.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


#define  METHOD_HEAD(function)  GENERIC_METHOD_HEAD(ErrorWidget,ew,function)


// +=================================================================+
// |             "PyErrorWidget" Python Module Code Part             |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  // Standart Destroy (Attribute).
  DirectDestroyAttribute(PyErrorWidget_destroy, PyErrorWidget)


  static PyObject* PyErrorWidget_run ( PyObject*, PyObject* args )
  {
    cdebug_log(20,0) << "PyErrorWidget_run()" << endl;
    HTRY
      char* message = NULL;
      char* trace   = NULL;
      if (not PyArg_ParseTuple(args,"s|s:ErrorWidget.run()", &message,&trace)) {
        PyErr_SetString( ConstructorError, "ErrorWidget.run(): Takes one or two *string* arguments." );
        return NULL;
      }
      if (ErrorWidget::run( QString(message), QString(trace) ))
        Py_RETURN_TRUE;
    HCATCH
    Py_RETURN_FALSE;
  }


  // ---------------------------------------------------------------
  // PyErrorWidget Attribute Method table.

  PyMethodDef PyErrorWidget_Methods[] =
    { { "run"               , (PyCFunction)PyErrorWidget_run, METH_STATIC|METH_VARARGS
                            , "Launch the error widget. Return True if continue." }
    , {NULL, NULL, 0, NULL} /* sentinel */
    };


  PythonOnlyDeleteMethod(ErrorWidget)
  PyTypeObjectLinkPyTypeWithoutObject(ErrorWidget,ErrorWidget)

#else  // End of Python Module Code Part.


// +=================================================================+
// |            "PyErrorWidget" Shared Library Code Part             |
// +=================================================================+


  PyTypeRootObjectDefinitions(ErrorWidget)

#endif  // Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
