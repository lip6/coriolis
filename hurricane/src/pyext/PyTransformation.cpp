
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
// $Id: PyTransformation.cpp,v 1.17 2007/01/30 14:47:24 cobell Exp $
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyTransformation.cpp"                   |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x




#include "PyPoint.h"
#include "PyBox.h"
#include "PyTransformation.h"


namespace  Isobar {


USING_NAMESPACE_HURRICANE


extern "C" {


#define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Transformation,transf,function)


// x=================================================================x
// |           "PyTransformation" Python Module Code Part            |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |                  Global Constants Loading                   |
  // x-------------------------------------------------------------x

  extern void  TransformationLoadConstants ( PyObject* dictionnary )
  {
    PyObject* constant;

    LOAD_CONSTANT ( Transformation::Orientation::ID, "OrientationID" )
    LOAD_CONSTANT ( Transformation::Orientation::R1, "OrientationR1" )
    LOAD_CONSTANT ( Transformation::Orientation::R2, "OrientationR2" )
    LOAD_CONSTANT ( Transformation::Orientation::R3, "OrientationR3" )
    LOAD_CONSTANT ( Transformation::Orientation::MX, "OrientationMX" )
    LOAD_CONSTANT ( Transformation::Orientation::XR, "OrientationXR" )
    LOAD_CONSTANT ( Transformation::Orientation::MY, "OrientationMY" )
    LOAD_CONSTANT ( Transformation::Orientation::YR, "OrientationYR" )
  }




  // x-------------------------------------------------------------x
  // |           "PyTransformation" Attribute Methods              |
  // x-------------------------------------------------------------x


  // Standart Accessors (Attributes).
  DirectGetLongAttribute(PyTransformation_GetTx, getTx, PyTransformation, Transformation)
  DirectGetLongAttribute(PyTransformation_GetTy, getTy, PyTransformation, Transformation)


  // Standart Delete (Attribute).
  DirectDeleteAttribute(PyTransformation_Delete, PyTransformation)




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyBox_GetTranslation ()"

  static PyObject* PyTransformation_GetTranslation ( PyTransformation *self )
  {
    trace << "PyBox_GetTranslation ()" << endl;
    
    METHOD_HEAD ( "Translation.GetTranslation()" )

    PyPoint* pyPoint = PyObject_NEW ( PyPoint, &PyTypePoint );
    if (pyPoint == NULL) { return NULL; }
    
    trace_in ();
    trace <<  "new PyPoint [" << hex << pyPoint << "]" << endl;
    trace_out ();

    HTRY

    pyPoint->_object = new Point ( transf->getTranslation() );

    HCATCH
    
    return ( (PyObject*)pyPoint );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyTransformation_GetOrientation ()"

  static PyObject* PyTransformation_GetOrientation ( PyTransformation *self ) {
    trace << "PyTransformation_GetOrientation ()" << endl;
    
    METHOD_HEAD ( "Translation.GetOrientation()" )

    return ( (PyObject*)Py_BuildValue("i",(long)transf->getOrientation().GetCode()) );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyTransformation_GetX ()"

  static PyObject* PyTransformation_GetX ( PyTransformation *self, PyObject* args )
  {
    trace << "PyTransformation_GetX ()" << endl;
    
    METHOD_HEAD ( "Transformation.GetX()" )

    PyObject* arg0;
    PyObject* arg1;
    long      result = 0;

    HTRY

    __cs.Init ("Transformation.GetX");
    if ( ! PyArg_ParseTuple(args,"|O&O&:Transformation.GetX",Converter,&arg0,Converter,&arg1) )
      return ( NULL );

    if      ( __cs.GetObjectIds() == POINT_ARG ) { result = transf->getX ( *PYPOINT_O(arg0) ); }
    else if ( __cs.GetObjectIds() == INTS2_ARG ) { result = transf->getX ( PyInt_AsLong(arg0)
                                                                         , PyInt_AsLong(arg1) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Tranformation.GetX()." );
      return ( NULL );
    }

    HCATCH

    return ( (PyObject*)Py_BuildValue("i",result) );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyTransformation_GetY ()"

  static PyObject* PyTransformation_GetY ( PyTransformation *self, PyObject* args )
  {
    trace << "PyTransformation_GetY ()" << endl;
    
    METHOD_HEAD ( "Transformation.GetY()" )

    PyObject* arg0;
    PyObject* arg1;
    long      result = 0;

    HTRY

    __cs.Init ("Transformation.GetY");
    if ( ! PyArg_ParseTuple(args,"|O&O&:Transformation.GetY",Converter,&arg0,Converter,&arg1) )
      return ( NULL );

    if      ( __cs.GetObjectIds() == POINT_ARG ) { result = transf->getY ( *PYPOINT_O(arg0) ); }
    else if ( __cs.GetObjectIds() == INTS2_ARG ) { result = transf->getY ( PyInt_AsLong(arg0)
                                                                         , PyInt_AsLong(arg1) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Tranformation.GetY()." );
      return ( NULL );
    }

    HCATCH

    return ( (PyObject*)Py_BuildValue("i",result) );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyTransformation_GetDx ()"

  static PyObject* PyTransformation_GetDx ( PyTransformation *self, PyObject* args )
  {
    trace << "PyTransformation_GetDx ()" << endl;
    
    METHOD_HEAD ( "Transformation.GetDx()" )

    PyObject* arg0;
    PyObject* arg1;
    long      result = 0;

    HTRY

    if ( ! ParseTwoArg ( "Transformation.GetDx", args, INTS2_ARG, &arg0, &arg1 ) ) return ( NULL );
    result = transf->getDx ( PyInt_AsLong(arg0), PyInt_AsLong(arg1) );

    HCATCH

    return ( (PyObject*)Py_BuildValue("i",result) );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyTransformation_GetDy ()"

  static PyObject* PyTransformation_GetDy ( PyTransformation *self, PyObject* args )
  {
    trace << "PyTransformation_GetDy ()" << endl;
    
    METHOD_HEAD ( "Transformation.GetDy()" )

    PyObject* arg0;
    PyObject* arg1;
    long      result = 0;

    HTRY

    if ( ! ParseTwoArg ( "Transformation.GetDy", args, INTS2_ARG, &arg0, &arg1 ) ) return ( NULL );
    result = transf->getDy ( PyInt_AsLong(arg0), PyInt_AsLong(arg1) );

    HCATCH

    return ( (PyObject*)Py_BuildValue("i",result) );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyTransformation_GetTransformation ()"

  static PyObject* PyTransformation_GetTransformation ( PyTransformation *self, PyObject* args )
  {
    trace << "PyTransformation_GetTransformation ()" << endl;
    
    METHOD_HEAD ( "Transformation.GetTransformation()" )

    PyObject*       arg0;
    Transformation  result;

    if ( ! ParseOneArg ( "Transformation.GetTransformation", args, TRANS_ARG, &arg0 ) ) return ( NULL );
    result = transf->getTransformation ( *PYTRANSFORMATION_O(arg0) );

    PyTransformation* resultPyTransf = PyObject_NEW ( PyTransformation, &PyTypeTransformation );
    if ( resultPyTransf == NULL ) { return NULL; }
    
    trace_in ();
    trace << "new PyTransformation [" << hex << resultPyTransf << "]" << endl;
    trace_out ();

    HTRY

    resultPyTransf->_object = new Transformation ( result );

    HCATCH

    return ( (PyObject*)resultPyTransf );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyTransformation_GetPoint ()"

  static PyObject* PyTransformation_GetPoint ( PyTransformation *self, PyObject* args )
  {
    trace << "PyTransformation_GetPoint ()" << endl;
    
    METHOD_HEAD ( "Transformation.GetPoint()" )

    PyObject* arg0;
    PyObject* arg1;
    Point     result;

    __cs.Init ("Transformation.GetPoint");
    if ( ! PyArg_ParseTuple(args,"|O&O&:Transformation.GetPoint",Converter,&arg0,Converter,&arg1) )
      return ( NULL );

    if      ( __cs.GetObjectIds() == POINT_ARG ) { result = transf->getPoint ( *PYPOINT_O(arg0) ); }
    else if ( __cs.GetObjectIds() == INTS2_ARG ) { result = transf->getPoint ( PyInt_AsLong(arg0)
                                                                             , PyInt_AsLong(arg1) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Tranformation.GetPoint()." );
      return ( NULL );
    }

    PyPoint* resultPyPoint = PyObject_NEW ( PyPoint, &PyTypePoint );
    if ( resultPyPoint == NULL ) { return NULL; }
    
    trace_in ();
    trace << "new PyPoint [" << hex << resultPyPoint << "]" << endl;
    trace_out ();
    
    HTRY
    
    resultPyPoint->_object = new Point ( result );

    HCATCH

    return ( (PyObject*)resultPyPoint );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyTransformation_GetBox ()"

  static PyObject* PyTransformation_GetBox ( PyTransformation *self, PyObject* args )
  {
    trace << "PyTransformation_GetBox ()" << endl;
    
    METHOD_HEAD ( "Transformation.GetBox()" )

    PyObject* arg0;
    PyObject* arg1;
    PyObject* arg2;
    PyObject* arg3;
    Box       result;

    __cs.Init ("Transformation.GetBox");
    if ( ! PyArg_ParseTuple(args,"|O&O&O&O&:Transformation.GetBox"
                           ,Converter,&arg0
                           ,Converter,&arg1
                           ,Converter,&arg2
                           ,Converter,&arg3) )
      return ( NULL );

    if      ( __cs.GetObjectIds() == BOX_ARG     ) { result = transf->getBox ( *PYBOX_O(arg0) ); }
    else if ( __cs.GetObjectIds() == POINTS2_ARG ) { result = transf->getBox ( *PYPOINT_O(arg0)
                                                                             , *PYPOINT_O(arg1) ); }
    else if ( __cs.GetObjectIds() == INTS4_ARG   ) { result = transf->getBox ( PyInt_AsLong(arg0)
                                                                             , PyInt_AsLong(arg1)
                                                                             , PyInt_AsLong(arg2)
                                                                             , PyInt_AsLong(arg3) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Tranformation.GetBox()." );
      return ( NULL );
    }

    PyBox* resultPyBox = PyObject_NEW ( PyBox, &PyTypeBox );
    if ( resultPyBox == NULL ) { return NULL; }

    trace_in ();
    trace << "new PyBox [" << hex << resultPyBox << "]" << endl;
    trace_out ();
    
    HTRY

    resultPyBox->_object = new Box ( result );

    HCATCH

    return ( (PyObject*)resultPyBox );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyTransformation_GetInvert ()"

  static PyObject* PyTransformation_GetInvert ( PyTransformation *self )
  {
    trace << "PyTransformation_GetInvert ()" << endl;
    
    METHOD_HEAD ( "Transformation.GetInvert()" )

    Transformation  result = transf->getInvert ();

    PyTransformation* resultPyTransf = PyObject_NEW ( PyTransformation, &PyTypeTransformation );
    if ( resultPyTransf == NULL ) { return NULL; }
    
    trace_in ();
    trace << "new PyTransformation [" << hex << resultPyTransf << "]" << endl;
    trace_out ();

    HTRY

    resultPyTransf->_object = new Transformation ( result );

    HCATCH

    return ( (PyObject*)resultPyTransf );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyTransformation_Invert ()"

  static PyObject* PyTransformation_Invert ( PyTransformation *self )
  {
    trace << "PyTransformation_Invert ()" << endl;
    
    METHOD_HEAD ( "Transformation.Invert()" )

    HTRY

    transf->invert ();

    HCATCH
    
    Py_INCREF ( self );
    return ( (PyObject*)self );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyTransformation_ApplyOn ()"

  static PyObject* PyTransformation_ApplyOn ( PyTransformation *self, PyObject* args )
  {
    trace << "PyTransformation_ApplyOn ()" << endl;
    
    METHOD_HEAD ( "Transformation.ApplyOn()" )

    PyObject* arg0;
    PyObject* arg1;

    HTRY

    __cs.Init ("Transformation.ApplyOn");
    if ( ! PyArg_ParseTuple(args,"|O&O&:Transformation.ApplyOn",Converter,&arg0,Converter,&arg1) )
      return ( NULL );

    if      ( __cs.GetObjectIds() == POINT_ARG ) { transf->applyOn ( *PYPOINT_O(arg0) ); }
    else if ( __cs.GetObjectIds() == BOX_ARG   ) { transf->applyOn ( *PYBOX_O(arg0) ); }
    else if ( __cs.GetObjectIds() == TRANS_ARG ) { transf->applyOn ( *PYTRANSFORMATION_O(arg0) ); }
    else if ( __cs.GetObjectIds() == INTS2_ARG ) {
      PyErr_SetString ( ConstructorError, "This set of ApplyOn() parameters has not been implemented." );
      return ( NULL );
    } else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Tranformation.ApplyOn()." );
      return ( NULL );
    }

    HCATCH

    Py_RETURN_NONE;
  }




  // ---------------------------------------------------------------
  // PyTransformation Attribute Method table.

  PyMethodDef PyTransformation_Methods[] =
    { { "GetTx"            , (PyCFunction)PyTransformation_GetTx            , METH_NOARGS, "Return the horizontal component of a Transformation." }
    , { "GetTy"            , (PyCFunction)PyTransformation_GetTy            , METH_NOARGS, "Return the vertical component of a Transformation." }
    , { "GetTranslation"   , (PyCFunction)PyTransformation_GetTranslation   , METH_NOARGS , "Return the translation component of a Transformation." }
    , { "GetOrientation"   , (PyCFunction)PyTransformation_GetOrientation   , METH_NOARGS , "Return the orientation component of a Transformation." }
    , { "GetX"             , (PyCFunction)PyTransformation_GetX             , METH_VARARGS, "Return X of transformed point <x,y>." }
    , { "GetY"             , (PyCFunction)PyTransformation_GetY             , METH_VARARGS, "Return Y of transformed point <x,y>." }
    , { "GetDx"            , (PyCFunction)PyTransformation_GetDx            , METH_VARARGS, "Return X of transformed point <x,y>." }
    , { "GetDy"            , (PyCFunction)PyTransformation_GetDy            , METH_VARARGS, "Return Y of transformed point <x,y>." }
    , { "GetTransformation", (PyCFunction)PyTransformation_GetTransformation, METH_VARARGS, "Return combined transformation." }
    , { "GetPoint"         , (PyCFunction)PyTransformation_GetPoint         , METH_VARARGS, "Return transformed point <x,y>." }
    , { "GetBox"           , (PyCFunction)PyTransformation_GetBox           , METH_VARARGS, "Return transformed box." }
    , { "GetInvert"        , (PyCFunction)PyTransformation_GetInvert        , METH_NOARGS, "Return inverse transformation." }
    , { "Invert"           , (PyCFunction)PyTransformation_Invert           , METH_NOARGS, "Inverse transformation." }
    , { "ApplyOn"          , (PyCFunction)PyTransformation_ApplyOn          , METH_VARARGS, "Apply transformation to object." }
    , { "Delete"           , (PyCFunction)PyTransformation_Delete           , METH_NOARGS
                           , "Delete associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };




  // x-------------------------------------------------------------x
  // |             "PyTransformation" Object Methods               |
  // x-------------------------------------------------------------x

  static Transformation::Orientation  PyInt_AsOrientation ( PyObject* object ) {
    switch ( PyInt_AsLong(object) ) {
      case Transformation::Orientation::ID : return ( Transformation::Orientation(Transformation::Orientation::ID) );
      case Transformation::Orientation::R1 : return ( Transformation::Orientation(Transformation::Orientation::R1) );
      case Transformation::Orientation::R2 : return ( Transformation::Orientation(Transformation::Orientation::R2) );
      case Transformation::Orientation::R3 : return ( Transformation::Orientation(Transformation::Orientation::R3) );
      case Transformation::Orientation::MX : return ( Transformation::Orientation(Transformation::Orientation::MX) );
      case Transformation::Orientation::XR : return ( Transformation::Orientation(Transformation::Orientation::XR) );
      case Transformation::Orientation::MY : return ( Transformation::Orientation(Transformation::Orientation::MY) );
      case Transformation::Orientation::YR : return ( Transformation::Orientation(Transformation::Orientation::YR) );
    }

    return ( Transformation::Orientation(Transformation::Orientation::ID) );
  }

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyTransformation_new ()"

  static PyObject* PyTransformation_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    trace << "PyTransformation_new()" << endl;

    Transformation* transf;
    PyObject* arg0;
    PyObject* arg1;
    PyObject* arg2;

    __cs.Init ("Transformation.new");
    if ( ! PyArg_ParseTuple(args,"|O&O&O&:Transformation.new"
                           ,Converter,&arg0
                           ,Converter,&arg1
                           ,Converter,&arg2
                           ) ) return ( NULL );

    if      ( __cs.GetObjectIds() == NO_ARG        ) { transf = new Transformation (); }
    else if ( __cs.GetObjectIds() == POINT_ARG     ) { transf = new Transformation ( *PYPOINT_O(arg0) ); }
    else if ( __cs.GetObjectIds() == TRANS_ARG     ) { transf = new Transformation ( *PYTRANSFORMATION_O(arg0) ); }
    else if ( __cs.GetObjectIds() == INTS2_ARG     ) { transf = new Transformation ( PyInt_AsLong(arg0)
                                                                                   , PyInt_AsLong(arg1) ); }
    else if ( __cs.GetObjectIds() == POINT_INT_ARG ) { transf = new Transformation ( *PYPOINT_O(arg0)
                                                                                   , PyInt_AsOrientation(arg1) ); }
    else if ( __cs.GetObjectIds() == INTS3_ARG     ) { transf = new Transformation ( PyInt_AsLong(arg0)
                                                                                   , PyInt_AsLong(arg1)
                                                                                   , PyInt_AsOrientation(arg2) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Transformation constructor." );
      return ( NULL );
    }

    PyTransformation* pyTransformation = PyObject_NEW(PyTransformation, &PyTypeTransformation);
    if (pyTransformation == NULL) { ; return NULL; }
    
    trace_in ();
    trace << "new PyTransformation [" << hex << pyTransformation << "]" << endl;
    trace_out ();
    
    HTRY

    pyTransformation->_object = transf;

    HCATCH

    return ( (PyObject*)pyTransformation );
  }

  DirectDeleteMethod(PyTransformation_DeAlloc,PyTransformation)
  PyTypeObjectLinkPyType(Transformation)
  PyTypeObjectConstructor(Transformation)


#else  // End of Python Module Code Part.


// x=================================================================x
// |          "PyTransformation" Shared Library Code Part            |
// x=================================================================x
  

  // x-------------------------------------------------------------x
  // |            "PyTransformation" Local Functions               |
  // x-------------------------------------------------------------x

  // ---------------------------------------------------------------
  // PyTransformation Object Definitions.
  PyTypeRootObjectDefinitions(Transformation)


#endif  // End of Shared Library Code Part.


}  // End of extern "C".

}  // End of Isobar namespace.
