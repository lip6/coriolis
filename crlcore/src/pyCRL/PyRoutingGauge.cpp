// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2012-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@lip6.fr                   |
// | =============================================================== |
// |  C++ Module  :  "./PyRoutingGauge.cpp"                          |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyLayer.h"
#include "hurricane/isobar/PyTechnology.h"
#include "crlcore/PyRoutingLayerGauge.h"
#include "crlcore/PyRoutingGauge.h"
#include <string>
#include <sstream>


namespace  CRL {

  using std::cerr;
  using std::endl;
  using std::hex;
  using std::string;
  using std::ostringstream;
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
  using Isobar::PyAny_AsLong;
  using Isobar::getPyHash;
  using Isobar::ParseOneArg;
  using Isobar::ParseTwoArg;
  using Isobar::__cs;
  using Isobar::Converter;
  using Isobar::PyLayer;
  using Isobar::PyTypeLayer;
  using Isobar::PyLayer_LinkDerived;
  using Isobar::PyTechnology;
  using Isobar::PyTypeTechnology;
  using Isobar::PyTechnology_Link;


extern "C" {


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(RoutingGauge,rg,function)


#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |            "PyRoutingGauge" Python Module Code Part             |
// +=================================================================+


  static PyObject* PyRoutingGauge_create ( PyObject*, PyObject* args )
  {
    cdebug_log(30,0) << "PyRoutingGauge_create()" << endl;

    RoutingGauge*   rg   = NULL;
    PyRoutingGauge* pyRg = NULL;
    
    HTRY
    char* name = NULL;
    if (PyArg_ParseTuple(args,"s:RoutingGauge.create", &name)) {
      rg = RoutingGauge::create(name);
    } else {
      PyErr_SetString ( ConstructorError, "Bad parameters given to RoutingGauge.create()." );
      return NULL;
    }

    pyRg = PyObject_NEW ( PyRoutingGauge, &PyTypeRoutingGauge );
    if ( pyRg == NULL ) {
      rg->destroy();
      return NULL;
    }

    pyRg->_object = rg;
    HCATCH

    return (PyObject*)pyRg;
  }


  static PyObject* PyRoutingGauge_getTechnology ( PyRoutingGauge* self )
  {
    cdebug_log(30,0) << "PyRoutingGauge_getTechnology()" << endl;

    Technology* technology = NULL;

    HTRY
    METHOD_HEAD("RoutingGauge.getTechnology()")
    technology = rg->getTechnology();
    HCATCH

    return PyTechnology_Link(technology);
  }


  static PyObject* PyRoutingGauge_getDepth ( PyRoutingGauge* self )
  {
    cdebug_log(30,0) << "PyRoutingGauge_getDepth()" << endl;

    size_t depth = 0;

    HTRY
    METHOD_HEAD("RoutingGauge.getDepth()")
    depth = rg->getDepth();
    HCATCH

    return Py_BuildValue("I",depth);
  }


  static PyObject* PyRoutingGauge_getHorizontalPitch ( PyRoutingGauge* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyRoutingGauge_getHorizontalPitch()" << endl;

    DbU::Unit pitch = 0;
    HTRY
      METHOD_HEAD("RoutingGauge.getHorizontalPitch()")
      pitch = rg->getHorizontalPitch();
    HCATCH

    return Py_BuildValue("I",pitch);
  }


  static PyObject* PyRoutingGauge_getVerticalPitch ( PyRoutingGauge* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyRoutingGauge_getVerticalPitch()" << endl;

    DbU::Unit pitch = 0;
    HTRY
      METHOD_HEAD("RoutingGauge.getVerticalPitch()")
      pitch = rg->getVerticalPitch();
    HCATCH

    return Py_BuildValue("I",pitch);
  }


  static PyObject* PyRoutingGauge_getLayerDepth ( PyRoutingGauge* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyRoutingGauge_getLayerDepth()" << endl;

    size_t depth = 0;

    HTRY
    METHOD_HEAD("RoutingGauge.getLayerDepth()")

    PyObject* pyLayer = NULL;
    
    if (PyArg_ParseTuple( args, "O:RoutingGauge.getLayerDepth", &pyLayer)) {
      if ( not PyObject_IsInstance(pyLayer,(PyObject*)&PyTypeLayer) ) {
        PyErr_SetString ( ConstructorError, "Bad type for layer argument of RoutingGauge.getLayerDepth()." );
        return NULL;
      }
      depth = rg->getLayerDepth( PYLAYER_O(pyLayer) );
    } else {
      PyErr_SetString ( ConstructorError, "Bad parameters given to RoutingGauge.getLayerDepth()." );
      return NULL;
    }
    HCATCH

    return Py_BuildValue("I",depth);
  }


  static PyObject* PyRoutingGauge_getPitch ( PyRoutingGauge* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyRoutingGauge_getPitch()" << endl;

    DbU::Unit pitch = 0;
    HTRY
      METHOD_HEAD("RoutingGauge.getPitch()")
      PyObject* pyLayer = NULL;
      
      if (PyArg_ParseTuple( args, "O:RoutingGauge.getPitch", &pyLayer)) {
        if ( not PyObject_IsInstance(pyLayer,(PyObject*)&PyTypeLayer) ) {
          PyErr_SetString ( ConstructorError, "Bad type for layer argument of RoutingGauge.getLayerPitch()." );
          return NULL;
        }
        pitch = rg->getPitch( PYLAYER_O(pyLayer) );
      } else {
        PyErr_SetString ( ConstructorError, "Bad parameters given to RoutingGauge.getPitch()." );
        return NULL;
      }
    HCATCH
    return Py_BuildValue("I",pitch);
  }


  static PyObject* PyRoutingGauge_getOffset ( PyRoutingGauge* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyRoutingGauge_getOffset()" << endl;

    DbU::Unit offset = 0;
    HTRY
      METHOD_HEAD("RoutingGauge.getOffset()")
      PyObject* pyLayer = NULL;
      
      if (PyArg_ParseTuple( args, "O:RoutingGauge.getOffset", &pyLayer)) {
        if ( not PyObject_IsInstance(pyLayer,(PyObject*)&PyTypeLayer) ) {
          PyErr_SetString ( ConstructorError, "Bad type for layer argument of RoutingGauge.getOffset()." );
          return NULL;
        }
        offset = rg->getOffset( PYLAYER_O(pyLayer) );
      } else {
        PyErr_SetString ( ConstructorError, "Bad parameters given to RoutingGauge.getOffset()." );
        return NULL;
      }
    HCATCH
    return Py_BuildValue("I",offset);
  }


  static PyObject* PyRoutingGauge_getWireWidth ( PyRoutingGauge* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyRoutingGauge_getWireWidth()" << endl;

    DbU::Unit wireWidth = 0;
    HTRY
      METHOD_HEAD("RoutingGauge.getWireWidth()")
      PyObject* pyLayer = NULL;
      
      if (PyArg_ParseTuple( args, "O:RoutingGauge.getWireWidth", &pyLayer)) {
        if ( not PyObject_IsInstance(pyLayer,(PyObject*)&PyTypeLayer) ) {
          PyErr_SetString ( ConstructorError, "Bad type for layer argument of RoutingGauge.getWireWidth()." );
          return NULL;
        }
        wireWidth = rg->getWireWidth( PYLAYER_O(pyLayer) );
      } else {
        PyErr_SetString ( ConstructorError, "Bad parameters given to RoutingGauge.getWireWidth()." );
        return NULL;
      }
    HCATCH
    return Py_BuildValue("I",wireWidth);
  }


  static PyObject* PyRoutingGauge_getPWireWidth ( PyRoutingGauge* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyRoutingGauge_getPWireWidth()" << endl;

    DbU::Unit wireWidth = 0;
    HTRY
      METHOD_HEAD("RoutingGauge.getPWireWidth()")
      PyObject* pyLayer = NULL;
      
      if (PyArg_ParseTuple( args, "O:RoutingGauge.getPWireWidth", &pyLayer)) {
        if ( not PyObject_IsInstance(pyLayer,(PyObject*)&PyTypeLayer) ) {
          PyErr_SetString ( ConstructorError, "Bad type for layer argument of RoutingGauge.getPWireWidth()." );
          return NULL;
        }
        wireWidth = rg->getPWireWidth( PYLAYER_O(pyLayer) );
      } else {
        PyErr_SetString ( ConstructorError, "Bad parameters given to RoutingGauge.getPWireWidth()." );
        return NULL;
      }
    HCATCH
    return Py_BuildValue("I",wireWidth);
  }


  static PyObject* PyRoutingGauge_getViaWidth ( PyRoutingGauge* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyRoutingGauge_getViaWidth()" << endl;

    DbU::Unit pitch = 0;
    HTRY
      METHOD_HEAD("RoutingGauge.getViaWidth()")
      PyObject* pyLayer = NULL;
      
      if (PyArg_ParseTuple( args, "O:RoutingGauge.getViaWidth", &pyLayer)) {
        if ( not PyObject_IsInstance(pyLayer,(PyObject*)&PyTypeLayer) ) {
          PyErr_SetString ( ConstructorError, "Bad type for layer argument of RoutingGauge.getViaWidth()." );
          return NULL;
        }
        pitch = rg->getViaWidth( PYLAYER_O(pyLayer) );
      } else {
        PyErr_SetString ( ConstructorError, "Bad parameters given to RoutingGauge.getViaWidth()." );
        return NULL;
      }
    HCATCH
    return Py_BuildValue("I",pitch);
  }


  static PyObject* PyRoutingGauge_getLayerGauge ( PyRoutingGauge* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyRoutingGauge_getLayerGauge()" << endl;

    RoutingLayerGauge* rlg = NULL;

    HTRY
    METHOD_HEAD("RoutingGauge.getLayerGauge()")

    PyObject* arg0 = NULL;
    
    __cs.init ("RoutingGauge.getLayerGauge");
    if (PyArg_ParseTuple( args, "O&:RoutingGauge.getLayerGauge", Converter, &arg0)) {
      string layerName;
      if (__cs.getObjectIds() == ":layer") {
        rlg = rg->getLayerGauge( PYLAYER_O(arg0) );
        layerName = "\"" + getString(PYLAYER_O(arg0)) + "\"";
      } else if (__cs.getObjectIds() == ":int") {
        rlg = rg->getLayerGauge( (size_t)PyAny_AsLong(arg0) );
        layerName = "depth=" + getString(PyAny_AsLong(arg0));
      } else {
        PyErr_SetString ( ConstructorError, "invalid parameter type for RoutingGauge.getLayerGauge()." );
        return NULL;
      }
      if ( rlg == NULL ) {
        string message = "RoutingGauge.getLayerDepth(), requested Layer "
                       + layerName + " has no RoutingLayerGauge.";
        PyErr_SetString ( ConstructorError, message.c_str() );
        return NULL;
      }
    } else {
      PyErr_SetString ( ConstructorError, "Invalid number of parameters passed to RoutingGauge.getLayerDepth()." );
      return NULL;
    }
    HCATCH

    return PyRoutingLayerGauge_Link(rlg);
  }


  static PyObject* PyRoutingGauge_getLayerDirection ( PyRoutingGauge* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyRoutingGauge_getLayerDirection()" << endl;

    unsigned int direction = 0;

    HTRY
    METHOD_HEAD("RoutingGauge.getLayerDirection()")

    PyObject* arg0 = NULL;
    
    __cs.init ("RoutingGauge.getLayerDirection");
    if (PyArg_ParseTuple( args, "O&:RoutingGauge.getLayerDirection", Converter, &arg0)) {
      if ( __cs.getObjectIds() == ":layer" )
        direction = rg->getLayerDirection( PYLAYER_O(arg0) );
      else if  ( __cs.getObjectIds() == ":int" )
        direction = rg->getLayerDirection( (size_t)PyAny_AsLong(arg0) );
      else {
        PyErr_SetString ( ConstructorError, "invalid parameter type for RoutingGauge.getLayerDirection()." );
        return NULL;
      }
    } else {
      PyErr_SetString ( ConstructorError, "Invalid number of parameters passed to RoutingGauge.getLayerDirection()." );
      return NULL;
    }
    HCATCH

    return Py_BuildValue("I",direction);
  }


  static PyObject* PyRoutingGauge_getLayerPitch ( PyRoutingGauge* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyRoutingGauge_getLayerPitch()" << endl;

    DbU::Unit pitch = 0;

    HTRY
    METHOD_HEAD("RoutingGauge.getLayerPitch()")

    PyObject* arg0 = NULL;
    
    __cs.init ("RoutingGauge.getLayerPitch");
    if (PyArg_ParseTuple( args, "O&:RoutingGauge.getLayerPitch", Converter, &arg0)) {
      if ( __cs.getObjectIds() == ":layer" ) {
      //pitch = rg->getLayerPitch( PYLAYER_O(arg0) );
      } else if  ( __cs.getObjectIds() == ":int" )
        pitch = rg->getLayerPitch( (size_t)PyAny_AsLong(arg0) );
      else {
        PyErr_SetString ( ConstructorError, "invalid parameter type for RoutingGauge.getLayerPitch()." );
        return NULL;
      }
    } else {
      PyErr_SetString ( ConstructorError, "Invalid number of parameters passed to RoutingGauge.getLayerPitch()." );
      return NULL;
    }
    HCATCH

    return Py_BuildValue("I",pitch);
  }


  static PyObject* PyRoutingGauge_getPowerSupplyGauge ( PyRoutingGauge* self )
  {
    cdebug_log(30,0) << "PyRoutingGauge_getPowerSupplyGauge()" << endl;
    RoutingLayerGauge* rlg = NULL;
    HTRY
      METHOD_HEAD("RoutingGauge.getPowerSupplyGauge()")
      rlg = rg->getPowerSupplyGauge();
      if (not rlg) Py_RETURN_NONE;
    HCATCH
    return PyRoutingLayerGauge_Link( rlg );
  }


  static PyObject* PyRoutingGauge_getRoutingLayer ( PyRoutingGauge* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyRoutingGauge_getRoutingLayer()" << endl;

    Layer* layer = NULL;

    HTRY
    METHOD_HEAD("RoutingGauge.getRoutingLayer()")

    unsigned int depth = 0;
    
    if (PyArg_ParseTuple( args, "I:RoutingGauge.getRoutingLayer", &depth)) {
      layer = const_cast<Layer*>(rg->getRoutingLayer( (size_t)depth ));
      if ( layer == NULL ) {
        string message
          = "RoutingGauge.getRoutingLayer(): No layer at the requested depth " + getString(depth)
          + " (must be < " + getString(rg->getDepth()) + ").";
        PyErr_SetString ( ConstructorError, message.c_str() );
        return NULL;
      }
    } else {
      PyErr_SetString ( ConstructorError, "Bad parameters given to RoutingGauge.getRoutingLayer()." );
      return NULL;
    }
    HCATCH

    return PyLayer_LinkDerived(layer);
  }


  static PyObject* PyRoutingGauge_getContactLayer ( PyRoutingGauge* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyRoutingGauge_getContactLayer()" << endl;

    Layer* layer = NULL;

    HTRY
    METHOD_HEAD("RoutingGauge.getContactLayer()")

    unsigned int depth = 0;
    
    if (PyArg_ParseTuple( args, "I:RoutingGauge.getContactLayer", &depth)) {
      layer = rg->getContactLayer( (size_t)depth );
      if ( layer == NULL ) {
        string message
          = "RoutingGauge.getContactLayer(): No layer at the requested depth " + getString(depth)
          + " (must be < " + getString(rg->getDepth()-1) + ").";
        PyErr_SetString ( ConstructorError, message.c_str() );
        return NULL;
      }
    } else {
      PyErr_SetString ( ConstructorError, "Bad parameters given to RoutingGauge.getContactLayer()." );
      return NULL;
    }
    HCATCH

    return PyLayer_LinkDerived(layer);
  }


  PyObject* PyRoutingGauge_addLayerGauge ( PyRoutingGauge* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyRoutingGauge_addLayerGauge()" << endl;

    HTRY
    METHOD_HEAD("RoutingGauge.addLayerGauge()")
    PyObject* pyRlg = NULL;

    if (PyArg_ParseTuple( args, "O:RoutingGauge.addLayerGauge", &pyRlg)) {
      if ( not IsPyRoutingLayerGauge(pyRlg) ) {
        PyErr_SetString ( ConstructorError, "Argument of RoutingGauge.addRoutingGauge() is not of RoutingLayerGauge type." );
        return NULL;
      }

      rg->addLayerGauge( PYROUTINGLAYERGAUGE_O(pyRlg) );
    } else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for RoutingGauge.addLayerGauge." );
      return NULL;
    }
    HCATCH

    Py_RETURN_NONE;
  }


