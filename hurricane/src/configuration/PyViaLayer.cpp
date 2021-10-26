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
// |  C++ Module  :       "./PyViaLayer.cpp"                         |
// +-----------------------------------------------------------------+


#include "hurricane/configuration/PyName.h"
#include "hurricane/configuration/PyTechnology.h"
#include "hurricane/configuration/PyBasicLayer.h"
#include "hurricane/configuration/PyViaLayer.h"


namespace  Isobar3 {

using namespace Hurricane;

extern "C" {


// Python methods.

  static PyObject* PyViaLayer_create ( PyObject* self, PyObject* args )
  { return callFunction("ViaLayer.create",&ViaLayer::create,args); }

  static PyObject* PyViaLayer_destroy ( PyObject* self, PyObject* args )
  { return callMethod("ViaLayer.destroy",&ViaLayer::destroy,self,args); }


  // ---------------------------------------------------------------
  // PyViaLayer Attribute Method table.

  PyMethodDef PyViaLayer_Methods[] =
    { { "create"     , (PyCFunction)PyViaLayer_create     , METH_VARARGS|METH_STATIC
                     , "Create a new ViaLayer." }
    , { "destroy"    , (PyCFunction)PyViaLayer_destroy    , METH_NOARGS
                     , "destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}  /* sentinel */
    };


}  // extern "C".

}  // Isobar3 namespace.
