// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU 2021-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PySpice.cpp"                                 |
// +-----------------------------------------------------------------+


#include "crlcore/PySpice.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/isobar/PyLibrary.h"
#include "crlcore/SpiceEntity.h"
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
// |               "PySpice" Python Module Code Part                 |
// +=================================================================+


  static PyObject* PySpice_save ( PyObject*, PyObject* args )
  {
    cdebug_log(30,0) << "PySpice_save()" << endl;

    HTRY
      long      flags  = 0;
      PyObject* pyCell = NULL;
      if (PyArg_ParseTuple( args, "Ol:Spice.save", &pyCell, &flags )) {
        if (IsPyCell(pyCell)) {
          Spice::save( PYCELL_O(pyCell), flags );
        } else {
          PyErr_SetString( ConstructorError, "Spice.save(): Bad parameter type (not a Cell)." );
          return NULL;
        }
      } else {
        PyErr_SetString( ConstructorError, "Spice.save(): Bad number of parameters." );
        return NULL;
      }
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PySpice_load ( PyObject*, PyObject* args )
  {
    cdebug_log(30,0) << "PySpice_load()" << endl;

    char* path = NULL;
    
    HTRY
      PyObject* pyLibrary = NULL;
      if (PyArg_ParseTuple( args, "Os:Spice.load", &pyLibrary, &path )) {
        if (IsPyLibrary(pyLibrary)) {
          Spice::load( PYLIBRARY_O(pyLibrary), string(path) );
        } else {
          PyErr_SetString( ConstructorError, "Spice.load(): Bad parameter type (not a Library)." );
          return NULL;
        }
      } else {
        PyErr_SetString( ConstructorError, "Spice.load(): Bad number of parameters." );
        return NULL;
      }
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PySpice_clearProperties ( PyObject* )
  {
    cdebug_log(30,0) << "PySpice_clearProperties()" << endl;
    HTRY
      Spice::clearProperties();
    HCATCH
    Py_RETURN_NONE;
  }


  // Standart Destroy (Attribute).


  PyMethodDef PySpice_Methods[] =
    { { "save"                , (PyCFunction)PySpice_save     , METH_VARARGS|METH_STATIC
                              , "Save a complete Spice design." }
    , { "load"                , (PyCFunction)PySpice_load     , METH_VARARGS|METH_STATIC
                              , "Load a Spice layout inside a Cell (cumulative)." }
    , { "clearProperties"     , (PyCFunction)PySpice_clearProperties, METH_NOARGS|METH_STATIC
                              , "Remove all SPICE related properties from the Cells." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  NoObjectDeleteMethod(Spice)
  PyTypeObjectLinkPyTypeWithoutObject(Spice,Spice)


#else  // End of Python Module Code Part.


// +=================================================================+
// |                "PySpice" Shared Library Code Part               |
// +=================================================================+

  // Type Definition.
  PyTypeObjectDefinitionsOfModule(CRL,Spice)


  extern  void  PySpice_postModuleInit ()
  {
    PyObject* constant;

    LoadObjectConstant(PyTypeSpice.tp_dict,::Spice::Entity::TopCell,"TopCell");
  }


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // CRL namespace.
