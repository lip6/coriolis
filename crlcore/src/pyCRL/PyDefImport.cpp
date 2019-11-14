// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2017-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyDefImport.cpp"                             |
// +-----------------------------------------------------------------+


#include "crlcore/PyDefImport.h"
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
  using Hurricane::Cell;
  using Isobar::ProxyProperty;
  using Isobar::ProxyError;
  using Isobar::ConstructorError;
  using Isobar::HurricaneError;
  using Isobar::HurricaneWarning;
  using Isobar::getPyHash;
  using Isobar::ParseOneArg;
  using Isobar::ParseTwoArg;
  using Isobar::__cs;
  using Isobar::PyCell_Link;


extern "C" {


#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |             "PyDefImport" Python Module Code Part               |
// +=================================================================+


  static PyObject* PyDefImport_load ( PyObject*, PyObject* args )
  {
    cdebug_log(30,0) << "PyDefImport_load()" << endl;

    Cell* cell = NULL;
    
    HTRY
      char* defFile = NULL;
      
      if (PyArg_ParseTuple( args, "s:DefImport.load", &defFile )) {
        cell = DefImport::load( defFile, 0 );
      } else {
        PyErr_SetString ( ConstructorError, "DefImport.load(): Bad type or bad number of parameters." );
        return NULL;
      }
    HCATCH

    return (PyObject*)PyCell_Link( cell );
  }


  static PyObject* PyDefImport_reset ( PyObject*, PyObject* )
  {
    cdebug_log(30,0) << "PyDefImport_reset()" << endl;
    HTRY
      DefImport::reset();
    HCATCH
    Py_RETURN_NONE;
  }


  // Standart Destroy (Attribute).


  PyMethodDef PyDefImport_Methods[] =
    { { "load"                , (PyCFunction)PyDefImport_load     , METH_VARARGS|METH_STATIC
                              , "Load a complete Cadence LEF library." }
    , { "reset"               , (PyCFunction)PyDefImport_reset    , METH_NOARGS|METH_STATIC
                              , "Reset the Cadence LEF parser (clear technology)." }
  //, { "destroy"             , (PyCFunction)PyDefImport_destroy  , METH_VARARGS
  //                          , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  NoObjectDeleteMethod(DefImport)
  PyTypeObjectLinkPyTypeWithoutObject(DefImport,DefImport)


#else  // End of Python Module Code Part.


// +=================================================================+
// |              "PyDefImport" Shared Library Code Part             |
// +=================================================================+

  // Type Definition.
  PyTypeObjectDefinitionsOfModule(CRL,DefImport)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // CRL namespace.
