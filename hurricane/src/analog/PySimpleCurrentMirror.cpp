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
// |  C++ Module  :  "./PySimpleCurrentMirror.cpp"                   |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyLibrary.h"
#include "hurricane/analog/PySimpleCurrentMirror.h"


namespace  Isobar {

using namespace Hurricane;
using namespace Analog;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT            _baseObject._baseObject._baseObject._baseObject._baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject._baseObject._baseObject._baseObject._baseObject)
#define METHOD_HEAD(function)    GENERIC_METHOD_HEAD(SimpleCurrentMirror,transistor,function)

#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |        "PySimpleCurrentMirror" Python Module Code Part          |
// +=================================================================+


  static PyObject* PySimpleCurrentMirror_create ( PyObject*, PyObject* args )
  {
    cdebug.log(49) << "PySimpleCurrentMirror_create()" << endl;

    SimpleCurrentMirror*   transistor     = NULL;
    
    HTRY
    PyObject* pyLibrary       = NULL;
    char*     name            = NULL;
    long      pyType          = 0;
    PyObject* pyBulkConnected = NULL;
    
    if (PyArg_ParseTuple( args
                        , "OslO:SimpleCurrentMirror.create"
                        , &pyLibrary
                        , &name
                        , &pyType
                        , &pyBulkConnected
                        )) {
      if (not IsPyLibrary(pyLibrary)) {
        PyErr_SetString ( ConstructorError, "SimpleCurrentMirror.create(): First argument is not of type Library." );
        return NULL;
      }
      switch ( pyType ) {
        case SimpleCurrentMirror::NMOS:
        case SimpleCurrentMirror::PMOS: break;
        default:
          PyErr_SetString ( ConstructorError, "SimpleCurrentMirror.create(): Type argument is neither NMOS nor PMOS." );
          return NULL;
      }

      transistor = SimpleCurrentMirror::create( PYLIBRARY_O(pyLibrary)
                                         , Name(name)
                                         , (SimpleCurrentMirror::Type)pyType
                                         , PyObject_IsTrue(pyBulkConnected)
                                         );
    } else {
      PyErr_SetString ( ConstructorError, "Bad parameters given to SimpleCurrentMirror.create()." );
      return NULL;
    }
    HCATCH

    return PySimpleCurrentMirror_Link(transistor);
  }


  // Standart Destroy (Attribute).
  DBoDestroyAttribute(PySimpleCurrentMirror_destroy, PySimpleCurrentMirror)

  //GetNameMethod(SimpleCurrentMirror, device)


  // ---------------------------------------------------------------
  // PySimpleCurrentMirror Attribute Method table.

  PyMethodDef PySimpleCurrentMirror_Methods[] =
    { { "create"            , (PyCFunction)PySimpleCurrentMirror_create         , METH_VARARGS|METH_STATIC
                            , "Create an anlogic device made of one transistor." }
    , { "destroy"           , (PyCFunction)PySimpleCurrentMirror_destroy        , METH_NOARGS
                            , "Destroys associated hurricane object, the python object remains." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |           "PySimpleCurrentMirror" Object Methods            |
  // +-------------------------------------------------------------+


  DBoDeleteMethod(SimpleCurrentMirror)
  PyTypeObjectLinkPyType(SimpleCurrentMirror)


#else  // End of Python Module Code Part.

// +=================================================================+
// |         "PySimpleCurrentMirror" Shared Library Code Part        |
// +=================================================================+
  

  DBoLinkCreateMethod(SimpleCurrentMirror)
  PyTypeInheritedObjectDefinitions(SimpleCurrentMirror, TransistorPair)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
