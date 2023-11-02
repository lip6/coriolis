// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU 2021-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Verilog / Hurricane  Interface                         |
// |                                                                 |
// |  Authors     :          Jean-Paul CHAPUT, Serge Rabyking        |
// |  E-mail      :          Jean-Paul.Chaput@lip6.fr                |
// | =============================================================== |
// |  C++ Module  :  "./PyVerilog.cpp"                               |
// +-----------------------------------------------------------------+


#include "crlcore/PyVerilog.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/isobar/PyLibrary.h"
//#include "crlcore/VerilogEntity.h"
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
// |              "PyVerilog" Python Module Code Part                |
// +=================================================================+


  static PyObject* PyVerilog_save ( PyObject*, PyObject* args )
  {
    cdebug_log(30,0) << "PyVerilog_save()" << endl;

    HTRY
      long      flags  = 0;
      PyObject* pyCell = NULL;
      if (PyArg_ParseTuple( args, "Ol:Verilog.save", &pyCell, &flags )) {
        if (IsPyCell(pyCell)) {
          Verilog::save( PYCELL_O(pyCell), flags );
        } else {
          PyErr_SetString( ConstructorError, "Verilog.save(): Bad parameter type (not a Cell)." );
          return NULL;
        }
      } else {
        std::cout << ConstructorError << std::endl;
        PyErr_SetString( ConstructorError, "Verilog.save(): Bad number of parameters." );
        return NULL;
      }
    HCATCH

    Py_RETURN_NONE;
  }


//   static PyObject* PyVerilog_load ( PyObject*, PyObject* args )
//   {
//     cdebug_log(30,0) << "PyVerilog_load()" << endl;

    
//     HTRY
//       unsigned long  mode      = 0;
//       char*          path      = NULL;
//       PyObject*      pyLibrary = NULL;
//       if (PyArg_ParseTuple( args, "Osk:Verilog.load", &pyLibrary, &path, &mode )) {
//         if (IsPyLibrary(pyLibrary)) {
//           Verilog::load( PYLIBRARY_O(pyLibrary), string(path), (uint64_t)mode );
//         } else {
//           PyErr_SetString( ConstructorError, "Verilog.load(): Bad parameter type (not a Library)." );
//           return NULL;
//         }
//       } else {
//         PyErr_SetString( ConstructorError, "Verilog.load(): Bad number of parameters." );
//         return NULL;
//       }
//     HCATCH

//     Py_RETURN_NONE;
//   }


//   static PyObject* PyVerilog_clearProperties ( PyObject* )
//   {
//     cdebug_log(30,0) << "PyVerilog_clearProperties()" << endl;
//     HTRY
//       Verilog::clearProperties();
//     HCATCH
//     Py_RETURN_NONE;
//   }


  // Standart Destroy (Attribute).


  PyMethodDef PyVerilog_Methods[] =
    { { "save"                , (PyCFunction)PyVerilog_save     , METH_VARARGS|METH_STATIC
                              , "Save a complete Verilog design." }
    // , { "load"                , (PyCFunction)PyVerilog_load     , METH_VARARGS|METH_STATIC
    //                           , "Load a Verilog layout inside a Cell (cumulative)." }
    // , { "clearProperties"     , (PyCFunction)PyVerilog_clearProperties, METH_NOARGS|METH_STATIC
    //                           , "Remove all Verilog related properties from the Cells." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  NoObjectDeleteMethod(Verilog)
  PyTypeObjectLinkPyTypeWithoutObject(Verilog,Verilog)


#else  // End of Python Module Code Part.


// +=================================================================+
// |               "PyVerilog" Shared Library Code Part              |
// +=================================================================+

  // Type Definition.
  PyTypeObjectDefinitionsOfModule(CRL,Verilog)


  extern  void  PyVerilog_postModuleInit ()
  {
    PyObject* constant;

    LoadObjectConstant(PyTypeVerilog.tp_dict,::CRL::Verilog::TopCell  ,"TopCell");
  }


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // CRL namespace.
