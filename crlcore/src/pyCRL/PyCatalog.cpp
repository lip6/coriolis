
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2010-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyCatalog.cpp"                          |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include "hurricane/isobar/PyCell.h"
#include "hurricane/isobar/PyLibrary.h"
#include "crlcore/Catalog.h"  // TEMPORARY
#include "crlcore/PyCatalogState.h"
#include "crlcore/PyCatalog.h"


namespace  CRL {

  using std::cerr;
  using std::endl;
  using std::hex;
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
  using Isobar::PyLibrary;
  using Isobar::PyLibrary_Link;
  using Isobar::PyCell;
  using Isobar::PyCell_Link;


extern "C" {


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(Catalog,catalog,function)


// x=================================================================x
// |              "PyCatalog" Python Module Code Part                |
// x=================================================================x

#if defined(__PYTHON_MODULE__)

  
  // Standart Accessors (Attributes).


  // Standart Destroy (Attribute).
  // DBoDestroyAttribute(PyCatalog_destroy,PyCatalog)


  PyMethodDef PyCatalog_Methods[] =
    { {NULL, NULL, 0, NULL}           /* sentinel */
    };


  PythonOnlyDeleteMethod(Catalog)
  PyTypeObjectLinkPyType(Catalog)


#else  // End of Python Module Code Part.


// x=================================================================x
// |               "PyCatalog" Shared Library Code Part              |
// x=================================================================x

  // Link/Creation Method.
  PyTypeObjectDefinitions(Catalog)


  extern  void  PyCatalog_postModuleInit ()
  {
    PyCatalogState_postModuleInit ();

    PyDict_SetItemString ( PyTypeCatalog.tp_dict, "State", (PyObject*)&PyTypeCatalogState );
  }


#endif  // End of Shared Library Code Part.


}  // End of extern "C".


}  // End of CRL namespace.
 
