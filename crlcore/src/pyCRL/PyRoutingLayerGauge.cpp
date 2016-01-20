// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2012-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyRoutingLayerGauge.cpp"                     |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyLayer.h"
#include "crlcore/PyRoutingLayerGauge.h"
#include <string>
#include <sstream>


namespace  CRL {

  using std::cerr;
  using std::endl;
  using std::hex;
  using std::string;
  using std::ostringstream;
  using Hurricane::tab;
  using Hurricane::in_trace;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Isobar::ProxyProperty;
  using Isobar::ProxyError;
  using Isobar::ConstructorError;
  using Isobar::HurricaneError;
  using Isobar::HurricaneWarning;
  using Isobar::ParseOneArg;
  using Isobar::ParseTwoArg;
  using Isobar::__cs;
  using Isobar::PyLayer;
  using Isobar::PyTypeLayer;
  using Isobar::PyLayer_LinkDerived;


extern "C" {


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(RoutingLayerGauge,rlg,function)


#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |         "PyRoutingLayerGauge" Python Module Code Part           |
// +=================================================================+


  static PyObject* PyRoutingLayerGauge_create ( PyObject*, PyObject* args )
  {
    trace << "PyRoutingLayerGauge_create()" << endl;

    RoutingLayerGauge*   rlg   = NULL;
    PyRoutingLayerGauge* pyRlg = NULL;
    
    HTRY
    PyObject* pyLayer = NULL;
    int       direction;
    int       type;
    int       depth;
    double    density;
    long      offset;
    long      pitch;
    long      wireWidth;
    long      viaWidth;
    
    if (PyArg_ParseTuple( args
                        , "OIIIdllll:RoutingLayerGauge.create"
                        , &pyLayer
                        , &direction
                        , &type
                        , &depth
                        , &density
                        , &offset
                        , &pitch
                        , &wireWidth
                        , &viaWidth
                        )) {
      if ( not PyObject_IsInstance(pyLayer,(PyObject*)&PyTypeLayer) ) {
        PyErr_SetString ( ConstructorError, "Bad type for layer argument of RoutingLayerGauge.create()." );
        return NULL;
      }
      switch( direction ) {
        case Constant::Horizontal:
        case Constant::Vertical: break;
        default:
          PyErr_SetString ( ConstructorError, "Bad value for direction argument of RoutingLayerGauge.create()." );
          return NULL;
      }
      switch( type ) {
        case Constant::Default:
        case Constant::PinOnly: break;
        default:
          PyErr_SetString ( ConstructorError, "Bad value for type argument of RoutingLayerGauge.create()." );
          return NULL;
      }

      rlg = RoutingLayerGauge::create( PYLAYER_O(pyLayer)
                                     , direction
                                     , type
                                     , depth
                                     , density
                                     , offset
                                     , pitch
                                     , wireWidth
                                     , viaWidth
                                     );
    } else {
      PyErr_SetString ( ConstructorError, "Bad parameters given to RoutingLayerGauge.create()." );
      return NULL;
    }

    pyRlg = PyObject_NEW ( PyRoutingLayerGauge, &PyTypeRoutingLayerGauge );
    if ( pyRlg == NULL ) {
      rlg->destroy();
      return NULL;
    }

    pyRlg->_object = rlg;
    HCATCH

    return (PyObject*)pyRlg;
  }


  static PyObject* PyRoutingLayerGauge_getLayer ( PyRoutingLayerGauge* self )
  {
    trace << "PyRoutingLayerGauge_getLayer()" << endl;

    Layer* layer = NULL;

    HTRY
    METHOD_HEAD("RoutingLayerGauge.getLayer()")
    layer = const_cast<Layer*>(rlg->getLayer());
    HCATCH

    return PyLayer_LinkDerived(layer);
  }


  static PyObject* PyRoutingLayerGauge_getBlockageLayer ( PyRoutingLayerGauge* self )
  {
    trace << "PyRoutingLayerGauge_getBlockageLayer()" << endl;

    Layer* layer = NULL;

    HTRY
    METHOD_HEAD("RoutingLayerGauge.getBlockageLayer()")
    layer = const_cast<Layer*>(rlg->getBlockageLayer());
    HCATCH

    return PyLayer_LinkDerived(layer);
  }


  static PyObject* PyRoutingLayerGauge_getTrackNumber ( PyRoutingLayerGauge* self, PyObject* args )
  {
    trace << "PyRoutingLayerGauge_getTrackNumber()" << endl;

    unsigned int  trackNumber = 0;

    HTRY
    METHOD_HEAD("RoutingLayerGauge.getTrackNumber()")

    long  start = 0;
    long  stop  = 0;
    
    if (PyArg_ParseTuple( args, "ll:RoutingLayerGauge.getTrackNumber", &start, &stop)) {
      trackNumber = rlg->getTrackNumber( (DbU::Unit)start, (DbU::Unit)stop );
    } else {
      PyErr_SetString ( ConstructorError, "Bad parameters given to RoutingLayerGauge.getTrackNumber()." );
      return NULL;
    }
    HCATCH

    return PyLong_FromLong(trackNumber);
  }


  static PyObject* PyRoutingLayerGauge_getTrackIndex ( PyRoutingLayerGauge* self, PyObject* args )
  {
    trace << "PyRoutingLayerGauge_getTrackIndex()" << endl;

    unsigned int  trackIndex = 0;

    HTRY
    METHOD_HEAD("RoutingLayerGauge.getTrackIndex()")

    long  start    = 0;
    long  stop     = 0;
    long  position = 0;
    long  mode     = 0;
    
    if (PyArg_ParseTuple( args, "lllI:RoutingLayerGauge.getTrackIndex", &start, &stop, &position, &mode)) {
      switch(mode) {
        case Constant::Superior:
        case Constant::Inferior:
        case Constant::Nearest:
        case Constant::Exact: break;
        default:
          PyErr_SetString ( ConstructorError, "RoutingLayerGauge.getTrackIndex(): The <mode> parameter has an invalid value ." );
          return NULL;
      }
      trackIndex = rlg->getTrackIndex( (DbU::Unit)start
                                     , (DbU::Unit)stop
                                     , (DbU::Unit)position
                                     , (unsigned int)mode );
    } else {
      PyErr_SetString ( ConstructorError, "Bad parameters given to RoutingLayerGauge.getTrackIndex()." );
      return NULL;
    }
    HCATCH

    return Py_BuildValue("I",trackIndex);
  }


  static PyObject* PyRoutingLayerGauge_getTrackPosition ( PyRoutingLayerGauge* self, PyObject* args )
  {
    trace << "PyRoutingLayerGauge_getTrackPosition()" << endl;

    DbU::Unit  trackPosition = 0;

    HTRY
    METHOD_HEAD("RoutingLayerGauge.getTrackPosition()")

    long          start = 0;
    unsigned int  depth = 0;
    
    if (PyArg_ParseTuple( args, "lI:RoutingLayerGauge.getTrackIndex", &start, &depth)) {
      trackPosition = rlg->getTrackNumber( (DbU::Unit)start , depth);
    } else {
      PyErr_SetString ( ConstructorError, "Bad parameters given to RoutingLayerGauge.getTrackPosition()." );
      return NULL;
    }
    HCATCH

    return PyLong_FromLong((long)trackPosition);
  }


  DirectGetUIntAttribute  (PyRoutingLayerGauge_getDepth        ,getDepth        ,PyRoutingLayerGauge,RoutingLayerGauge)
  DirectGetUIntAttribute  (PyRoutingLayerGauge_getDirection    ,getDirection    ,PyRoutingLayerGauge,RoutingLayerGauge)
  DirectGetUIntAttribute  (PyRoutingLayerGauge_getType         ,getType         ,PyRoutingLayerGauge,RoutingLayerGauge)
  DirectGetDoubleAttribute(PyRoutingLayerGauge_getDensity      ,getDensity      ,PyRoutingLayerGauge,RoutingLayerGauge)
  DirectGetLongAttribute  (PyRoutingLayerGauge_getOffset       ,getOffset       ,PyRoutingLayerGauge,RoutingLayerGauge)
  DirectGetLongAttribute  (PyRoutingLayerGauge_getPitch        ,getPitch        ,PyRoutingLayerGauge,RoutingLayerGauge)
  DirectGetLongAttribute  (PyRoutingLayerGauge_getHalfPitch    ,getHalfPitch    ,PyRoutingLayerGauge,RoutingLayerGauge)
  DirectGetLongAttribute  (PyRoutingLayerGauge_getWireWidth    ,getWireWidth    ,PyRoutingLayerGauge,RoutingLayerGauge)
  DirectGetLongAttribute  (PyRoutingLayerGauge_getHalfWireWidth,getHalfWireWidth,PyRoutingLayerGauge,RoutingLayerGauge)
  DirectGetLongAttribute  (PyRoutingLayerGauge_getViaWidth     ,getViaWidth     ,PyRoutingLayerGauge,RoutingLayerGauge)
  DirectGetLongAttribute  (PyRoutingLayerGauge_getHalfViaWidth ,getHalfViaWidth ,PyRoutingLayerGauge,RoutingLayerGauge)


  // Standart Destroy (Attribute).


  PyMethodDef PyRoutingLayerGauge_Methods[] =
    { { "create"                , (PyCFunction)PyRoutingLayerGauge_create          , METH_VARARGS|METH_STATIC
                                , "Create a new RoutingLayerGauge." }
    , { "getLayer"              , (PyCFunction)PyRoutingLayerGauge_getLayer        , METH_NOARGS
                                , "Returns the associated routing layer." }
    , { "getBlockageLayer"      , (PyCFunction)PyRoutingLayerGauge_getBlockageLayer, METH_NOARGS
                                , "Returns the associated blockage layer." }
    , { "getDepth"              , (PyCFunction)PyRoutingLayerGauge_getDepth        , METH_NOARGS
                                , "Returns the layer's depth in the RoutingGauge." }
    , { "getDirection"          , (PyCFunction)PyRoutingLayerGauge_getDirection    , METH_NOARGS
                                , "Returns the layer's preferred routing direction." }
    , { "getType"               , (PyCFunction)PyRoutingLayerGauge_getType         , METH_NOARGS
                                , "Returns the layer's type (pin only or routing)." }
    , { "getDensity"            , (PyCFunction)PyRoutingLayerGauge_getDensity      , METH_NOARGS
                                , "Returns the routing track density." }
    , { "getOffset"             , (PyCFunction)PyRoutingLayerGauge_getOffset       , METH_NOARGS
                                , "Returns the routing track offset (from start position)." }
    , { "getPitch"              , (PyCFunction)PyRoutingLayerGauge_getPitch        , METH_NOARGS
                                , "Returns the track pitch (center to center)." }
    , { "getHalfPitch"          , (PyCFunction)PyRoutingLayerGauge_getHalfPitch    , METH_NOARGS
                                , "Returns the half track pitch (center to center)." }
    , { "getWireWidth"          , (PyCFunction)PyRoutingLayerGauge_getWireWidth    , METH_NOARGS
                                , "Returns the wire width." }
    , { "getHalfWireWidth"      , (PyCFunction)PyRoutingLayerGauge_getHalfWireWidth, METH_NOARGS
                                , "Returns the half wire width." }
    , { "getViaWidth"           , (PyCFunction)PyRoutingLayerGauge_getViaWidth     , METH_NOARGS
                                , "Returns the VIA width." }
    , { "getHalfViaWidth"       , (PyCFunction)PyRoutingLayerGauge_getHalfViaWidth , METH_NOARGS
                                , "Returns the half VIA width." }
    , { "getTrackNumber"        , (PyCFunction)PyRoutingLayerGauge_getTrackNumber  , METH_VARARGS
                                , "Compute the number of tracks included between <start> & <stop>." }
    , { "getTrackIndex"         , (PyCFunction)PyRoutingLayerGauge_getTrackIndex   , METH_VARARGS
                                , "Returns the index of track at the given position (with rounding)." }
    , { "getTrackPosition"      , (PyCFunction)PyRoutingLayerGauge_getTrackPosition, METH_VARARGS
                                , "Compute the position of track number <depth>." }
  //, { "destroy"               , (PyCFunction)PyRoutingLayerGauge_destroy         , METH_VARARGS
  //                            , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL}     /* sentinel */
    };


  PythonOnlyDeleteMethod(RoutingLayerGauge)
  PyTypeObjectLinkPyType(RoutingLayerGauge)


#else  // End of Python Module Code Part.

// +=================================================================+
// |          "PyRoutingLayerGauge" Shared Library Code Part         |
// +=================================================================+

  // Type Definition.
  PyTypeObjectDefinitionsOfModule(CRL,RoutingLayerGauge)

  // Link/Creation Method.
  LinkCreateMethod(RoutingLayerGauge)


  extern  void  PyRoutingLayerGauge_postModuleInit ()
  {
    PyObject* constant;

    LoadObjectConstant(PyTypeRoutingLayerGauge.tp_dict,Constant::Horizontal ,"Horizontal");
    LoadObjectConstant(PyTypeRoutingLayerGauge.tp_dict,Constant::Vertical   ,"Vertical"  );
    LoadObjectConstant(PyTypeRoutingLayerGauge.tp_dict,Constant::Default    ,"Default"   );
    LoadObjectConstant(PyTypeRoutingLayerGauge.tp_dict,Constant::PinOnly    ,"PinOnly"   );
    LoadObjectConstant(PyTypeRoutingLayerGauge.tp_dict,Constant::Superior   ,"Superior"  );
    LoadObjectConstant(PyTypeRoutingLayerGauge.tp_dict,Constant::Inferior   ,"Inferior"  );
    LoadObjectConstant(PyTypeRoutingLayerGauge.tp_dict,Constant::Nearest    ,"Nearest"   );
    LoadObjectConstant(PyTypeRoutingLayerGauge.tp_dict,Constant::Exact      ,"Exact"     );
  }


#endif  // End of Shared Library Code Part.


}  // extern "C".


}  // CRL namespace.
