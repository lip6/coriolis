// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2017-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyDefExport.cpp"                             |
// +-----------------------------------------------------------------+


#include "crlcore/PyDefExport.h"
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
  using Isobar::PyTypeCell;
  using Isobar::PyCell;


extern "C" {


#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |             "PyDefExport" Python Module Code Part               |
// +=================================================================+


  static PyObject* PyDefExport_drive ( PyObject*, PyObject* args )
  {
    cdebug_log(30,0) << "PyDefExport_drive()" << endl;

    PyCell* pyCell = NULL;
    
    HTRY
      unsigned int flags = 0;
      
      if (PyArg_ParseTuple( args, "O!I:DefExport.drive", &PyTypeCell, &pyCell, &flags)) {
        DefExport::drive( PYCELL_O(pyCell) , flags );
      } else {
        PyErr_SetString ( ConstructorError, "DefExport.drive(): Bad type or bad number of parameters." );
        return NULL;
      }
    HCATCH

    Py_RETURN_NONE;
  }

  // Standart Destroy (Attribute).


  PyMethodDef PyDefExport_Methods[] =
    { { "drive"                , (PyCFunction)PyDefExport_drive     , METH_VARARGS|METH_STATIC
                              , "Save a complete Cadence DEF design." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  NoObjectDeleteMethod(DefExport)
  PyTypeObjectLinkPyTypeWithoutObject(DefExport,DefExport)


#else  // End of Python Module Code Part.


// +=================================================================+
// |              "PyDefExport" Shared Library Code Part             |
// +=================================================================+

  // Type Definition.
  PyTypeObjectDefinitionsOfModule(CRL,DefExport)


  extern  void  PyDefExport_postModuleInit ()
  {
    PyObject* constant;
    LoadObjectConstant(PyTypeDefExport.tp_dict,DefExport::WithLEF        ,"WithLEF");
    LoadObjectConstant(PyTypeDefExport.tp_dict,DefExport::ExpandDieArea  ,"ExpandDieArea");
    LoadObjectConstant(PyTypeDefExport.tp_dict,DefExport::ProtectNetNames,"ProtectNetNames");
  }


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // CRL namespace.
