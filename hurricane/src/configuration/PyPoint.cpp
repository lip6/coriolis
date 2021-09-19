// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2020-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyPoint.cpp"                            |
// +-----------------------------------------------------------------+


#include "hurricane/configuration/PyPoint.h"


namespace  Isobar3 {

using namespace Hurricane;

extern "C" {


// Thin wrappers for overloadeds member functions.

  static Point* ctorPoint0 ()
  {
    // cerr << "Point* ctorPoint()" << endl;
    return new Point();
  }

  static Point* ctorPoint1 ( Point* other )
  {
    // cerr << "Point* ctorPoint(Point*)" << endl;
    return new Point( *other );
  }

  static Point* ctorPoint2 ( DbU::Unit x, DbU::Unit y )
  {
    // cerr << "Point* ctorPoint(DbU::Unit,DbU::Unit)" << endl;
    return new Point( x, y );
  }


// Python methods.

  static PyObject* PyPoint_getX ( PyVoidPointer* self, PyObject* args )
  { return callMethod("Point.getX",&Point::getX,self,args); }

  static PyObject* PyPoint_getY ( PyVoidPointer* self, PyObject* args )
  { return callMethod("Point.getY",&Point::getY,self,args); }

  static PyObject* PyPoint_setX ( PyVoidPointer* self, PyObject* args )
  { return callMethod("Point.setX",&Point::setX,self,args); }

  static PyObject* PyPoint_setY ( PyVoidPointer* self, PyObject* args )
  { return callMethod("Point.setY",&Point::setY,self,args); }

  PyObject* PyPoint_NEW ( PyTypeObject* pyType, PyObject* args, PyObject* kwargs )
  {
    // cerr << "PyPoint_NEW() args=" << PyTuple_Size(args)
    //      << " kwargs=" << PyDict_Size(kwargs)<< endl;
    PyObject*       rvalue = callFunction("Point",&ctorPoint2,args);
    // cerr << "  after ctorPoint2() rvalue=" << rvalue << endl;
    if (not rvalue) rvalue = callFunction("Point",&ctorPoint1,args);
    // cerr << "  after ctorPoint1() rvalue=" << rvalue << endl;
    if (not rvalue) rvalue = callFunction("Point",&ctorPoint0,args);
    // cerr << "  after ctorPoint0() rvalue=" << rvalue << endl;
    return rvalue;
  }

  int  PyPoint_Init ( PyObject* self, PyObject* args, PyObject* kwargs )
  {
    cdebug_log(20,0) << "PyPoint_Init(): " << (void*)self << endl;
    return 0;
  }


  // ---------------------------------------------------------------
  // PyPoint Method table.

  PyMethodDef PyPoint_Methods[] =
    { { "getX"              , (PyCFunction)PyPoint_getX             , METH_NOARGS , "Return the Point X value." }
    , { "getY"              , (PyCFunction)PyPoint_getY             , METH_NOARGS , "Return the Point Y value." }
    , { "setX"              , (PyCFunction)PyPoint_setX             , METH_VARARGS, "Modify the Point X value." }
    , { "setY"              , (PyCFunction)PyPoint_setY             , METH_VARARGS, "Modify the Point Y value." }
  //, { "translate"         , (PyCFunction)PyPoint_translate        , METH_VARARGS, "Translate the point of dx and dy." }
  //, { "manhattanDistance" , (PyCFunction)PyPoint_manhattanDistance, METH_VARARGS, "Compute the Manhattan distance between the two points." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


}  // extern "C".

}  // Isobar3 namespace.
