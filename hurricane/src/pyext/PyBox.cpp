
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




# include  "PyPoint.h"
# include  "PyBox.h"


namespace  Isobar {


USING_NAMESPACE_HURRICANE

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
  DirectGetLongAttribute(PyBox_GetXMin      ,getXMin      ,PyBox,Box)
  DirectGetLongAttribute(PyBox_GetYMin      ,getYMin      ,PyBox,Box)
  DirectGetLongAttribute(PyBox_GetXMax      ,getXMax      ,PyBox,Box)
  DirectGetLongAttribute(PyBox_GetYMax      ,getYMax      ,PyBox,Box)
  DirectGetLongAttribute(PyBox_GetXCenter   ,getXCenter   ,PyBox,Box)
  DirectGetLongAttribute(PyBox_GetYCenter   ,getYCenter   ,PyBox,Box)
  DirectGetLongAttribute(PyBox_GetWidth     ,getWidth     ,PyBox,Box)
  DirectGetLongAttribute(PyBox_GetHalfWidth ,getHalfWidth ,PyBox,Box)
  DirectGetLongAttribute(PyBox_GetHeight    ,getHeight    ,PyBox,Box)
  DirectGetLongAttribute(PyBox_GetHalfHeight,getHalfHeight,PyBox,Box)


  // Standart Predicates (Attributes).
  DirectGetBoolAttribute(PyBox_IsEmpty   ,isEmpty   ,PyBox,Box)
  DirectGetBoolAttribute(PyBox_IsFlat    ,isFlat    ,PyBox,Box)
  DirectGetBoolAttribute(PyBox_IsPonctual,isPonctual,PyBox,Box)


  // Standart Delete (Attribute).
  DirectDeleteAttribute(PyBox_Delete,PyBox)




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyBox_GetCenter ()"

  static PyObject* PyBox_GetCenter ( PyBox *self )
  {
    trace << "PyBox_GetCenter()" << endl;

    METHOD_HEAD ( "Box.Center()" )

    PyPoint* pyPoint = PyObject_NEW ( PyPoint, &PyTypePoint );
    if (pyPoint == NULL) { return NULL; }
    
    HTRY
    pyPoint->_object = new Point ( box->getCenter() );
    HCATCH    

    return ( (PyObject*)pyPoint );
  }





  // ---------------------------------------------------------------
  // Attribute Method  :  "PyBox_GetUnion ()"

  static PyObject* PyBox_GetUnion ( PyBox *self, PyObject* args )
  {
    trace << "PyBox_GetUnion()" << endl;

    METHOD_HEAD ( "Box.GetUnion()" )

    PyBox* otherPyBox;
    if ( ! ParseOneArg( "Box.GetUnion", args, BOX_ARG, (PyObject**)&otherPyBox ) ) return ( NULL );

    PyBox* unionPyBox = PyObject_NEW ( PyBox, &PyTypeBox );
    if (unionPyBox == NULL) { return NULL; }
    
    HTRY
    unionPyBox->_object = new Box ( box->getUnion(*(otherPyBox->_object)) );
    HCATCH

    return ( (PyObject*)unionPyBox );
  }





  // ---------------------------------------------------------------
  // Attribute Method  :  "PyBox_GetIntersection ()"

  static PyObject* PyBox_GetIntersection ( PyBox *self, PyObject* args )
  {
    trace << "PyBox_GetIntersection()" << endl;

    METHOD_HEAD ( "Box.GetIntersection()" )

    PyBox* otherPyBox;
    if ( ! ParseOneArg ( "Box.GetIntersection", args, BOX_ARG, (PyObject**)&otherPyBox ) ) return ( NULL );

    PyBox* intersectionPyBox = PyObject_NEW ( PyBox, &PyTypeBox );
    if (intersectionPyBox == NULL) { return NULL; }

    HTRY
    intersectionPyBox->_object = new Box ( box->getIntersection(*(otherPyBox->_object)) );
    HCATCH
    
    return ( (PyObject*)intersectionPyBox );
  }





  // ---------------------------------------------------------------
  // Attribute Method  :  "PyBox_Contains ()"

