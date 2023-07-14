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
// |  C++ Module  :       "./PyRegularLayer.cpp"                     |
// +-----------------------------------------------------------------+


#include "hurricane/configuration/PyName.h"
#include "hurricane/configuration/PyTechnology.h"
#include "hurricane/configuration/PyBasicLayer.h"
#include "hurricane/configuration/PyRegularLayer.h"


namespace  Isobar3 {

using namespace Hurricane;

extern "C" {


// Python methods.

  static PyObject* PyRegularLayer_create ( PyObject* self, PyObject* args )
  { return callFunction("RegularLayer.create",&RegularLayer::create,args); }

  static PyObject* PyRegularLayer_getBasicLayer ( PyObject* self, PyObject* args )
  { return callMethod("RegularLayer.getBasicLayer",&RegularLayer::getBasicLayer,self,args); }

  static PyObject* PyRegularLayer_setBasicLayer ( PyObject* self, PyObject* args )
  { return callMethod("RegularLayer.setBasicLayer",&RegularLayer::setBasicLayer,self,args); }

  static PyObject* PyRegularLayer_destroy ( PyObject* self, PyObject* args )
  { return callMethod("RegularLayer.destroy",&RegularLayer::destroy,self,args); }


  // ---------------------------------------------------------------
  // PyRegularLayer Attribute Method table.

  PyMethodDef PyRegularLayer_Methods[] =
    { { "create"           , (PyCFunction)PyRegularLayer_create           , METH_VARARGS|METH_STATIC
                           , "Create a new RegularLayer." }
    , { "getBasicLayer"    , (PyCFunction)PyRegularLayer_getBasicLayer    , METH_NOARGS
                           , "Get the RegularLayer associated to this RegularLayer." }
    , { "setBasicLayer"    , (PyCFunction)PyRegularLayer_setBasicLayer    , METH_VARARGS
                           , "Associate a RegularLayer with this RegularLayer." }
    , { "destroy"          , (PyCFunction)PyRegularLayer_destroy          , METH_NOARGS
                           , "destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}  /* sentinel */
    };


}  // extern "C".

}  // Isobar3 namespace.
