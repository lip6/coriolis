// -*- C++ -*-
//
// This file is part of the Coriols Software.
// Copyright (c) UPMC 2009-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |           H u r r i c a n e   A n a l o g                       |
// |                                                                 |
// |  Author      :                       Damien Dupuis              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyBJTArguments.cpp"                          |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/analog/PyBJTArguments.h"
#include "hurricane/analog/PyDevice.h"


namespace  Isobar {

using namespace Hurricane;
using namespace Analog;

extern "C" {


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(BJTArguments,argument,function)

// +=================================================================+
// |            "PyBJTArguments" Python Module Code Part             |
// +=================================================================+

#if defined(__PYTHON_MODULE__)

  // +-------------------------------------------------------------+
  // |             "PyBJTArguments" Attribute Methods              |
  // +-------------------------------------------------------------+

  // ---------------------------------------------------------------
  // Standart Destroy (Attribute).
  DirectDestroyAttribute(PyBJTArguments_destroy, PyBJTArguments)


  // Standart Accessors (Attributes).
  DirectGetBoolAttribute(PyBJTArguments_isNPN          , isNPN            , PyBJTArguments, BJTArguments)
  DirectGetBoolAttribute(PyBJTArguments_isExternalDummy, isExternalDummy  , PyBJTArguments, BJTArguments)
  DirectGetUIntAttribute(PyBJTArguments_getM           , getM             , PyBJTArguments, BJTArguments)
  DirectGetUIntAttribute(PyBJTArguments_getN           , getN             , PyBJTArguments, BJTArguments)

                                                                               
  // ---------------------------------------------------------------
  // PyBJTArguments Attribute Method table.

  PyMethodDef PyBJTArguments_Methods[] =
    { { "isNPN"           , (PyCFunction)PyBJTArguments_isNPN           , METH_NOARGS , "Returns true if NPN, false for PNP." }
    , { "isExternalDummy" , (PyCFunction)PyBJTArguments_isExternalDummy , METH_NOARGS , "Returns true if there sre external dummies." }
    , { "getM"            , (PyCFunction)PyBJTArguments_getM            , METH_NOARGS , "Returns m value." }
    , { "getN"            , (PyCFunction)PyBJTArguments_getN            , METH_NOARGS , "Returns n value." }
    , { "destroy"         , (PyCFunction)PyBJTArguments_destroy         , METH_NOARGS , "Destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |               "PyBJTArguments" Object Methods               |
  // +-------------------------------------------------------------+

  DirectDeleteMethod(PyBJTArguments_DeAlloc,PyBJTArguments)
  PyTypeObjectLinkPyType(BJTArguments)

#else  // End of Python Module Code Part.

// +=================================================================+
// |            "PyBJTArguments" Shared Library Code Part            |
// +=================================================================+
  
  PyTypeObjectDefinitions(BJTArguments)

#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
