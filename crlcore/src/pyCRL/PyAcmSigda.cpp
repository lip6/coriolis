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
// |  C++ Module  :  "./PyAcmSigda.cpp"                              |
// +-----------------------------------------------------------------+


#include "crlcore/PyAcmSigda.h"
#include "hurricane/isobar/PyCell.h"
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
  using Isobar::PyCell_Link;


extern "C" {


#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |             "PyAcmSigda" Python Module Code Part                |
// +=================================================================+


  static PyObject* PyAcmSigda_load ( PyObject*, PyObject* args )
  {
    cdebug.log(30) << "PyAcmSigda_load()" << endl;

    Cell* cell = NULL;
    
    HTRY
    char*     benchName = NULL;
    
    if (PyArg_ParseTuple( args, "s:AcmSigda.load", &benchName )) {
      cell = AcmSigda::load( benchName );
    } else {
      PyErr_SetString ( ConstructorError, "AcmSigda.load(): Bad type or bad number of parameters." );
      return NULL;
    }
    HCATCH

    return (PyObject*)PyCell_Link(cell);
  }


  // Standart Destroy (Attribute).


  PyMethodDef PyAcmSigda_Methods[] =
    { { "load"                , (PyCFunction)PyAcmSigda_load     , METH_VARARGS|METH_STATIC
                              , "Create a new AcmSigda." }
  //, { "destroy"             , (PyCFunction)PyAcmSigda_destroy  , METH_VARARGS
  //                          , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  NoObjectDeleteMethod(AcmSigda)
  PyTypeObjectLinkPyTypeWithoutObject(AcmSigda,AcmSigda)


#else  // End of Python Module Code Part.


// +=================================================================+
// |              "PyAcmSigda" Shared Library Code Part              |
// +=================================================================+

  // Type Definition.
  PyTypeObjectDefinitionsOfModule(CRL,AcmSigda)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // CRL namespace.
