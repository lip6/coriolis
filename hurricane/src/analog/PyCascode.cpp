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
// |  C++ Module  :  "./PyCascode.cpp"                               |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyLibrary.h"
#include "hurricane/analog/PyCascode.h"


namespace  Isobar {

using namespace Hurricane;
using namespace Analog;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT            _baseObject._baseObject._baseObject._baseObject._baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject._baseObject._baseObject._baseObject._baseObject)
#define METHOD_HEAD(function)    GENERIC_METHOD_HEAD(Cascode,transistor,function)

#if defined(__PYTHON_MODULE__)


// +=================================================================+
// |               "PyCascode" Python Module Code Part               |
// +=================================================================+


  static PyObject* PyCascode_create ( PyObject*, PyObject* args )
  {
    cdebug.log(49) << "PyCascode_create()" << endl;

    Cascode*   transistor     = NULL;
    
    HTRY
    PyObject* pyLibrary       = NULL;
    char*     name            = NULL;
    long      pyType          = 0;
    PyObject* pyBulkConnected = NULL;
    
    if (PyArg_ParseTuple( args
                        , "OslO:Cascode.create"
                        , &pyLibrary
                        , &name
                        , &pyType
                        , &pyBulkConnected
                        )) {
      if (not IsPyLibrary(pyLibrary)) {
        PyErr_SetString ( ConstructorError, "Cascode.create(): First argument is not of type Library." );
        return NULL;
      }
      switch ( pyType ) {
        case Cascode::NMOS:
        case Cascode::PMOS: break;
        default:
          PyErr_SetString ( ConstructorError, "Cascode.create(): Type argument is neither NMOS nor PMOS." );
          return NULL;
      }

      transistor = Cascode::create( PYLIBRARY_O(pyLibrary)
                                         , Name(name)
                                         , (Cascode::Type)pyType
                                         , PyObject_IsTrue(pyBulkConnected)
                                         );
    } else {
      PyErr_SetString ( ConstructorError, "Bad parameters given to Cascode.create()." );
      return NULL;
    }
    HCATCH

    return PyCascode_Link(transistor);
  }


  DBoDestroyAttribute(PyCascode_destroy, PyCascode)


  // ---------------------------------------------------------------
  // PyCascode Attribute Method table.

  PyMethodDef PyCascode_Methods[] =
    { { "create"            , (PyCFunction)PyCascode_create         , METH_VARARGS|METH_STATIC
                            , "Create an anlogic device made of one transistor." }
    , { "destroy"           , (PyCFunction)PyCascode_destroy        , METH_NOARGS
                            , "Destroys associated hurricane object, the python object remains." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |                  "PyCascode" Object Methods                 |
  // +-------------------------------------------------------------+

  DBoDeleteMethod(Cascode)
  PyTypeObjectLinkPyType(Cascode)


#else  // End of Python Module Code Part.

// +=================================================================+
// |                "PyCascode" Shared Library Code Part             |
// +=================================================================+
  

  DBoLinkCreateMethod(Cascode)
  PyTypeInheritedObjectDefinitions(Cascode, TransistorPair)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
