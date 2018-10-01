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
// |  C++ Module  :  "./PyTransistorPair.cpp"                        |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyLibrary.h"
#include "hurricane/analog/PyTransistorPair.h"


namespace  Isobar {

using namespace Hurricane;
using namespace Analog;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT            _baseObject._baseObject._baseObject._baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject._baseObject._baseObject._baseObject)
#define METHOD_HEAD(function)    GENERIC_METHOD_HEAD(TransistorPair,transistor,function)

#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |             "PyTransistorPair" Python Module Code Part          |
// +=================================================================+


  DirectGetLongAttribute(PyTransistorPair_getMint,getMint,PyTransistorPair,TransistorPair)
  DirectSetLongAttribute(PyTransistorPair_setMint,setMint,PyTransistorPair,TransistorPair)


  // ---------------------------------------------------------------
  // PyTransistorPair Attribute Method table.

  PyMethodDef PyTransistorPair_Methods[] =
    { { "getMint"           , (PyCFunction)PyTransistorPair_getMint        , METH_NOARGS
                            , "Return the Mint value." }
    , { "setMint"           , (PyCFunction)PyTransistorPair_setMint        , METH_VARARGS
                            , "Sets the Mint value." }
    , { NULL, NULL, 0, NULL }  /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |              "PyTransistorPair" Object Methods              |
  // +-------------------------------------------------------------+


  DBoDeleteMethod(TransistorPair)
  PyTypeObjectLinkPyType(TransistorPair)


#else  // End of Python Module Code Part.

// +=================================================================+
// |            "PyTransistorPair" Shared Library Code Part          |
// +=================================================================+
  

  DBoLinkCreateMethod(TransistorPair)
  PyTypeInheritedObjectDefinitions(TransistorPair, TransistorFamily)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
