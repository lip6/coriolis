
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2013, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyComponents.cpp"                       |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyLayer.h"
#include "hurricane/isobar/PyPoint.h"
#include "hurricane/isobar/PyBox.h"
#include "hurricane/isobar/PyBasicLayer.h"
#include "hurricane/isobar/PyHook.h"
#include "hurricane/isobar/PyComponent.h"
#include "hurricane/isobar/PyPlug.h"
#include "hurricane/isobar/PyHorizontal.h"
#include "hurricane/isobar/PyVertical.h"
#include "hurricane/isobar/PyContact.h"
#include "hurricane/isobar/PyPin.h"
#include "hurricane/isobar/PyComponentCollection.h"


namespace  Isobar {

using namespace Hurricane;


extern "C" {

#undef   ACCESS_OBJECT
#undef   ACCESS_CLASS
#define  ACCESS_OBJECT           _baseObject._object
#define  ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Component,component,function)


// +=================================================================+
// |             "PyComponent" Python Module Code Part               |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  // Standart Accessors (Attributes).
  DirectGetLongAttribute(PyComponent_getX,getX,PyComponent,Component)
  DirectGetLongAttribute(PyComponent_getY,getY,PyComponent,Component)
  accessorHook(getBodyHook,PyComponent,Component)

  // Standart Destroy (Attribute).
  DBoDestroyAttribute(PyComponent_destroy,PyComponent)
  

  static PyObject* PyComponent_getPosition ( PyComponent *self )
  {
    trace << "PyComponent_getPosition ()" << endl;
    METHOD_HEAD ( "Component.getPosition()" )

    PyPoint* pyPoint = PyObject_NEW ( PyPoint, &PyTypePoint );
    if (pyPoint == NULL) { return NULL; }
    
    HTRY
    pyPoint->_object = new Point ( component->getPosition() );
    HCATCH    

    return ( (PyObject*)pyPoint );
  }


  static PyObject* PyComponent_getNet ( PyComponent *self )
  {
    trace << "PyComponent_getNet ()" << endl;
    
    Net* net = NULL;

    HTRY
    METHOD_HEAD ( "Component.getNet()" )

    net = component->getNet ( );
    HCATCH
    
    return PyNet_Link ( net );
  }
 

  static PyObject* PyComponent_getLayer ( PyComponent *self )
  {
    trace << "PyComponent_getLayer ()" << endl;
    METHOD_HEAD ( "Component.getLayer()" )

    Layer* layer = NULL;

    HTRY
    layer = const_cast<Layer*>(component->getLayer ());
    HCATCH

    return PyLayer_LinkDerived ( layer );
  }
  

  static PyObject* PyComponent_getCenter ( PyComponent *self )
  {
    trace << "PyComponent_getCenter ()" << endl;
    METHOD_HEAD( "Component.getCenter()" )

    PyPoint* pyPoint = PyObject_NEW( PyPoint, &PyTypePoint );
    if (pyPoint == NULL) return NULL;
    
    HTRY
    pyPoint->_object = new Point( component->getCenter() );
    HCATCH    

    return (PyObject*)pyPoint;
  }


  static PyObject* PyComponent_getBoundingBox ( PyComponent *self, PyObject* args )
  {
    trace << "PyComponent_getBoundingBox ()" << endl;
    METHOD_HEAD ( "Component.getBoundingBox()" )

    PyBox* pyBox = PyObject_NEW ( PyBox, &PyTypeBox );
    if (pyBox == NULL) return NULL;

    HTRY
    PyObject* pyLayer = NULL;
    if (PyArg_ParseTuple( args, "|O:Component.getBoundingBox", &pyLayer )) {
      if (pyLayer) {
        if (not IsPyBasicLayer(pyLayer)) {
          PyErr_SetString( ConstructorError, "Component.getBoundingBox(): First argument is not of type BasicLayer." );
          return NULL;
        }
        pyBox->_object = new Box ( component->getBoundingBox(PYBASICLAYER_O(pyLayer)) );
      } else {
        pyBox->_object = new Box ( component->getBoundingBox() );
      }
    } else {
      PyErr_SetString( ConstructorError, "Bad parameters given to Component.getBoundingBox()." );
      return NULL;
    }
    HCATCH

    return (PyObject*)pyBox;
  }


  static PyObject* PyComponent_getConnexComponents ( PyComponent *self )
  {
    trace << "PyComponent_getConnexComponents()" << endl;
    METHOD_HEAD( "PyComponent.getConnexComponents()" )

    PyComponentCollection* pyComponentCollection = NULL;

    HTRY
    Components* components = new Components(component->getConnexComponents());

    pyComponentCollection = PyObject_NEW( PyComponentCollection, &PyTypeComponentCollection );
    if (pyComponentCollection == NULL) { 
      return NULL;
    }

    pyComponentCollection->_object = components;
    HCATCH
    
    return (PyObject*)pyComponentCollection;
  }


  static PyObject* PyComponent_getSlaveComponents ( PyComponent *self )
  {
    trace << "PyComponent_getSlaveComponents()" << endl;
    METHOD_HEAD( "PyComponent.getSlaveComponents()" )

    PyComponentCollection* pyComponentCollection = NULL;

    HTRY
    Components* components = new Components(component->getSlaveComponents());

    pyComponentCollection = PyObject_NEW( PyComponentCollection, &PyTypeComponentCollection );
    if (pyComponentCollection == NULL) { 
      return NULL;
    }

    pyComponentCollection->_object = components;
    HCATCH
    
    return (PyObject*)pyComponentCollection;
  }


  PyMethodDef PyComponent_Methods[] =
    { { "getBodyHook"          , (PyCFunction)PyComponent_getBodyHook        , METH_NOARGS , "Return the component body hook (is a master Hook)." }
    , { "getX"                 , (PyCFunction)PyComponent_getX               , METH_NOARGS , "Return the Component X value." }
    , { "getY"                 , (PyCFunction)PyComponent_getY               , METH_NOARGS , "Return the Component Y value." }
    , { "getPosition"          , (PyCFunction)PyComponent_getPosition        , METH_NOARGS , "Return the Component position." }
    , { "getCenter"            , (PyCFunction)PyComponent_getCenter          , METH_NOARGS , "Return the Component center position." }
    , { "getNet"               , (PyCFunction)PyComponent_getNet             , METH_NOARGS , "Returns the net owning the component." }
    , { "getLayer"             , (PyCFunction)PyComponent_getLayer           , METH_NOARGS , "Return the component layer." }
    , { "getBoundingBox"       , (PyCFunction)PyComponent_getBoundingBox     , METH_VARARGS, "Return the component boundingBox (optionally on a BasicLayer)." }
    , { "getConnexComponents"  , (PyCFunction)PyComponent_getConnexComponents, METH_NOARGS, "All the components connecteds to this one through hyper hooks." }
    , { "getSlaveComponents"   , (PyCFunction)PyComponent_getSlaveComponents , METH_NOARGS, "All the components anchored directly or indirectly on this one." }
    , { "destroy"              , (PyCFunction)PyComponent_destroy            , METH_NOARGS
                               , "destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}    /* sentinel */
    };


  DBoDeleteMethod(Component)
  PyTypeObjectLinkPyType(Component)


#else  // End of Python Module Code Part.


// +=================================================================+
// |            "PyComponent" Shared Library Code Part               |
// +=================================================================+


  PyTypeInheritedObjectDefinitions(Component, Entity)

#endif  // End of Shared Library Code Part.


}  // extern "C".

}  // Isobar namespace.
