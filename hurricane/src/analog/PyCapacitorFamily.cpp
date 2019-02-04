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
// |  C++ Module  :  "./PyCapacitorFamily.cpp"                       |
// +-----------------------------------------------------------------+


#include "hurricane/analog/PyCapacitorFamily.h"


namespace  Isobar {

using namespace Hurricane;
using namespace Analog;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._baseObject._baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject._baseObject._baseObject)
#define METHOD_HEAD(function)   GENERIC_METHOD_HEAD(CapacitorFamily,family,function)

#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |            "PyCapacitorFamily" Python Module Code Part          |
// +=================================================================+


  static PyObject* PyCapacitorFamily_getType ( PyCapacitorFamily* self )
  {
    CapacitorFamily::Type  type = CapacitorFamily::MIM;
      
    HTRY
      METHOD_HEAD ( "CapacitorFamily.getType()" )
      type = family->getType();
    HCATCH

      return Py_BuildValue( "i", (int)type );
  }


  DirectGetBoolAttribute  (PyCapacitorFamily_isPIP ,isPIP ,PyCapacitorFamily,CapacitorFamily)
  DirectGetBoolAttribute  (PyCapacitorFamily_isMIM ,isMIM ,PyCapacitorFamily,CapacitorFamily)
  DirectGetBoolAttribute  (PyCapacitorFamily_isMOM ,isMOM ,PyCapacitorFamily,CapacitorFamily)
  DirectGetIntAttribute   (PyCapacitorFamily_getRow,getRow,PyCapacitorFamily,CapacitorFamily)

  DirectGetDoubleAttribute(PyCapacitorFamily_getCE ,getCE ,PyCapacitorFamily,CapacitorFamily)
  DirectSetDoubleAttribute(PyCapacitorFamily_setCE ,setCE ,PyCapacitorFamily,CapacitorFamily)


  // ---------------------------------------------------------------
  // PyCapacitorFamily Attribute Method table.

  PyMethodDef PyCapacitorFamily_Methods[] =
    { { "getType"           , (PyCFunction)PyCapacitorFamily_getType        , METH_NOARGS
                            , "Returns the type of the transistor (as a numric constant)." }
    , { "isPIP"             , (PyCFunction)PyCapacitorFamily_isPIP          , METH_NOARGS
                            , "Returns True if it is a PIP capacitor." }
    , { "isMIM"             , (PyCFunction)PyCapacitorFamily_isMIM          , METH_NOARGS
                            , "Returns True if it is a MIM capacitor." }
    , { "isMOM"             , (PyCFunction)PyCapacitorFamily_isMOM          , METH_NOARGS
                            , "Returns True if it is a MOM capacitor." }
    , { "getCE"             , (PyCFunction)PyCapacitorFamily_getCE          , METH_NOARGS
                            , "Capacitor value C (requested)." }
    , { "setCE"             , (PyCFunction)PyCapacitorFamily_setCE          , METH_VARARGS
                            , "Capacitor value C (requested)." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |             "PyCapacitorFamily" Object Methods              |
  // +-------------------------------------------------------------+

  DBoDeleteMethod(CapacitorFamily)
  PyTypeObjectLinkPyType(CapacitorFamily)


#else  // End of Python Module Code Part.

// +=================================================================+
// |           "PyCapacitorFamily" Shared Library Code Part          |
// +=================================================================+

  DBoLinkCreateMethod(CapacitorFamily)
  PyTypeInheritedObjectDefinitions(CapacitorFamily, Device)


  extern  void  PyCapacitorFamily_postModuleInit ()
  {
    PyObject* constant;

    LoadObjectConstant(PyTypeCapacitorFamily.tp_dict,CapacitorFamily::PIP,"PIP");
    LoadObjectConstant(PyTypeCapacitorFamily.tp_dict,CapacitorFamily::MIM,"MIM");
    LoadObjectConstant(PyTypeCapacitorFamily.tp_dict,CapacitorFamily::MOM,"MOM");
  }


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
