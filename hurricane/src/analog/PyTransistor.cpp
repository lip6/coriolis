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
// |  C++ Module  :  "./PyTransistor.cpp"                            |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyLibrary.h"
#include "hurricane/analog/PyTransistor.h"


namespace  Isobar {

using namespace Hurricane;
using namespace Analog;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT            _baseObject._baseObject._baseObject._baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject._baseObject._baseObject._baseObject)
#define METHOD_HEAD(function)    GENERIC_METHOD_HEAD(Transistor,transistor,function)

#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |               "PyTransistor" Python Module Code Part            |
// +=================================================================+


  static PyObject* PyTransistor_create ( PyObject*, PyObject* args )
  {
    cdebug.log(49) << "PyTransistor_create()" << endl;

    Transistor*   transistor     = NULL;
    
    HTRY
    PyObject* pyLibrary       = NULL;
    char*     name            = NULL;
    long      pyType          = 0;
    PyObject* pyBulkConnected = NULL;
    
    if (PyArg_ParseTuple( args
                        , "OslO:Transistor.create"
                        , &pyLibrary
                        , &name
                        , &pyType
                        , &pyBulkConnected
                        )) {
      if (not IsPyLibrary(pyLibrary)) {
        PyErr_SetString ( ConstructorError, "Transistor.create(): First argument is not of type Library." );
        return NULL;
      }
      switch ( pyType ) {
        case Transistor::NMOS:
        case Transistor::PMOS: break;
        default:
          PyErr_SetString ( ConstructorError, "Transistor.create(): Type argument is neither NMOS nor PMOS." );
          return NULL;
      }

      transistor = Transistor::create( PYLIBRARY_O(pyLibrary)
                                     , Name(name)
                                     , (Transistor::Type)pyType
                                     , PyObject_IsTrue(pyBulkConnected)
                                     );
    } else {
      PyErr_SetString ( ConstructorError, "Bad parameters given to Transistor.create()." );
      return NULL;
    }
    HCATCH

    return PyTransistor_Link(transistor);
  }


  // Standart Destroy (Attribute).
  DBoDestroyAttribute(PyTransistor_destroy, PyTransistor)

  //GetNameMethod(Transistor, device)


  // ---------------------------------------------------------------
  // PyTransistor Attribute Method table.

  PyMethodDef PyTransistor_Methods[] =
    { { "create"            , (PyCFunction)PyTransistor_create         , METH_VARARGS|METH_STATIC
                            , "Create an anlogic device made of one transistor." }
    , { "destroy"           , (PyCFunction)PyTransistor_destroy        , METH_NOARGS
                            , "Destroys associated hurricane object, the python object remains." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |                "PyTransistor" Object Methods                |
  // +-------------------------------------------------------------+


  DBoDeleteMethod(Transistor)
  PyTypeObjectLinkPyType(Transistor)


#else  // End of Python Module Code Part.

// +=================================================================+
// |              "PyTransistor" Shared Library Code Part            |
// +=================================================================+
  

  DBoLinkCreateMethod(Transistor)
  PyTypeInheritedObjectDefinitions(Transistor, TransistorFamily)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
