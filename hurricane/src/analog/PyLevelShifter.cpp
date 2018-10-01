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
// |  C++ Module  :  "./PyLevelShifter.cpp"                          |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyLibrary.h"
#include "hurricane/analog/PyLevelShifter.h"


namespace  Isobar {

using namespace Hurricane;
using namespace Analog;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT            _baseObject._baseObject._baseObject._baseObject._baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject._baseObject._baseObject._baseObject._baseObject)
#define METHOD_HEAD(function)    GENERIC_METHOD_HEAD(LevelShifter,transistor,function)

#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |            "PyLevelShifter" Python Module Code Part             |
// +=================================================================+


  static PyObject* PyLevelShifter_create ( PyObject*, PyObject* args )
  {
    cdebug.log(49) << "PyLevelShifter_create()" << endl;

    LevelShifter*   transistor     = NULL;
    
    HTRY
    PyObject* pyLibrary       = NULL;
    char*     name            = NULL;
    long      pyType          = 0;
    PyObject* pyBulkConnected = NULL;
    
    if (PyArg_ParseTuple( args
                        , "OslO:LevelShifter.create"
                        , &pyLibrary
                        , &name
                        , &pyType
                        , &pyBulkConnected
                        )) {
      if (not IsPyLibrary(pyLibrary)) {
        PyErr_SetString ( ConstructorError, "LevelShifter.create(): First argument is not of type Library." );
        return NULL;
      }
      switch ( pyType ) {
        case LevelShifter::NMOS:
        case LevelShifter::PMOS: break;
        default:
          PyErr_SetString ( ConstructorError, "LevelShifter.create(): Type argument is neither NMOS nor PMOS." );
          return NULL;
      }

      transistor = LevelShifter::create( PYLIBRARY_O(pyLibrary)
                                         , Name(name)
                                         , (LevelShifter::Type)pyType
                                         , PyObject_IsTrue(pyBulkConnected)
                                         );
    } else {
      PyErr_SetString ( ConstructorError, "Bad parameters given to LevelShifter.create()." );
      return NULL;
    }
    HCATCH

    return PyLevelShifter_Link(transistor);
  }


  // Standart Destroy (Attribute).
  DBoDestroyAttribute(PyLevelShifter_destroy, PyLevelShifter)

  //GetNameMethod(LevelShifter, device)


  // ---------------------------------------------------------------
  // PyLevelShifter Attribute Method table.

  PyMethodDef PyLevelShifter_Methods[] =
    { { "create"            , (PyCFunction)PyLevelShifter_create         , METH_VARARGS|METH_STATIC
                            , "Create an anlogic device made of one transistor." }
    , { "destroy"           , (PyCFunction)PyLevelShifter_destroy        , METH_NOARGS
                            , "Destroys associated hurricane object, the python object remains." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |               "PyLevelShifter" Object Methods               |
  // +-------------------------------------------------------------+


  DBoDeleteMethod(LevelShifter)
  PyTypeObjectLinkPyType(LevelShifter)


#else  // End of Python Module Code Part.

// +=================================================================+
// |             "PyLevelShifter" Shared Library Code Part           |
// +=================================================================+
  

  DBoLinkCreateMethod(LevelShifter)
  PyTypeInheritedObjectDefinitions(LevelShifter, TransistorPair)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
