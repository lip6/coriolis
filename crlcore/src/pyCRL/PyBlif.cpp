// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyBlif.cpp"                                  |
// +-----------------------------------------------------------------+


#include "crlcore/PyBlif.h"
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
  using Hurricane::in_trace;
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
// |               "PyBlif" Python Module Code Part                  |
// +=================================================================+


  static PyObject* PyBlif_load ( PyObject*, PyObject* args )
  {
    trace << "PyBlif_load()" << endl;

    Cell* cell = NULL;
    
    HTRY
      char* benchName = NULL;
      
      if (PyArg_ParseTuple( args, "s:Blif.load", &benchName )) {
        cell = Blif::load( benchName );
      } else {
        PyErr_SetString ( ConstructorError, "Blif.load(): Bad type or bad number of parameters." );
        return NULL;
      }
    HCATCH

    return (PyObject*)PyCell_Link(cell);
  }


  // Standart Destroy (Attribute).


  PyMethodDef PyBlif_Methods[] =
    { { "load"                , (PyCFunction)PyBlif_load     , METH_VARARGS|METH_STATIC
                              , "Load a complete Blif design." }
  //, { "destroy"             , (PyCFunction)PyBlif_destroy  , METH_VARARGS
  //                          , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  NoObjectDeleteMethod(Blif)
  PyTypeObjectLinkPyTypeWithoutObject(Blif,Blif)


#else  // End of Python Module Code Part.


// +=================================================================+
// |              "PyBlif" Shared Library Code Part              |
// +=================================================================+

  // Type Definition.
  PyTypeObjectDefinitionsOfModule(CRL,Blif)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // CRL namespace.
