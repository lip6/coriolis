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
// |  C++ Module  :  "./PyCrossCoupledPair.cpp"                      |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyLibrary.h"
#include "hurricane/analog/PyCrossCoupledPair.h"


namespace  Isobar {

using namespace Hurricane;
using namespace Analog;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT            _baseObject._baseObject._baseObject._baseObject._baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject._baseObject._baseObject._baseObject._baseObject)
#define METHOD_HEAD(function)    GENERIC_METHOD_HEAD(CrossCoupledPair,transistor,function)

#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |          "PyCrossCoupledPair" Python Module Code Part           |
// +=================================================================+


  static PyObject* PyCrossCoupledPair_create ( PyObject*, PyObject* args )
  {
    cdebug.log(49) << "PyCrossCoupledPair_create()" << endl;

    CrossCoupledPair*   transistor     = NULL;
    
    HTRY
    PyObject* pyLibrary       = NULL;
    char*     name            = NULL;
    long      pyType          = 0;
    PyObject* pyBulkConnected = NULL;
    
    if (PyArg_ParseTuple( args
                        , "OslO:CrossCoupledPair.create"
                        , &pyLibrary
                        , &name
                        , &pyType
                        , &pyBulkConnected
                        )) {
      if (not IsPyLibrary(pyLibrary)) {
        PyErr_SetString ( ConstructorError, "CrossCoupledPair.create(): First argument is not of type Library." );
        return NULL;
      }
      switch ( pyType ) {
        case CrossCoupledPair::NMOS:
        case CrossCoupledPair::PMOS: break;
        default:
          PyErr_SetString ( ConstructorError, "CrossCoupledPair.create(): Type argument is neither NMOS nor PMOS." );
          return NULL;
      }

      transistor = CrossCoupledPair::create( PYLIBRARY_O(pyLibrary)
                                         , Name(name)
                                         , (CrossCoupledPair::Type)pyType
                                         , PyObject_IsTrue(pyBulkConnected)
                                         );
    } else {
      PyErr_SetString ( ConstructorError, "Bad parameters given to CrossCoupledPair.create()." );
      return NULL;
    }
    HCATCH

    return PyCrossCoupledPair_Link(transistor);
  }


  // Standart Destroy (Attribute).
  DBoDestroyAttribute(PyCrossCoupledPair_destroy, PyCrossCoupledPair)

  //GetNameMethod(CrossCoupledPair, device)


  // ---------------------------------------------------------------
  // PyCrossCoupledPair Attribute Method table.

  PyMethodDef PyCrossCoupledPair_Methods[] =
    { { "create"            , (PyCFunction)PyCrossCoupledPair_create         , METH_VARARGS|METH_STATIC
                            , "Create an anlogic device made of one transistor." }
    , { "destroy"           , (PyCFunction)PyCrossCoupledPair_destroy        , METH_NOARGS
                            , "Destroys associated hurricane object, the python object remains." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |             "PyCrossCoupledPair" Object Methods             |
  // +-------------------------------------------------------------+


  DBoDeleteMethod(CrossCoupledPair)
  PyTypeObjectLinkPyType(CrossCoupledPair)


#else  // End of Python Module Code Part.

// +=================================================================+
// |           "PyCrossCoupledPair" Shared Library Code Part         |
// +=================================================================+
  

  DBoLinkCreateMethod(CrossCoupledPair)
  PyTypeInheritedObjectDefinitions(CrossCoupledPair, TransistorPair)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
