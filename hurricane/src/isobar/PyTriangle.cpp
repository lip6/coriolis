// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2018-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyTriangle.cpp"                              |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyPoint.h"
#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyLayer.h"
#include "hurricane/isobar/PyBox.h"
#include "hurricane/isobar/PyTriangle.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject._baseObject)
#define METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Triangle,triangle,function)


// +=================================================================+
// |              "PyTriangle" Python Module Code Part               |
// +=================================================================+

#if defined(__PYTHON_MODULE__)

  // Standard Accessors (Attributes).
  DirectGetLongAttribute(PyTriangle_getX, getX, PyTriangle, Triangle)
  DirectGetLongAttribute(PyTriangle_getY, getY, PyTriangle, Triangle)

  // Standard Destroy (Attribute).
  DBoDestroyAttribute(PyTriangle_destroy, PyTriangle)


  static PyObject* PyTriangle_create ( PyObject*, PyObject *args )
  {
    cdebug_log(20,0) << "PyTriangle_create()" << endl;

    PyObject* arg0     = NULL;
    PyObject* arg1     = NULL;
    PyObject* arg2     = NULL;
    PyObject* arg3     = NULL;
    PyObject* arg4     = NULL;
    Triangle* triangle = NULL;

    HTRY
      __cs.init( "Triangle.create" );
      if (!PyArg_ParseTuple(args,"O&O&O&O&O&:Triangle.create"
                             ,Converter,&arg0
                             ,Converter,&arg1
                             ,Converter,&arg2
                             ,Converter,&arg3
                             ,Converter,&arg4
                             )) {
        PyErr_SetString( ConstructorError, "Invalid number of parameters for Triangle constructor." );
        return NULL;
      }
      
      if (__cs.getObjectIds() == ":ent:layer:point:point:point") {
        triangle = Triangle::create(  PYNET_O  (arg0)
                                   ,  PYLAYER_O(arg1)
                                   , *PYPOINT_O(arg2)
                                   , *PYPOINT_O(arg3)
                                   , *PYPOINT_O(arg4)
                                   );
      } else {
        PyErr_SetString( ConstructorError, "Bad type(s) of parameters for Triangle constructor." );
        return NULL;
      }
    HCATCH

    return PyTriangle_Link(triangle);
  }


  static PyObject* PyTriangle_getBoundingBox ( PyTriangle *self )
  {
    cdebug_log(20,0) << "PyTriangle_getBoundingBox()" << endl;

    METHOD_HEAD( "Triangle.BoundingBox()" )

    PyBox* pyBox = PyObject_NEW( PyBox, &PyTypeBox );
    if (pyBox == NULL) { return NULL; }
    
    HTRY
      pyBox->_object = new Box ( triangle->getBoundingBox() );
    HCATCH    

    return (PyObject*)pyBox;
  }


  static PyObject* PyTriangle_setPoints ( PyTriangle *self, PyObject* args )
  {
    cdebug_log(20,0) << "Triangle.setPoints()" << endl;

    HTRY
      METHOD_HEAD( "Triangle.setPoints()" )
      
      PyObject* arg0 = NULL;
      PyObject* arg1 = NULL;
      PyObject* arg2 = NULL;
      if (not PyArg_ParseTuple( args,"O&O&O&:Triangle.setPoints"
                              ,Converter,&arg0
                              ,Converter,&arg1
                              ,Converter,&arg2
                              )) {
        PyErr_SetString( ConstructorError, "Invalid number of parameters for Triangle.setPoints()." );
        return NULL;
      }
      
      if (__cs.getObjectIds() == ":point:point:point") {
        triangle->setPoints( *PYPOINT_O(arg0)
                           , *PYPOINT_O(arg1)
                           , *PYPOINT_O(arg2)
                           );
      } else {
        PyErr_SetString( ConstructorError, "Bad type(s) of parameters for Triangle.setPoints()." );
        return NULL;
      }
    HCATCH

    Py_RETURN_NONE;
  }

  
  static PyObject* PyTriangle_translate ( PyTriangle *self, PyObject* args )
  {
    cdebug_log(20,0) << "PyTriangle_translate ()" << endl;
    
    HTRY
      METHOD_HEAD ( "Triangle.translate()" )
      PyObject* arg0 = NULL;
      PyObject* arg1 = NULL;
      __cs.init ("Triangle.translate");
      if (PyArg_ParseTuple(args,"O&O&:Triangle.translate", Converter, &arg0, Converter, &arg1)) {
        if (__cs.getObjectIds() == INTS2_ARG) triangle->translate( PyAny_AsLong(arg0), PyAny_AsLong(arg1) );
        else {
          PyErr_SetString ( ConstructorError, "Triangle.translate(): Invalid type for parameter(s)." );
          return NULL;
        }
      } else {
        PyErr_SetString ( ConstructorError, "Triangle.translate(): Invalid number of parameters." );
        return NULL;
      }
    HCATCH

    Py_RETURN_NONE;
  }


  // ---------------------------------------------------------------
  // PyTriangle Attribute Method table.

  PyMethodDef PyTriangle_Methods[] =
    { { "create"        , (PyCFunction)PyTriangle_create        , METH_VARARGS|METH_STATIC
                        , "Create a new Triangle." }
    , { "getX"          , (PyCFunction)PyTriangle_getX          , METH_NOARGS , "Return the Triangle X value." }
    , { "getY"          , (PyCFunction)PyTriangle_getY          , METH_NOARGS , "Return the Triangle Y value." }
    , { "getBoundingBox", (PyCFunction)PyTriangle_getBoundingBox, METH_NOARGS , "Return the Triangle Bounding Box." }
    , { "setPoints"     , (PyCFunction)PyTriangle_setPoints     , METH_VARARGS, "Sets the Triangle Bounding Box." }
    , { "translate"     , (PyCFunction)PyTriangle_translate     , METH_VARARGS, "Translates the Triangle of dx and dy." }
    , { "destroy"       , (PyCFunction)PyTriangle_destroy       , METH_NOARGS
                        , "Destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}  /* sentinel */
    };


  DBoDeleteMethod(Triangle)
  PyTypeObjectLinkPyType(Triangle)


#else  // Python Module Code Part.


// +=================================================================+
// |           "PyTriangle" Shared Library Code Part                 |
// +=================================================================+


  // Link/Creation Method.
  DBoLinkCreateMethod(Triangle)
  PyTypeInheritedObjectDefinitions(Triangle,Component)

#endif  // Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
