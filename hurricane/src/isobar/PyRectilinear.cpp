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
// |  C++ Module  :  "./PyRectilinear.cpp"                           |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyPoint.h"
#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyLayer.h"
#include "hurricane/isobar/PyBox.h"
#include "hurricane/isobar/PyRectilinear.h"


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
        string message =  "Rectilinear: Item at position " + getString(i) + "has wrong type.";
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
#define METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Rectilinear,rectilinear,function)


// +=================================================================+
// |             "PyRectilinear" Python Module Code Part             |
// +=================================================================+

#if defined(__PYTHON_MODULE__)

  // Standard Accessors (Attributes).
  DirectGetLongAttribute(PyRectilinear_getX, getX, PyRectilinear, Rectilinear)
  DirectGetLongAttribute(PyRectilinear_getY, getY, PyRectilinear, Rectilinear)

  // Standard Destroy (Attribute).
  DBoDestroyAttribute(PyRectilinear_destroy, PyRectilinear)


  static PyObject* PyRectilinear_create ( PyObject*, PyObject *args )
  {
    cdebug_log(20,0) << "PyRectilinear_create()" << endl;

    PyObject* arg0     = NULL;
    PyObject* arg1     = NULL;
    PyObject* arg2     = NULL;
    Rectilinear* rectilinear = NULL;

    HTRY
      if (not PyArg_ParseTuple(args, "OOO:Rectilinear.create" ,&arg0 ,&arg1 ,&arg2 )) {
        PyErr_SetString( ConstructorError, "Invalid number of parameters for Rectilinear constructor." );
        return NULL;
      }
      if (not IsPyNet(arg0)) {
        PyErr_SetString( ConstructorError, "First parameter of Rectilinear constructor must be a Net." );
        return NULL;
      }
      if (not IsPyDerivedLayer(arg1)) {
        PyErr_SetString( ConstructorError, "Second parameter of Rectilinear constructor must be a Layer." );
        return NULL;
      }

      vector<Point> points;
      if (not ListToVector<Point>(arg2,&PyTypePoint,points)) return NULL;

      rectilinear = Rectilinear::create( PYNET_O(arg0), PYDERIVEDLAYER_O(arg1), points );
    HCATCH

    return PyRectilinear_Link(rectilinear);
  }


  static PyObject* PyRectilinear_getBoundingBox ( PyRectilinear *self )
  {
    cdebug_log(20,0) << "PyRectilinear_getBoundingBox()" << endl;

    METHOD_HEAD( "Rectilinear.BoundingBox()" )

    PyBox* pyBox = PyObject_NEW( PyBox, &PyTypeBox );
    if (pyBox == NULL) { return NULL; }
    
    HTRY
      pyBox->_object = new Box ( rectilinear->getBoundingBox() );
    HCATCH    

    return (PyObject*)pyBox;
  }


  static PyObject* PyRectilinear_setPoints ( PyRectilinear *self, PyObject* args )
  {
    cdebug_log(20,0) << "Rectilinear.setPoints()" << endl;

    HTRY
      METHOD_HEAD( "Rectilinear.setPoints()" )
      
      PyObject* arg0 = NULL;
      if (not PyArg_ParseTuple( args, "O:Rectilinear.setPoints", &arg0 )) {
        PyErr_SetString( ConstructorError, "Invalid number of parameters for Rectilinear.setPoints()." );
        return NULL;
      }

      vector<Point> points;
      if (not ListToVector<Point>(arg0,&PyTypePoint,points)) return NULL;
      
      rectilinear->setPoints( points );
    HCATCH

    Py_RETURN_NONE;
  }

  
  static PyObject* PyRectilinear_translate ( PyRectilinear *self, PyObject* args )
  {
    cdebug_log(20,0) << "PyRectilinear_translate ()" << endl;
    
    HTRY
      METHOD_HEAD ( "Rectilinear.translate()" )
      PyObject* arg0 = NULL;
      PyObject* arg1 = NULL;
      __cs.init ("Rectilinear.translate");
      if (PyArg_ParseTuple(args,"O&O&:Rectilinear.translate", Converter, &arg0, Converter, &arg1)) {
        if (__cs.getObjectIds() == INTS2_ARG) rectilinear->translate( PyAny_AsLong(arg0), PyAny_AsLong(arg1) );
        else {
          PyErr_SetString ( ConstructorError, "Rectilinear.translate(): Invalid type for parameter(s)." );
          return NULL;
        }
      } else {
        PyErr_SetString ( ConstructorError, "Rectilinear.translate(): Invalid number of parameters." );
        return NULL;
      }
    HCATCH

    Py_RETURN_NONE;
  }


  // ---------------------------------------------------------------
  // PyRectilinear Attribute Method table.

  PyMethodDef PyRectilinear_Methods[] =
    { { "create"        , (PyCFunction)PyRectilinear_create        , METH_VARARGS|METH_STATIC
                        , "Create a new Rectilinear polygon." }
    , { "getX"          , (PyCFunction)PyRectilinear_getX          , METH_NOARGS , "Return the Rectilinear X value." }
    , { "getY"          , (PyCFunction)PyRectilinear_getY          , METH_NOARGS , "Return the Rectilinear Y value." }
    , { "getBoundingBox", (PyCFunction)PyRectilinear_getBoundingBox, METH_NOARGS , "Return the Rectilinear Bounding Box." }
    , { "setPoints"     , (PyCFunction)PyRectilinear_setPoints     , METH_VARARGS, "Sets the Rectilinear Bounding Box." }
    , { "translate"     , (PyCFunction)PyRectilinear_translate     , METH_VARARGS, "Translates the Rectilinear of dx and dy." }
    , { "destroy"       , (PyCFunction)PyRectilinear_destroy       , METH_NOARGS
                        , "Destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}  /* sentinel */
    };


  DBoDeleteMethod(Rectilinear)
  PyTypeObjectLinkPyType(Rectilinear)


#else  // Python Module Code Part.


// +=================================================================+
// |          "PyRectilinear" Shared Library Code Part               |
// +=================================================================+


  // Link/Creation Method.
  DBoLinkCreateMethod(Rectilinear)
  PyTypeInheritedObjectDefinitions(Rectilinear,Component)

#endif  // Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
