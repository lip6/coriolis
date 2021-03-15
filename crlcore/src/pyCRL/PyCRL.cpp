// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2010-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyCRL.cpp"                              |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/isobar/PyLibrary.h"
#include "hurricane/isobar/PyCell.h"
#include "crlcore/PySystem.h"
#include "crlcore/PyBanner.h"
#include "crlcore/PyCatalog.h"
#include "crlcore/PyCatalogState.h"
#include "crlcore/PyAllianceFramework.h"
#include "crlcore/PyEnvironment.h"
#include "crlcore/PyAllianceLibrary.h"
#include "crlcore/PyCellGauge.h"
#include "crlcore/PyRoutingGauge.h"
#include "crlcore/PyRoutingLayerGauge.h"
#include "crlcore/PyToolBox.h"
#include "crlcore/PyToolEngine.h"
#include "crlcore/PyToolEngineCollection.h"
#include "crlcore/PyAcmSigda.h"
#include "crlcore/PyIspd05.h"
#include "crlcore/PyBlif.h"
#include "crlcore/PyGds.h"
#include "crlcore/PyLefImport.h"
#include "crlcore/PyDefImport.h"
#include "crlcore/VhdlEntity.h"


namespace CRL {

  using std::cerr;
  using std::endl;
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
  using Isobar::__cs;
  using Isobar::getPyHash;
  using Vhdl::EntityExtension;


#if !defined(__PYTHON_MODULE__)

// +=================================================================+
// |                "PyCRL" Shared Library Code Part                 |
// +=================================================================+


# else // End of PyHurricane Shared Library Code Part.


// +=================================================================+
// |                 "PyCRL" Python Module Code Part                 |
// +=================================================================+


extern "C" {


  static PyObject* PyVhdl_destroyAllVHDL ( PyObject* module )
  {
    cdebug_log(30,0) << "PyVhdl_destroyAllVHDL()" << endl;
    HTRY
      EntityExtension::destroyAll();
    HCATCH
    Py_RETURN_NONE;
  }


  // x-------------------------------------------------------------x
  // |                  "PyCRL" Module Methods                     |
  // x-------------------------------------------------------------x


  static PyMethodDef PyCRL_Methods[] =
    { { "createPartRing"      , (PyCFunction)PyToolBox_createPartRing      , METH_VARARGS
                              , "Partial build of a ring" }
    , { "restoreNetsDirection", (PyCFunction)PyToolBox_restoreNetsDirection, METH_VARARGS
                              , "Compute and set nets direction of a complete cell hierarchy." }
    , { "destroyAllVHDL"      , (PyCFunction)PyVhdl_destroyAllVHDL         , METH_NOARGS
                              , "Clear all VHDL informations on all cells." }
    , {NULL, NULL, 0, NULL}     /* sentinel */
    };




  // ---------------------------------------------------------------
  // Module Initialization  :  "initCRL ()"

  DL_EXPORT(void) initCRL () {
    cdebug_log(30,0) << "initCRL()" << endl;

    PySystem_LinkPyType ();
    PyBanner_LinkPyType ();
    PyCatalogState_LinkPyType ();
    PyCatalog_LinkPyType ();
    PyEnvironment_LinkPyType ();
    PyAllianceLibrary_LinkPyType ();
    PyCellGauge_LinkPyType ();
    PyRoutingGauge_LinkPyType ();
    PyRoutingLayerGauge_LinkPyType ();
    PyRoutingLayerGaugeVector_LinkPyType ();
    PyAllianceFramework_LinkPyType ();
    PyToolEngine_LinkPyType ();
    PyToolEngineCollection_LinkPyType ();
    PyAcmSigda_LinkPyType ();
    PyIspd05_LinkPyType ();
    PyBlif_LinkPyType ();
    PyGds_LinkPyType ();
    PyLefImport_LinkPyType ();
    PyDefImport_LinkPyType ();

    PYTYPE_READY ( System );
    PYTYPE_READY ( Banner );
    PYTYPE_READY ( CatalogState );
    PYTYPE_READY ( Catalog );
    PYTYPE_READY ( Environment );
    PYTYPE_READY ( AllianceLibrary );
    PYTYPE_READY ( CellGauge );
    PYTYPE_READY ( RoutingGauge );
    PYTYPE_READY ( RoutingLayerGaugeVector );
    PYTYPE_READY ( RoutingLayerGaugeVectorIterator );
    PYTYPE_READY ( RoutingLayerGauge );
    PYTYPE_READY ( AllianceFramework );
    PYTYPE_READY ( ToolEngine );
    PYTYPE_READY ( ToolEngineCollection );
    PYTYPE_READY ( ToolEngineCollectionLocator );
    PYTYPE_READY ( AcmSigda );
    PYTYPE_READY ( Ispd05 );
    PYTYPE_READY ( Blif );
    PYTYPE_READY ( Gds );
    PYTYPE_READY ( LefImport );
    PYTYPE_READY ( DefImport );
   
    // Identifier string can take up to 10 characters.
    __cs.addType ( "alcLib"     , &PyTypeAllianceLibrary  , "<AllianceLibrary>"  , false );
    __cs.addType ( "alcEnv"     , &PyTypeEnvironment      , "<Environment>"      , false );
    __cs.addType ( "cellGauge"  , &PyTypeCellGauge        , "<CellGauge>"        , false );
    __cs.addType ( "routGauge"  , &PyTypeRoutingGauge     , "<RoutingGauge>"     , false );
    __cs.addType ( "routLyGaug" , &PyTypeRoutingLayerGauge, "<RoutingLayerGauge>", false );
    __cs.addType ( "alcFw"      , &PyTypeAllianceFramework, "<AllianceFramework>", false );
    __cs.addType ( "alcCatalog" , &PyTypeCatalog          , "<Catalog>"          , false );
    __cs.addType ( "alcCatStat" , &PyTypeCatalogState     , "<Catalog::State>"   , false );

    PyObject* module = Py_InitModule ( "CRL", PyCRL_Methods );
    if ( module == NULL ) {
      cerr << "[ERROR]\n"
           << "  Failed to initialize CRL module." << endl;
      return;
    }

    Py_INCREF ( &PyTypeSystem );
    PyModule_AddObject ( module, "System", (PyObject*)&PyTypeSystem );
    Py_INCREF ( &PyTypeBanner );
    PyModule_AddObject ( module, "Banner", (PyObject*)&PyTypeBanner );
    Py_INCREF ( &PyTypeCatalog );
    PyModule_AddObject ( module, "Catalog", (PyObject*)&PyTypeCatalog );
    Py_INCREF ( &PyTypeAllianceLibrary );
    PyModule_AddObject ( module, "AllianceLibrary", (PyObject*)&PyTypeAllianceLibrary );
    Py_INCREF ( &PyTypeEnvironment );
    PyModule_AddObject ( module, "Environment", (PyObject*)&PyTypeEnvironment );
    Py_INCREF ( &PyTypeCellGauge );
    PyModule_AddObject ( module, "CellGauge", (PyObject*)&PyTypeCellGauge );
    Py_INCREF ( &PyTypeRoutingGauge );
    PyModule_AddObject ( module, "RoutingGauge", (PyObject*)&PyTypeRoutingGauge );
    Py_INCREF ( &PyTypeRoutingLayerGauge );
    PyModule_AddObject ( module, "RoutingLayerGauge", (PyObject*)&PyTypeRoutingLayerGauge );
    Py_INCREF ( &PyTypeRoutingLayerGaugeVector );
    PyModule_AddObject ( module, "RoutingLayerGaugeVector", (PyObject*)&PyTypeRoutingLayerGaugeVector );
    Py_INCREF ( &PyTypeRoutingLayerGaugeVectorIterator );
    PyModule_AddObject ( module, "RoutingLayerGaugeVectorIterator", (PyObject*)&PyTypeRoutingLayerGaugeVectorIterator );
    Py_INCREF ( &PyTypeAllianceFramework );
    PyModule_AddObject ( module, "AllianceFramework", (PyObject*)&PyTypeAllianceFramework );
    Py_INCREF ( &PyTypeToolEngine );
    PyModule_AddObject ( module, "ToolEngine", (PyObject*)&PyTypeToolEngine );
    Py_INCREF ( &PyTypeToolEngineCollection );
    PyModule_AddObject ( module, "ToolEngineCollection", (PyObject*)&PyTypeToolEngineCollection );
    Py_INCREF ( &PyTypeToolEngineCollectionLocator );
    PyModule_AddObject ( module, "ToolEngineCollectionLocator", (PyObject*)&PyTypeToolEngineCollectionLocator );
    Py_INCREF ( &PyTypeAcmSigda );
    PyModule_AddObject ( module, "AcmSigda", (PyObject*)&PyTypeAcmSigda );
    Py_INCREF ( &PyTypeIspd05 );
    PyModule_AddObject ( module, "Ispd05", (PyObject*)&PyTypeIspd05 );
    Py_INCREF ( &PyTypeBlif );
    PyModule_AddObject ( module, "Blif", (PyObject*)&PyTypeBlif );
    Py_INCREF ( &PyTypeGds );
    PyModule_AddObject ( module, "Gds", (PyObject*)&PyTypeGds );
    Py_INCREF ( &PyTypeLefImport );
    PyModule_AddObject ( module, "LefImport", (PyObject*)&PyTypeLefImport );
    Py_INCREF ( &PyTypeDefImport );
    PyModule_AddObject ( module, "DefImport", (PyObject*)&PyTypeDefImport );

    PyCatalog_postModuleInit ();
    PyEnvironment_postModuleInit ();
    PyRoutingGauge_postModuleInit ();
    PyRoutingLayerGauge_postModuleInit ();
    PyAllianceFramework_postModuleInit ();
    
  //PyObject* dictionnary = PyModule_GetDict ( module );
  //DbULoadConstants ( dictionnary );

    cdebug_log(30,0) << "CRL.so loaded " << (void*)&typeid(string) << endl;
  }

  
} // End of extern "C".


#endif // End of Python Module Code Part.


}  // End of CRL namespace.
