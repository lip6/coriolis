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
// |  C++ Module  :  "./PyCommonGatePair.cpp"                        |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyLibrary.h"
#include "hurricane/analog/PyCommonGatePair.h"


namespace  Isobar {

using namespace Hurricane;
using namespace Analog;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT            _baseObject._baseObject._baseObject._baseObject._baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject._baseObject._baseObject._baseObject._baseObject)
#define METHOD_HEAD(function)    GENERIC_METHOD_HEAD(CommonGatePair,transistor,function)

#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |           "PyCommonGatePair" Python Module Code Part            |
// +=================================================================+


  static PyObject* PyCommonGatePair_create ( PyObject*, PyObject* args )
  {
    cdebug.log(49) << "PyCommonGatePair_create()" << endl;

    CommonGatePair*   transistor     = NULL;
    
    HTRY
    PyObject* pyLibrary       = NULL;
    char*     name            = NULL;
    long      pyType          = 0;
    PyObject* pyBulkConnected = NULL;
    
    if (PyArg_ParseTuple( args
                        , "OslO:CommonGatePair.create"
                        , &pyLibrary
                        , &name
                        , &pyType
                        , &pyBulkConnected
                        )) {
      if (not IsPyLibrary(pyLibrary)) {
        PyErr_SetString ( ConstructorError, "CommonGatePair.create(): First argument is not of type Library." );
        return NULL;
      }
      switch ( pyType ) {
        case CommonGatePair::NMOS:
        case CommonGatePair::PMOS: break;
        default:
          PyErr_SetString ( ConstructorError, "CommonGatePair.create(): Type argument is neither NMOS nor PMOS." );
          return NULL;
      }

      transistor = CommonGatePair::create( PYLIBRARY_O(pyLibrary)
                                         , Name(name)
                                         , (CommonGatePair::Type)pyType
                                         , PyObject_IsTrue(pyBulkConnected)
                                         );
    } else {
      PyErr_SetString ( ConstructorError, "Bad parameters given to CommonGatePair.create()." );
      return NULL;
    }
    HCATCH

    return PyCommonGatePair_Link(transistor);
  }


  // Standart Destroy (Attribute).
  DBoDestroyAttribute(PyCommonGatePair_destroy, PyCommonGatePair)

  //GetNameMethod(CommonGatePair, device)


  // ---------------------------------------------------------------
  // PyCommonGatePair Attribute Method table.

  PyMethodDef PyCommonGatePair_Methods[] =
    { { "create"            , (PyCFunction)PyCommonGatePair_create         , METH_VARARGS|METH_STATIC
                            , "Create an anlogic device made of one transistor." }
    , { "destroy"           , (PyCFunction)PyCommonGatePair_destroy        , METH_NOARGS
                            , "Destroys associated hurricane object, the python object remains." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |              "PyCommonGatePair" Object Methods              |
  // +-------------------------------------------------------------+


  DBoDeleteMethod(CommonGatePair)
  PyTypeObjectLinkPyType(CommonGatePair)


#else  // End of Python Module Code Part.

// +=================================================================+
// |            "PyCommonGatePair" Shared Library Code Part          |
// +=================================================================+
  

  DBoLinkCreateMethod(CommonGatePair)
  PyTypeInheritedObjectDefinitions(CommonGatePair, TransistorPair)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
