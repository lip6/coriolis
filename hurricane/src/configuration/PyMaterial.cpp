// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2020-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyMaterial.cpp"                         |
// +-----------------------------------------------------------------+


#include "hurricane/configuration/PyMaterial.h"


namespace  Isobar3 {

using namespace Hurricane;

extern "C" {

// Thin wrappers for overloadeds member functions.

  static BasicLayer::Material* ctorMaterial ( long code )
  { return new BasicLayer::Material ( (BasicLayer::Material::Code)code ); }

// Python methods.

  static PyObject* PyMaterial_getCode ( PyObject* self, PyObject* args )
  { return callMethod("Material.getCode",&BasicLayer::Material::getCode,self,args); }

  PyObject* PyMaterial_NEW ( PyTypeObject* pyType, PyObject* args, PyObject* kwargs )
  {
    return callFunction("BasicLayer::Material",&ctorMaterial,args);
  }

  int  PyMaterial_Init ( PyObject* self, PyObject* args, PyObject* kwargs )
  {
    cdebug_log(20,0) << "PyMaterial_Init(): " << (void*)self << endl;
    return 0;
  }


  // ---------------------------------------------------------------
  // PyMaterial Method table.

  PyMethodDef PyMaterial_Methods[] =
    { { "getCode"             , (PyCFunction)PyMaterial_getCode, METH_NOARGS
                              , "Returns the numerical code of the material (enum)." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


}  // extern "C".

}  // Isobar3 namespace.
