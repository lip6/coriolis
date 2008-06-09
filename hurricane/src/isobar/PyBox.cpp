
// -*- C++ -*-
//
// This file is part of the Coriolis Project.
// Copyright (C) Laboratoire LIP6 - Departement ASIM
// Universite Pierre et Marie Curie
//
// Main contributors :
//        Christophe Alexandre   <Christophe.Alexandre@lip6.fr>
//        Sophie Belloeil             <Sophie.Belloeil@lip6.fr>
//        Hugo Clément                   <Hugo.Clement@lip6.fr>
//        Jean-Paul Chaput           <Jean-Paul.Chaput@lip6.fr>
//        Damien Dupuis                 <Damien.Dupuis@lip6.fr>
//        Christian Masson           <Christian.Masson@lip6.fr>
//        Marek Sroka                     <Marek.Sroka@lip6.fr>
// 
// The  Coriolis Project  is  free software;  you  can redistribute it
// and/or modify it under the  terms of the GNU General Public License
// as published by  the Free Software Foundation; either  version 2 of
// the License, or (at your option) any later version.
// 
// The  Coriolis Project is  distributed in  the hope that it  will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY  or FITNESS FOR  A PARTICULAR PURPOSE.   See the
// GNU General Public License for more details.
// 
// You should have  received a copy of the  GNU General Public License
// along with the Coriolis Project; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA
//
// License-Tag
// Authors-Tag
// ===================================================================
//
// $Id: PyBox.cpp,v 1.18 2006/09/22 11:27:30 tsunami Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyBox.cpp"                              |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#include "hurricane/isobar/PyPoint.h"
#include "hurricane/isobar/PyBox.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


#define  METHOD_HEAD(function)  GENERIC_METHOD_HEAD(Box,box,function)


