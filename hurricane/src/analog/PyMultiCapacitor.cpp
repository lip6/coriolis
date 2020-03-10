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
// |  C++ Module  :  "./PyMultiCapacitor.cpp"                        |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyLibrary.h"
#include "hurricane/analog/PyMultiCapacitor.h"


namespace  Isobar {

using namespace Hurricane;
using namespace Analog;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT            _baseObject._baseObject._baseObject._baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject._baseObject._baseObject._baseObject)
#define METHOD_HEAD(function)    GENERIC_METHOD_HEAD(MultiCapacitor,transistor,function)

#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |             "PyMultiCapacitor" Python Module Code Part          |
// +=================================================================+


  static PyObject* PyMultiCapacitor_create ( PyObject*, PyObject* args )
  {
    cdebug.log(49) << "PyMultiCapacitor_create()" << endl;

    MultiCapacitor* mcapacitor = NULL;
    
    HTRY
    PyObject*     pyLibrary = NULL;
    char*         name      = NULL;
    long          pyType    = 0;
    unsigned int  count     = 1;
    
    if (PyArg_ParseTuple( args
                        , "OslI:MultiCapacitor.create"
                        , &pyLibrary
                        , &name
                        , &pyType
                        , &count
                        )) {
      if (not IsPyLibrary(pyLibrary)) {
        PyErr_SetString ( ConstructorError, "MultiCapacitor.create(): First argument is not of type Library." );
        return NULL;
      }
      switch ( pyType ) {
        case CapacitorFamily::PIP:
        case CapacitorFamily::MIM:
        case CapacitorFamily::MOM: break;
        default:
          PyErr_SetString ( ConstructorError, "MultiCapacitor.create(): Type argument is neither PIP, Multi nor MOM." );
          return NULL;
      }

      mcapacitor = MultiCapacitor::create( PYLIBRARY_O(pyLibrary)
                                         , Name(name)
                                         , (MultiCapacitor::Type)pyType
                                         , (size_t)count
                                         );
    } else {
      PyErr_SetString ( ConstructorError, "Bad parameters given to MultiCapacitor.create()." );
      return NULL;
    }
    HCATCH

    return PyMultiCapacitor_Link(mcapacitor);
  }


  // DirectGetDoubleAttribute(PyCapacitorFamily_getC        ,getC        ,PyMultiCapacitor,MultiCapacitor)
  // DirectGetDoubleAttribute(PyCapacitorFamily_getCDraw    ,getCDraw    ,PyMultiCapacitor,MultiCapacitor)
  // DirectGetDoubleAttribute(PyCapacitorFamily_getCBorder  ,getCBorder  ,PyMultiCapacitor,MultiCapacitor)
  // DirectGetDoubleAttribute(PyCapacitorFamily_getCParasite,getCParasite,PyMultiCapacitor,MultiCapacitor)
  DirectGetBoolAttribute(PyMultiCapacitor_hasDummy,hasDummy,PyMultiCapacitor,MultiCapacitor)
  DirectSetBoolAttribute(PyMultiCapacitor_setDummy,setDummy,PyMultiCapacitor,MultiCapacitor)

  // Standart Destroy (Attribute).
  DBoDestroyAttribute(PyMultiCapacitor_destroy, PyMultiCapacitor)


  // ---------------------------------------------------------------
  // PyMultiCapacitor Attribute Method table.

  PyMethodDef PyMultiCapacitor_Methods[] =
    { { "create"            , (PyCFunction)PyMultiCapacitor_create         , METH_VARARGS|METH_STATIC
                            , "Create an anlogic device made of one transistor." }
  //, { "getC"              , (PyCFunction)PyCapacitorFamily_getC        , METH_NOARGS
  //                        , "Capacitor value C (requested)." }
  //, { "getCDraw"          , (PyCFunction)PyCapacitorFamily_getCDraw    , METH_NOARGS
  //                        , "Capacitor value C (requested)." }
  //, { "getCBorder"        , (PyCFunction)PyCapacitorFamily_getCBorder  , METH_NOARGS
  //                        , "Capacitor value C (requested)." }
  //, { "getCParasite"      , (PyCFunction)PyCapacitorFamily_getCParasite, METH_NOARGS
  //                        , "Capacitor value C (requested)." }
    , { "hasDummy"          , (PyCFunction)PyMultiCapacitor_hasDummy     , METH_NOARGS
                            , "Self explanatory." }
    , { "setDummy"          , (PyCFunction)PyMultiCapacitor_setDummy     , METH_VARARGS
                            , "Self explanatory." }
    , { "destroy"           , (PyCFunction)PyMultiCapacitor_destroy      , METH_NOARGS
                            , "Destroys associated hurricane object, the python object remains." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |              "PyMultiCapacitor" Object Methods              |
  // +-------------------------------------------------------------+


  DBoDeleteMethod(MultiCapacitor)
  PyTypeObjectLinkPyType(MultiCapacitor)


#else  // End of Python Module Code Part.

// +=================================================================+
// |            "PyMultiCapacitor" Shared Library Code Part          |
// +=================================================================+
  

  DBoLinkCreateMethod(MultiCapacitor)
  PyTypeInheritedObjectDefinitions(MultiCapacitor, CapacitorFamily)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
