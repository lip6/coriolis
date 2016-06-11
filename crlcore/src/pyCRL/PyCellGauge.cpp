// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2012-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyCellGauge.cpp"                             |
// +-----------------------------------------------------------------+


#include "crlcore/PyCellGauge.h"
#include <string>
#include <sstream>


namespace  CRL {

  using std::cerr;
  using std::endl;
  using std::hex;
  using std::string;
  using std::ostringstream;
  using Hurricane::tab;
  using Hurricane::Exception;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Isobar::ProxyProperty;
  using Isobar::ProxyError;
  using Isobar::ConstructorError;
  using Isobar::HurricaneError;
  using Isobar::HurricaneWarning;
  using Isobar::ParseOneArg;
  using Isobar::ParseTwoArg;
  using Isobar::__cs;


extern "C" {


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(CellGauge,cg,function)


#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |             "PyCellGauge" Python Module Code Part               |
// +=================================================================+


  DirectGetLongAttribute(PyCellGauge_getSliceHeight,getSliceHeight,PyCellGauge,CellGauge)
  DirectGetLongAttribute(PyCellGauge_getSliceStep  ,getSliceStep  ,PyCellGauge,CellGauge)
  DirectGetLongAttribute(PyCellGauge_getPitch      ,getPitch      ,PyCellGauge,CellGauge)


  static PyObject* PyCellGauge_create ( PyObject*, PyObject* args )
  {
    cdebug_log(30,0) << "PyCellGauge_create()" << endl;

    CellGauge*   cg   = NULL;
    PyCellGauge* pyCg = NULL;
    
    HTRY
    char*     name;
    char*     pinLayerName;
    long      pitch;
    long      sliceHeight;
    long      sliceStep;
    
    if (PyArg_ParseTuple( args
                        , "sslll:CellGauge.create"
                        , &name
                        , &pinLayerName
                        , &pitch
                        , &sliceHeight
                        , &sliceStep
                        )) {
      cg = CellGauge::create( name
                            , pinLayerName
                            , pitch
                            , sliceHeight
                            , sliceStep
                            );
    } else {
      PyErr_SetString ( ConstructorError, "Bad parameters given to CellGauge.create()." );
      return NULL;
    }

    pyCg = PyObject_NEW ( PyCellGauge, &PyTypeCellGauge );
    if ( pyCg == NULL ) {
      cg->destroy();
      return NULL;
    }

    pyCg->_object = cg;
    HCATCH

    return (PyObject*)pyCg;
  }


  // Standart Destroy (Attribute).


  PyMethodDef PyCellGauge_Methods[] =
    { { "create"                , (PyCFunction)PyCellGauge_create        , METH_VARARGS|METH_STATIC
                                , "Create a new CellGauge." }
    , { "getSliceHeight"        , (PyCFunction)PyCellGauge_getSliceHeight, METH_NOARGS , "Return the slice height." }
    , { "getSliceStep"          , (PyCFunction)PyCellGauge_getSliceStep  , METH_NOARGS , "Return the slice step." }
    , { "getPitch"              , (PyCFunction)PyCellGauge_getPitch      , METH_NOARGS , "Return the smallest common pitch." }
  //, { "destroy"               , (PyCFunction)PyCellGauge_destroy       , METH_VARARGS
  //                            , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  PythonOnlyDeleteMethod(CellGauge)
  PyTypeObjectLinkPyType(CellGauge)


#else  // End of Python Module Code Part.

// +=================================================================+
// |              "PyCellGauge" Shared Library Code Part             |
// +=================================================================+

  // Type Definition.
  PyTypeObjectDefinitionsOfModule(CRL,CellGauge)

  // Link/Creation Method.
  LinkCreateMethod(CellGauge)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // CRL namespace.
