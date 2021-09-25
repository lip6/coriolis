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
// |  C++ Module  :       "./PyDiffusionLayer.cpp"                   |
// +-----------------------------------------------------------------+


#include "hurricane/configuration/PyName.h"
#include "hurricane/configuration/PyTechnology.h"
#include "hurricane/configuration/PyBasicLayer.h"
#include "hurricane/configuration/PyDiffusionLayer.h"


namespace  Isobar3 {

using namespace Hurricane;

extern "C" {


// Python methods.

  static PyObject* PyDiffusionLayer_create ( PyObject* self, PyObject* args )
  { return callFunction("DiffusionLayer.create",&DiffusionLayer::create,args); }

  static PyObject* PyDiffusionLayer_destroy ( PyObject* self, PyObject* args )
  { return callMethod("DiffusionLayer.destroy",&DiffusionLayer::destroy,self,args); }


  // ---------------------------------------------------------------
  // PyDiffusionLayer Attribute Method table.

  PyMethodDef PyDiffusionLayer_Methods[] =
    { { "create"     , (PyCFunction)PyDiffusionLayer_create     , METH_VARARGS|METH_STATIC
                     , "Create a new DiffusionLayer." }
    , { "destroy"    , (PyCFunction)PyDiffusionLayer_destroy    , METH_NOARGS
                     , "destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}  /* sentinel */
    };


}  // extern "C".

}  // Isobar3 namespace.
