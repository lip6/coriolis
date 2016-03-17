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
// |  C++ Module  :  "./PyIspd05.cpp"                                |
// +-----------------------------------------------------------------+


#include "crlcore/PyIspd05.h"
#include "hurricane/isobar/PyCell.h"
#include "crlcore/Ispd05Bookshelf.h"
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
// |              "PyIspd05" Python Module Code Part                 |
// +=================================================================+


  static PyObject* PyIspd05_load ( PyObject*, PyObject* args )
  {
    trace << "PyIspd05_load()" << endl;

    Cell* cell = NULL;
    
    HTRY
    char*     benchName = NULL;
    
    if (PyArg_ParseTuple( args, "s:Ispd05.load", &benchName )) {
      cell = Ispd05::load( benchName );
    } else {
      PyErr_SetString ( ConstructorError, "Ispd05.load(): Bad type or bad number of parameters." );
      return NULL;
    }
    HCATCH

    return (PyObject*)PyCell_Link(cell);
  }


  // Standart Destroy (Attribute).


  PyMethodDef PyIspd05_Methods[] =
    { { "load"                , (PyCFunction)PyIspd05_load     , METH_VARARGS|METH_STATIC
                              , "Create a new Ispd05." }
  //, { "destroy"             , (PyCFunction)PyIspd05_destroy  , METH_VARARGS
  //                          , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  NoObjectDeleteMethod(Ispd05)
  PyTypeObjectLinkPyTypeWithoutObject(Ispd05,Ispd05)


#else  // End of Python Module Code Part.


// +=================================================================+
// |               "PyIspd05" Shared Library Code Part               |
// +=================================================================+

  // Type Definition.
  PyTypeObjectDefinitionsOfModule(CRL,Ispd05)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // CRL namespace.
