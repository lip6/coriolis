// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2010-2022, All Rights Reserved
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

#include <cstdio>

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
// #include "crlcore/PyIspd05.h"
#include "crlcore/PySpice.h"
#include "crlcore/PyBlif.h"
#include "crlcore/PyMeasures.h"
#include "crlcore/PyGds.h"
#include "crlcore/PyLefImport.h"
#include "crlcore/PyLefExport.h"
#include "crlcore/PyDefImport.h"
#include "crlcore/PyDefExport.h"
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
  using Utilities::Path;


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

  static int PyCRL_module_exec(PyObject* module)
  {
    cdebug_log(30,0) << "PyInit_CRL()" << endl;

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
    // PyIspd05_LinkPyType ();
    PySpice_LinkPyType ();
    PyBlif_LinkPyType ();
    PyMeasures_LinkPyType ();
    PyGds_LinkPyType ();
    PyLefImport_LinkPyType ();
    PyDefImport_LinkPyType ();
    PyLefExport_LinkPyType ();
    PyDefExport_LinkPyType ();

    PYTYPE_READY_NEW ( System );
    PYTYPE_READY_NEW ( Banner );
    PYTYPE_READY_NEW ( CatalogState );
    PYTYPE_READY_NEW ( Catalog );
    PYTYPE_READY_NEW ( Environment );
    PYTYPE_READY_NEW ( AllianceLibrary );
    PYTYPE_READY_NEW ( CellGauge );
    PYTYPE_READY_NEW ( RoutingGauge );
    PYTYPE_READY_NEW ( RoutingLayerGaugeVector );
    PYTYPE_READY_NEW ( RoutingLayerGaugeVectorIterator );
    PYTYPE_READY_NEW ( RoutingLayerGauge );
    PYTYPE_READY_NEW ( AllianceFramework );
    PYTYPE_READY_NEW ( ToolEngine );
    PYTYPE_READY_NEW ( ToolEngineCollection );
    PYTYPE_READY_NEW ( ToolEngineCollectionLocator );
    PYTYPE_READY_NEW ( AcmSigda );
    // PYTYPE_READY_NEW ( Ispd05 );
    PYTYPE_READY_NEW ( Spice );
    PYTYPE_READY_NEW ( Blif );
    PYTYPE_READY_NEW ( Measures );
    PYTYPE_READY_NEW ( Gds );
    PYTYPE_READY_NEW ( LefImport );
    PYTYPE_READY_NEW ( DefImport );
    PYTYPE_READY_NEW ( LefExport );
    PYTYPE_READY_NEW ( DefExport );

    // Identifier string can take up to 10 characters.
    __cs.addType ( "alcLib"     , &PyTypeAllianceLibrary  , "<AllianceLibrary>"  , false );
    __cs.addType ( "alcEnv"     , &PyTypeEnvironment      , "<Environment>"      , false );
    __cs.addType ( "cellGauge"  , &PyTypeCellGauge        , "<CellGauge>"        , false );
    __cs.addType ( "routGauge"  , &PyTypeRoutingGauge     , "<RoutingGauge>"     , false );
    __cs.addType ( "routLyGaug" , &PyTypeRoutingLayerGauge, "<RoutingLayerGauge>", false );
    __cs.addType ( "alcFw"      , &PyTypeAllianceFramework, "<AllianceFramework>", false );
    __cs.addType ( "alcCatalog" , &PyTypeCatalog          , "<Catalog>"          , false );
    __cs.addType ( "alcCatStat" , &PyTypeCatalogState     , "<Catalog::State>"   , false );

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
    // Py_INCREF ( &PyTypeIspd05 );
    // PyModule_AddObject ( module, "Ispd05", (PyObject*)&PyTypeIspd05 );
    Py_INCREF ( &PyTypeSpice );
    PyModule_AddObject ( module, "Spice", (PyObject*)&PyTypeSpice );
    Py_INCREF ( &PyTypeBlif );
    PyModule_AddObject ( module, "Blif", (PyObject*)&PyTypeBlif );
    Py_INCREF ( &PyTypeMeasures );
    PyModule_AddObject ( module, "Measures", (PyObject*)&PyTypeMeasures );
    Py_INCREF ( &PyTypeGds );
    PyModule_AddObject ( module, "Gds", (PyObject*)&PyTypeGds );
    Py_INCREF ( &PyTypeLefImport );
    PyModule_AddObject ( module, "LefImport", (PyObject*)&PyTypeLefImport );
    Py_INCREF ( &PyTypeDefImport );
    PyModule_AddObject ( module, "DefImport", (PyObject*)&PyTypeDefImport );
    Py_INCREF ( &PyTypeLefExport );
    PyModule_AddObject ( module, "LefExport", (PyObject*)&PyTypeLefExport );
    Py_INCREF ( &PyTypeDefExport );
    PyModule_AddObject ( module, "DefExport", (PyObject*)&PyTypeDefExport );

    PyCatalog_postModuleInit ();
    PyEnvironment_postModuleInit ();
    PyRoutingGauge_postModuleInit ();
    PyRoutingLayerGauge_postModuleInit ();
    PyAllianceFramework_postModuleInit ();
    PySpice_postModuleInit ();
    PyGds_postModuleInit ();
    PyDefExport_postModuleInit ();
    
  //PyObject* dictionnary = PyModule_GetDict ( module );
  //DbULoadConstants ( dictionnary );

    PyObject *name_obj;
    Path name, dir;

    name_obj = PyModule_GetFilenameObject((PyObject *)module);
    name = Path(PyUnicode_AsUTF8(name_obj));
    dir = name.dirname();
  //  std::cout << "coriolis dir" << dir.toString() << std::endl;
    setenv ( "CORIOLIS_TOP", (const char*) dir.toString().c_str(), 1);

    cdebug_log(30,0) << "CRL.so loaded " << (void*)&typeid(string) << endl;
    return 0;
  }

  static struct PyModuleDef_Slot PyCRL_Slots[] = {
    {Py_mod_exec, (void*) PyCRL_module_exec},
    {0, NULL},
  };

  static PyModuleDef  PyCRL_ModuleDef =
    {
      .m_base = PyModuleDef_HEAD_INIT,
      .m_name = "CRL",
      .m_doc = "Coriolis Core I/O framework",
      .m_size =  0,
      .m_methods = PyCRL_Methods,
      .m_slots = PyCRL_Slots,
    };

  PyMODINIT_FUNC PyInit_CRL(void)
  {
    return PyModuleDef_Init(&PyCRL_ModuleDef);
  }
} // End of extern "C".


#endif // End of Python Module Code Part.


}  // End of CRL namespace.
