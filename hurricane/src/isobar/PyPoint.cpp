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
// |  C++ Module  :  "./PyPoint.cpp"                                 |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyPoint.h"


namespace Isobar {

using namespace Hurricane;

extern "C" {


#define METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Point,point,function)


// +=================================================================+
// |                "PyPoint" Python Module Code Part                |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  // +-------------------------------------------------------------+
  // |               "PyPoint" Attribute Methods                   |
  // +-------------------------------------------------------------+


  static PyObject* PyPoint_NEW ( PyObject* module, PyObject *args )
  {
    cdebug_log(20,0) << "PyPoint_NEW()" << endl;

    Point*    point;
    PyObject* arg0;
    PyObject* arg1;

    __cs.init ("Point.Point");
    if ( ! PyArg_ParseTuple(args,"|O&O&:Point.Point"
                           ,Converter,&arg0
                           ,Converter,&arg1
                           )) {
        PyErr_SetString ( ConstructorError, "invalid number of parameters for Point constructor." );
        return NULL;
    }

    if      ( __cs.getObjectIds() == NO_ARG    ) { point = new Point (); }
    else if ( __cs.getObjectIds() == POINT_ARG ) { point = new Point ( *PYPOINT_O(arg0) ); }
    else if ( __cs.getObjectIds() == INTS2_ARG ) { point = new Point ( PyAny_AsLong(arg0)
                                                                     , PyAny_AsLong(arg1) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Point constructor." );
      return NULL;
    }

    PyPoint* pyPoint = PyObject_NEW(PyPoint, &PyTypePoint);
    if (pyPoint == NULL) { return NULL; }
    
    HTRY
    pyPoint->_object = point;
    HCATCH

    return ( (PyObject*)pyPoint );
  }


  static int  PyPoint_Init ( PyPoint* self, PyObject* args, PyObject* kwargs )
  {
    cdebug_log(20,0) << "PyPoint_Init(): " << (void*)self << endl;
    return 0;
  }


  static PyObject* PyPoint_translate ( PyPoint *self, PyObject* args )
  {
    cdebug_log(20,0) << "PyPoint_translate()" << endl;
    HTRY
      METHOD_HEAD( "Point.translate()" )
      PyObject* arg0;
      PyObject* arg1;
      if (not ParseTwoArg( "Point.translate", args, INTS2_ARG, &arg0, &arg1 )) return NULL;
      point->translate( PyAny_AsLong(arg0), PyAny_AsLong(arg1) );
    HCATCH
    Py_RETURN_NONE;
  }


  static PyObject* PyPoint_manhattanDistance ( PyPoint *self, PyObject* args )
  {
    cdebug_log(20,0) << "PyPoint_manhattanDistance()" << endl;
    DbU::Unit distance = 0;
    HTRY
      METHOD_HEAD ( "Point.manahattanDistance()" )
      PyObject* pyPoint = NULL;
      if (not PyArg_ParseTuple(args,"O:Point.manhattanDistance()", &pyPoint)) {
        PyErr_SetString ( ConstructorError, "Point.manhattanDistance(): Takes exactly one argument." );
        return NULL;
      }
      if (not IsPyPoint(pyPoint)) {
        PyErr_SetString ( ConstructorError, "Point.manhattanDistance(): Argument is not a Point." );
        return NULL;
      }
      distance = point->manhattanDistance( *(PYPOINT_O(pyPoint)) );
    HCATCH
    return PyDbU_FromLong(distance);
  }




  // Standart Accessors (Attributes).
  DirectGetLongAttribute(PyPoint_getX,getX,PyPoint,Point)
  DirectGetLongAttribute(PyPoint_getY,getY,PyPoint,Point)
  DirectSetLongAttribute(PyPoint_setX,setX,PyPoint,Point)
  DirectSetLongAttribute(PyPoint_setY,setY,PyPoint,Point)


  // Standart destroy (Attribute).
  DirectDestroyAttribute(PyPoint_destroy, PyPoint)




  // ---------------------------------------------------------------
  // PyPoint Attribute Method table.

  PyMethodDef PyPoint_Methods[] =
    { { "getX"              , (PyCFunction)PyPoint_getX             , METH_NOARGS , "Return the Point X value." }
    , { "getY"              , (PyCFunction)PyPoint_getY             , METH_NOARGS , "Return the Point Y value." }
    , { "setX"              , (PyCFunction)PyPoint_setX             , METH_VARARGS, "Modify the Point X value." }
    , { "setY"              , (PyCFunction)PyPoint_setY             , METH_VARARGS, "Modify the Point Y value." }
    , { "translate"         , (PyCFunction)PyPoint_translate        , METH_VARARGS, "Translate the point of dx and dy." }
    , { "manhattanDistance" , (PyCFunction)PyPoint_manhattanDistance, METH_VARARGS, "Compute the Manhattan distance between the two points." }
    , { "destroy"           , (PyCFunction)PyPoint_destroy          , METH_NOARGS
                            , "Destroy associated hurricane object The python object remains." }
    , {NULL, NULL, 0, NULL} /* sentinel */
    };




  // x-------------------------------------------------------------x
  // |                 "PyPoint" Object Methods                    |
  // x-------------------------------------------------------------x



  DirectDeleteMethod(PyPoint_DeAlloc,PyPoint)
  PyTypeObjectLinkPyTypeNewInit(Point)
//PyTypeObjectLinkPyType(Point)

#else  // End of Python Module Code Part.


// x=================================================================x
// |               "PyPoint" Shared Library Code Part                |
// x=================================================================x
  

  PyTypeObjectDefinitions(Point)


#endif  // Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
 
