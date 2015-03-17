// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2015, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyReference.cpp"                        |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyReference.h"
#include "hurricane/isobar/PyPoint.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyBox.h"
#include "hurricane/Cell.h"
using namespace Hurricane;

namespace  Isobar {

extern "C" {
#undef   ACCESS_OBJECT
#undef   ACCESS_CLASS
#define  ACCESS_OBJECT           _baseObject._object
#define  ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define  METHOD_HEAD(function)    GENERIC_METHOD_HEAD(Reference,reference,function)


// x=================================================================x
// |             "PyReference" Python Module Code Part               |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |              "PyReference" Attribute Methods                |
  // x-------------------------------------------------------------x


  DBoDestroyAttribute(PyReference_destroy, PyReference)

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyReference_getName ()"

  GetNameMethod(Reference, reference)

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyReference_create ()"

  PyObject* PyReference_create ( PyObject*, PyObject *args ) {
    trace << "PyReference_create()" << endl;
    Reference*  reference = NULL;

    PyObject* arg0;
    PyObject* arg1;
    PyObject* arg2;
    PyObject* arg3;

    HTRY

    __cs.init ("Reference.create");
    if ( ! PyArg_ParseTuple(args,"O&O&O&|O&:Reference.create"
                           ,Converter,&arg0
                           ,Converter,&arg1
                           ,Converter,&arg2
                           ,Converter,&arg3
                           )) {
        PyErr_SetString ( ConstructorError, "invalid number of parameters for Reference constructor." );
        return NULL;
    }

    if      ( __cs.getObjectIds() == ":ent:string:int:int" )
      reference = Reference::create (  PYCELL_O(arg0)
                                    ,  Name(PyString_AsString(arg1))
                                    ,  PyAny_AsLong(arg2)
                                    ,  PyAny_AsLong(arg3) );
    else if ( __cs.getObjectIds() == ":ent:name:point" )
      reference = Reference::create (  PYCELL_O(arg0)
                                    ,  Name(PyString_AsString(arg1))
                                    , *PYPOINT_O(arg2) );
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Reference constructor." );
      return NULL;
    }

    HCATCH
    return PyReference_Link ( reference );
  }

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyReference_getPoint ()"

  static PyObject* PyReference_getPoint ( PyReference *self )
  {
    trace << "PyReference_getPoint()" << endl;

    METHOD_HEAD ( "Reference.getPoint()" )

    PyPoint* pyPoint = PyObject_NEW ( PyPoint, &PyTypePoint );
    if (pyPoint == NULL) { return NULL; }
    
    HTRY
    pyPoint->_object = new Point ( reference->getPoint() );
    HCATCH    

    return ( (PyObject*)pyPoint );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyReference_getBoundingBox ()"

  static PyObject* PyReference_getBoundingBox( PyReference *self ) {
    trace << "PyReference_getBoundingBox()" << endl;

    METHOD_HEAD ( "Reference.BoundingBox()" )

    PyBox* pyBox = PyObject_NEW ( PyBox, &PyTypeBox );
    if (pyBox == NULL) { return NULL; }
    
    HTRY
    pyBox->_object = new Box ( reference->getBoundingBox() );
    HCATCH    

    return ( (PyObject*)pyBox );
  }



  // ---------------------------------------------------------------
  // Attribute Method  :  "PyReference_translate ()"

  static PyObject* PyReference_translate ( PyReference *self, PyObject* args ) {
    trace << "PyReference_translate ()" << endl;
    
    METHOD_HEAD ( "Reference.translate()" )

    PyObject* arg0;
    PyObject* arg1;

    HTRY

    if ( ! ParseTwoArg ( "Reference.translate", args, INTS2_ARG, &arg0, &arg1 ) ) return ( NULL );

    reference->translate ( PyAny_AsLong(arg0), PyAny_AsLong(arg1) );

    HCATCH

    Py_RETURN_NONE;
  }




  // ---------------------------------------------------------------
  // PyReference Attribute Method table.

  PyMethodDef PyReference_Methods[] =
    { { "create"         , (PyCFunction)PyReference_create         , METH_NOARGS|METH_STATIC
                         , "Create a new Reference." }
    , { "destroy"        , (PyCFunction)PyReference_destroy        , METH_NOARGS
                         , "destroy associated hurricane object, the python object remains." }
    , { "getName"        , (PyCFunction)PyReference_getName        , METH_NOARGS , "Returns the name of the reference." }
    , { "getPoint"       , (PyCFunction)PyReference_getPoint       , METH_NOARGS , "Return the reference point." }
    , { "getBoundingBox" , (PyCFunction)PyReference_getBoundingBox , METH_NOARGS , "Return the reference boundingBox." }
    , { "translate"      , (PyCFunction)PyReference_translate      , METH_VARARGS, "Translate the reference of dx and dy." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };




  // x-------------------------------------------------------------x
  // |               "PyReference" Object Methods                  |
  // x-------------------------------------------------------------x

  DBoDeleteMethod(Reference)
  PyTypeObjectLinkPyType(Reference)


# else  // End of Python Module Code Part.


// x=================================================================x
// |            "PyReference" Shared Library Code Part               |
// x=================================================================x



  // Link/Creation Method.
  DBoLinkCreateMethod(Reference)


  // ---------------------------------------------------------------
  // PyReference Object Definitions.
  PyTypeInheritedObjectDefinitions(Reference, Entity)


#endif  // End of Shared Library Code Part.


}  // End of extern "C".




}  // End of Isobar namespace.
 