// x=================================================================x
// |                 "PyBox" Python Module Code Part                 |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |                "PyBox" Attribute Methods                    |
  // x-------------------------------------------------------------x


  // Standart Accessors (Attributes).
  DirectGetLongAttribute(PyBox_getXMin      ,getXMin      ,PyBox,Box)
  DirectGetLongAttribute(PyBox_getYMin      ,getYMin      ,PyBox,Box)
  DirectGetLongAttribute(PyBox_getXMax      ,getXMax      ,PyBox,Box)
  DirectGetLongAttribute(PyBox_getYMax      ,getYMax      ,PyBox,Box)
  DirectGetLongAttribute(PyBox_getXCenter   ,getXCenter   ,PyBox,Box)
  DirectGetLongAttribute(PyBox_getYCenter   ,getYCenter   ,PyBox,Box)
  DirectGetLongAttribute(PyBox_getWidth     ,getWidth     ,PyBox,Box)
  DirectGetLongAttribute(PyBox_getHalfWidth ,getHalfWidth ,PyBox,Box)
  DirectGetLongAttribute(PyBox_getHeight    ,getHeight    ,PyBox,Box)
  DirectGetLongAttribute(PyBox_getHalfHeight,getHalfHeight,PyBox,Box)


  // Standart Predicates (Attributes).
  DirectGetBoolAttribute(PyBox_isEmpty   ,isEmpty   ,PyBox,Box)
  DirectGetBoolAttribute(PyBox_isFlat    ,isFlat    ,PyBox,Box)
  DirectGetBoolAttribute(PyBox_isPonctual,isPonctual,PyBox,Box)


  // Standart Destroy (Attribute).
  DirectDestroyAttribute(PyBox_destroy, PyBox)




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyBox_getCenter ()"

  static PyObject* PyBox_getCenter ( PyBox *self ) {
    trace << "PyBox_getCenter()" << endl;

    METHOD_HEAD ( "Box.Center()" )

    PyPoint* pyPoint = PyObject_NEW ( PyPoint, &PyTypePoint );
    if (pyPoint == NULL) { return NULL; }
    
    HTRY
    pyPoint->_object = new Point ( box->getCenter() );
    HCATCH    

    return ( (PyObject*)pyPoint );
  }





  // ---------------------------------------------------------------
  // Attribute Method  :  "PyBox_getUnion ()"

  static PyObject* PyBox_getUnion ( PyBox *self, PyObject* args )
  {
    trace << "PyBox_getUnion()" << endl;

    METHOD_HEAD ( "Box.getUnion()" )

    PyBox* otherPyBox;
    if ( ! ParseOneArg( "Box.getUnion", args, BOX_ARG, (PyObject**)&otherPyBox ) ) return ( NULL );

    PyBox* unionPyBox = PyObject_NEW ( PyBox, &PyTypeBox );
    if (unionPyBox == NULL) { return NULL; }
    
    HTRY
    unionPyBox->_object = new Box ( box->getUnion(*(otherPyBox->_object)) );
    HCATCH

    return ( (PyObject*)unionPyBox );
  }





  // ---------------------------------------------------------------
  // Attribute Method  :  "PyBox_getIntersection ()"

  static PyObject* PyBox_getIntersection ( PyBox *self, PyObject* args )
  {
    trace << "PyBox_getIntersection()" << endl;

    METHOD_HEAD ( "Box.getIntersection()" )

    PyBox* otherPyBox;
    if ( ! ParseOneArg ( "Box.getIntersection", args, BOX_ARG, (PyObject**)&otherPyBox ) ) return ( NULL );

    PyBox* intersectionPyBox = PyObject_NEW ( PyBox, &PyTypeBox );
    if (intersectionPyBox == NULL) { return NULL; }

    HTRY
    intersectionPyBox->_object = new Box ( box->getIntersection(*(otherPyBox->_object)) );
    HCATCH
    
    return ( (PyObject*)intersectionPyBox );
  }





  // ---------------------------------------------------------------
  // Attribute Method  :  "PyBox_contains ()"

  static PyObject* PyBox_contains ( PyBox *self, PyObject* args )
  {
    trace << "PyBox_contains ()" << endl;
    
    METHOD_HEAD ( "Box.contains()" )

    PyObject* arg0;
    PyObject* arg1;
    long      result = 0;

    HTRY

    __cs.Init ("Box.contains");
    if ( ! PyArg_ParseTuple(args,"|O&O&:Box.contains",Converter,&arg0,Converter,&arg1) )
      return ( NULL );

    if      ( __cs.getObjectIds() == BOX_ARG   ) { result = box->contains ( *PYBOX_O(arg0) ); }
    else if ( __cs.getObjectIds() == POINT_ARG ) { result = box->contains ( *PYPOINT_O(arg0) ); }
    else if ( __cs.getObjectIds() == INTS2_ARG ) { result = box->contains ( PyInt_AsLong(arg0)
                                                                          , PyInt_AsLong(arg1) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Box.contains constructor." );
      return ( NULL );
    }

    HCATCH

    return ( Py_BuildValue("i",result) );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyBox_intersect ()"

  static PyObject* PyBox_intersect ( PyBox *self, PyObject* args )
  {
    trace << "PyBox_intersect ()" << endl;
    
    METHOD_HEAD ( "Box.intersect()" )

    PyBox* otherPyBox;

    HTRY
    if ( ! ParseOneArg ( "Box.intersect", args, BOX_ARG, (PyObject**)&otherPyBox ) ) return ( NULL );
    HCATCH

    return ( Py_BuildValue("i",(long)box->intersect(*PYBOX_O(otherPyBox))) );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyBox_isConstrainedBy ()"

  static PyObject* PyBox_isConstrainedBy ( PyBox *self, PyObject* args )
  {
    trace << "PyBox_isConstrainedBy ()" << endl;
    
    METHOD_HEAD ( "Box.isConstrainedBy()" )

    PyBox* otherPyBox;

    HTRY
    if ( ! ParseOneArg ( "Box.getisConstrainedBy", args, BOX_ARG, (PyObject**)&otherPyBox ) ) return ( NULL );
    HCATCH

    return ( Py_BuildValue("i",(long)box->isConstrainedBy(*PYBOX_O(otherPyBox))) );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyBox_makeEmpty ()"

  static PyObject* PyBox_makeEmpty ( PyBox *self, PyObject* args )
  {
    trace << "PyBox_makeEmpty ()" << endl;

    HTRY
    METHOD_HEAD ( "Box.makeEmpty()" )

    if ( ! PyArg_ParseTuple ( args,":Box" ) ) return ( NULL );
    box->makeEmpty ();
    HCATCH
    
    Py_INCREF ( self );
    return ( (PyObject*)self );
  }





  // ---------------------------------------------------------------
  // Attribute Method  :  "PyBox_inflate ()"

  static PyObject* PyBox_inflate ( PyBox *self, PyObject* args )
  {
    trace << "PyBox_inflate ()" << endl;
    
    METHOD_HEAD ( "Box.inflate()" )

    PyObject* arg0;
    PyObject* arg1;
    PyObject* arg2 = NULL;
    PyObject* arg3 = NULL;

    HTRY

    __cs.Init ("Box.inflate");
    if ( ! PyArg_ParseTuple(args,"|O&O&O&O&:Box.inflate",Converter,&arg0,Converter,&arg1) )
      return ( NULL );

    if      ( __cs.getObjectIds() == INT_ARG   ) { box->inflate ( PyInt_AsLong(arg0) ); }
    else if ( __cs.getObjectIds() == INTS2_ARG ) { box->inflate ( PyInt_AsLong(arg0)
                                                                , PyInt_AsLong(arg1) ); }
    else if ( __cs.getObjectIds() == INTS4_ARG ) { box->inflate ( PyInt_AsLong(arg0)
                                                                , PyInt_AsLong(arg1)
                                                                , PyInt_AsLong(arg2)
                                                                , PyInt_AsLong(arg3) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Box.inflate()" );
      return ( NULL );
    }

    HCATCH

    Py_INCREF ( self );
    return ( (PyObject*)self );
  }





  // ---------------------------------------------------------------
  // Attribute Method  :  "PyBox_merge ()"

  static PyObject* PyBox_merge ( PyBox *self, PyObject* args )
  {
    trace << "Box_merge()" << endl;
    
    METHOD_HEAD ( "Box.merge()" )
    
    PyObject* arg0;
    PyObject* arg1;
    PyObject* arg2;
    PyObject* arg3;

    HTRY

    __cs.Init ("Box.merge");
    if ( ! PyArg_ParseTuple(args,"|O&O&O&O&:Box.merge",Converter,&arg0,Converter,&arg1,Converter,&arg2,Converter,&arg3) )
      return ( NULL );

    if      ( __cs.getObjectIds() == POINT_ARG ) { box->merge ( *PYPOINT_O(arg0) ); }
    else if ( __cs.getObjectIds() == BOX_ARG   ) { box->merge ( *PYBOX_O(arg0) ); }
    else if ( __cs.getObjectIds() == INTS2_ARG ) { box->merge ( PyInt_AsLong(arg0)
                                                              , PyInt_AsLong(arg1) ); }
    else if ( __cs.getObjectIds() == INTS4_ARG ) { box->merge ( PyInt_AsLong(arg0)
                                                              , PyInt_AsLong(arg1)
                                                              , PyInt_AsLong(arg2)
                                                              , PyInt_AsLong(arg3) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Box.merge()" );
      return ( NULL );
    }
    
    HCATCH

    Py_INCREF ( self );
    return ( (PyObject*)self );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyBox_translate ()"

  static PyObject* PyBox_translate ( PyBox *self, PyObject* args )
  {
    trace << "PyBox_translate ()" << endl;
    
    METHOD_HEAD ( "Box.translate()" )

    PyObject* arg0;
    PyObject* arg1;

    HTRY
    if ( ! ParseTwoArg ( "Box.translate", args, INTS2_ARG, &arg0, &arg1 ) ) return ( NULL );
    box->translate ( PyInt_AsLong(arg0), PyInt_AsLong(arg1) );
    HCATCH
    Py_RETURN_NONE;
  }




  // ---------------------------------------------------------------
  // PyBox Attribute Method table.

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
    , { "translate"      , (PyCFunction)PyBox_translate      , METH_VARARGS, "translate the box od dx ans dy." }
    , { "destroy"        , (PyCFunction)PyBox_destroy        , METH_NOARGS
                         , "Destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyBox_new ()"

  PyObject* PyBox_new (PyTypeObject *type, PyObject *args, PyObject *kwds) {
    trace << "PyBox_new()" << endl;

    Box*      box;
    PyObject* arg0;
    PyObject* arg1;
    PyObject* arg2;
    PyObject* arg3;

    __cs.Init ("Box.new");
    if ( ! PyArg_ParseTuple(args,"|O&O&O&O&:Box.new"
                           ,Converter,&arg0
                           ,Converter,&arg1
                           ,Converter,&arg2
                           ,Converter,&arg3
                           ) ) return ( NULL );

    if      ( __cs.getObjectIds() == NO_ARG      ) { box = new Box (); }
    else if ( __cs.getObjectIds() == POINT_ARG   ) { box = new Box ( *PYPOINT_O(arg0) ); }
    else if ( __cs.getObjectIds() == BOX_ARG     ) { box = new Box ( *PYBOX_O(arg0) ); }
    else if ( __cs.getObjectIds() == POINTS2_ARG ) { box = new Box ( *PYPOINT_O(arg0)
                                                                   , *PYPOINT_O(arg1) ); }
    else if ( __cs.getObjectIds() == INTS2_ARG   ) { box = new Box ( PyInt_AsLong(arg0) 
                                                                   , PyInt_AsLong(arg1) ); }
    else if ( __cs.getObjectIds() == INTS4_ARG   ) { box = new Box ( PyInt_AsLong(arg0)
                                                                   , PyInt_AsLong(arg1)
                                                                   , PyInt_AsLong(arg2)
                                                                   , PyInt_AsLong(arg3) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Box constructor." );
      return ( NULL );
    }

    PyBox* pyBox = PyObject_NEW(PyBox, &PyTypeBox);
    if (pyBox == NULL) return NULL;

    trace_in ();
    trace << "new PyBox [" << hex << pyBox << "]" << endl;
    trace_out ();
    
    HTRY

    pyBox->_object = box;
    //box->SetProxy ( (void**)&(pyBox->_object) );

    HCATCH

    return ( (PyObject*)pyBox );
  }



  // x-------------------------------------------------------------x
  // |                  "PyBox" Object Methods                     |
  // x-------------------------------------------------------------x


  DirectDeleteMethod(PyBox_DeAlloc,PyBox)
  PyTypeObjectLinkPyType(Box)
  PyTypeObjectConstructor(Box)

#else  // End of Python Module Code Part.


// x=================================================================x
// |                "PyBox" Shared Library Code Part                 |
// x=================================================================x
  





  // ---------------------------------------------------------------
  // PyBox Object Definitions.
  
  PyTypeRootObjectDefinitions(Box)

# endif  // End of Shared Library Code Part.




}  // End of extern "C".





}  // End of Isobar namespace.
 
