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
// |  C++ Module  :       "./PyLayer.cpp"                            |
// +-----------------------------------------------------------------+


#include "hurricane/configuration/PyName.h"
#include "hurricane/configuration/PyTechnology.h"
#include "hurricane/configuration/PyLayer.h"
#include "hurricane/BasicLayer.h"


namespace  Isobar3 {

using namespace Hurricane;

extern "C" {


// Thin wrappers for overloadeds member functions.
  
  static DbU::Unit  getEnclosure1 ( Layer* layer, uint32_t flags )
  { return layer->getEnclosure( flags ); }
  
  static DbU::Unit  getEnclosure2 ( Layer* layer, const BasicLayer* basicLayer, uint32_t flags )
  { return layer->getEnclosure( basicLayer, flags ); }
  
  static DbU::Unit  getExtentionCap0 ( Layer* layer )
  { return layer->getExtentionCap(); }
  
  static DbU::Unit  getExtentionCap1 ( Layer* layer, const BasicLayer* basicLayer )
  { return layer->getExtentionCap( basicLayer ); }
  
  static DbU::Unit  getExtentionWidth0 ( Layer* layer )
  { return layer->getExtentionWidth(); }
  
  static DbU::Unit  getExtentionWidth1 ( Layer* layer, const BasicLayer* basicLayer )
  { return layer->getExtentionWidth( basicLayer ); }


// Python methods.

  static PyObject* PyLayer_getTechnology ( PyObject* self, PyObject* args )
  { return callMethod("Layer.getTechnology",&Layer::getTechnology,self,args); }

  static PyObject* PyLayer_getName ( PyObject* self, PyObject* args )
  { return callMethod("Layer.getName",&Layer::getName,self,args); }

  static PyObject* PyLayer_getMask ( PyObject* self, PyObject* args )
  { return callMethod("Layer.getMask",&Layer::getMask,self,args); }

  static PyObject* PyLayer_getExtractMask ( PyObject* self, PyObject* args )
  { return callMethod("Layer.getExtractMask",&Layer::getExtractMask,self,args); }

  static PyObject* PyLayer_getMinimalSize ( PyObject* self, PyObject* args )
  { return callMethod("Layer.getMinimalSize",&Layer::getMinimalSize,self,args); }

  static PyObject* PyLayer_getMinimalSpacing ( PyObject* self, PyObject* args )
  { return callMethod("Layer.getMinimalSpacing",&Layer::getMinimalSpacing,self,args); }

  static PyObject* PyLayer_getBasicLayers ( PyObject* self, PyObject* args )
  { return callMethod("Layer.getBasicLayers",&Layer::getBasicLayers,self,args); }

  static PyObject* PyLayer_getBlockageLayer ( PyObject* self, PyObject* args )
  { return callMethod("Layer.getBlockageLayer",&Layer::getBlockageLayer,self,args); }

  static PyObject* PyLayer_getCut ( PyObject* self, PyObject* args )
  { return callMethod("Layer.getCut",&Layer::getCut,self,args); }

  static PyObject* PyLayer_getTop ( PyObject* self, PyObject* args )
  { return callMethod("Layer.getTop",&Layer::getTop,self,args); }

  static PyObject* PyLayer_getBottom ( PyObject* self, PyObject* args )
  { return callMethod("Layer.getBottom",&Layer::getBottom,self,args); }

  static PyObject* PyLayer_getOpposite ( PyObject* self, PyObject* args )
  { return callMethod("Layer.getOpposite",&Layer::getOpposite,self,args); }

  static PyObject* PyLayer_getMetalAbove ( PyObject* self, PyObject* args )
  { return callMethod("Layer.getMetalAbove",&Layer::getMetalAbove,self,args); }

  static PyObject* PyLayer_getMetalBelow ( PyObject* self, PyObject* args )
  { return callMethod("Layer.getMetalBelow",&Layer::getMetalBelow,self,args); }

  static PyObject* PyLayer_getCutAbove ( PyObject* self, PyObject* args )
  { return callMethod("Layer.getCutAbove",&Layer::getCutAbove,self,args); }

  static PyObject* PyLayer_getCutBelow ( PyObject* self, PyObject* args )
  { return callMethod("Layer.getCutBelow",&Layer::getCutBelow,self,args); }

  static PyObject* PyLayer_getEnclosure ( PyObject* self, PyObject* args )
  { 
    PyObject*       rvalue = callMethod("Layer.getEnclosure",&getEnclosure2,self,args);
    if (not rvalue) rvalue = callMethod("Layer.getEnclosure",&getEnclosure1,self,args);
    return rvalue;
  }

  static PyObject* PyLayer_getExtentionCap ( PyObject* self, PyObject* args )
  {
    PyObject*       rvalue = callMethod("Layer.getExtensionCap",&getExtentionCap1,self,args);
    if (not rvalue) rvalue = callMethod("Layer.getExtensionCap",&getExtentionCap0,self,args);
    return rvalue;
  }

  static PyObject* PyLayer_getExtentionWidth ( PyObject* self, PyObject* args )
  {
    PyObject*       rvalue = callMethod("Layer.getExtensionWidth",&getExtentionWidth1,self,args);
    if (not rvalue) rvalue = callMethod("Layer.getExtensionWidth",&getExtentionWidth0,self,args);
    return rvalue;
  }

  static PyObject* PyLayer_getTopEnclosure ( PyObject* self, PyObject* args )
  { return callMethod("Layer.getTopEnclosure",&Layer::getTopEnclosure,self,args); }

  static PyObject* PyLayer_getBottomEnclosure ( PyObject* self, PyObject* args )
  { return callMethod("Layer.getBottomEnclosure",&Layer::getBottomEnclosure,self,args); }

  static PyObject* PyLayer_getMinimalArea ( PyObject* self, PyObject* args )
  { return callMethod("Layer.getMinimalArea",&Layer::getMinimalArea,self,args); }

  static PyObject* PyLayer_above ( PyObject* self, PyObject* args )
  { return callMethod("Layer.above",&Layer::above,self,args); }

  static PyObject* PyLayer_below ( PyObject* self, PyObject* args )
  { return callMethod("Layer.below",&Layer::below,self,args); }

  static PyObject* PyLayer_contains ( PyObject* self, PyObject* args )
  { return callMethod("Layer.contains",&Layer::contains,self,args); }

  static PyObject* PyLayer_intersect ( PyObject* self, PyObject* args )
  { return callMethod("Layer.intersect",&Layer::intersect,self,args); }

  static PyObject* PyLayer_isSymbolic ( PyObject* self, PyObject* args )
  { return callMethod("Layer.isSymbolic",&Layer::isSymbolic,self,args); }

  static PyObject* PyLayer_isBlockage ( PyObject* self, PyObject* args )
  { return callMethod("Layer.isBlockage",&Layer::isBlockage,self,args); }

  static PyObject* PyLayer_setName ( PyObject* self, PyObject* args )
  { return callMethod("Layer.setName",&Layer::setName,self,args); }

  static PyObject* PyLayer_setSymbolic ( PyObject* self, PyObject* args )
  { return callMethod("Layer.setSymbolic",&Layer::setSymbolic,self,args); }

  static PyObject* PyLayer_setBlockage ( PyObject* self, PyObject* args )
  { return callMethod("Layer.setBlockage",&Layer::setBlockage,self,args); }

  static PyObject* PyLayer_setMinimalSize ( PyObject* self, PyObject* args )
  { return callMethod("Layer.setMinimalSize",&Layer::setMinimalSize,self,args); }

  static PyObject* PyLayer_setMinimalSpacing ( PyObject* self, PyObject* args )
  { return callMethod("Layer.setMinimalSpacing",&Layer::setMinimalSpacing,self,args); }

  static PyObject* PyLayer_setEnclosure ( PyObject* self, PyObject* args )
  { return callMethod("Layer.setEnclosure",&Layer::setEnclosure,self,args); }

  static PyObject* PyLayer_setExtentionCap ( PyObject* self, PyObject* args )
  { return callMethod("Layer.setExtentionCap",&Layer::setExtentionCap,self,args); }

  static PyObject* PyLayer_setExtentionWidth ( PyObject* self, PyObject* args )
  { return callMethod("Layer.setExtentionWidth",&Layer::setExtentionWidth,self,args); }

  static PyObject* PyLayer_setMinimalArea ( PyObject* self, PyObject* args )
  { return callMethod("Layer.setMinimalArea",&Layer::setMinimalArea,self,args); }

  static PyObject* PyLayer_destroy ( PyObject* self, PyObject* args )
  { return callMethod("Layer.destroy",&Layer::destroy,self,args); }


  // ---------------------------------------------------------------
  // PyLayer Method table.

  PyMethodDef PyLayer_Methods[] =
    { { "getTechnology"       , (PyCFunction)PyLayer_getTechnology       , METH_NOARGS
                              , "Returns the technology owning the layer." }
    , { "getName"             , (PyCFunction)PyLayer_getName             , METH_NOARGS
                              , "Returns the name of the layer." }
    , { "getMask"             , (PyCFunction)PyLayer_getMask             , METH_NOARGS
                              , "Returns the mask (bits) of the layer." }
    , { "getExtractMask"      , (PyCFunction)PyLayer_getExtractMask      , METH_NOARGS
                              , "Returns the extract mask of the layer (for GDSII)." }
    , { "getMinimalSize"      , (PyCFunction)PyLayer_getMinimalSize      , METH_NOARGS
                              , "Returns the minimum width allowed for the layer." }
    , { "getMinimalArea"      , (PyCFunction)PyLayer_getMinimalArea      , METH_NOARGS
                              , "Returns the minimum area allowed for the layer." }
    , { "getMinimalSpacing"   , (PyCFunction)PyLayer_getMinimalSpacing   , METH_NOARGS
                              , "Returns the spacing allowed for the layer (edge to edge)." }
    , { "getBasicLayers"      , (PyCFunction)PyLayer_getBasicLayers      , METH_NOARGS
                              , "Returns the collection of BasicLayer the Layer is built upon." }
    , { "getBlockageLayer"    , (PyCFunction)PyLayer_getBlockageLayer    , METH_NOARGS
                              , "Returns the associated connector layer." }
    , { "getCut"              , (PyCFunction)PyLayer_getCut              , METH_NOARGS
                              , "Returns the cut layer (in case of a muti-layer Contact)." }
    , { "getTop"              , (PyCFunction)PyLayer_getTop              , METH_NOARGS
                              , "Returns the top layer (in case of a muti-layer)." }
    , { "getBottom"           , (PyCFunction)PyLayer_getBottom           , METH_NOARGS
                              , "Returns the bottom layer (in case of a muti-layer)." }
    , { "getOpposite"         , (PyCFunction)PyLayer_getOpposite         , METH_VARARGS
                              , "Returns the layer opposite the one given." }
    , { "getMetalAbove"       , (PyCFunction)PyLayer_getMetalAbove       , METH_VARARGS
                              , "Returns the metal layer above this one." }
    , { "getMetalBelow"       , (PyCFunction)PyLayer_getMetalBelow       , METH_VARARGS
                              , "Returns the metal layer below this one." }
    , { "getCutAbove"         , (PyCFunction)PyLayer_getCutAbove         , METH_VARARGS
                              , "Returns the cut layer above this one." }
    , { "getCutBelow"         , (PyCFunction)PyLayer_getCutBelow         , METH_VARARGS
                              , "Returns the cut layer below this one." }
    , { "getEnclosure"        , (PyCFunction)PyLayer_getEnclosure        , METH_VARARGS
                              , "Returns the enclosure (global or for one BasicLayer)." }
    , { "getTopEnclosure"     , (PyCFunction)PyLayer_getTopEnclosure     , METH_VARARGS
                              , "Returns the top layer enclosure." }
    , { "getBottomEnclosure"  , (PyCFunction)PyLayer_getBottomEnclosure  , METH_VARARGS
                              , "Returns the bottom layer enclosure." }
    , { "getExtentionCap"     , (PyCFunction)PyLayer_getExtentionCap     , METH_VARARGS
                              , "Returns the extention cap (global or for one BasicLayer)." }
    , { "getExtentionWidth"   , (PyCFunction)PyLayer_getExtentionWidth   , METH_VARARGS
                              , "Returns the extention width (global or for one BasicLayer)." }
    , { "above"               , (PyCFunction)PyLayer_above               , METH_VARARGS
                              , "Tells if the layer is above the one passed as argument." }
    , { "below"               , (PyCFunction)PyLayer_below               , METH_VARARGS
                              , "Tells if the layer is below the one passed as argument." }
    , { "contains"            , (PyCFunction)PyLayer_contains            , METH_VARARGS
                              , "Tells if the layer fully contains the one passed as argument." }
    , { "intersect"           , (PyCFunction)PyLayer_intersect           , METH_VARARGS
                              , "Tells if the layer share some BasicLayer with the one passed as argument." }
    , { "isSymbolic"          , (PyCFunction)PyLayer_isSymbolic          , METH_NOARGS
                              , "Tells if the layer is the symbolic one for this BasicLayer." }
    , { "isBlockage"          , (PyCFunction)PyLayer_isBlockage          , METH_NOARGS
                              , "Tells if the layer represent blockage." }
    , { "setName"             , (PyCFunction)PyLayer_setName             , METH_VARARGS
                              , "Allows to change the layer name." }
    , { "setSymbolic"         , (PyCFunction)PyLayer_setSymbolic         , METH_VARARGS
                              , "Sets the layer as the symbolic one." }
    , { "setBlockage"         , (PyCFunction)PyLayer_setBlockage         , METH_VARARGS
                              , "Sets the layer as blockage." }
    , { "setMinimalSize"      , (PyCFunction)PyLayer_setMinimalSize      , METH_VARARGS
                              , "Sets the layer minimal size (width)." }
    , { "setMinimalSpacing"   , (PyCFunction)PyLayer_setMinimalSpacing   , METH_VARARGS
                              , "Sets the layer minimal spacing (edge to edge)." }
    , { "setEnclosure"        , (PyCFunction)PyLayer_setEnclosure        , METH_VARARGS
                              , "Sets the enclosure for the given BasicLayer sub-component." }
    , { "setExtentionCap"     , (PyCFunction)PyLayer_setExtentionCap     , METH_VARARGS
                              , "Sets the extention cap for the given BasiLayer sub-component." }
    , { "setExtentionWidth"   , (PyCFunction)PyLayer_setExtentionWidth   , METH_VARARGS
                              , "Sets the extention width for the given BasiLayer sub-component." }
    , { "setMinimalArea"      , (PyCFunction)PyLayer_setMinimalArea      , METH_VARARGS
                              , "Sets the minimum area allowed for the layer." }
    , { "destroy"             , (PyCFunction)PyLayer_destroy             , METH_NOARGS
                              , "Destroy associated hurricane object The python object remains." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


}  // extern "C".

}  // Isobar3 namespace.
