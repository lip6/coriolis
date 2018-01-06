// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2013-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                   Jean-Paul Chaput               |
// |  E-mail      :           Jean-Paul.Chaput@lip6.fr               |
// | =============================================================== |
// |  C++ Module  :       "./isobar/PyRoutingPad.cpp"                |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyPoint.h"
#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyLayer.h"
#include "hurricane/isobar/PyPin.h"
#include "hurricane/isobar/PyOccurrence.h"
#include "hurricane/isobar/PyRoutingPad.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {

#undef   ACCESS_OBJECT
#undef   ACCESS_CLASS
#define  ACCESS_OBJECT            _baseObject._baseObject._object
#define  ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject._baseObject)
#define  METHOD_HEAD(function)    GENERIC_METHOD_HEAD(RoutingPad,rp,function)


// +=================================================================+
// |             "PyRoutingPad" Python Module Code Part              |
// +=================================================================+

# if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |             "PyRoutingPad" Attribute Methods                |
  // x-------------------------------------------------------------x


  DirectGetLongAttribute(PyRoutingPad_getX       ,getX       ,PyRoutingPad,RoutingPad)
  DirectGetLongAttribute(PyRoutingPad_getY       ,getY       ,PyRoutingPad,RoutingPad)
  DirectGetLongAttribute(PyRoutingPad_getSourceX ,getSourceX ,PyRoutingPad,RoutingPad)
  DirectGetLongAttribute(PyRoutingPad_getSourceY ,getSourceY ,PyRoutingPad,RoutingPad)
  DirectGetLongAttribute(PyRoutingPad_getTargetX ,getTargetX ,PyRoutingPad,RoutingPad)
  DirectGetLongAttribute(PyRoutingPad_getTargetY ,getTargetY ,PyRoutingPad,RoutingPad)
  //DirectSetLongAttribute(PyRoutingPad_setX       ,setX       ,PyRoutingPad,RoutingPad)
  //DirectSetLongAttribute(PyRoutingPad_setY       ,setY       ,PyRoutingPad,RoutingPad)

  // Standart destroy (Attribute).
  DBoDestroyAttribute(PyRoutingPad_destroy, PyRoutingPad)


  static PyObject* PyRoutingPad_create ( PyObject*, PyObject* args )
  {
    cdebug_log(20,0) << "PyRoutingPad_create()" << endl;

    PyObject*     arg0 = NULL;
    PyObject*     arg1 = NULL;
    PyObject*     arg2 = NULL;
    RoutingPad*   rp   = NULL;

    HTRY
    __cs.init ("RoutingPad.create");
    if (not PyArg_ParseTuple(args,"O&|O&O&:RoutingPad.create"
                            ,Converter,&arg0
                            ,Converter,&arg1
                            ,Converter,&arg2
                            )) {
      PyErr_SetString( ConstructorError, "RoutingPad.create(): Invalid number/bad type of parameters." );
      return NULL;
    }

    if ( __cs.getObjectIds() == ":ent:occur:int" )
      rp = RoutingPad::create( PYNET_O(arg0), *(PYOCCURRENCE_O(arg1)), PyAny_AsLong(arg2) );
    else if ( __cs.getObjectIds() == ":pin" )
      rp = RoutingPad::create( dynamic_cast<Pin*>(EntityCast(arg0)) );
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for RoutingPad constructor." );
      return NULL;
    }
    HCATCH

    return PyRoutingPad_Link( rp );
  }
  

  static PyObject* PyRoutingPad_getOccurrence ( PyRoutingPad *self )
  {
    cdebug_log(20,0) << "PyRoutingPad_getOccurrence ()" << endl;
    METHOD_HEAD( "Component.getOccurrence()" )

    PyOccurrence* pyOccurrence = PyObject_NEW(PyOccurrence, &PyTypeOccurrence);
    if (pyOccurrence == NULL) return NULL;
    
    HTRY
    pyOccurrence->_object = new Occurrence(rp->getOccurrence());
    HCATCH

    return (PyObject*)pyOccurrence;
  }
  

  static PyObject* PyRoutingPad_getPlugOccurrence ( PyRoutingPad *self )
  {
    cdebug_log(20,0) << "PyRoutingPad_getPlugOccurrence ()" << endl;
    METHOD_HEAD( "Component.getPlugOccurrence()" )

    PyOccurrence* pyOccurrence = PyObject_NEW(PyOccurrence, &PyTypeOccurrence);
    if (pyOccurrence == NULL) return NULL;
    
    HTRY
    pyOccurrence->_object = new Occurrence(rp->getPlugOccurrence());
    HCATCH

    return (PyObject*)pyOccurrence;
  }
  

  static PyObject* PyRoutingPad_getSourcePosition ( PyRoutingPad *self )
  {
    cdebug_log(20,0) << "PyRoutingPad_getSourcePosition ()" << endl;
    METHOD_HEAD( "Component.getSourcePosition()" )

    PyPoint* pyPoint = PyObject_NEW( PyPoint, &PyTypePoint );
    if (pyPoint == NULL) return NULL;
    
    HTRY
    pyPoint->_object = new Point( rp->getSourcePosition() );
    HCATCH    

    return (PyObject*)pyPoint;
  }
  

  static PyObject* PyRoutingPad_getTargetPosition ( PyRoutingPad *self )
  {
    cdebug_log(20,0) << "PyRoutingPad_getTargetPosition ()" << endl;
    METHOD_HEAD( "Component.getTargetPosition()" )

    PyPoint* pyPoint = PyObject_NEW( PyPoint, &PyTypePoint );
    if (pyPoint == NULL) return NULL;
    
    HTRY
    pyPoint->_object = new Point( rp->getTargetPosition() );
    HCATCH    

    return (PyObject*)pyPoint;
  }


  static PyObject* PyRoutingPad_translate ( PyRoutingPad *self, PyObject* args )
  {
    cdebug_log(20,0) << "PyRoutingPad_translate ()" << endl;
    
    HTRY
    METHOD_HEAD ( "RoutingPad.translate()" )
    PyObject* arg0 = NULL;
    PyObject* arg1 = NULL;
    __cs.init ("RoutingPad.translate");
    if (PyArg_ParseTuple(args,"O&O&:RoutingPad.translate", Converter, &arg0, Converter, &arg1)) {
      if (__cs.getObjectIds() == INTS2_ARG) rp->translate( PyAny_AsLong(arg0), PyAny_AsLong(arg1) );
      else {
        PyErr_SetString ( ConstructorError, "RoutingPad.translate(): Invalid type for parameter(s)." );
        return NULL;
      }
    } else {
      PyErr_SetString ( ConstructorError, "RoutingPad.translate(): Invalid number of parameters." );
      return NULL;
    }
    HCATCH

    Py_RETURN_NONE;
  }


