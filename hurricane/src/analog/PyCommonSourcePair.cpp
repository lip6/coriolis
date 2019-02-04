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
// |  C++ Module  :  "./PyCommonSourcePair.cpp"                      |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyLibrary.h"
#include "hurricane/analog/PyCommonSourcePair.h"


namespace  Isobar {

using namespace Hurricane;
using namespace Analog;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT            _baseObject._baseObject._baseObject._baseObject._baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject._baseObject._baseObject._baseObject._baseObject)
#define METHOD_HEAD(function)    GENERIC_METHOD_HEAD(CommonSourcePair,transistor,function)

#if defined(__PYTHON_MODULE__)


// +=================================================================+
// |          "PyCommonSourcePair" Python Module Code Part           |
// +=================================================================+


  static PyObject* PyCommonSourcePair_create ( PyObject*, PyObject* args )
  {
    cdebug.log(49) << "PyCommonSourcePair_create()" << endl;

    CommonSourcePair*   transistor     = NULL;
    
    HTRY
    PyObject* pyLibrary       = NULL;
    char*     name            = NULL;
    long      pyType          = 0;
    PyObject* pyBulkConnected = NULL;
    
    if (PyArg_ParseTuple( args
                        , "OslO:CommonSourcePair.create"
                        , &pyLibrary
                        , &name
                        , &pyType
                        , &pyBulkConnected
                        )) {
      if (not IsPyLibrary(pyLibrary)) {
        PyErr_SetString ( ConstructorError, "CommonSourcePair.create(): First argument is not of type Library." );
        return NULL;
      }
      switch ( pyType ) {
        case CommonSourcePair::NMOS:
        case CommonSourcePair::PMOS: break;
        default:
          PyErr_SetString ( ConstructorError, "CommonSourcePair.create(): Type argument is neither NMOS nor PMOS." );
          return NULL;
      }

      transistor = CommonSourcePair::create( PYLIBRARY_O(pyLibrary)
                                         , Name(name)
                                         , (CommonSourcePair::Type)pyType
                                         , PyObject_IsTrue(pyBulkConnected)
                                         );
    } else {
      PyErr_SetString ( ConstructorError, "Bad parameters given to CommonSourcePair.create()." );
      return NULL;
    }
    HCATCH

    return PyCommonSourcePair_Link(transistor);
  }


  // Standart Destroy (Attribute).
  DBoDestroyAttribute(PyCommonSourcePair_destroy, PyCommonSourcePair)

  //GetNameMethod(CommonSourcePair, device)


  // ---------------------------------------------------------------
  // PyCommonSourcePair Attribute Method table.

  PyMethodDef PyCommonSourcePair_Methods[] =
    { { "create"            , (PyCFunction)PyCommonSourcePair_create         , METH_VARARGS|METH_STATIC
                            , "Create an anlogic device made of one transistor." }
    , { "destroy"           , (PyCFunction)PyCommonSourcePair_destroy        , METH_NOARGS
                            , "Destroys associated hurricane object, the python object remains." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |             "PyCommonSourcePair" Object Methods             |
  // +-------------------------------------------------------------+


  DBoDeleteMethod(CommonSourcePair)
  PyTypeObjectLinkPyType(CommonSourcePair)


#else  // End of Python Module Code Part.

// +=================================================================+
// |           "PyCommonSourcePair" Shared Library Code Part         |
// +=================================================================+
  

  DBoLinkCreateMethod(CommonSourcePair)
  PyTypeInheritedObjectDefinitions(CommonSourcePair, TransistorPair)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
