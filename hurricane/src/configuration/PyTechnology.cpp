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
// |  C++ Module  :       "./PyTechnology.cpp"                       |
// +-----------------------------------------------------------------+


#include "hurricane/configuration/PyName.h"
#include "hurricane/configuration/PyDataBase.h"
#include "hurricane/configuration/PyTechnology.h"
#include "hurricane/DataBase.h"
#include "hurricane/Technology.h"
#include "hurricane/BasicLayer.h"
#include "hurricane/RegularLayer.h"
#include "hurricane/ViaLayer.h"
#include "hurricane/Library.h"
#include "hurricane/Cell.h"


namespace  Isobar3 {

using namespace Hurricane;

extern "C" {


// Thin wrappers for overloadeds member functions.
  
  static Layer* getLayer2 ( Technology* tech, string name )
  { return tech->getLayer( Name(name) ); }

  static Layer* getLayer1 ( Technology* tech, Layer::Mask mask, bool useSymbolic )
  { return tech->getLayer( mask, useSymbolic ); }

  static bool  setSymbolicLayerString ( Technology* tech, string name )
  { return tech->setSymbolicLayer( Name(name) ); }

  static bool  setSymbolicLayerLayer ( Technology* tech, Layer* layer )
  { return tech->setSymbolicLayer( layer ); }

  static PhysicalRule* getPhysicalRule3 ( Technology* tech, string ruleName, string layer1, string layer2 )
  { return tech->getPhysicalRule( ruleName, layer1, layer2 ); }

  static PhysicalRule* getPhysicalRule2 ( Technology* tech, string ruleName, string layer1 )
  { return tech->getPhysicalRule( ruleName, layer1 ); }

  static PhysicalRule* getPhysicalRule1 ( Technology* tech, string ruleName )
  { return tech->getPhysicalRule( ruleName ); }

  static PhysicalRule* addPhysicalRule4 ( Technology* tech, string ruleName, string layer1, string layer2, string ref )
  { return tech->addPhysicalRule( ruleName, layer1, layer2, ref ); }

  static PhysicalRule* addPhysicalRule3 ( Technology* tech, string ruleName, string layer1, string ref )
  { return tech->addPhysicalRule( ruleName, layer1, ref ); }

  static PhysicalRule* addPhysicalRule2 ( Technology* tech, string ruleName, string ref )
  { return tech->addPhysicalRule( ruleName, ref ); }

  static BasicLayers  getBasicLayers1 ( Technology* tech, Layer::Mask mask )
  { return tech->getBasicLayers( mask ); }

  static BasicLayers  getBasicLayers0 ( Technology* tech )
  { return tech->getBasicLayers(); }

  static void  setName ( Technology* tech, string name )
  { return tech->setName( Name(name) ); }


// Python methods.

  static PyObject* PyTechnology_create ( PyObject* self, PyObject* args )
  {
  //Name test;
  //pyToC( NULL, &test );
    return callFunction("Technology.create",&Technology::create,args);
  }

  static PyObject* PyTechnology_getDataBase ( PyObject* self, PyObject* args )
  { return callMethod("Technology.getDataBase",&Technology::getDataBase,self,args); }

  static PyObject* PyTechnology_getLayer ( PyObject* self, PyObject* args )
  {
    PyObject*       rvalue = callMethod("Technology.getLayer",&getLayer2,self,args);
    if (not rvalue) rvalue = callMethod("Technology.getLayer",&getLayer1,self,args);
    return rvalue;
  }

  static PyObject* PyTechnology_getBasicLayers ( PyObject* self, PyObject* args )
  {
    PyObject*       rvalue = callMethod("Technology.getBasicLayers",&getBasicLayers1,self,args);
    if (not rvalue) rvalue = callMethod("Technology.getBasicLayers",&getBasicLayers0,self,args);
    return rvalue;
  }

  static PyObject* PyTechnology_setSymbolicLayer ( PyObject* self, PyObject* args )
  {
    PyObject*       rvalue = callMethod("Technology.setSymbolicLayer",&setSymbolicLayerLayer ,self,args);
    if (not rvalue) rvalue = callMethod("Technology.setSymbolicLayer",&setSymbolicLayerString,self,args);
    return rvalue;
  }

  static PyObject* PyTechnology_getUnitRule ( PyObject* self, PyObject* args )
  { return callMethod("Technology.getUnitRule",&Technology::getUnitRule,self,args); }

  static PyObject* PyTechnology_getPhysicalRule ( PyObject* self, PyObject* args )
  {
    PyObject*       rvalue = callMethod("Technology.getPhysicalRule",&getPhysicalRule3,self,args);
    if (not rvalue) rvalue = callMethod("Technology.getPhysicalRule",&getPhysicalRule2,self,args);
    if (not rvalue) rvalue = callMethod("Technology.getPhysicalRule",&getPhysicalRule1,self,args);
    return rvalue;
  }

  static PyObject* PyTechnology_addUnitRule ( PyObject* self, PyObject* args )
  { return callMethod("Technology.addUnitRule",&Technology::addUnitRule,self,args); }

  static PyObject* PyTechnology_addPhysicalRule ( PyObject* self, PyObject* args )
  {
    PyObject*       rvalue = callMethod("Technology.addPhysicalRule",&addPhysicalRule4,self,args);
    if (not rvalue) rvalue = callMethod("Technology.addPhysicalRule",&addPhysicalRule3,self,args);
    if (not rvalue) rvalue = callMethod("Technology.addPhysicalRule",&addPhysicalRule2,self,args);
    return rvalue;
  }

  static PyObject* PyTechnology_getDeviceDescriptor ( PyObject* self, PyObject* args )
  { return callMethod("Technology.getDeviceDescriptor",&Technology::getDeviceDescriptor,self,args); }

  static PyObject* PyTechnology_addDeviceDescriptor ( PyObject* self, PyObject* args )
  { return callMethod("Technology.addDeviceDescriptor",&Technology::addDeviceDescriptor,self,args); }

  static PyObject* PyTechnology_getName ( PyObject* self, PyObject* args )
  { return callMethod("Technology.getName",&Technology::getName,self,args); }

  static PyObject* PyTechnology_setName ( PyObject* self, PyObject* args )
  { return callMethod("Technology.setName",&setName,self,args); }

  static PyObject* PyTechnology_isMetal ( PyObject* self, PyObject* args )
  { return callMethod("Technology.isMetal",&Technology::isMetal,self,args); }

  static PyObject* PyTechnology_getBasicLayer ( PyObject* self, PyObject* args )
  { return callMethod("Technology.getBasicLayer",&Technology::getBasicLayer,self,args); }

  static PyObject* PyTechnology_getRegularLayer ( PyObject* self, PyObject* args )
  { return callMethod("Technology.getRegularLayer",&Technology::getRegularLayer,self,args); }

  static PyObject* PyTechnology_getViaLayer ( PyObject* self, PyObject* args )
  { return callMethod("Technology.getViaLayer",&Technology::getViaLayer,self,args); }

  static PyObject* PyTechnology_getLayers ( PyObject* self, PyObject* args )
  { return callMethod("Technology.getLayers",&Technology::getLayers,self,args); }

  static PyObject* PyTechnology_getRegularLayers ( PyObject* self, PyObject* args )
  { return callMethod("Technology.getRegularLayers",&Technology::getRegularLayers,self,args); }

  static PyObject* PyTechnology_getViaLayers ( PyObject* self, PyObject* args )
  { return callMethod("Technology.getViaLayers",&Technology::getViaLayers,self,args); }

  static PyObject* PyTechnology_getMetalAbove ( PyObject* self, PyObject* args )
  { return callMethod("Technology.getMetalAbove",&Technology::getMetalAbove,self,args); }

  static PyObject* PyTechnology_getMetalBelow ( PyObject* self, PyObject* args )
  { return callMethod("Technology.getMetalBelow",&Technology::getMetalBelow,self,args); }

  static PyObject* PyTechnology_getCutAbove ( PyObject* self, PyObject* args )
  { return callMethod("Technology.getCutAbove",&Technology::getCutAbove,self,args); }

  static PyObject* PyTechnology_getCutBelow ( PyObject* self, PyObject* args )
  { return callMethod("Technology.getCutBelow",&Technology::getCutBelow,self,args); }

  static PyObject* PyTechnology_getViaBetween ( PyObject* self, PyObject* args )
  { return callMethod("Technology.getViaBetween",&Technology::getViaBetween,self,args); }

  static PyObject* PyTechnology_getNthMetal ( PyObject* self, PyObject* args )
  { return callMethod("Technology.getNthMetal",&Technology::getNthMetal,self,args); }
  
  static PyObject* PyTechnology_destroy ( PyObject* self, PyObject* args )
  { return callMethod("Technology.destroy",&Technology::destroy,self,args); }


  // ---------------------------------------------------------------
  // PyTechnology Method table.

  PyMethodDef PyTechnology_Methods[] =
    { { "create"             , (PyCFunction)PyTechnology_create               , METH_VARARGS|METH_STATIC
                             , "Create the Technology object." }
    , { "isMetal"            , (PyCFunction)PyTechnology_isMetal              , METH_VARARGS
                             , "Tells if the given layer is of metal kind (Material)." }
    , { "getDataBase"        , (PyCFunction)PyTechnology_getDataBase          , METH_NOARGS
                             , "Returns the associated DataBase." }
    , { "getName"            , (PyCFunction)PyTechnology_getName              , METH_NOARGS
                             , "Returns the name of the technology." }
    , { "getLayer"           , (PyCFunction)PyTechnology_getLayer             , METH_VARARGS
                             , "Returns the layer named name." }
    , { "getBasicLayer"      , (PyCFunction)PyTechnology_getBasicLayer        , METH_VARARGS
                             , "Returns the BasicLayer named name." }
    , { "getRegularLayer"    , (PyCFunction)PyTechnology_getRegularLayer      , METH_VARARGS
                             , "Returns the RegularLayer named name." }
    , { "getViaLayer"        , (PyCFunction)PyTechnology_getViaLayer          , METH_VARARGS
                             , "Returns the ViaLayer named name." }
    , { "getLayers"          , (PyCFunction)PyTechnology_getLayers            , METH_NOARGS
                             , "Returns the collection of all Layers." }
    , { "getBasicLayers"     , (PyCFunction)PyTechnology_getBasicLayers       , METH_VARARGS
                             , "Returns the collection of all BasicLayers." }
    , { "getRegularLayers"   , (PyCFunction)PyTechnology_getRegularLayers     , METH_NOARGS
                             , "Returns the collection of all RegularLayers." }
    , { "getViaLayers"       , (PyCFunction)PyTechnology_getViaLayers         , METH_NOARGS
                             , "Returns the collection of all BasicLayers." }
    , { "getMetalAbove"      , (PyCFunction)PyTechnology_getMetalAbove        , METH_VARARGS
                             , "Returns the metal layer immediatly above this one." }
    , { "getMetalBelow"      , (PyCFunction)PyTechnology_getMetalBelow        , METH_VARARGS
                             , "Returns the metal layer immediatly below this one." }
    , { "getCutAbove"        , (PyCFunction)PyTechnology_getCutAbove          , METH_VARARGS
                             , "Returns the cut layer immediatly above this one." }
    , { "getCutBelow"        , (PyCFunction)PyTechnology_getCutBelow          , METH_VARARGS
                             , "Returns the cut layer immediatly below." }
    , { "getViaBetween"      , (PyCFunction)PyTechnology_getViaBetween        , METH_VARARGS
                             , "Returns the VIA between those two layers (must be adjacent)." }
    , { "getNthMetal"        , (PyCFunction)PyTechnology_getNthMetal          , METH_VARARGS
                             , "Returns Nth metal (zero is nearest substrate)." }
    , { "setName"            , (PyCFunction)PyTechnology_setName              , METH_VARARGS
                             , "Allows to change the technology name." }
    , { "setSymbolicLayer"   , (PyCFunction)PyTechnology_setSymbolicLayer     , METH_VARARGS
                             , "Mark a Layer as the symbolic one (by name or by Layer)." }
    , { "getUnitRule"        , (PyCFunction)PyTechnology_getUnitRule          , METH_VARARGS
                             , "Returns the Unit Rule named name." }
    , { "getPhysicalRule"    , (PyCFunction)PyTechnology_getPhysicalRule      , METH_VARARGS
                             , "Returns the Physical Rule named name." }
    , { "getLayer"           , (PyCFunction)PyTechnology_getLayer             , METH_VARARGS
                             , "Returns the Layer named name." }
    , { "addPhysicalRule"    , (PyCFunction)PyTechnology_addPhysicalRule      , METH_VARARGS
                             , "Adds a new physical rule." }
    , { "addUnitRule"        , (PyCFunction)PyTechnology_addUnitRule          , METH_VARARGS
                             , "Adds a new Unit rule." }
    , { "getDeviceDescriptor", (PyCFunction)PyTechnology_getDeviceDescriptor  , METH_VARARGS
                             , "Get the DeviceDescriptor <name>." }
    , { "addDeviceDescriptor", (PyCFunction)PyTechnology_addDeviceDescriptor  , METH_VARARGS
                             , "Add (create) the DeviceDescriptor <name>." }
    , { "destroy"            , (PyCFunction)PyTechnology_destroy              , METH_NOARGS
                             , "Destroy associated hurricane object The python object remains." }
    , {NULL, NULL, 0, NULL} /* sentinel */
    };


}  // extern "C".

}  // Isobar3 namespace.
