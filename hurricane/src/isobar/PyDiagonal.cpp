// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2006-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyDiagonal.cpp"                              |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyLayer.h"
#include "hurricane/isobar/PyPoint.h"
#include "hurricane/isobar/PyBox.h"
#include "hurricane/isobar/PyDiagonal.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject._baseObject)
#define METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Diagonal,diagonal,function)


// +=================================================================+
// |             "PyDiagonal" Python Module Code Part                |
// +=================================================================+

#if defined(__PYTHON_MODULE__)

  // Standard Accessors (Attributes).
  DirectGetLongAttribute(PyDiagonal_getX         , getX         , PyDiagonal, Diagonal)
  DirectGetLongAttribute(PyDiagonal_getY         , getY         , PyDiagonal, Diagonal)
  DirectGetLongAttribute(PyDiagonal_getSourceX   , getSourceX   , PyDiagonal, Diagonal)
  DirectGetLongAttribute(PyDiagonal_getSourceY   , getSourceY   , PyDiagonal, Diagonal)
  DirectGetLongAttribute(PyDiagonal_getTargetX   , getTargetX   , PyDiagonal, Diagonal)
  DirectGetLongAttribute(PyDiagonal_getTargetY   , getTargetY   , PyDiagonal, Diagonal)
  DirectGetLongAttribute(PyDiagonal_getWidth     , getWidth     , PyDiagonal, Diagonal)
  DirectGetUIntAttribute(PyDiagonal_getPointsSize, getPointsSize, PyDiagonal, Diagonal)
  DirectSetLongAttribute(PyDiagonal_setWidth     , setWidth     , PyDiagonal, Diagonal)

  // Standard Destroy (Attribute).
  DBoDestroyAttribute(PyDiagonal_destroy, PyDiagonal)


  static PyObject* PyDiagonal_create ( PyObject*, PyObject *args )
  {
    cdebug_log(20,0) << "PyDiagonal_create()" << endl;

    PyObject* arg0     = NULL;
    PyObject* arg1     = NULL;
    PyObject* arg2     = NULL;
    PyObject* arg3     = NULL;
    PyObject* arg4     = NULL;
    Diagonal* diagonal = NULL;

    HTRY
      __cs.init( "Diagonal.create" );
      if (not PyArg_ParseTuple(args,"O&O&O&O&O&:Diagonal.create"
                              ,Converter,&arg0
                              ,Converter,&arg1
                              ,Converter,&arg2
                              ,Converter,&arg3
                              ,Converter,&arg4
                              )) {
          PyErr_SetString( ConstructorError, "Diagonal.create(): Invalid number of parameters." );
          return NULL;
      }
  
      if ( __cs.getObjectIds() == ":ent:layer:point:point:int") {
        diagonal = Diagonal::create( PYNET_O(arg0)
                                   , PYLAYER_O(arg1)
                                   , *PYPOINT_O(arg2)
                                   , *PYPOINT_O(arg3)
                                   , PyAny_AsLong(arg4)
                                   );
      } else {
        PyErr_SetString( ConstructorError, "Diagonal.create(): Bad type of parameter(s)." );
        return NULL;
      }
    HCATCH

    return PyDiagonal_Link(diagonal);
  }
  

  static PyObject* PyDiagonal_getSourcePosition ( PyDiagonal *self )
  {
    cdebug_log(20,0) << "PyDiagonal_getSourcePosition()" << endl;
    METHOD_HEAD( "Diagonal.getSourcePosition()" )

    PyPoint* pyPoint = PyObject_NEW ( PyPoint, &PyTypePoint );
    if (pyPoint == NULL) return NULL;
    
    HTRY
      pyPoint->_object = new Point ( diagonal->getSourcePosition() );
    HCATCH    

    return (PyObject*)pyPoint;
  }
  

  static PyObject* PyDiagonal_getTargetPosition ( PyDiagonal *self )
  {
    cdebug_log(20,0) << "PyDiagonal_getTargetPosition()" << endl;
    METHOD_HEAD( "Diagonal.getTargetPosition()" )

    PyPoint* pyPoint = PyObject_NEW ( PyPoint, &PyTypePoint );
    if (pyPoint == NULL) return NULL;
    
    HTRY
      pyPoint->_object = new Point ( diagonal->getTargetPosition() );
    HCATCH    

    return (PyObject*)pyPoint;
  }
  

  static PyObject* PyDiagonal_getPoint ( PyDiagonal *self, PyObject* args )
  {
    cdebug_log(20,0) << "PyDiagonal_getPoint()" << endl;
    METHOD_HEAD( "Diagonal.getTargetPoint()" )

    PyPoint* pyPoint = PyObject_NEW ( PyPoint, &PyTypePoint );
    if (pyPoint == NULL) return NULL;
    
    HTRY
      int i = 0;
      if (not PyArg_ParseTuple( args, "i:Diagonal.getPoint", &i)) {
        PyErr_SetString( ConstructorError, "Diagonal.getPoint(): Invalid parameter type." );
        return NULL;
      }
      pyPoint->_object = new Point ( diagonal->getPoint(i) );
    HCATCH    

    return (PyObject*)pyPoint;
  }


  static PyObject* PyDiagonal_getBoundingBox( PyDiagonal *self )
  {
    cdebug_log(20,0) << "PyDiagonal_getBoundingBox()" << endl;

    METHOD_HEAD ( "Diagonal.BoundingBox()" )

    PyBox* pyBox = PyObject_NEW ( PyBox, &PyTypeBox );
    if (pyBox == NULL) { return NULL; }
    
    HTRY
    pyBox->_object = new Box ( diagonal->getBoundingBox() );
    HCATCH    

    return ( (PyObject*)pyBox );
  }


  static PyObject* PyDiagonal_setSource ( PyDiagonal *self, PyObject* args )
  {
    cdebug_log(20,0) << "Diagonal.setSource()" << endl;
    HTRY
      METHOD_HEAD( "Diagonal.setSource()" )

      PyPoint* point = NULL;
      if (not ParseOneArg("Diagonal.setSource", args, POINT_ARG, (PyObject**)&point))
        return NULL;
      diagonal->setSource(*PYPOINT_O(point));
    HCATCH
    Py_RETURN_NONE;
  }


  static PyObject* PyDiagonal_setTarget ( PyDiagonal *self, PyObject* args )
  {
    cdebug_log(20,0) << "Diagonal.setTarget()" << endl;
    HTRY
      METHOD_HEAD( "Diagonal.setTarget()" )

      PyPoint* point = NULL;
      if (not ParseOneArg("Diagonal.setTarget", args, POINT_ARG, (PyObject**)&point))
        return NULL;
      diagonal->setTarget(*PYPOINT_O(point));
    HCATCH
    Py_RETURN_NONE;
  }


  static PyObject* PyDiagonal_translate ( PyDiagonal *self, PyObject* args )
  {
    cdebug_log(20,0) << "PyDiagonal_translate ()" << endl;
    
    HTRY
    METHOD_HEAD ( "Diagonal.translate()" )
    PyObject* arg0 = NULL;
    PyObject* arg1 = NULL;
    __cs.init ("Diagonal.translate");
    if (PyArg_ParseTuple(args,"O&O&:Diagonal.translate", Converter, &arg0, Converter, &arg1)) {
      if (__cs.getObjectIds() == INTS2_ARG) diagonal->translate( PyAny_AsLong(arg0), PyAny_AsLong(arg1) );
      else {
        PyErr_SetString ( ConstructorError, "Diagonal.translate(): Invalid type for parameter(s)." );
        return NULL;
      }
    } else {
      PyErr_SetString ( ConstructorError, "Diagonal.translate(): Invalid number of parameters." );
      return NULL;
    }
    HCATCH

    Py_RETURN_NONE;
  }


  // ---------------------------------------------------------------
  // PyDiagonal Attribute Method table.

  PyMethodDef PyDiagonal_Methods[] =
    { { "create"           , (PyCFunction)PyDiagonal_create           , METH_VARARGS|METH_STATIC
                           , "Create a new Diagonal." }               
    , { "getX"             , (PyCFunction)PyDiagonal_getX             , METH_NOARGS , "Return the Diagonal X value." }
    , { "getY"             , (PyCFunction)PyDiagonal_getY             , METH_NOARGS , "Return the Diagonal Y value." }
    , { "getSourceX"       , (PyCFunction)PyDiagonal_getSourceX       , METH_NOARGS , "Return the Diagonal source X value." }
    , { "getSourceY"       , (PyCFunction)PyDiagonal_getSourceY       , METH_NOARGS , "Return the Diagonal source Y value." }
    , { "getTargetX"       , (PyCFunction)PyDiagonal_getTargetX       , METH_NOARGS , "Return the Diagonal target X value." }
    , { "getTargetY"       , (PyCFunction)PyDiagonal_getTargetY       , METH_NOARGS , "Return the Diagonal target Y value." }
    , { "getSourcePosition", (PyCFunction)PyDiagonal_getSourcePosition, METH_NOARGS , "Return the Diagonal source position." }
    , { "getTargetPosition", (PyCFunction)PyDiagonal_getTargetPosition, METH_NOARGS , "Return the Diagonal target position." }
    , { "getWidth"         , (PyCFunction)PyDiagonal_getWidth         , METH_NOARGS , "Return the Diagonal wire width." }
    , { "getPointsSize"    , (PyCFunction)PyDiagonal_getPointsSize    , METH_NOARGS , "Return the Diagonal polygon number of points (vertexes)." }
    , { "getPoint"         , (PyCFunction)PyDiagonal_getPoint         , METH_VARARGS, "Return the Diagonal polygon point at index i." }
    , { "getBoundingBox"   , (PyCFunction)PyDiagonal_getBoundingBox   , METH_NOARGS , "Return the Diagonal Bounding Box." }
    , { "translate"        , (PyCFunction)PyDiagonal_translate        , METH_VARARGS, "Translates the Diagonal of dx and dy." }
    , { "setWidth"         , (PyCFunction)PyDiagonal_setWidth         , METH_VARARGS, "Set the Diagonal wire width." }
    , { "setSource"        , (PyCFunction)PyDiagonal_setSource        , METH_VARARGS, "Set the Diagonal source point (must be diagonal to target)." }
    , { "setTarget"        , (PyCFunction)PyDiagonal_setTarget        , METH_VARARGS, "Set the Diagonal target point (must be diagonal to source)." }
    , { "destroy"          , (PyCFunction)PyDiagonal_destroy          , METH_NOARGS
                           , "Destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}  /* sentinel */
    };


  DBoDeleteMethod(Diagonal)
  PyTypeObjectLinkPyType(Diagonal)


#else  // Python Module Code Part.

// +=================================================================+
// |           "PyDiagonal" Shared Library Code Part                 |
// +=================================================================+


  // Link/Creation Method.
  DBoLinkCreateMethod(Diagonal)
  PyTypeInheritedObjectDefinitions(Diagonal, Component)

#endif  // Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
