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
// |  C++ Module  :  "./PyCapacitorArguments.h"                      |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/analog/PyCapacitorArguments.h"
#include "hurricane/analog/PyDevice.h"


namespace  Isobar {

using namespace Hurricane;
using namespace Analog;

extern "C" {


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(CapacitorArguments,argument,function)

#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |         "PyCapacitorArguments" Python Module Code Part          |
// +=================================================================+
  
  DirectDestroyAttribute(PyCapacitorArguments_destroy, PyCapacitorArguments)
  DirectGetDoubleAttribute(PyCapacitorArguments_getC, getC, PyCapacitorArguments, CapacitorArguments)

                                                                               
  // ---------------------------------------------------------------
  // PyCapacitorArguments Attribute Method table.

  PyMethodDef PyCapacitorArguments_Methods[] =
    { { "getC"            , (PyCFunction)PyCapacitorArguments_getC   , METH_NOARGS , "Returns C value." }
    , { "destroy"         , (PyCFunction)PyCapacitorArguments_destroy, METH_NOARGS , "Destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |            "PyCapacitorArguments" Object Methods            |
  // +-------------------------------------------------------------+

  DirectDeleteMethod(PyCapacitorArguments_DeAlloc,PyCapacitorArguments)
  PyTypeObjectLinkPyType(CapacitorArguments)

#else  // End of Python Module Code Part.

// +=================================================================+
// |         "PyCapacitorArguments" Shared Library Code Part         |
// +=================================================================+
  
  PyTypeObjectDefinitions(CapacitorArguments)

#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
