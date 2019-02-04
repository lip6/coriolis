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
// |  C++ Module  :  "./PyMIMCapacitor.cpp"                          |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyLibrary.h"
#include "hurricane/analog/PyMIMCapacitor.h"


namespace  Isobar {

using namespace Hurricane;
using namespace Analog;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT            _baseObject._baseObject._baseObject._baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject._baseObject._baseObject._baseObject)
#define METHOD_HEAD(function)    GENERIC_METHOD_HEAD(MIMCapacitor,transistor,function)

#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |              "PyMIMCapacitor" Python Module Code Part           |
// +=================================================================+


  static PyObject* PyMIMCapacitor_create ( PyObject*, PyObject* args )
  {
    cdebug.log(49) << "PyMIMCapacitor_create()" << endl;

    MIMCapacitor*   transistor     = NULL;
    
    HTRY
    PyObject* pyLibrary       = NULL;
    char*     name            = NULL;
    long      pyType          = 0;
    
    if (PyArg_ParseTuple( args
                        , "Osl:MIMCapacitor.create"
                        , &pyLibrary
                        , &name
                        , &pyType
                        )) {
      if (not IsPyLibrary(pyLibrary)) {
        PyErr_SetString ( ConstructorError, "MIMCapacitor.create(): First argument is not of type Library." );
        return NULL;
      }
      switch ( pyType ) {
        case CapacitorFamily::PIP:
        case CapacitorFamily::MIM:
        case CapacitorFamily::MOM: break;
        default:
          PyErr_SetString ( ConstructorError, "MIMCapacitor.create(): Type argument is neither PIP, MIM nor MOM." );
          return NULL;
      }

      transistor = MIMCapacitor::create( PYLIBRARY_O(pyLibrary)
                                       , Name(name)
                                       , (MIMCapacitor::Type)pyType
                                       );
    } else {
      PyErr_SetString ( ConstructorError, "Bad parameters given to MIMCapacitor.create()." );
      return NULL;
    }
    HCATCH

    return PyMIMCapacitor_Link(transistor);
  }


  DirectGetDoubleAttribute(PyCapacitorFamily_getC        ,getC        ,PyMIMCapacitor,MIMCapacitor)
  DirectGetDoubleAttribute(PyCapacitorFamily_getCDraw    ,getCDraw    ,PyMIMCapacitor,MIMCapacitor)
  DirectGetDoubleAttribute(PyCapacitorFamily_getCBorder  ,getCBorder  ,PyMIMCapacitor,MIMCapacitor)
  DirectGetDoubleAttribute(PyCapacitorFamily_getCParasite,getCParasite,PyMIMCapacitor,MIMCapacitor)

  // Standart Destroy (Attribute).
  DBoDestroyAttribute(PyMIMCapacitor_destroy, PyMIMCapacitor)


  // ---------------------------------------------------------------
  // PyMIMCapacitor Attribute Method table.

  PyMethodDef PyMIMCapacitor_Methods[] =
    { { "create"            , (PyCFunction)PyMIMCapacitor_create         , METH_VARARGS|METH_STATIC
                            , "Create an anlogic device made of one transistor." }
    , { "getC"              , (PyCFunction)PyCapacitorFamily_getC        , METH_NOARGS
                            , "Capacitor value C (requested)." }
    , { "getCDraw"          , (PyCFunction)PyCapacitorFamily_getCDraw    , METH_NOARGS
                            , "Capacitor value C (requested)." }
    , { "getCBorder"        , (PyCFunction)PyCapacitorFamily_getCBorder  , METH_NOARGS
                            , "Capacitor value C (requested)." }
    , { "getCParasite"      , (PyCFunction)PyCapacitorFamily_getCParasite, METH_NOARGS
                            , "Capacitor value C (requested)." }
    , { "destroy"           , (PyCFunction)PyMIMCapacitor_destroy        , METH_NOARGS
                            , "Destroys associated hurricane object, the python object remains." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |               "PyMIMCapacitor" Object Methods               |
  // +-------------------------------------------------------------+


  DBoDeleteMethod(MIMCapacitor)
  PyTypeObjectLinkPyType(MIMCapacitor)


#else  // End of Python Module Code Part.

// +=================================================================+
// |             "PyMIMCapacitor" Shared Library Code Part           |
// +=================================================================+
  

  DBoLinkCreateMethod(MIMCapacitor)
  PyTypeInheritedObjectDefinitions(MIMCapacitor, CapacitorFamily)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