#if THIS_IS_DEPRECATED
  static PyObject* PyRoutingPad_setPosition ( PyRoutingPad *self, PyObject* args )
  {
    cdebug_log(20,0) << "PyRoutingPad_setPosition ()" << endl;
    
    HTRY
    METHOD_HEAD ( "RoutingPad.setPosition()" )
    PyObject* arg0 = NULL;
    PyObject* arg1 = NULL;

    __cs.init ("RoutingPad.setPosition");
    if (not PyArg_ParseTuple(args,"O&|O&:RoutingPad.setPosition"
                            ,Converter,&arg0
                            ,Converter,&arg1
                            )) {
      PyErr_SetString( ConstructorError, "RoutingPad.setPosition(): Invalid number/bad type of parameters." );
      return NULL;
    }

    if ( __cs.getObjectIds() == ":int:int" )
      rp->setPosition( PyAny_AsLong(arg0), PyAny_AsLong(arg1) );
    else if ( __cs.getObjectIds() == ":point" )
      rp->setPosition( *PYPOINT_O(arg0) );
    else {
      PyErr_SetString ( ConstructorError, "RoutingPad.setPosition(): Invalid parameters types." );
      return NULL;
    }
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyRoutingPad_setOffset ( PyRoutingPad *self, PyObject* args )
  {
    cdebug_log(20,0) << "PyRoutingPad_setOffset ()" << endl;
    
    HTRY
    METHOD_HEAD ( "RoutingPad.setOffset()" )
    PyObject* arg0 = NULL;
    PyObject* arg1 = NULL;
    __cs.init ("RoutingPad.setOffset");
    if (PyArg_ParseTuple(args,"O&O&:RoutingPad.setOffset", Converter, &arg0, Converter, &arg1)) {
      if (__cs.getObjectIds() == INTS2_ARG) rp->setOffset( PyAny_AsLong(arg0), PyAny_AsLong(arg1) );
      else {
        PyErr_SetString ( ConstructorError, "RoutingPad.setOffset(): Invalid type for parameter(s)." );
        return NULL;
      }
    } else {
      PyErr_SetString ( ConstructorError, "RoutingPad.setOffset(): Invalid number of parameters." );
      return NULL;
    }
    HCATCH

    Py_RETURN_NONE;
  }
#endif


  static PyObject* PyRoutingPad_setExternalComponent ( PyRoutingPad *self, PyObject* args )
  {
    cdebug_log(20,0) << "PyRoutingPad_setExternalComponent ()" << endl;
    
    HTRY
    METHOD_HEAD ( "RoutingPad.setExternalComponent()" )
    PyObject* pyComponent = NULL;
    if (PyArg_ParseTuple(args,"O:RoutingPad.setExternalComponent", &pyComponent)) {
      if (not IsPyComponent(pyComponent)) {
        PyErr_SetString( ConstructorError, "RoutingPad.setExternalComponent(): Argument is not of type Component." );
        return NULL;
      }
      rp->setExternalComponent( PYCOMPONENT_O(pyComponent) );
    } else {
      PyErr_SetString( ConstructorError, "RoutingPad.setExternalComponent(): Invalid number/bad type of parameters." );
      return NULL;
    }
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyRoutingPad_setOnBestComponent ( PyRoutingPad *self, PyObject* args )
  {
    cdebug_log(20,0) << "PyRoutingPad_setOnBestComponent ()" << endl;
    
    Component* selected = NULL;

    HTRY
    METHOD_HEAD ( "RoutingPad.setOnBestComponent()" )
    unsigned int  flags = 0;
    if (PyArg_ParseTuple(args,"I:RoutingPad.setOnBestComponent", &flags)) {
      selected = rp->setOnBestComponent( flags );
    } else {
      PyErr_SetString( ConstructorError, "RoutingPad.setOnBestComponent(): Invalid number/bad type of parameters." );
      return NULL;
    }
    HCATCH

    return PyEntity_NEW(selected);
  }


  static PyObject* PyRoutingPad_restorePlugOccurrence ( PyRoutingPad *self, PyObject* args )
  {
    cdebug_log(20,0) << "PyRoutingPad_restorePlugOccurrence ()" << endl;
    
    HTRY
    METHOD_HEAD ( "RoutingPad.restorePlugOccurrence()" )
    rp->restorePlugOccurrence();
    HCATCH

    Py_RETURN_NONE;
  }


  // ---------------------------------------------------------------
  // PyRoutingPad Attribute Method table.

  PyMethodDef PyRoutingPad_Methods[] =
    { { "create"               , (PyCFunction)PyRoutingPad_create               , METH_VARARGS|METH_STATIC
                               , "Create a RoutingPad." }
    , { "getOccurrence"        , (PyCFunction)PyRoutingPad_getOccurrence        , METH_NOARGS , "Get the RoutingPad occurrence (may be a component)." }
    , { "getPlugOccurrence"    , (PyCFunction)PyRoutingPad_getPlugOccurrence    , METH_NOARGS , "Get the RoutingPad plug occurrence." }
    , { "getX"                 , (PyCFunction)PyRoutingPad_getX                 , METH_NOARGS , "Get the RoutingPad X position." }
    , { "getY"                 , (PyCFunction)PyRoutingPad_getY                 , METH_NOARGS , "Get the RoutingPad Y position." }
    , { "getSourcePosition"    , (PyCFunction)PyRoutingPad_getSourcePosition    , METH_NOARGS , "Get the RoutingPad source position." }
    , { "getTargetPosition"    , (PyCFunction)PyRoutingPad_getTargetPosition    , METH_NOARGS , "Get the RoutingPad target position." }
    , { "getSourceX"           , (PyCFunction)PyRoutingPad_getSourceX           , METH_NOARGS , "Get the RoutingPad source X position." }
    , { "getSourceY"           , (PyCFunction)PyRoutingPad_getSourceY           , METH_NOARGS , "Get the RoutingPad source Y position." }
    , { "getTargetX"           , (PyCFunction)PyRoutingPad_getTargetX           , METH_NOARGS , "Get the RoutingPad target X position." }
    , { "getTargetY"           , (PyCFunction)PyRoutingPad_getTargetY           , METH_NOARGS , "Get the RoutingPad target Y position." }
    , { "translate"            , (PyCFunction)PyRoutingPad_translate            , METH_VARARGS, "Translates the RoutingPad segment of dx and dy." }
 // , { "setX"                 , (PyCFunction)PyRoutingPad_setX                 , METH_VARARGS, "Modify the RoutingPad X position." }
 // , { "setY"                 , (PyCFunction)PyRoutingPad_setY                 , METH_VARARGS, "Modify the RoutingPad Y position." }
 // , { "setPosition"          , (PyCFunction)PyRoutingPad_setPosition          , METH_VARARGS, "Sets the RoutingPad absolute position." }
 // , { "setOffset"            , (PyCFunction)PyRoutingPad_setOffset            , METH_VARARGS, "Sets the RoutingPad relative position." }
    , { "setExternalComponent" , (PyCFunction)PyRoutingPad_setExternalComponent , METH_VARARGS, "Sets the RoutingPad on an master net external component." }
    , { "setOnBestComponent"   , (PyCFunction)PyRoutingPad_setOnBestComponent   , METH_VARARGS, "Sets the RoutingPad on master net best external component." }
    , { "restorePlugOccurrence", (PyCFunction)PyRoutingPad_restorePlugOccurrence, METH_VARARGS, "Revert back from an external component to the plug." }
    , { "destroy"              , (PyCFunction)PyRoutingPad_destroy              , METH_NOARGS
                               , "destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}    /* sentinel */
    };




  // x-------------------------------------------------------------x
  // |               "PyRoutingPad" Object Methods                 |
  // x-------------------------------------------------------------x


  DBoDeleteMethod(RoutingPad)
  PyTypeObjectLinkPyType(RoutingPad)


#else  // End of Python Module Code Part.


// x=================================================================x
// |            "PyRoutingPad" Shared Library Code Part              |
// x=================================================================x


  // Link/Creation Method.
  DBoLinkCreateMethod(RoutingPad)


  PyTypeInheritedObjectDefinitions(RoutingPad, Component)


  static void  RoutingPadLoadConstants ( PyObject* dictionnary ) {
    PyObject* constant;

    LoadObjectConstant( dictionnary, RoutingPad::BiggestArea       , "BiggestArea" )
    LoadObjectConstant( dictionnary, RoutingPad::HighestLayer      , "HighestLayer" )
    LoadObjectConstant( dictionnary, RoutingPad::LowestLayer       , "LowestLayer" )
    LoadObjectConstant( dictionnary, RoutingPad::ComponentSelection, "ComponentSelection" )
  }


  extern  void  PyRoutingPad_postModuleInit ()
  {
    RoutingPadLoadConstants(PyTypeRoutingPad.tp_dict);
  }

#endif  // End of Shared Library Code Part.


}  // extern "C".


}  // Isobar namespace.
 
