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
// |  C++ Module  :  "./PyTRansistorMultiArguments.h"                |

// +-----------------------------------------------------------------+

#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/analog/PyTransistorMultiArguments.h"
#include "hurricane/analog/PyDevice.h"


namespace  Isobar {

using namespace Hurricane;
using namespace Analog;

extern "C" {


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(TransistorMultiArguments,argument,function)

#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |      "PyTransistorMultiArguments" Python Module Code Part       |
// +=================================================================+
  

  static PyObject* PyTransistorMultiArguments_getBulkType(PyTransistorMultiArguments *self) {
    cdebug.log(49) << "PyTransistorMultiArguments_getBulkType ()" << endl;
    METHOD_HEAD ( "TransistorMultiArguments.getBulkType()" )

    string bulkType = "";
    HTRY
    bulkType = argument->getBulkType();
    HCATCH

    return ( Py_BuildValue ( "s", bulkType.c_str() ) );
  }


  DirectDestroyAttribute(PyTransistorMultiArguments_destroy, PyTransistorMultiArguments)

  DirectGetBoolAttribute(PyTransistorMultiArguments_isNMOS           , isNMOS          , PyTransistorMultiArguments, TransistorMultiArguments)
  DirectGetBoolAttribute(PyTransistorMultiArguments_isBulkConnected  , isBulkConnected , PyTransistorMultiArguments, TransistorMultiArguments)
  DirectGetBoolAttribute(PyTransistorMultiArguments_isSourceFirst    , isSourceFirst   , PyTransistorMultiArguments, TransistorMultiArguments)
  DirectGetLongAttribute(PyTransistorMultiArguments_getW             , getW            , PyTransistorMultiArguments, TransistorMultiArguments)
  DirectGetLongAttribute(PyTransistorMultiArguments_getL             , getL            , PyTransistorMultiArguments, TransistorMultiArguments)
  DirectGetUIntAttribute(PyTransistorMultiArguments_getM             , getM            , PyTransistorMultiArguments, TransistorMultiArguments)
  DirectGetUIntAttribute(PyTransistorMultiArguments_getMInt          , getMInt         , PyTransistorMultiArguments, TransistorMultiArguments)
  DirectGetUIntAttribute(PyTransistorMultiArguments_getExternalDummy , getExternalDummy, PyTransistorMultiArguments, TransistorMultiArguments)


  // ---------------------------------------------------------------
  // PyTransistorMultiArguments Attribute Method table.

  PyMethodDef PyTransistorMultiArguments_Methods[] =
    { { "isNMOS"          , (PyCFunction)PyTransistorMultiArguments_isNMOS          , METH_NOARGS, "Returns true if NMOS, false for PMOS." }
    , { "isBulkConnected" , (PyCFunction)PyTransistorMultiArguments_isBulkConnected , METH_NOARGS, "Returns true if bulk is connected." }
    , { "isSourceFirst"   , (PyCFunction)PyTransistorMultiArguments_isSourceFirst   , METH_NOARGS, "Returns true if source is first." }
    , { "getBulkType"     , (PyCFunction)PyTransistorMultiArguments_getBulkType     , METH_NOARGS, "Returns the bulk type." }
    , { "getW"            , (PyCFunction)PyTransistorMultiArguments_getW            , METH_NOARGS, "Returns w value." }
    , { "getL"            , (PyCFunction)PyTransistorMultiArguments_getL            , METH_NOARGS, "Returns l value." }
    , { "getM"            , (PyCFunction)PyTransistorMultiArguments_getM            , METH_NOARGS, "Returns m value." }
    , { "getMInt"         , (PyCFunction)PyTransistorMultiArguments_getMInt         , METH_NOARGS, "Returns mInt value." }
    , { "getExternalDummy", (PyCFunction)PyTransistorMultiArguments_getExternalDummy, METH_NOARGS, "Returns external dummy of one terbminal value." }
    , { "destroy"         , (PyCFunction)PyTransistorMultiArguments_destroy       , METH_NOARGS, "Destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |         "PyTransistorMultiArguments" Object Methods         |
  // +-------------------------------------------------------------+


  DirectDeleteMethod(PyTransistorMultiArguments_DeAlloc,PyTransistorMultiArguments)
  PyTypeObjectLinkPyType(TransistorMultiArguments)

#else  // End of Python Module Code Part.


// +=================================================================+
// |      "PyTransistorMultiArguments" Shared Library Code Part      |
// +=================================================================+
  

  PyTypeObjectDefinitions(TransistorMultiArguments)


# endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
