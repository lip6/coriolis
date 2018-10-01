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
// |  C++ Module  :  "./PyDifferentialPair.cpp"                      |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyLibrary.h"
#include "hurricane/analog/PyDifferentialPair.h"


namespace  Isobar {

using namespace Hurricane;
using namespace Analog;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT            _baseObject._baseObject._baseObject._baseObject._baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject._baseObject._baseObject._baseObject._baseObject)
#define METHOD_HEAD(function)    GENERIC_METHOD_HEAD(DifferentialPair,transistor,function)

#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |          "PyDifferentialPair" Python Module Code Part           |
// +=================================================================+


  static PyObject* PyDifferentialPair_create ( PyObject*, PyObject* args )
  {
    cdebug.log(49) << "PyDifferentialPair_create()" << endl;

    DifferentialPair*   transistor     = NULL;
    
    HTRY
    PyObject* pyLibrary       = NULL;
    char*     name            = NULL;
    long      pyType          = 0;
    PyObject* pyBulkConnected = NULL;
    
    if (PyArg_ParseTuple( args
                        , "OslO:DifferentialPair.create"
                        , &pyLibrary
                        , &name
                        , &pyType
                        , &pyBulkConnected
                        )) {
      if (not IsPyLibrary(pyLibrary)) {
        PyErr_SetString ( ConstructorError, "DifferentialPair.create(): First argument is not of type Library." );
        return NULL;
      }
      switch ( pyType ) {
        case DifferentialPair::NMOS:
        case DifferentialPair::PMOS: break;
        default:
          PyErr_SetString ( ConstructorError, "DifferentialPair.create(): Type argument is neither NMOS nor PMOS." );
          return NULL;
      }

      transistor = DifferentialPair::create( PYLIBRARY_O(pyLibrary)
                                         , Name(name)
                                         , (DifferentialPair::Type)pyType
                                         , PyObject_IsTrue(pyBulkConnected)
                                         );
    } else {
      PyErr_SetString ( ConstructorError, "Bad parameters given to DifferentialPair.create()." );
      return NULL;
    }
    HCATCH

    return PyDifferentialPair_Link(transistor);
  }


  // Standart Destroy (Attribute).
  DBoDestroyAttribute(PyDifferentialPair_destroy, PyDifferentialPair)

  //GetNameMethod(DifferentialPair, device)


  // ---------------------------------------------------------------
  // PyDifferentialPair Attribute Method table.

  PyMethodDef PyDifferentialPair_Methods[] =
    { { "create"            , (PyCFunction)PyDifferentialPair_create         , METH_VARARGS|METH_STATIC
                            , "Create an anlogic device made of one transistor." }
    , { "destroy"           , (PyCFunction)PyDifferentialPair_destroy        , METH_NOARGS
                            , "Destroys associated hurricane object, the python object remains." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |             "PyDifferentialPair" Object Methods             |
  // +-------------------------------------------------------------+


  DBoDeleteMethod(DifferentialPair)
  PyTypeObjectLinkPyType(DifferentialPair)


#else  // End of Python Module Code Part.

// +=================================================================+
// |           "PyDifferentialPair" Shared Library Code Part         |
// +=================================================================+
  

  DBoLinkCreateMethod(DifferentialPair)
  PyTypeInheritedObjectDefinitions(DifferentialPair, TransistorPair)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
