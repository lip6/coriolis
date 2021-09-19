// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU/LIP6 2010-2021, All Rights Reserved
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
#include "hurricane/configuration/PyDbU.h"
#include "hurricane/configuration/PyPoint.h"
#include "hurricane/configuration/PyBox.h"
#include "hurricane/configuration/PyDataBase.h"
#include "hurricane/configuration/PyTechnology.h"
#include "hurricane/configuration/PyLayerMask.h"
#include "hurricane/configuration/PyLayer.h"
#include "hurricane/configuration/PyBasicLayer.h"


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
    PyTypeManagerDBo<BasicLayer>::create( module
                                        , PyBasicLayer_Methods
                                        , NULL
                                        , PyTypeManager::NoCppDelete
                                        );

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
