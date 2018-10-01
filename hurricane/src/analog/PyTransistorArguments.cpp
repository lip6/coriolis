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
// |  C++ Module  :  "./PyTransistorArguments.cpp"                   |
// +-----------------------------------------------------------------+

#include "hurricane/analog/PyTransistorArguments.h"
#include "hurricane/analog/PyDevice.h"


namespace  Isobar {

using namespace Hurricane;
using namespace Analog;

extern "C" {


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(TransistorArguments,argument,function)

#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |         "PyTransistorArguments" Python Module Code Part         |
// +=================================================================+

  
  static PyObject* PyTransistorArguments_getBulkType(PyTransistorArguments *self) {
    cdebug.log(49) << "PyTransistorArguments_getBulkType ()" << endl;
    METHOD_HEAD ( "TransistorArguments.getBulkType()" )

    string bulkType = "";
    HTRY
    bulkType = argument->getBulkType();
    HCATCH

    return ( Py_BuildValue ( "s", bulkType.c_str() ) );
  }


  DirectDestroyAttribute(PyTransistorArguments_destroy, PyTransistorArguments)

  DirectGetBoolAttribute(PyTransistorArguments_isNMOS          , isNMOS          , PyTransistorArguments, TransistorArguments)
  DirectGetBoolAttribute(PyTransistorArguments_isBulkConnected , isBulkConnected , PyTransistorArguments, TransistorArguments)
  DirectGetBoolAttribute(PyTransistorArguments_isSourceFirst   , isSourceFirst   , PyTransistorArguments, TransistorArguments)
  DirectGetLongAttribute(PyTransistorArguments_getW            , getW            , PyTransistorArguments, TransistorArguments)
  DirectGetUIntAttribute(PyTransistorArguments_getL            , getL            , PyTransistorArguments, TransistorArguments)
  DirectGetUIntAttribute(PyTransistorArguments_getM            , getM            , PyTransistorArguments, TransistorArguments)
  DirectGetUIntAttribute(PyTransistorArguments_getExternalDummy, getExternalDummy, PyTransistorArguments, TransistorArguments)

                                                                               
  // ---------------------------------------------------------------
  // PyTransistorArguments Attribute Method table.

  PyMethodDef PyTransistorArguments_Methods[] =
    { { "isNMOS"          , (PyCFunction)PyTransistorArguments_isNMOS          , METH_NOARGS, "Returns true if NMOS, false for PMOS." }
    , { "isBulkConnected" , (PyCFunction)PyTransistorArguments_isBulkConnected , METH_NOARGS, "Returns true if bulk is connected." }
    , { "isSourceFirst"   , (PyCFunction)PyTransistorArguments_isSourceFirst   , METH_NOARGS, "Returns true if source is first." }
    , { "getBulkType"     , (PyCFunction)PyTransistorArguments_getBulkType     , METH_NOARGS, "Returns the bulk type." }
    , { "getW"            , (PyCFunction)PyTransistorArguments_getW            , METH_NOARGS, "Returns w value." }
    , { "getL"            , (PyCFunction)PyTransistorArguments_getL            , METH_NOARGS, "Returns l value." }
    , { "getM"            , (PyCFunction)PyTransistorArguments_getM            , METH_NOARGS, "Returns m value." }
    , { "getExternalDummy", (PyCFunction)PyTransistorArguments_getExternalDummy, METH_NOARGS, "Returns external dummy of one terbminal value." }
    , { "destroy"         , (PyCFunction)PyTransistorArguments_destroy         , METH_NOARGS, "Destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };


  // +-------------------------------------------------------------+
  // |            "PyTransistorArguments" Object Methods           |
  // +-------------------------------------------------------------+


  DirectDeleteMethod(PyTransistorArguments_DeAlloc,PyTransistorArguments)
  PyTypeObjectLinkPyType(TransistorArguments)

#else  // End of Python Module Code Part.

// +=================================================================+
// |         "PyTransistorArguments" Shared Library Code Part        |
// +=================================================================+
  

  PyTypeObjectDefinitions(TransistorArguments)


# endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
