// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2006-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyTransformation.cpp"                        |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyPoint.h"
#include "hurricane/isobar/PyBox.h"
#include "hurricane/isobar/PyTransformation.h"
#include "hurricane/isobar/PyOrientation.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


#define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Transformation,transf,function)


// x=================================================================x
// |           "PyTransformation" Python Module Code Part            |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |           "PyTransformation" Attribute Methods              |
  // x-------------------------------------------------------------x


  // Standart Accessors (Attributes).
  DirectGetLongAttribute(PyTransformation_getTx, getTx, PyTransformation, Transformation)
  DirectGetLongAttribute(PyTransformation_getTy, getTy, PyTransformation, Transformation)


  // Standard destroy (Attribute).
  DirectDestroyAttribute(PyTransformation_destroy, PyTransformation)
  

  static Transformation::Orientation  PyInt_AsOrientation ( PyObject* object ) {
    switch ( PyAny_AsLong(object) ) {
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
  // Attribute Method  :  "PyTransformation_NEW ()"

  static PyObject* PyTransformation_NEW (PyObject *module, PyObject *args) {
    trace << "PyTransformation_NEW()" << endl;

    Transformation* transf;
    PyObject* arg0;
    PyObject* arg1;
    PyObject* arg2;

    __cs.init ("Transformation.Transformation");
    if ( ! PyArg_ParseTuple(args,"|O&O&O&:Transformation.Transformation"
                           ,Converter,&arg0
                           ,Converter,&arg1
                           ,Converter,&arg2
                           )) {
        PyErr_SetString ( ConstructorError, "invalid number of parameters for Transformation constructor." );
        return NULL;
    }

    if      ( __cs.getObjectIds() == NO_ARG        ) { transf = new Transformation (); }
    else if ( __cs.getObjectIds() == POINT_ARG     ) { transf = new Transformation ( *PYPOINT_O(arg0) ); }
    else if ( __cs.getObjectIds() == TRANS_ARG     ) { transf = new Transformation ( *PYTRANSFORMATION_O(arg0) ); }
    else if ( __cs.getObjectIds() == INTS2_ARG     ) { transf = new Transformation ( PyAny_AsLong(arg0)
                                                                                   , PyAny_AsLong(arg1) ); }
    else if ( __cs.getObjectIds() == POINT_INT_ARG ) { transf = new Transformation ( *PYPOINT_O(arg0)
                                                                                   , PyInt_AsOrientation(arg1) ); }
    else if ( __cs.getObjectIds() == INTS3_ARG     ) { transf = new Transformation ( PyAny_AsLong(arg0)
                                                                                   , PyAny_AsLong(arg1)
                                                                                   , PyInt_AsOrientation(arg2) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Transformation constructor." );
      return NULL;
    }

    PyTransformation* pyTransformation = PyObject_NEW(PyTransformation, &PyTypeTransformation);
    if (pyTransformation == NULL) { return NULL; }
    
    trace_in ();
    trace << "new PyTransformation [" << hex << pyTransformation << "]" << endl;
    trace_out ();
    
    HTRY

    pyTransformation->_object = transf;

    HCATCH

    return (PyObject*)pyTransformation;
  }


  static int  PyTransformation_Init ( PyTransformation* self, PyObject* args, PyObject* kwargs )
  {
    trace << "PyTransformation_Init(): " << (void*)self << endl;
    return 0;
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyBox_getTranslation ()"

  static PyObject* PyTransformation_getTranslation ( PyTransformation *self ) {
    trace << "PyBox_getTranslation ()" << endl;
    
    METHOD_HEAD ( "Translation.getTranslation()" )

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
  // Attribute Method  :  "PyTransformation_getOrientation ()"

  static PyObject* PyTransformation_getOrientation ( PyTransformation *self ) {
    trace << "PyTransformation_getOrientation ()" << endl;
    
    METHOD_HEAD ( "Translation.getOrientation()" )

    return ( (PyObject*)PyLong_FromLong( (long)transf->getOrientation().getCode()) );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyTransformation_getX ()"

  static PyObject* PyTransformation_getX ( PyTransformation *self, PyObject* args )
  {
    trace << "PyTransformation_getX ()" << endl;
    
    METHOD_HEAD ( "Transformation.getX()" )

    PyObject* arg0;
    PyObject* arg1;
    long      result = 0;

    HTRY

    __cs.init ("Transformation.getX");
    if ( ! PyArg_ParseTuple(args,"|O&O&:Transformation.getX",Converter,&arg0,Converter,&arg1) )
      return ( NULL );

    if      ( __cs.getObjectIds() == POINT_ARG ) { result = transf->getX ( *PYPOINT_O(arg0) ); }
    else if ( __cs.getObjectIds() == INTS2_ARG ) { result = transf->getX ( PyAny_AsLong(arg0)
                                                                         , PyAny_AsLong(arg1) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Tranformation.getX()." );
      return ( NULL );
    }

    HCATCH

    return ( (PyObject*)PyLong_FromLong(result) );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyTransformation_getY ()"

  static PyObject* PyTransformation_getY ( PyTransformation *self, PyObject* args )
  {
    trace << "PyTransformation_getY ()" << endl;
    
    METHOD_HEAD ( "Transformation.getY()" )

    PyObject* arg0;
    PyObject* arg1;
    long      result = 0;

    HTRY

    __cs.init ("Transformation.getY");
    if ( ! PyArg_ParseTuple(args,"|O&O&:Transformation.getY",Converter,&arg0,Converter,&arg1) )
      return ( NULL );

    if      ( __cs.getObjectIds() == POINT_ARG ) { result = transf->getY ( *PYPOINT_O(arg0) ); }
    else if ( __cs.getObjectIds() == INTS2_ARG ) { result = transf->getY ( PyAny_AsLong(arg0)
                                                                         , PyAny_AsLong(arg1) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Tranformation.getY()." );
      return ( NULL );
    }

    HCATCH

    return ( (PyObject*)PyLong_FromLong(result) );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyTransformation_getDx ()"

  static PyObject* PyTransformation_getDx ( PyTransformation *self, PyObject* args )
  {
    trace << "PyTransformation_getDx ()" << endl;
    
    METHOD_HEAD ( "Transformation.getDx()" )

    PyObject* arg0;
    PyObject* arg1;
    long      result = 0;

    HTRY

    if ( ! ParseTwoArg ( "Transformation.getDx", args, INTS2_ARG, &arg0, &arg1 ) ) return ( NULL );
    result = transf->getDx ( PyAny_AsLong(arg0), PyAny_AsLong(arg1) );

    HCATCH

    return ( (PyObject*)PyLong_FromLong(result) );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyTransformation_getDy ()"

  static PyObject* PyTransformation_getDy ( PyTransformation *self, PyObject* args )
  {
    trace << "PyTransformation_getDy ()" << endl;
    
    METHOD_HEAD ( "Transformation.getDy()" )

    PyObject* arg0;
    PyObject* arg1;
    long      result = 0;

    HTRY

    if ( ! ParseTwoArg ( "Transformation.getDy", args, INTS2_ARG, &arg0, &arg1 ) ) return ( NULL );
    result = transf->getDy ( PyAny_AsLong(arg0), PyAny_AsLong(arg1) );

    HCATCH

    return ( (PyObject*)PyLong_FromLong(result) );
  }




  // ---------------------------------------------------------------
  // Attribute Method  :  "PyTransformation_getTransformation ()"

  static PyObject* PyTransformation_getTransformation ( PyTransformation *self, PyObject* args )
  {
    trace << "PyTransformation_getTransformation ()" << endl;
    
    METHOD_HEAD ( "Transformation.getTransformation()" )

    PyObject*       arg0;
    Transformation  result;

    if ( ! ParseOneArg ( "Transformation.getTransformation", args, TRANS_ARG, &arg0 ) ) return ( NULL );
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
  // Attribute Method  :  "PyTransformation_getPoint ()"

  static PyObject* PyTransformation_getPoint ( PyTransformation *self, PyObject* args )
  {
    trace << "PyTransformation_getPoint ()" << endl;
    
    METHOD_HEAD ( "Transformation.getPoint()" )

    PyObject* arg0;
    PyObject* arg1;
    Point     result;

    __cs.init ("Transformation.getPoint");
    if ( ! PyArg_ParseTuple(args,"|O&O&:Transformation.getPoint",Converter,&arg0,Converter,&arg1) )
      return ( NULL );

    if      ( __cs.getObjectIds() == POINT_ARG ) { result = transf->getPoint ( *PYPOINT_O(arg0) ); }
    else if ( __cs.getObjectIds() == INTS2_ARG ) { result = transf->getPoint ( PyAny_AsLong(arg0)
                                                                             , PyAny_AsLong(arg1) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Tranformation.getPoint()." );
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
  // Attribute Method  :  "PyTransformation_getBox ()"

  static PyObject* PyTransformation_getBox ( PyTransformation *self, PyObject* args )
  {
    trace << "PyTransformation_getBox ()" << endl;
    
    METHOD_HEAD ( "Transformation.getBox()" )

    PyObject* arg0;
    PyObject* arg1;
    PyObject* arg2;
    PyObject* arg3;
    Box       result;

    __cs.init ("Transformation.getBox");
    if ( ! PyArg_ParseTuple(args,"|O&O&O&O&:Transformation.getBox"
                           ,Converter,&arg0
                           ,Converter,&arg1
                           ,Converter,&arg2
                           ,Converter,&arg3) )
      return ( NULL );

    if      ( __cs.getObjectIds() == BOX_ARG     ) { result = transf->getBox ( *PYBOX_O(arg0) ); }
    else if ( __cs.getObjectIds() == POINTS2_ARG ) { result = transf->getBox ( *PYPOINT_O(arg0)
                                                                             , *PYPOINT_O(arg1) ); }
    else if ( __cs.getObjectIds() == INTS4_ARG   ) { result = transf->getBox ( PyAny_AsLong(arg0)
                                                                             , PyAny_AsLong(arg1)
                                                                             , PyAny_AsLong(arg2)
                                                                             , PyAny_AsLong(arg3) ); }
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Tranformation.getBox()." );
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
  // Attribute Method  :  "PyTransformation_getInvert ()"

  static PyObject* PyTransformation_getInvert ( PyTransformation *self )
  {
    trace << "PyTransformation_getInvert ()" << endl;
    
    METHOD_HEAD ( "Transformation.getInvert()" )

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

    __cs.init ("Transformation.ApplyOn");
    if ( ! PyArg_ParseTuple(args,"|O&O&:Transformation.ApplyOn",Converter,&arg0,Converter,&arg1) )
      return ( NULL );

    if      ( __cs.getObjectIds() == POINT_ARG ) { transf->applyOn ( *PYPOINT_O(arg0) ); }
    else if ( __cs.getObjectIds() == BOX_ARG   ) { transf->applyOn ( *PYBOX_O(arg0) ); }
    else if ( __cs.getObjectIds() == TRANS_ARG ) { transf->applyOn ( *PYTRANSFORMATION_O(arg0) ); }
    else if ( __cs.getObjectIds() == INTS2_ARG ) {
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
    { { "getTx"            , (PyCFunction)PyTransformation_getTx            , METH_NOARGS, "Return the horizontal component of a Transformation." }
    , { "getTy"            , (PyCFunction)PyTransformation_getTy            , METH_NOARGS, "Return the vertical component of a Transformation." }
    , { "getTranslation"   , (PyCFunction)PyTransformation_getTranslation   , METH_NOARGS , "Return the translation component of a Transformation." }
    , { "getOrientation"   , (PyCFunction)PyTransformation_getOrientation   , METH_NOARGS , "Return the orientation component of a Transformation." }
    , { "getX"             , (PyCFunction)PyTransformation_getX             , METH_VARARGS, "Return X of transformed point <x,y>." }
    , { "getY"             , (PyCFunction)PyTransformation_getY             , METH_VARARGS, "Return Y of transformed point <x,y>." }
    , { "getDx"            , (PyCFunction)PyTransformation_getDx            , METH_VARARGS, "Return X of transformed point <x,y>." }
    , { "getDy"            , (PyCFunction)PyTransformation_getDy            , METH_VARARGS, "Return Y of transformed point <x,y>." }
    , { "getTransformation", (PyCFunction)PyTransformation_getTransformation, METH_VARARGS, "Return combined transformation." }
    , { "getPoint"         , (PyCFunction)PyTransformation_getPoint         , METH_VARARGS, "Return transformed point <x,y>." }
    , { "getBox"           , (PyCFunction)PyTransformation_getBox           , METH_VARARGS, "Return transformed box." }
    , { "getInvert"        , (PyCFunction)PyTransformation_getInvert        , METH_NOARGS, "Return inverse transformation." }
    , { "Invert"           , (PyCFunction)PyTransformation_Invert           , METH_NOARGS, "Inverse transformation." }
    , { "invert"           , (PyCFunction)PyTransformation_Invert           , METH_NOARGS, "Inverse transformation." }
    , { "ApplyOn"          , (PyCFunction)PyTransformation_ApplyOn          , METH_VARARGS, "Apply transformation to object." }
    , { "applyOn"          , (PyCFunction)PyTransformation_ApplyOn          , METH_VARARGS, "Apply transformation to object." }
    , { "destroy"          , (PyCFunction)PyTransformation_destroy          , METH_NOARGS
                           , "Destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };


  // x-------------------------------------------------------------x
  // |             "PyTransformation" Object Methods               |
  // x-------------------------------------------------------------x

  DirectDeleteMethod(PyTransformation_DeAlloc,PyTransformation)
  PyTypeObjectLinkPyTypeNewInit(Transformation)
//PyTypeObjectLinkPyType(Transformation)


#else  // End of Python Module Code Part.


// x=================================================================x
// |          "PyTransformation" Shared Library Code Part            |
// x=================================================================x


  // ---------------------------------------------------------------
  // PyTransformation Object Definitions.
  PyTypeObjectDefinitions(Transformation)


  extern  void  PyTransformation_postModuleInit ()
  {
    PyOrientation_postModuleInit();

    PyDict_SetItemString( PyTypeTransformation.tp_dict, "Orientation", (PyObject*)&PyTypeOrientation );
  }


#endif  // End of Shared Library Code Part.


}  // End of extern "C".

}  // End of Isobar namespace.
