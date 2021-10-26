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
// |  C++ Module  :       "./PyTransistorLayer.cpp"                  |
// +-----------------------------------------------------------------+


#include "hurricane/configuration/PyName.h"
#include "hurricane/configuration/PyTechnology.h"
#include "hurricane/configuration/PyBasicLayer.h"
#include "hurricane/configuration/PyTransistorLayer.h"


namespace  Isobar3 {

using namespace Hurricane;

extern "C" {


// Python methods.

  static PyObject* PyTransistorLayer_create ( PyObject* self, PyObject* args )
  { return callFunction("TransistorLayer.create",&TransistorLayer::create,args); }

  static PyObject* PyTransistorLayer_destroy ( PyObject* self, PyObject* args )
  { return callMethod("TransistorLayer.destroy",&TransistorLayer::destroy,self,args); }


  // ---------------------------------------------------------------
  // PyTransistorLayer Attribute Method table.

  PyMethodDef PyTransistorLayer_Methods[] =
    { { "create"     , (PyCFunction)PyTransistorLayer_create     , METH_VARARGS|METH_STATIC
                     , "Create a new TransistorLayer." }
    , { "destroy"    , (PyCFunction)PyTransistorLayer_destroy    , METH_NOARGS
                     , "destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}  /* sentinel */
    };


}  // extern "C".

}  // Isobar3 namespace.