  static PyObject* PyBox_Contains ( PyBox *self, PyObject* args )
  {
    trace << "PyBox_Contains ()" << endl;
    
    METHOD_HEAD ( "Box.Contains()" )

    PyObject* arg0;
    PyObject* arg1;
    long      result = 0;

    HTRY

    __cs.Init ("Box.Contains");
    if ( ! PyArg_ParseTuple(args,"|O&O&:Box.Contains",Converter,&arg0,Converter,&arg1) )
      return ( NULL );

    if      ( __cs.GetObjectIds() == BOX_ARG   ) { result = box->contains ( *PYBOX_O(arg0) ); }
    else if ( __cs.GetObjectIds() == POINT_ARG ) { result = box->contains ( *PYPOINT_O(arg0) ); }
    else if ( __cs.GetObjectIds() == INTS2_ARG ) { result = box->contains ( PyInt_AsLong(arg0)
                                                                          , PyInt_AsLong(arg1) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Box.Contains constructor." );
      return ( NULL );
    }

    HCATCH

    return ( Py_BuildValue("i",result) );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyBox_Intersect ()"

  static PyObject* PyBox_Intersect ( PyBox *self, PyObject* args )
  {
    trace << "PyBox_Intersect ()" << endl;
    
    METHOD_HEAD ( "Box.Intersect()" )

    PyBox* otherPyBox;

    HTRY
    if ( ! ParseOneArg ( "Box.Intersect", args, BOX_ARG, (PyObject**)&otherPyBox ) ) return ( NULL );
    HCATCH

    return ( Py_BuildValue("i",(long)box->intersect(*PYBOX_O(otherPyBox))) );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyBox_IsConstrainedBy ()"

  static PyObject* PyBox_IsConstrainedBy ( PyBox *self, PyObject* args )
  {
    trace << "PyBox_IsConstrainedBy ()" << endl;
    
    METHOD_HEAD ( "Box.IsConstrainedBy()" )

    PyBox* otherPyBox;

    HTRY
    if ( ! ParseOneArg ( "Box.GetIsConstrainedBy", args, BOX_ARG, (PyObject**)&otherPyBox ) ) return ( NULL );
    HCATCH

    return ( Py_BuildValue("i",(long)box->isConstrainedBy(*PYBOX_O(otherPyBox))) );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyBox_MakeEmpty ()"

  static PyObject* PyBox_MakeEmpty ( PyBox *self, PyObject* args )
  {
    trace << "PyBox_MakeEmpty ()" << endl;

    HTRY
    METHOD_HEAD ( "Box.MakeEmpty()" )

    if ( ! PyArg_ParseTuple ( args,":Box" ) ) return ( NULL );
    box->makeEmpty ();
    HCATCH
    
    Py_INCREF ( self );
    return ( (PyObject*)self );
  }





  // ---------------------------------------------------------------
  // Attribute Method  :  "PyBox_Inflate ()"

  static PyObject* PyBox_Inflate ( PyBox *self, PyObject* args )
  {
    trace << "PyBox_Inflate ()" << endl;
    
    METHOD_HEAD ( "Box.Inflate()" )

    PyObject* arg0;
    PyObject* arg1;
    PyObject* arg2 = NULL;
    PyObject* arg3 = NULL;

    HTRY

    __cs.Init ("Box.Inflate");
    if ( ! PyArg_ParseTuple(args,"|O&O&O&O&:Box.Inflate",Converter,&arg0,Converter,&arg1) )
      return ( NULL );

    if      ( __cs.GetObjectIds() == INT_ARG   ) { box->inflate ( PyInt_AsLong(arg0) ); }
    else if ( __cs.GetObjectIds() == INTS2_ARG ) { box->inflate ( PyInt_AsLong(arg0)
                                                                , PyInt_AsLong(arg1) ); }
    else if ( __cs.GetObjectIds() == INTS4_ARG ) { box->inflate ( PyInt_AsLong(arg0)
                                                                , PyInt_AsLong(arg1)
                                                                , PyInt_AsLong(arg2)
                                                                , PyInt_AsLong(arg3) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Box.Inflate()" );
      return ( NULL );
    }

    HCATCH

    Py_INCREF ( self );
    return ( (PyObject*)self );
  }





  // ---------------------------------------------------------------
  // Attribute Method  :  "PyBox_Merge ()"

  static PyObject* PyBox_Merge ( PyBox *self, PyObject* args )
  {
    trace << "Box_Merge()" << endl;
    
    METHOD_HEAD ( "Box.Merge()" )
    
    PyObject* arg0;
    PyObject* arg1;
    PyObject* arg2;
    PyObject* arg3;

    HTRY

    __cs.Init ("Box.Merge");
    if ( ! PyArg_ParseTuple(args,"|O&O&O&O&:Box.Merge",Converter,&arg0,Converter,&arg1,Converter,&arg2,Converter,&arg3) )
      return ( NULL );

    if      ( __cs.GetObjectIds() == POINT_ARG ) { box->merge ( *PYPOINT_O(arg0) ); }
    else if ( __cs.GetObjectIds() == BOX_ARG   ) { box->merge ( *PYBOX_O(arg0) ); }
    else if ( __cs.GetObjectIds() == INTS2_ARG ) { box->merge ( PyInt_AsLong(arg0)
                                                              , PyInt_AsLong(arg1) ); }
    else if ( __cs.GetObjectIds() == INTS4_ARG ) { box->merge ( PyInt_AsLong(arg0)
                                                              , PyInt_AsLong(arg1)
                                                              , PyInt_AsLong(arg2)
                                                              , PyInt_AsLong(arg3) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Box.Merge()" );
      return ( NULL );
    }
    
    HCATCH

    Py_INCREF ( self );
    return ( (PyObject*)self );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyBox_Translate ()"

  static PyObject* PyBox_Translate ( PyBox *self, PyObject* args )
  {
    trace << "PyBox_Translate ()" << endl;
    
    METHOD_HEAD ( "Box.Translate()" )

    PyObject* arg0;
    PyObject* arg1;

    HTRY
    if ( ! ParseTwoArg ( "Box.Translate", args, INTS2_ARG, &arg0, &arg1 ) ) return ( NULL );
    box->translate ( PyInt_AsLong(arg0), PyInt_AsLong(arg1) );
    HCATCH
    Py_RETURN_NONE;
  }




  // ---------------------------------------------------------------
  // PyBox Attribute Method table.

  PyMethodDef PyBox_Methods[] =
    { { "GetXMin"        , (PyCFunction)PyBox_GetXMin        , METH_NOARGS , "Return the XMin value." }
    , { "GetYMin"        , (PyCFunction)PyBox_GetYMin        , METH_NOARGS , "Return the YMin value." }
    , { "GetXMax"        , (PyCFunction)PyBox_GetXMax        , METH_NOARGS , "Return the XMax value." }
    , { "GetYMax"        , (PyCFunction)PyBox_GetYMax        , METH_NOARGS , "Return the YMax value." }
    , { "GetXCenter"     , (PyCFunction)PyBox_GetXCenter     , METH_NOARGS , "Return the X box center value." }
    , { "GetYCenter"     , (PyCFunction)PyBox_GetYCenter     , METH_NOARGS , "Return the Y box center value." }
    , { "GetCenter"      , (PyCFunction)PyBox_GetCenter      , METH_NOARGS , "Return the box center Point." }
    , { "GetWidth"       , (PyCFunction)PyBox_GetWidth       , METH_NOARGS , "Return the box width." }
    , { "GetHalfWidth"   , (PyCFunction)PyBox_GetHalfWidth   , METH_NOARGS , "Return the box half width." }
    , { "GetHeight"      , (PyCFunction)PyBox_GetHeight      , METH_NOARGS , "Return the box height." }
    , { "GetHalfHeight"  , (PyCFunction)PyBox_GetHalfHeight  , METH_NOARGS , "Return the box half height." }
    , { "GetUnion"       , (PyCFunction)PyBox_GetUnion       , METH_VARARGS, "Return the smallest enclosing box." }
    , { "GetIntersection", (PyCFunction)PyBox_GetIntersection, METH_VARARGS, "Return the overlapping area." }
    , { "IsEmpty"        , (PyCFunction)PyBox_IsEmpty        , METH_NOARGS , "Return true if the box is empty." }
    , { "IsFlat"         , (PyCFunction)PyBox_IsFlat         , METH_NOARGS , "Return true if the box is flat." }
    , { "IsPonctual"     , (PyCFunction)PyBox_IsPonctual     , METH_NOARGS , "Return true if the box reduced to a point." }
    , { "Contains"       , (PyCFunction)PyBox_Contains       , METH_VARARGS, "Return true if the box contains the argument." }
    , { "Intersect"      , (PyCFunction)PyBox_Intersect      , METH_VARARGS, "Return true if two boxes overlap." }
    , { "IsConstrainedBy", (PyCFunction)PyBox_IsConstrainedBy, METH_VARARGS, "Return true if the argment box is included and share at least a side." }
    , { "MakeEmpty"      , (PyCFunction)PyBox_MakeEmpty      , METH_NOARGS , "Transform the box in an empty one." }
    , { "Inflate"        , (PyCFunction)PyBox_Inflate        , METH_VARARGS, "Expand the box to contains the arguments." }
    , { "Merge"          , (PyCFunction)PyBox_Merge          , METH_VARARGS, "Expand or contract the box to contains the arguments." }
    , { "Translate"      , (PyCFunction)PyBox_Translate      , METH_VARARGS, "Translate the box od dx ans dy." }
    , { "Delete"         , (PyCFunction)PyBox_Delete         , METH_NOARGS
                         , "Delete associated hurricane object, the python object remains." }
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

    if      ( __cs.GetObjectIds() == NO_ARG      ) { box = new Box (); }
    else if ( __cs.GetObjectIds() == POINT_ARG   ) { box = new Box ( *PYPOINT_O(arg0) ); }
    else if ( __cs.GetObjectIds() == BOX_ARG     ) { box = new Box ( *PYBOX_O(arg0) ); }
    else if ( __cs.GetObjectIds() == POINTS2_ARG ) { box = new Box ( *PYPOINT_O(arg0)
                                                                   , *PYPOINT_O(arg1) ); }
    else if ( __cs.GetObjectIds() == INTS2_ARG   ) { box = new Box ( PyInt_AsLong(arg0) 
                                                                   , PyInt_AsLong(arg1) ); }
    else if ( __cs.GetObjectIds() == INTS4_ARG   ) { box = new Box ( PyInt_AsLong(arg0)
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
 
