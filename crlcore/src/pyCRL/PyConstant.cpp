// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2018-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyConstant.cpp"                         |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/PyRoutingLayerGauge.h"


namespace Constant {

  using std::cerr;
  using std::endl;
  using std::string;
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
  using Isobar::__cs;


#if !defined(__PYTHON_MODULE__)

// +=================================================================+
// |             "PyConstant" Shared Library Code Part               |
// +=================================================================+


# else // End of PyHurricane Shared Library Code Part.


// +=================================================================+
// |              "PyConstant" Python Module Code Part               |
// +=================================================================+


extern "C" {


  static void  LoadConstants ( PyObject* dictionnary ) {
    PyObject* constant;

    LoadObjectConstant( dictionnary, Constant::Horizontal, "Horizontal" )
    LoadObjectConstant( dictionnary, Constant::Vertical  , "Vertical"   )
    LoadObjectConstant( dictionnary, Constant::Default   , "Default"    )
    LoadObjectConstant( dictionnary, Constant::PinOnly   , "PinOnly"    )
    LoadObjectConstant( dictionnary, Constant::Superior  , "Superior"   )
    LoadObjectConstant( dictionnary, Constant::Inferior  , "Inferior"   )
    LoadObjectConstant( dictionnary, Constant::Nearest   , "Nearest"    )
    LoadObjectConstant( dictionnary, Constant::Exact     , "Exact"      )
  }


  static PyMethodDef PyConstant_Methods[] =
    { {NULL, NULL, 0, NULL}     /* sentinel */
    };


  DL_EXPORT(void) initConstant () {
    cdebug_log(30,0) << "initConstant()" << endl;

    PyObject* module = Py_InitModule( "Constant", PyConstant_Methods );
    if (module == NULL) {
      cerr << "[ERROR]\n"
           << "  Failed to initialize Constant module." << endl;
      return;
    }

    PyObject* dictionnary = PyModule_GetDict( module );
    LoadConstants( dictionnary );

    cdebug_log(30,0) << "Constant.so loaded " << (void*)&typeid(string) << endl;
  }

  
} // extern "C".


#endif  // Python Module Code Part.

}  // Constant namespace.
