// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU 2021-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyHurricane3.cpp"                            |
// +-----------------------------------------------------------------+


#include "hurricane/configuration/PyTypeManager.h"
#include "hurricane/configuration/PyCollection.h"
#include "hurricane/configuration/PyDbU.h"
#include "hurricane/configuration/PyPoint.h"
#include "hurricane/configuration/PyBox.h"
#include "hurricane/configuration/PyDataBase.h"
#include "hurricane/configuration/PyTechnology.h"
#include "hurricane/configuration/PyLayerMask.h"
#include "hurricane/configuration/PyLayer.h"
#include "hurricane/configuration/PyBasicLayer.h"
#include "hurricane/configuration/PyViaLayer.h"
#include "hurricane/configuration/PyRegularLayer.h"
#include "hurricane/configuration/PyDiffusionLayer.h"
#include "hurricane/configuration/PyTransistorLayer.h"


namespace Isobar3 {

  using std::cerr;
  using std::endl;
  using std::string;


extern "C" {


  static PyMethodDef PyHurricane3_Methods[] =
    { {NULL, NULL, 0, NULL}  /* sentinel */
    };


  static PyModuleDef  PyHurricane3_ModuleDef =
    { PyModuleDef_HEAD_INIT
    , .m_name    = "Hurricane3"
    , .m_doc     = "Hurricane database."
    , .m_size    = -1
    , .m_methods = PyHurricane3_Methods
    };


  PyMODINIT_FUNC PyInit_Hurricane3 ( void )
  {
    cdebug_log(20,0) << "PyInit_Hurricane3() (Python3+template)" << endl;
    cerr << "PyInit_Hurricane3() (Python3+template)" << endl;
    
    PyObject* module = PyModule_Create( &PyHurricane3_ModuleDef );
    if (not module) {
      cerr << "[ERROR]\n"
           << "  Failed to initialize Hurricane3 module." << endl;
      return NULL;
    }
    PyObject* dictionnary = PyModule_GetDict( module );

    try {
      PyTypeManagerNonDBo<DbU>::create  ( module
                                        , PyDbU_Methods
                                        , NULL
                                        , PyTypeManager::NoFlags
                                        );
      PyTypeManagerNonDBo<Point>::create( module
                                        , PyPoint_Methods
                                        , NULL
                                        , PyTypeManager::NoFlags
                                        , ""
                                        , PyPoint_NEW
                                        , PyPoint_Init
                                        , tpRichCompareByValue<Point>
                                        );
      PyTypeManagerNonDBo<Box>::create  ( module
                                        , PyBox_Methods
                                        , NULL
                                        , PyTypeManager::NoFlags
                                        , ""
                                        , PyBox_NEW
                                        , PyBox_Init
                                        , tpRichCompareByValue<Box>
                                        );
      PyTypeManagerDBo<DataBase>::create( module
                                        , PyDataBase_Methods
                                        , NULL
                                        , PyTypeManager::NoCppDelete
                                        );
      PyTypeManagerDBo<Technology>::create( module
                                          , PyTechnology_Methods
                                          , NULL
                                          , PyTypeManager::NoCppDelete
                                          );
      PyTypeManagerNonDBo<Layer::Mask>::create( module
                                              , PyLayerMask_Methods
                                              , NULL
                                              , PyTypeManager::NoFlags
                                              , "LayerMask"
                                              , PyLayerMask_NEW
                                              , PyLayerMask_Init
                                              , tpRichCompareLayerMask
                                              , &PyLayerMask_NumberMethods
                                              );
      PyTypeManagerDBo<Layer>::create( module
                                     , PyLayer_Methods
                                     , NULL
                                     , PyTypeManager::NoCppDelete
                                     );
      PyTypeManagerDerivedDBo<BasicLayer,Layer>::create( module
                                                       , PyBasicLayer_Methods
                                                       , NULL
                                                       , PyTypeManager::NoCppDelete
                                                       );
      PyTypeManagerDerivedDBo<ViaLayer,Layer>::create( module
                                                     , PyViaLayer_Methods
                                                     , NULL
                                                     , PyTypeManager::NoCppDelete
                                                     );
      PyTypeManagerDerivedDBo<RegularLayer,Layer>::create( module
                                                         , PyRegularLayer_Methods
                                                         , NULL
                                                         , PyTypeManager::NoCppDelete
                                                         );
      PyTypeManagerDerivedDBo<DiffusionLayer,Layer>::create( module
                                                           , PyDiffusionLayer_Methods
                                                           , NULL
                                                           , PyTypeManager::NoCppDelete
                                                           );
      PyTypeManagerDerivedDBo<TransistorLayer,Layer>::create( module
                                                            , PyTransistorLayer_Methods
                                                            , NULL
                                                            , PyTypeManager::NoCppDelete
                                                            );
      PyTypeManagerCollection<       Layer>::create( module, PyTypeManager::NoCppDelete );
      PyTypeManagerCollection<  BasicLayer>::create( module, PyTypeManager::NoCppDelete );
      PyTypeManagerCollection<    ViaLayer>::create( module, PyTypeManager::NoCppDelete );
      PyTypeManagerCollection<RegularLayer>::create( module, PyTypeManager::NoCppDelete );
    } catch ( const Warning& w ) {
      cerr << getString(w) << endl;
      throw;
    } catch ( const Error& e ) {
      cerr << getString(e) << endl;
      if (not e.where().empty()) cerr << e.where() << endl;
      throw;
    } catch ( const Bug& e ) {
      cerr << getString(e) << endl;
      throw;
    } catch ( const Exception& e ) {
      cerr <<  "\nUnknown Hurricane::Exception" << endl;
      throw;
    } catch ( const std::exception& e )  {
      cerr << e.what() << endl;
      throw;
    } catch ( ... ) {
      cerr << "Unmanaged exception, neither a Hurricane::Error nor a std::exception." << endl;
      throw;
    }

    cdebug_log(20,0)
      << "Hurricane3.so loaded " << (void*)&typeid(string)
      << " (" << demangle(typeid(string).name()) << ")" << endl;
    cerr
      << "Hurricane3.so loaded " << (void*)&typeid(string)
      << " (" << demangle(typeid(string).name()) << ")" << endl;

    return module;
  }

  
} // extern "C".

}  // Isobar3 namespace.
