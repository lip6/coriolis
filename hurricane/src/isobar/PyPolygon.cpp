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
// |  C++ Module  :  "./PyPolygon.cpp"                               |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyPoint.h"
#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyLayer.h"
#include "hurricane/isobar/PyBox.h"
#include "hurricane/isobar/PyPolygon.h"


namespace  Isobar {

  using namespace Hurricane;


  template< typename CppType >
  bool  ListToVector ( PyObject* list, PyTypeObject* itemType, std::vector<CppType>& v )
  {
    if (not PyList_Check(list)) return false;

    int length = PyList_Size( list );
    for ( int i=0 ; i<length ; ++i ) {
      PyObject* item = PyList_GetItem( list, i );
      if ((item)->ob_type != itemType) {
        string message =  "Polygon: Item at position " + getString(i) + "has wrong type.";
        PyErr_SetString( ConstructorError, message.c_str() );
        return false;
      }
      v.push_back( *PYPOINT_O(item) );
    }
    return true;
  }


extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject._baseObject)
#define METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Polygon,polygon,function)


// +=================================================================+
// |               "PyPolygon" Python Module Code Part               |
// +=================================================================+

#if defined(__PYTHON_MODULE__)

  // Standard Accessors (Attributes).
  DirectGetLongAttribute(PyPolygon_getX, getX, PyPolygon, Polygon)
  DirectGetLongAttribute(PyPolygon_getY, getY, PyPolygon, Polygon)

  // Standard Destroy (Attribute).
  DBoDestroyAttribute(PyPolygon_destroy, PyPolygon)


  static PyObject* PyPolygon_create ( PyObject*, PyObject *args )
  {
    cdebug_log(20,0) << "PyPolygon_create()" << endl;

    PyObject* arg0     = NULL;
    PyObject* arg1     = NULL;
    PyObject* arg2     = NULL;
    Polygon* polygon = NULL;

    HTRY
      if (not PyArg_ParseTuple(args, "OOO:Polygon.create" ,&arg0 ,&arg1 ,&arg2 )) {
        PyErr_SetString( ConstructorError, "Invalid number of parameters for Polygon constructor." );
        return NULL;
      }
      if (not IsPyNet(arg0)) {
        PyErr_SetString( ConstructorError, "First parameter of Polygon constructor must be a Net." );
        return NULL;
      }
      if (not IsPyDerivedLayer(arg1)) {
        PyErr_SetString( ConstructorError, "Second parameter of Polygon constructor must be a Layer." );
        return NULL;
      }

      vector<Point> points;
      if (not ListToVector<Point>(arg2,&PyTypePoint,points)) return NULL;

      polygon = Polygon::create( PYNET_O(arg0), PYDERIVEDLAYER_O(arg1), points );
    HCATCH

    return PyPolygon_Link(polygon);
  }


  static PyObject* PyPolygon_getBoundingBox ( PyPolygon *self )
  {
    cdebug_log(20,0) << "PyPolygon_getBoundingBox()" << endl;

    METHOD_HEAD( "Polygon.BoundingBox()" )

    PyBox* pyBox = PyObject_NEW( PyBox, &PyTypeBox );
    if (pyBox == NULL) { return NULL; }
    
    HTRY
      pyBox->_object = new Box ( polygon->getBoundingBox() );
    HCATCH    

    return (PyObject*)pyBox;
  }


  static PyObject* PyPolygon_setPoints ( PyPolygon *self, PyObject* args )
  {
    cdebug_log(20,0) << "Polygon.setPoints()" << endl;

    HTRY
      METHOD_HEAD( "Polygon.setPoints()" )
      
      PyObject* arg0 = NULL;
      PyObject* arg1 = NULL;
      PyObject* arg2 = NULL;
      if (not PyArg_ParseTuple( args, "O:Polygon.setPoints", &arg0 )) {
        PyErr_SetString( ConstructorError, "Invalid number of parameters for Polygon.setPoints()." );
        return NULL;
      }

      vector<Point> points;
      if (not ListToVector<Point>(arg0,&PyTypePoint,points)) return NULL;
      
      polygon->setPoints( points );
    HCATCH

    Py_RETURN_NONE;
  }

  
  static PyObject* PyPolygon_translate ( PyPolygon *self, PyObject* args )
  {
    cdebug_log(20,0) << "PyPolygon_translate ()" << endl;
    
    HTRY
      METHOD_HEAD ( "Polygon.translate()" )
      PyObject* arg0 = NULL;
      PyObject* arg1 = NULL;
      __cs.init ("Polygon.translate");
      if (PyArg_ParseTuple(args,"O&O&:Polygon.translate", Converter, &arg0, Converter, &arg1)) {
        if (__cs.getObjectIds() == INTS2_ARG) polygon->translate( PyAny_AsLong(arg0), PyAny_AsLong(arg1) );
        else {
          PyErr_SetString ( ConstructorError, "Polygon.translate(): Invalid type for parameter(s)." );
          return NULL;
        }
      } else {
        PyErr_SetString ( ConstructorError, "Polygon.translate(): Invalid number of parameters." );
        return NULL;
      }
    HCATCH

    Py_RETURN_NONE;
  }


  // ---------------------------------------------------------------
  // PyPolygon Attribute Method table.

  PyMethodDef PyPolygon_Methods[] =
    { { "create"        , (PyCFunction)PyPolygon_create        , METH_VARARGS|METH_STATIC
                        , "Create a new Polygon." }
    , { "getX"          , (PyCFunction)PyPolygon_getX          , METH_NOARGS , "Return the Polygon X value." }
    , { "getY"          , (PyCFunction)PyPolygon_getY          , METH_NOARGS , "Return the Polygon Y value." }
    , { "getBoundingBox", (PyCFunction)PyPolygon_getBoundingBox, METH_NOARGS , "Return the Polygon Bounding Box." }
    , { "setPoints"     , (PyCFunction)PyPolygon_setPoints     , METH_VARARGS, "Sets the Polygon Bounding Box." }
    , { "translate"     , (PyCFunction)PyPolygon_translate     , METH_VARARGS, "Translates the Polygon of dx and dy." }
    , { "destroy"       , (PyCFunction)PyPolygon_destroy       , METH_NOARGS
                        , "Destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}  /* sentinel */
    };


  DBoDeleteMethod(Polygon)
  PyTypeObjectLinkPyType(Polygon)


#else  // Python Module Code Part.


// +=================================================================+
// |            "PyPolygon" Shared Library Code Part                 |
// +=================================================================+


  // Link/Creation Method.
  DBoLinkCreateMethod(Polygon)
  PyTypeInheritedObjectDefinitions(Polygon,Component)

#endif  // Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
