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
// |  C++ Module  :  "./PyLefExport.cpp"                             |
// +-----------------------------------------------------------------+


#include "crlcore/PyLefExport.h"
#include "hurricane/isobar/PyLibrary.h"
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
  using Hurricane::Library;
  using Isobar::ProxyProperty;
  using Isobar::ProxyError;
  using Isobar::ConstructorError;
  using Isobar::HurricaneError;
  using Isobar::HurricaneWarning;
  using Isobar::getPyHash;
  using Isobar::ParseOneArg;
  using Isobar::ParseTwoArg;
  using Isobar::__cs;
  using Isobar::PyTypeLibrary;
  using Isobar::PyLibrary;


extern "C" {


#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |             "PyLefExport" Python Module Code Part               |
// +=================================================================+


  static PyObject* PyLefExport_drive ( PyObject*, PyObject* args )
  {
    cdebug_log(30,0) << "PyLefExport_drive()" << endl;

    PyLibrary* pyLibrary = NULL;
    
    HTRY
      unsigned int flags = 0;
      
      if (PyArg_ParseTuple( args, "O!I:LefExport.drive", &PyTypeLibrary, &pyLibrary, &flags)) {
        LefExport::drive( PYLIBRARY_O(pyLibrary) , flags );
      } else {
        PyErr_SetString ( ConstructorError, "LefExport.drive(): Bad type or bad number of parameters." );
        return NULL;
      }
    HCATCH

    Py_RETURN_NONE;
  }

  // Standart Destroy (Attribute).


  PyMethodDef PyLefExport_Methods[] =
    { { "drive"                , (PyCFunction)PyLefExport_drive     , METH_VARARGS|METH_STATIC
                              , "Save a complete Cadence LEF library." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  NoObjectDeleteMethod(LefExport)
  PyTypeObjectLinkPyTypeWithoutObject(LefExport,LefExport)


#else  // End of Python Module Code Part.


// +=================================================================+
// |              "PyLefExport" Shared Library Code Part             |
// +=================================================================+

  // Type Definition.
  PyTypeObjectDefinitionsOfModule(CRL,LefExport)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // CRL namespace.
