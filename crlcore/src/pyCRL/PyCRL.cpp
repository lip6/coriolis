
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
// |  C++ Module  :       "./PyCRL.cpp"                              |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/isobar/PyLibrary.h"
#include "hurricane/isobar/PyCell.h"
#include "crlcore/PyCatalog.h"
#include "crlcore/PyCatalogState.h"
#include "crlcore/PyAllianceFramework.h"
#include "crlcore/PyToolBox.h"


namespace CRL {

  using std::cerr;
  using std::endl;
  using Hurricane::tab;
  using Hurricane::in_trace;
  using Isobar::__cs;


#if !defined(__PYTHON_MODULE__)

// x=================================================================x
// |                "PyCRL" Shared Library Code Part                 |
// x=================================================================x


# else // End of PyHurricane Shared Library Code Part.


// x=================================================================x
// |                 "PyCRL" Python Module Code Part                 |
// x=================================================================x


extern "C" {


  // x-------------------------------------------------------------x
  // |                  "PyCRL" Module Methods                     |
  // x-------------------------------------------------------------x


  static PyMethodDef PyCRL_Methods[] =
    { { "createPartRing"      , (PyCFunction)PyToolBox_createPartRing, METH_VARARGS
                              , "Partial build of a ring" }
    , {NULL, NULL, 0, NULL}     /* sentinel */
    };




  // ---------------------------------------------------------------
  // Module Initialization  :  "initCRL ()"

  DL_EXPORT(void) initCRL () {
    trace << "initCRL()" << endl;

    PyCatalogState_LinkPyType ();
    PyCatalog_LinkPyType ();
    PyAllianceFramework_LinkPyType ();

    PYTYPE_READY ( CatalogState );
    PYTYPE_READY ( Catalog );
    PYTYPE_READY ( AllianceFramework );
   
    // Identifier string can take up to 10 characters.
    __cs.addType ( "alcFw"      , &PyTypeAllianceFramework, "<AllianceFramework>", false );
    __cs.addType ( "alcCatalog" , &PyTypeCatalog          , "<Catalog>"          , false );
    __cs.addType ( "alcCatStat" , &PyTypeCatalogState     , "<Catalog::State>"   , false );

    PyObject* module = Py_InitModule ( "CRL", PyCRL_Methods );
    if ( module == NULL ) {
      cerr << "[ERROR]\n"
           << "  Failed to initialize CRL module." << endl;
      return;
    }

    Py_INCREF ( &PyTypeCatalog );
    PyModule_AddObject ( module, "Catalog", (PyObject*)&PyTypeCatalog );
    Py_INCREF ( &PyTypeAllianceFramework );
    PyModule_AddObject ( module, "AllianceFramework", (PyObject*)&PyTypeAllianceFramework );

    PyCatalog_postModuleInit ();
    
    PyObject* dictionnary = PyModule_GetDict ( module );

  //DbULoadConstants ( dictionnary );

    trace << "CRL.so loaded " << (void*)&typeid(string) << endl;
  }

  
} // End of extern "C".


#endif // End of Python Module Code Part.


}  // End of CRL namespace.
