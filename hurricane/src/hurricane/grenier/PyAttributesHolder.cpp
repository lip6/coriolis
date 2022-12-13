// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2008-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyAttributesHolder.cpp"                 |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyAttributesHolder.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


// +=================================================================+
// |           "PyAttributesHolder" Python Module Code Part          |
// +=================================================================+

#if defined(__PYTHON_MODULE__)
  

  static PyObject* PyAttributesHolder_NEW ( PyObject *module, PyObject *args )
  {
    cdebug_log(20,0) << "PyAttributesHolder_NEW()" << endl;
    PyAttributesHolder* pyAttributeHolder = PyObject_NEW(PyAttributesHolder, &PyTypeAttributesHolder);
    if (pyAttributeHolder == NULL) return NULL;
    return ( (PyObject*)pyAttributeHolder );
  }


  static int  PyAttributesHolder_Init ( PyAttributesHolder* self, PyObject* args, PyObject* kwargs )
  {
    cdebug_log(20,0) << "PyAttributesHolder_Init(): " << (void*)self << endl;
    return 0;
  }


  PyMethodDef PyAttributesHolder_Methods[] =
    { {NULL, NULL, 0, NULL}           /* sentinel */
    };


//DirectDeleteMethod(PyAttributesHolder_DeAlloc,PyAttributesHolder)

#else  // End of Python Module Code Part.


// +=================================================================+
// |          "PyAttributesHolder" Shared Library Code Part          |
// +=================================================================+

  
  PyTypeInheritedObjectDefinitions(AttributesHolder,PyList_Type)

# endif  // End of Shared Library Code Part.

}  // End of extern "C".

}  // End of Isobar namespace.
 
