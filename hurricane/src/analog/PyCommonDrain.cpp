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
// |  C++ Module  :  "./PyCommonDrain.cpp"                           |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyLibrary.h"
#include "hurricane/analog/PyCommonDrain.h"


namespace  Isobar {

using namespace Hurricane;
using namespace Analog;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT            _baseObject._baseObject._baseObject._baseObject._baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject._baseObject._baseObject._baseObject._baseObject)
#define METHOD_HEAD(function)    GENERIC_METHOD_HEAD(CommonDrain,transistor,function)

#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |            "PyCommonDrain" Python Module Code Part              |
// +=================================================================+


  static PyObject* PyCommonDrain_create ( PyObject*, PyObject* args )
  {
    cdebug.log(49) << "PyCommonDrain_create()" << endl;

    CommonDrain*   transistor     = NULL;
    
    HTRY
    PyObject* pyLibrary       = NULL;
    char*     name            = NULL;
    long      pyType          = 0;
    PyObject* pyBulkConnected = NULL;
    
    if (PyArg_ParseTuple( args
                        , "OslO:CommonDrain.create"
                        , &pyLibrary
                        , &name
                        , &pyType
                        , &pyBulkConnected
                        )) {
      if (not IsPyLibrary(pyLibrary)) {
        PyErr_SetString ( ConstructorError, "CommonDrain.create(): First argument is not of type Library." );
        return NULL;
      }
      switch ( pyType ) {
        case CommonDrain::NMOS:
        case CommonDrain::PMOS: break;
        default:
          PyErr_SetString ( ConstructorError, "CommonDrain.create(): Type argument is neither NMOS nor PMOS." );
          return NULL;
      }

      transistor = CommonDrain::create( PYLIBRARY_O(pyLibrary)
                                         , Name(name)
                                         , (CommonDrain::Type)pyType
                                         , PyObject_IsTrue(pyBulkConnected)
                                         );
    } else {
      PyErr_SetString ( ConstructorError, "Bad parameters given to CommonDrain.create()." );
      return NULL;
    }
    HCATCH

    return PyCommonDrain_Link(transistor);
  }


  // Standart Destroy (Attribute).
  DBoDestroyAttribute(PyCommonDrain_destroy, PyCommonDrain)

  //GetNameMethod(CommonDrain, device)


  // ---------------------------------------------------------------
  // PyCommonDrain Attribute Method table.

  PyMethodDef PyCommonDrain_Methods[] =
    { { "create"            , (PyCFunction)PyCommonDrain_create         , METH_VARARGS|METH_STATIC
                            , "Create an anlogic device made of one transistor." }
    , { "destroy"           , (PyCFunction)PyCommonDrain_destroy        , METH_NOARGS
                            , "Destroys associated hurricane object, the python object remains." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |               "PyCommonDrain" Object Methods                |
  // +-------------------------------------------------------------+


  DBoDeleteMethod(CommonDrain)
  PyTypeObjectLinkPyType(CommonDrain)


#else  // End of Python Module Code Part.

// +=================================================================+
// |             "PyCommonDrain" Shared Library Code Part            |
// +=================================================================+
  

  DBoLinkCreateMethod(CommonDrain)
  PyTypeInheritedObjectDefinitions(CommonDrain, TransistorPair)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
