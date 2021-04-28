
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU/LIP6 2010-2020, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyCatalog.cpp"                          |
// +-----------------------------------------------------------------+


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
  

  // DBoDestroyAttribute(PyCatalog_destroy,PyCatalog)


  static PyObject* PyCatalog_getState ( PyCatalog* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyCatalog_getState ()" << endl;

    char*           name  = NULL;
    Catalog::State* state = NULL;
    PyObject*       pyAdd = NULL;
    HTRY
      METHOD_HEAD("Catalog.getState()")
      if ( not PyArg_ParseTuple(args,"s|O:Catalog.getState",&name,&pyAdd) ) {
        PyErr_SetString( ConstructorError, "Catalog.getState(): Invalid number or bad type of parameters.");
        return NULL;
      }
      bool add = (pyAdd) and PyObject_IsTrue(pyAdd);
      state = catalog->getState( Name(name), add );
    HCATCH
    if (not state) Py_RETURN_FALSE;
    return PyCatalogState_Link(state);
  }


  PyMethodDef PyCatalog_Methods[] =
    { { "getState"          , (PyCFunction)PyCatalog_getState, METH_VARARGS
                            , "Gets the catalog state of a cell." }
    , {NULL, NULL, 0, NULL} /* sentinel */
    };


  PythonOnlyDeleteMethod(Catalog)
  PyTypeObjectLinkPyType(Catalog)


#else  // End of Python Module Code Part.


// x=================================================================x
// |               "PyCatalog" Shared Library Code Part              |
// x=================================================================x

  // Link/Creation Method.
  PyTypeObjectDefinitions(Catalog)
  LinkCreateMethod(Catalog)


  extern  void  PyCatalog_postModuleInit ()
  {
    PyCatalogState_postModuleInit ();

    PyDict_SetItemString ( PyTypeCatalog.tp_dict, "State", (PyObject*)&PyTypeCatalogState );
  }


#endif  // End of Shared Library Code Part.


}  // End of extern "C".


}  // End of CRL namespace.
 
