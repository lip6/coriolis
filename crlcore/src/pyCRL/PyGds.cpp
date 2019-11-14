// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2018-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyGds.cpp"                                   |
// +-----------------------------------------------------------------+


#include "crlcore/PyGds.h"
#include "hurricane/isobar/PyCell.h"
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
  using Isobar::ProxyProperty;
  using Isobar::ProxyError;
  using Isobar::ConstructorError;
  using Isobar::HurricaneError;
  using Isobar::HurricaneWarning;
  using Isobar::getPyHash;
  using Isobar::ParseOneArg;
  using Isobar::ParseTwoArg;
  using Isobar::__cs;
  using Isobar::PyCell;
  using Isobar::PyTypeCell;
  using Isobar::PyCell_Link;
  using Isobar::PyLibrary;
  using Isobar::PyTypeLibrary;
  using Isobar::PyLibrary_Link;


extern "C" {


#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |                "PyGds" Python Module Code Part                  |
// +=================================================================+


  static PyObject* PyGds_save ( PyObject*, PyObject* args )
  {
    cdebug_log(30,0) << "PyGds_save()" << endl;

    Cell* cell = NULL;
    
    HTRY
      PyObject* pyCell = NULL;
      if (PyArg_ParseTuple( args, "O:Gds.save", &pyCell )) {
        if (IsPyCell(pyCell)) {
          Gds::save( PYCELL_O(pyCell) );
        } else {
          PyErr_SetString( ConstructorError, "Gds.load(): Bad parameter type (not a Cell)." );
          return NULL;
        }
      } else {
        PyErr_SetString( ConstructorError, "Gds.load(): Bad number of parameters." );
        return NULL;
      }
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyGds_load ( PyObject*, PyObject* args )
  {
    cdebug_log(30,0) << "PyGds_load()" << endl;

    Library* library = NULL;
    char*    path    = NULL;
    
    HTRY
      PyObject* pyLibrary = NULL;
      if (PyArg_ParseTuple( args, "Os:Gds.load", &pyLibrary, &path )) {
        if (IsPyLibrary(pyLibrary)) {
          Gds::load( PYLIBRARY_O(pyLibrary), string(path) );
        } else {
          PyErr_SetString( ConstructorError, "Gds.load(): Bad parameter type (not a Library)." );
          return NULL;
        }
      } else {
        PyErr_SetString( ConstructorError, "Gds.load(): Bad number of parameters." );
        return NULL;
      }
    HCATCH

    Py_RETURN_NONE;
  }


  // Standart Destroy (Attribute).


  PyMethodDef PyGds_Methods[] =
    { { "save"                , (PyCFunction)PyGds_save     , METH_VARARGS|METH_STATIC
                              , "Save a complete Gds design." }
    , { "load"                , (PyCFunction)PyGds_load     , METH_VARARGS|METH_STATIC
                              , "Load a Gds layout inside a Cell (cumulative)." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  NoObjectDeleteMethod(Gds)
  PyTypeObjectLinkPyTypeWithoutObject(Gds,Gds)


#else  // End of Python Module Code Part.


// +=================================================================+
// |                 "PyGds" Shared Library Code Part                |
// +=================================================================+

  // Type Definition.
  PyTypeObjectDefinitionsOfModule(CRL,Gds)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // CRL namespace.
