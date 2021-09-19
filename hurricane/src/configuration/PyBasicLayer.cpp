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
// |  C++ Module  :       "./PyBasicLayer.cpp"                       |
// +-----------------------------------------------------------------+


#include "hurricane/configuration/PyName.h"
#include "hurricane/configuration/PyTechnology.h"
#include "hurricane/configuration/PyBasicLayer.h"


namespace  Isobar3 {

using namespace Hurricane;

extern "C" {


// Thin wrappers for overloadeds member functions.
  
  static BasicLayer*  create5 ( Technology*           technology
                              , Name                  name
                              , BasicLayer::Material  material
                              , unsigned              gds2Layer
                              , unsigned              gds2Datatype
                              )
  { return BasicLayer::create( technology, name, material, gds2Layer, gds2Datatype ); }
  
  static BasicLayer*  create6 ( Technology*           technology
                              , Name                  name
                              , BasicLayer::Material  material
                              , unsigned              gds2Layer
                              , unsigned              gds2Datatype
                              , DbU::Unit             minimalSize
                              )
  { return BasicLayer::create( technology, name, material, gds2Layer, gds2Datatype, minimalSize ); }
  
  static BasicLayer*  create7 ( Technology*           technology
                              , Name                  name
                              , BasicLayer::Material  material
                              , unsigned              gds2Layer
                              , unsigned              gds2Datatype
                              , DbU::Unit             minimalSize
                              , DbU::Unit             minimalSpacing
                              )
  { return BasicLayer::create( technology, name, material, gds2Layer, gds2Datatype, minimalSize, minimalSpacing ); }


// Python methods.

  static PyObject* PyBasicLayer_create ( PyObject* self, PyObject* args )
  {
    PyObject*       rvalue = callFunction("BasicLayer.create",&create7,args);
    if (not rvalue) rvalue = callFunction("BasicLayer.create",&create6,args);
    if (not rvalue) rvalue = callFunction("BasicLayer.create",&create5,args);
    return rvalue;
  }

  static PyObject* PyBasicLayer_getMaterial ( PyVoidPointer* self )
  { return callMethod("BasicLayer.getMaterial",&BasicLayer::getMaterial,self,NULL); }

  static PyObject* PyBasicLayer_getBlockageLayer ( PyVoidPointer* self )
  { return callMethod("BasicLayer.getBlockageLayer",&BasicLayer::getBlockageLayer,self,NULL); }

  static PyObject* PyBasicLayer_setBlockageLayer ( PyVoidPointer* self, PyObject* args )
  { return callMethod("BasicLayer.setBlockageLayer",&BasicLayer::setBlockageLayer,self,args); }

  static PyObject* PyBasicLayer_setGds2Layer ( PyVoidPointer* self, PyObject* args )
  { return callMethod("BasicLayer.setGds2Layer",&BasicLayer::setGds2Layer,self,args); }

  static PyObject* PyBasicLayer_setGds2Datatype ( PyVoidPointer* self, PyObject* args )
  { return callMethod("BasicLayer.setGds2Datatype",&BasicLayer::setGds2Datatype,self,args); }

  static PyObject* PyBasicLayer_setRealName ( PyVoidPointer* self, PyObject* args )
  { return callMethod("BasicLayer.setRealName",&BasicLayer::setRealName,self,args); }

  static PyObject* PyBasicLayer_destroy ( PyVoidPointer* self )
  { return callMethod("BasicLayer.destroy",&BasicLayer::destroy,self,NULL); }


  // ---------------------------------------------------------------
  // PyBasicLayer Attribute Method table.

  PyMethodDef PyBasicLayer_Methods[] =
    { { "create"             , (PyCFunction)PyBasicLayer_create           , METH_VARARGS|METH_STATIC
                             , "Create a new BasicLayer." }
    , { "getMaterial"        , (PyCFunction)PyBasicLayer_getMaterial      , METH_NOARGS
                             , "Returns the type of Material." }
    , { "getBlockageLayer"   , (PyCFunction)PyBasicLayer_getBlockageLayer , METH_NOARGS
                             , "Returns the associated blockage layer, if any." }
    , { "setBlockageLayer"   , (PyCFunction)PyBasicLayer_setBlockageLayer , METH_VARARGS
                             , "Sets the blockage layer associated to this one." }
    , { "setGds2Layer"       , (PyCFunction)PyBasicLayer_setGds2Layer     , METH_VARARGS
                             , "Sets the GDSII layer number." }
    , { "setGds2Datatype"    , (PyCFunction)PyBasicLayer_setGds2Datatype  , METH_VARARGS
                             , "Sets the GDSII layer Datatype." }
    , { "setRealName"        , (PyCFunction)PyBasicLayer_setRealName      , METH_VARARGS
                             , "Sets the real name of this layer (as seen in GDSII)." }
    , { "destroy"            , (PyCFunction)PyBasicLayer_destroy          , METH_NOARGS
                             , "destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}  /* sentinel */
    };


}  // extern "C".

}  // Isobar3 namespace.
