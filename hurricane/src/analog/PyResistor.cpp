// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2019-2019, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyResistor.cpp"                              |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyLibrary.h"
#include "hurricane/analog/PyResistor.h"


namespace  Isobar {

using namespace Hurricane;
using namespace Analog;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT            _baseObject._baseObject._baseObject._baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject._baseObject._baseObject._baseObject)
#define METHOD_HEAD(function)    GENERIC_METHOD_HEAD(Resistor,resistor,function)

#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |                "PyResistor" Python Module Code Part             |
// +=================================================================+


  static PyObject* PyResistor_create ( PyObject*, PyObject* args )
  {
    cdebug.log(49) << "PyResistor_create()" << endl;

    Resistor*   resistor     = NULL;
    
    HTRY
      PyObject* pyLibrary       = NULL;
      char*     name            = NULL;
      long      pyType          = 0;
      
      if (PyArg_ParseTuple( args
                          , "Osl:Resistor.create"
                          , &pyLibrary
                          , &name
                          , &pyType
                          )) {
        if (not IsPyLibrary(pyLibrary)) {
          PyErr_SetString ( ConstructorError, "Resistor.create(): First argument is not of type Library." );
          return NULL;
        }
        switch ( pyType ) {
          case Resistor::LOWRES:
          case Resistor::HIRES: break;
          default:
            PyErr_SetString ( ConstructorError, "Resistor.create(): Type argument is neither LOWRES nor HIRES." );
            return NULL;
        }
      
        resistor = Resistor::create( PYLIBRARY_O(pyLibrary)
                                   , Name(name)
                                   , (Resistor::Type)pyType
                                   );
      } else {
        PyErr_SetString ( ConstructorError, "Bad parameters given to Resistor.create()." );
        return NULL;
      }
    HCATCH

    return PyResistor_Link(resistor);
  }


  // Standart Destroy (Attribute).
  DBoDestroyAttribute(PyResistor_destroy, PyResistor)

  //GetNameMethod(Resistor, device)


  // ---------------------------------------------------------------
  // PyResistor Attribute Method table.

  PyMethodDef PyResistor_Methods[] =
    { { "create"            , (PyCFunction)PyResistor_create         , METH_VARARGS|METH_STATIC
                            , "Create an anlogic device made of one resistor." }
    , { "destroy"           , (PyCFunction)PyResistor_destroy        , METH_NOARGS
                            , "Destroys associated hurricane object, the python object remains." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |                 "PyResistor" Object Methods                 |
  // +-------------------------------------------------------------+


  DBoDeleteMethod(Resistor)
  PyTypeObjectLinkPyType(Resistor)


#else  // End of Python Module Code Part.

// +=================================================================+
// |               "PyResistor" Shared Library Code Part             |
// +=================================================================+
  

  DBoLinkCreateMethod(Resistor)
  PyTypeInheritedObjectDefinitions(Resistor, ResistorFamily)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
