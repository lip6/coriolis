// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2014-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyInterval.cpp"                         |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyPoint.h"
#include "hurricane/isobar/PyInterval.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


#define  METHOD_HEAD(function)  GENERIC_METHOD_HEAD(Interval,interval,function)


// +=================================================================+
// |              "PyInterval" Python Module Code Part               |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  // Standart Accessors (Attributes).
  DirectGetLongAttribute(PyInterval_getVMin      ,getVMin      ,PyInterval,Interval)
  DirectGetLongAttribute(PyInterval_getVMax      ,getVMax      ,PyInterval,Interval)
  DirectGetLongAttribute(PyInterval_getCenter    ,getCenter    ,PyInterval,Interval)
  DirectGetLongAttribute(PyInterval_getSize      ,getSize      ,PyInterval,Interval)
  DirectGetLongAttribute(PyInterval_getHalfSize  ,getHalfSize  ,PyInterval,Interval)

  // Standart Predicates (Attributes).
  DirectGetBoolAttribute(PyInterval_isEmpty   ,isEmpty   ,PyInterval,Interval)
  DirectGetBoolAttribute(PyInterval_isPonctual,isPonctual,PyInterval,Interval)

  // Standart Destroy (Attribute).
  DirectDestroyAttribute(PyInterval_destroy, PyInterval)
  

  // ---------------------------------------------------------------
  // Class Method  :  "PyInterval_NEW ()"

  static PyObject* PyInterval_NEW (PyObject *module, PyObject *args) {
    cdebug.log(20) << "PyInterval_NEW()" << endl;

    Interval* interval = NULL;

    HTRY
    PyObject* arg0;
    PyObject* arg1;
    PyObject* arg2;
    __cs.init ("Interval.Interval");

    if (not PyArg_ParseTuple( args, "|O&O&O&:Interval.Interval"
                                  , Converter, &arg0
                                  , Converter, &arg1
                                  , Converter, &arg2 ))
      return NULL;

    if      (__cs.getObjectIds() == NO_ARG   ) { interval = new Interval (); }
    else if (__cs.getObjectIds() == INTS2_ARG) { interval = new Interval ( PyAny_AsLong(arg0) , PyAny_AsLong(arg1) ); }
    else if (__cs.getObjectIds() == INTV_ARG ) { interval = new Interval ( *PYINTERVAL_O(arg0) ); }
    else {
      PyErr_SetString(ConstructorError, "invalid number of parameters for Interval constructor." );
      return NULL;
    }
    HCATCH

    return PyInterval_Link( interval );
  }


  static int  PyInterval_Init ( PyInterval* self, PyObject* args, PyObject* kwargs )
  {
    cdebug.log(20) << "PyInterval_Init(): " << (void*)self << endl;
    return 0;
  }


  static PyObject* PyInterval_getUnion ( PyInterval *self, PyObject* args ) {
    cdebug.log(20) << "PyInterval_getUnion()" << endl;

    METHOD_HEAD ( "Interval.getUnion()" )

    Interval*   uinterval       = NULL;
    PyInterval* otherPyInterval = NULL;
    if (PyArg_ParseTuple(args,"O!:Interval.getUnion", &PyTypeInterval, &otherPyInterval)) {
      HTRY
      uinterval = new Interval ( interval->getUnion(*PYINTERVAL_O(otherPyInterval)));
      HCATCH
      return PyInterval_Link( uinterval );
    } else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Interval.getUnion.");
    }
    return NULL;
  }


  static PyObject* PyInterval_getIntersection ( PyInterval *self, PyObject* args ) {
    cdebug.log(20) << "PyInterval_getIntersection()" << endl;

    METHOD_HEAD ( "Interval.getIntersection()" )

    Interval*   iinterval       = NULL;
    PyInterval* otherPyInterval = NULL;
    if (PyArg_ParseTuple(args,"O!:Interval.getIntersection", &PyTypeInterval, &otherPyInterval)) {
      HTRY
        iinterval = new Interval(interval->getIntersection(*PYINTERVAL_O(otherPyInterval)));
      HCATCH
      return PyInterval_Link( iinterval );
    } else {
      PyErr_SetString(ConstructorError, "invalid number of parameters for Interval.getIntersection.");
    }
    return NULL;
  }


  static PyObject* PyInterval_contains ( PyInterval *self, PyObject* args ) {
    cdebug.log(20) << "PyInterval_contains ()" << endl;

    METHOD_HEAD( "Interval.contains()" )

    PyObject* arg0;
    bool      result = false;

    HTRY
    __cs.init("Interval.contains");
    if (not PyArg_ParseTuple(args,"|O&:Interval.contains",Converter,&arg0) )
      return NULL;

    if      (__cs.getObjectIds() == INT_ARG ) { result = interval->contains ( PyAny_AsLong(arg0) ); }
    else if (__cs.getObjectIds() == INTV_ARG) { result = interval->contains ( *PYINTERVAL_O(arg0) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Interval.contains constructor." );
      return NULL;
    }

    HCATCH
    if (result) Py_RETURN_TRUE;
    Py_RETURN_FALSE;
  }


  static PyObject* PyInterval_intersect ( PyInterval *self, PyObject* args ) {
    cdebug.log(20) << "PyInterval_intersect ()" << endl;
    
    bool result = false;
    HTRY
    PyInterval* pyInterval = NULL;
    METHOD_HEAD ( "Interval.intersect()" )

    if (PyArg_ParseTuple(args,"O!:Interval.intersects", &PyTypeInterval, &pyInterval)) {
      result = interval->intersect( *PYINTERVAL_O(pyInterval) );
    } else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Interval.intersect." );
      return NULL;
    }

    HCATCH
    if (result) Py_RETURN_TRUE;
    Py_RETURN_FALSE;
  }


  static PyObject* PyInterval_makeEmpty ( PyInterval *self, PyObject* args ) {
    cdebug.log(20) << "PyInterval_makeEmpty ()" << endl;
    
    METHOD_HEAD( "Interval.makeEmpty()" )
    interval->makeEmpty();

    Py_INCREF( self );
    return (PyObject*)self;
  }


  static PyObject* PyInterval_inflate ( PyInterval *self, PyObject* args ) {
    cdebug.log(20) << "PyInterval_inflate ()" << endl;

    METHOD_HEAD( "Interval.inflate()" )

    PyObject* arg0;
    PyObject* arg1;

    HTRY
    __cs.init( "Interval.inflate" );
    if (not PyArg_ParseTuple(args,"|O&O&:Interval.inflate",Converter,&arg0,Converter,&arg1))
      return NULL;

    if      (__cs.getObjectIds() == INT_ARG  ) { interval->inflate( PyAny_AsLong(arg0) ); }
    else if (__cs.getObjectIds() == INTS2_ARG) { interval->inflate( PyAny_AsLong(arg0), PyAny_AsLong(arg1) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Interval.inflate()" );
      return NULL;
    }
    HCATCH

    Py_INCREF( self );
    return (PyObject*)self;
  }


  static PyObject* PyInterval_merge ( PyInterval *self, PyObject* args ) {
    cdebug.log(20) << "PyInterval_merge ()" << endl;

    METHOD_HEAD( "Interval.merge()" )

    PyObject* arg0;
    PyObject* arg1;

    HTRY
    __cs.init( "Interval.merge" );
    if (not PyArg_ParseTuple(args,"|O&O&:Interval.merge",Converter,&arg0,Converter,&arg1))
      return NULL;

    if      (__cs.getObjectIds() == INT_ARG ) { interval->merge( PyAny_AsLong(arg0) ); }
    else if (__cs.getObjectIds() == INTV_ARG) { interval->merge( *PYINTERVAL_O(arg0) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Interval.merge()" );
      return NULL;
    }
    HCATCH

    Py_INCREF( self );
    return (PyObject*)self;
  }


  static PyObject* PyInterval_intersection ( PyInterval *self, PyObject* args ) {
    cdebug.log(20) << "PyInterval_intersection ()" << endl;

    METHOD_HEAD( "Interval.intersection()" )

    PyObject* arg0;
    PyObject* arg1;

    HTRY
    __cs.init( "Interval.intersection" );
    if (not PyArg_ParseTuple(args,"|O&O&:Interval.intersection",Converter,&arg0,Converter,&arg1))
      return NULL;

    if      (__cs.getObjectIds() == INTV_ARG ) { interval->intersection( *PYINTERVAL_O(arg0) ); }
    else if (__cs.getObjectIds() == INTS2_ARG) { interval->intersection( PyAny_AsLong(arg0), PyAny_AsLong(arg1) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Interval.intersection()" );
      return NULL;
    }
    HCATCH

    Py_INCREF( self );
    return (PyObject*)self;
  }


  static PyObject* PyInterval_translate ( PyInterval *self, PyObject* args ) {
    cdebug.log(20) << "PyInterval_translate ()" << endl;
    
    HTRY
    METHOD_HEAD( "Interval.translate()" )
    DbU::Unit delta = 0;
    if (PyArg_ParseTuple(args,"l:Interval.translate", &delta)) {
      interval->translate(delta);
    } else {
      PyErr_SetString( ConstructorError, "invalid number of parameters for Interval.translate()" );
      return NULL;
    }
    HCATCH

    Py_INCREF( self );
    return (PyObject*)self;
  }


  // ---------------------------------------------------------------
  // PyInterval Attribute Method table.

  PyMethodDef PyInterval_Methods[] =
    { { "getVMin"        , (PyCFunction)PyInterval_getVMin        , METH_NOARGS , "Return the VMin value." }
    , { "getVMax"        , (PyCFunction)PyInterval_getVMax        , METH_NOARGS , "Return the VMax value." }
    , { "getCenter"      , (PyCFunction)PyInterval_getCenter      , METH_NOARGS , "Return the interval center value." }
    , { "getSize"        , (PyCFunction)PyInterval_getSize        , METH_NOARGS , "Return the interval length." }
    , { "getHalfSize"    , (PyCFunction)PyInterval_getHalfSize    , METH_NOARGS , "Return the interval half length." }
    , { "getUnion"       , (PyCFunction)PyInterval_getUnion       , METH_VARARGS, "Return the smallest enclosing interval." }
    , { "getIntersection", (PyCFunction)PyInterval_getIntersection, METH_VARARGS, "Return the overlapping interval." }
    , { "isEmpty"        , (PyCFunction)PyInterval_isEmpty        , METH_NOARGS , "Return true if the interval is empty." }
    , { "isPonctual"     , (PyCFunction)PyInterval_isPonctual     , METH_NOARGS , "Return true if the interval reduced to a point." }
    , { "contains"       , (PyCFunction)PyInterval_contains       , METH_VARARGS, "Return true if the interval contains the argument." }
    , { "intersect"      , (PyCFunction)PyInterval_intersect      , METH_VARARGS, "Return true if two intervales overlap." }
    , { "makeEmpty"      , (PyCFunction)PyInterval_makeEmpty      , METH_NOARGS , "Transform the interval in an empty one." }
    , { "inflate"        , (PyCFunction)PyInterval_inflate        , METH_VARARGS, "Expand the interval of the given values." }
    , { "merge"          , (PyCFunction)PyInterval_merge          , METH_VARARGS, "Expand or contract the interval to contains the arguments." }
    , { "intersection"   , (PyCFunction)PyInterval_intersection   , METH_VARARGS, "Expand the interval to contains the arguments.." }
    , { "translate"      , (PyCFunction)PyInterval_translate      , METH_VARARGS, "translate the interval." }
    , { "destroy"        , (PyCFunction)PyInterval_destroy        , METH_NOARGS
                         , "Destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}  /* sentinel */
    };

  // x-------------------------------------------------------------x
  // |               "PyInterval" Object Methods                   |
  // x-------------------------------------------------------------x


  DirectDeleteMethod(PyInterval_DeAlloc,PyInterval)
  PyTypeObjectLinkPyTypeNewInit(Interval)
//PyTypeObjectLinkPyType(Interval)

#else  // End of Python Module Code Part.


// x=================================================================x
// |             "PyInterval" Shared Library Code Part               |
// x=================================================================x


  // ---------------------------------------------------------------
  // PyInterval Object Definitions.
  
  LinkCreateMethod(Interval)
  PyTypeObjectDefinitions(Interval)

# endif  // End of Shared Library Code Part.

}  // End of extern "C".

}  // End of Isobar namespace.
 
