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
// |  C++ Module  :       "./PyBox.cpp"                              |
// +-----------------------------------------------------------------+


#include "hurricane/configuration/PyPoint.h"
#include "hurricane/configuration/PyBox.h"


namespace  Isobar3 {

using namespace Hurricane;

extern "C" {


// Thin wrappers for overloadeds member functions.

  static Box* ctorBox0 ()
  { return new Box(); }

  static Box* ctorBox1 ( Point p )
  { return new Box( p ); }

  static Box* ctorBox2u ( DbU::Unit x, DbU::Unit y )
  { return new Box( x, y ); }

  static Box* ctorBox2p ( Point p1, Point p2 )
  { return new Box( p1, p2 ); }

  static Box* ctorBox4 ( DbU::Unit x1, DbU::Unit y1, DbU::Unit x2, DbU::Unit y2  )
  { return new Box( x1, y1, x2, y2 ); }

  static Box& inflate4 ( Box* box, DbU::Unit dxMin, DbU::Unit dyMin, DbU::Unit dxMax, DbU::Unit dyMax )
  { return box->inflate( dxMin, dyMin, dxMax, dyMax ); }

  static Box& inflate2 ( Box* box, DbU::Unit dx, DbU::Unit dy )
  { return box->inflate( dx, dy ); }

  static Box& inflate1 ( Box* box, DbU::Unit dxy )
  { return box->inflate( dxy ); }

  static Box& merge4 ( Box* box, DbU::Unit x1, DbU::Unit y1, DbU::Unit x2, DbU::Unit y2 )
  {
    cerr << "Box_merge4() called." << endl;
    return box->merge( x1, y1, x2, y2 );
  }

  static Box& merge2 ( Box* box, DbU::Unit x, DbU::Unit y )
  {
    cerr << "Box_merge2() called." << endl;
    return box->merge( x, y );
  }

  static Box& merge1p ( Box* box, Point p )
  {
    cerr << "Box_merge1p() called." << endl;
    return box->merge( p );
  }

  static Box& merge1b ( Box* box, Box b )
  {
    cerr << "Box_merge1b() called." << endl;
    return box->merge( b );
  }

  static bool  contains2 ( Box* box, DbU::Unit x, DbU::Unit y )
  { return box->contains( x, y ); }

  static bool  contains1p ( Box* box, Point p )
  { return box->contains( p ); }

  static bool  contains1b ( Box* box, Box b )
  { return box->contains( b ); }


// Python methods.

  static PyObject* PyBox_isEmpty ( PyVoidPointer* self )
  { return callMethod("Box.isEmpty",&Box::isEmpty,self,NULL); }

  static PyObject* PyBox_isFlat ( PyVoidPointer* self )
  { return callMethod("Box.isFlat",&Box::isFlat,self,NULL); }

  static PyObject* PyBox_isPonctual ( PyVoidPointer* self )
  { return callMethod("Box.isPonctual",&Box::isPonctual,self,NULL); }

  static PyObject* PyBox_getXMin ( PyVoidPointer* self, PyObject* args )
  { return callMethod("Box.getXMin",&Box::getXMin,self,args); }

  static PyObject* PyBox_getYMin ( PyVoidPointer* self, PyObject* args )
  { return callMethod("Box.getYMin",&Box::getYMin,self,args); }

  static PyObject* PyBox_getXMax ( PyVoidPointer* self, PyObject* args )
  { return callMethod("Box.getXMax",&Box::getXMax,self,args); }

  static PyObject* PyBox_getYMax ( PyVoidPointer* self, PyObject* args )
  { return callMethod("Box.getYMax",&Box::getYMax,self,args); }

  static PyObject* PyBox_getXCenter ( PyVoidPointer* self, PyObject* args )
  { return callMethod("Box.getXCenter",&Box::getXCenter,self,args); }

  static PyObject* PyBox_getYCenter ( PyVoidPointer* self, PyObject* args )
  { return callMethod("Box.getYCenter",&Box::getYCenter,self,args); }

  static PyObject* PyBox_getCenter ( PyVoidPointer* self, PyObject* args )
  { return callMethod("Box.getCenter",&Box::getCenter,self,args); }

  static PyObject* PyBox_getWidth ( PyVoidPointer* self, PyObject* args )
  { return callMethod("Box.getWidth",&Box::getWidth,self,args); }

  static PyObject* PyBox_getHalfWidth ( PyVoidPointer* self, PyObject* args )
  { return callMethod("Box.getHalfWidth",&Box::getHalfWidth,self,args); }

  static PyObject* PyBox_getHeight ( PyVoidPointer* self, PyObject* args )
  { return callMethod("Box.getHeight",&Box::getHeight,self,args); }

  static PyObject* PyBox_getHalfHeight ( PyVoidPointer* self, PyObject* args )
  { return callMethod("Box.getHalfHeight",&Box::getHalfHeight,self,args); }

  static PyObject* PyBox_getUnion ( PyVoidPointer* self, PyObject* args )
  { return callMethod("Box.getUnion",&Box::getUnion,self,args); }

  static PyObject* PyBox_getIntersection ( PyVoidPointer* self, PyObject* args )
  { return callMethod("Box.getIntersection",&Box::getIntersection,self,args); }

  static PyObject* PyBox_intersect ( PyVoidPointer* self, PyObject* args )
  { return callMethod("Box.intersect",&Box::intersect,self,args); }

  static PyObject* PyBox_isConstrainedBy ( PyVoidPointer* self, PyObject* args )
  { return callMethod("Box.isConstrainedBy",&Box::isConstrainedBy,self,args); }

  static PyObject* PyBox_inflate ( PyVoidPointer* self, PyObject* args )
  {
    PyObject*       rvalue = callMethod("Box.inflate",&inflate4,self,args);
    if (not rvalue) rvalue = callMethod("Box.inflate",&inflate2,self,args);
    if (not rvalue) rvalue = callMethod("Box.inflate",&inflate1,self,args);
    return rvalue;
  }

  static PyObject* PyBox_merge ( PyVoidPointer* self, PyObject* args )
  {
    PyObject*       rvalue = callMethod("Box.merge4",&merge4 ,self,args);
    if (not rvalue) rvalue = callMethod("Box.merge2",&merge2 ,self,args);
    if (not rvalue) rvalue = callMethod("Box.merge1b",&merge1b,self,args);
    if (not rvalue) rvalue = callMethod("Box.merge1p",&merge1p,self,args);
    return rvalue;
  }

  static PyObject* PyBox_contains ( PyVoidPointer* self, PyObject* args )
  {
    PyObject*       rvalue = callMethod("Box.contains",&contains2 ,self,args);
    if (not rvalue) rvalue = callMethod("Box.contains",&contains1b,self,args);
    if (not rvalue) rvalue = callMethod("Box.contains",&contains1p,self,args);
    return rvalue;
  }

  static PyObject* PyBox_makeEmpty ( PyVoidPointer* self, PyObject* args )
  { return callMethod("Box.makeEmpty",&Box::makeEmpty,self,args); }

  PyObject* PyBox_NEW ( PyTypeObject* pyType, PyObject* args, PyObject* kwargs )
  {
    PyObject*       rvalue = callFunction("Box",&ctorBox4 ,args);
    if (not rvalue) rvalue = callFunction("Box",&ctorBox2p,args);
    if (not rvalue) rvalue = callFunction("Box",&ctorBox2u,args);
    if (not rvalue) rvalue = callFunction("Box",&ctorBox1 ,args);
    if (not rvalue) rvalue = callFunction("Box",&ctorBox0 ,args);
    return rvalue;
  }

  int  PyBox_Init ( PyObject* self, PyObject* args, PyObject* kwargs )
  {
    cdebug_log(20,0) << "PyBox_Init(): " << (void*)self << endl;
    return 0;
  }


  // ---------------------------------------------------------------
  // PyBox Method table.

  PyMethodDef PyBox_Methods[] =
    { { "getXMin"        , (PyCFunction)PyBox_getXMin        , METH_NOARGS , "Return the XMin value." }
    , { "getYMin"        , (PyCFunction)PyBox_getYMin        , METH_NOARGS , "Return the YMin value." }
    , { "getXMax"        , (PyCFunction)PyBox_getXMax        , METH_NOARGS , "Return the XMax value." }
    , { "getYMax"        , (PyCFunction)PyBox_getYMax        , METH_NOARGS , "Return the YMax value." }
    , { "getXCenter"     , (PyCFunction)PyBox_getXCenter     , METH_NOARGS , "Return the X box center value." }
    , { "getYCenter"     , (PyCFunction)PyBox_getYCenter     , METH_NOARGS , "Return the Y box center value." }
    , { "getCenter"      , (PyCFunction)PyBox_getCenter      , METH_NOARGS , "Return the box center Point." }
    , { "getWidth"       , (PyCFunction)PyBox_getWidth       , METH_NOARGS , "Return the box width." }
    , { "getHalfWidth"   , (PyCFunction)PyBox_getHalfWidth   , METH_NOARGS , "Return the box half width." }
    , { "getHeight"      , (PyCFunction)PyBox_getHeight      , METH_NOARGS , "Return the box height." }
    , { "getHalfHeight"  , (PyCFunction)PyBox_getHalfHeight  , METH_NOARGS , "Return the box half height." }
    , { "getUnion"       , (PyCFunction)PyBox_getUnion       , METH_VARARGS, "Return the smallest enclosing box." }
    , { "getIntersection", (PyCFunction)PyBox_getIntersection, METH_VARARGS, "Return the overlapping area." }
    , { "isEmpty"        , (PyCFunction)PyBox_isEmpty        , METH_NOARGS , "Return true if the box is empty." }
    , { "isFlat"         , (PyCFunction)PyBox_isFlat         , METH_NOARGS , "Return true if the box is flat." }
    , { "isPonctual"     , (PyCFunction)PyBox_isPonctual     , METH_NOARGS , "Return true if the box reduced to a point." }
    , { "contains"       , (PyCFunction)PyBox_contains       , METH_VARARGS, "Return true if the box contains the argument." }
    , { "intersect"      , (PyCFunction)PyBox_intersect      , METH_VARARGS, "Return true if two boxes overlap." }
    , { "isConstrainedBy", (PyCFunction)PyBox_isConstrainedBy, METH_VARARGS, "Return true if the argment box is included and share at least a side." }
    , { "makeEmpty"      , (PyCFunction)PyBox_makeEmpty      , METH_NOARGS , "Transform the box in an empty one." }
    , { "inflate"        , (PyCFunction)PyBox_inflate        , METH_VARARGS, "Expand the box to contains the arguments." }
    , { "merge"          , (PyCFunction)PyBox_merge          , METH_VARARGS, "Expand or contract the box to contains the arguments." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


}  // extern "C".

}  // Isobar3 namespace.