  // Standard Attributes.
  GetNameMethod(RoutingGauge,rg)
  accessorVectorFromVoid(getLayerGauges,PyRoutingGauge,RoutingGauge,RoutingLayerGauge)
  DirectGetBoolAttribute(PyRoutingGauge_isSymbolic    ,isSymbolic    ,PyRoutingGauge,RoutingGauge)
  DirectGetBoolAttribute(PyRoutingGauge_isSuperPitched,isSuperPitched,PyRoutingGauge,RoutingGauge)
  DirectSetBoolAttribute(PyRoutingGauge_setSymbolic   ,setSymbolic   ,PyRoutingGauge,RoutingGauge)
  DirectGetBoolAttribute(PyRoutingGauge_isHV          ,isHV          ,PyRoutingGauge,RoutingGauge)
  DirectGetBoolAttribute(PyRoutingGauge_isVH          ,isVH          ,PyRoutingGauge,RoutingGauge)
  DirectGetBoolAttribute(PyRoutingGauge_hasPowerSupply,hasPowerSupply,PyRoutingGauge,RoutingGauge)


  // Standart Destroy (Attribute).


  PyMethodDef PyRoutingGauge_Methods[] =
    { { "create"                , (PyCFunction)PyRoutingGauge_create            , METH_VARARGS|METH_STATIC
                                , "Create a new RoutingGauge." }
    , { "isSymbolic"            , (PyCFunction)PyRoutingGauge_isSymbolic        , METH_NOARGS
                                , "The RoutingGauge is for symbolic technology." }
    , { "isSuperPitched"        , (PyCFunction)PyRoutingGauge_isSuperPitched    , METH_NOARGS
                                , "The RoutingGauge is super-pitched." }
    , { "isHV"                  , (PyCFunction)PyRoutingGauge_isHV              , METH_NOARGS
                                , "The first routing layer (metal2) is horizontal." }
    , { "isVH"                  , (PyCFunction)PyRoutingGauge_isVH              , METH_NOARGS
                                , "The first routing layer (metal2) is vertical." }
    , { "hasPowerSupply"        , (PyCFunction)PyRoutingGauge_hasPowerSupply    , METH_NOARGS
                                , "Is there a dedicated layer for power supplies." }
    , { "getName"               , (PyCFunction)PyRoutingGauge_getName           , METH_NOARGS
                                , "Return the maximum depth of the RoutingGauge." }
    , { "getTechnology"         , (PyCFunction)PyRoutingGauge_getTechnology     , METH_NOARGS
                                , "Return the Technology we are using." }
    , { "getDepth"              , (PyCFunction)PyRoutingGauge_getDepth          , METH_NOARGS
                                , "Return the maximum depth of the RoutingGauge." }
    , { "getHorizontalPitch"    , (PyCFunction)PyRoutingGauge_getHorizontalPitch, METH_NOARGS
                                , "Return the horizontal pitch of the metal closest to the substrate." }
    , { "getVerticalPitch"      , (PyCFunction)PyRoutingGauge_getVerticalPitch  , METH_NOARGS
                                , "Return the vertical pitch of the metal closest to the substrate." }
    , { "getLayerDepth"         , (PyCFunction)PyRoutingGauge_getLayerDepth     , METH_VARARGS
                                , "Return the depth of the given layer." }
    , { "getPitch"              , (PyCFunction)PyRoutingGauge_getPitch          , METH_VARARGS
                                , "Return the routing pitch of the given layer." }
    , { "getOffset"             , (PyCFunction)PyRoutingGauge_getOffset         , METH_VARARGS
                                , "Return the offset of the first track of the given layer." }
    , { "getWireWidth"          , (PyCFunction)PyRoutingGauge_getWireWidth      , METH_VARARGS
                                , "Return the default wire width of the given layer." }
    , { "getViaWidth"           , (PyCFunction)PyRoutingGauge_getViaWidth       , METH_VARARGS
                                , "Return the default via width of the given layer." }
    , { "getPWireWidth"         , (PyCFunction)PyRoutingGauge_getPWireWidth     , METH_VARARGS
                                , "Return the default perpandicular wire width of the given layer." }
    , { "getPowerSupplyGauge"   , (PyCFunction)PyRoutingGauge_getPowerSupplyGauge, METH_NOARGS
                                , "Return the power supply gauge (None if there isn't)." }
    , { "getLayerGauge"         , (PyCFunction)PyRoutingGauge_getLayerGauge     , METH_VARARGS
                                , "Return the RoutingLayerGauge of the given layer/depth." }
    , { "getLayerDirection"     , (PyCFunction)PyRoutingGauge_getLayerDirection , METH_VARARGS
                                , "Return the direction of the given layer/depth." }
    , { "getLayerPitch"         , (PyCFunction)PyRoutingGauge_getLayerPitch     , METH_VARARGS
                                , "Return the pitch of the given layer/depth." }
    , { "getRoutingLayer"       , (PyCFunction)PyRoutingGauge_getRoutingLayer   , METH_VARARGS
                                , "Return the routing layer used for the requested depth." }
    , { "getContactLayer"       , (PyCFunction)PyRoutingGauge_getContactLayer   , METH_VARARGS
                                , "Return the contact layer used for the requested depth." }
    , { "getLayerGauges"        , (PyCFunction)PyRoutingGauge_getLayerGauges    , METH_NOARGS
                                , "Return the list of RoutingLayerGauge." }
    , { "addLayerGauge"         , (PyCFunction)PyRoutingGauge_addLayerGauge     , METH_VARARGS
                                , "Adds a new RoutingLayerGauge to the RoutingGauge." }
    , { "setSymbolic"           , (PyCFunction)PyRoutingGauge_setSymbolic       , METH_VARARGS
                                , "Set the symbolic technology state." }
  //, { "destroy"               , (PyCFunction)PyRoutingGauge_destroy          , METH_VARARGS
  //                            , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL}     /* sentinel */
    };


  PythonOnlyDeleteMethod(RoutingGauge)
  PyTypeObjectLinkPyType(RoutingGauge)

  
  IteratorNextMethod(RoutingLayerGauge)
  VectorMethods     (RoutingLayerGauge)


  extern  void  PyRoutingGauge_postModuleInit ()
  {
    PyObject* constant;

    LoadObjectConstant(PyTypeRoutingGauge.tp_dict,RoutingGauge::nlayerdepth,"nlayerdepth");
  }


#else  // End of Python Module Code Part.

// +=================================================================+
// |             "PyRoutingGauge" Shared Library Code Part           |
// +=================================================================+

  // Type Definition.
  PyTypeObjectDefinitionsOfModule(CRL,RoutingGauge)

  // Link/Creation Method.
  LinkCreateMethod(RoutingGauge)

  PyTypeVectorObjectDefinitions(RoutingLayerGaugeVector)
  PyTypeVectorObjectDefinitions(RoutingLayerGaugeVectorIterator)


#endif  // End of Shared Library Code Part.


}  // extern "C".


}  // CRL namespace.
