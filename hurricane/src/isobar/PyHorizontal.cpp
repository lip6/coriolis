// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2007-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                     Sophie BELLOEIL              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyHorizontal.cpp"                            |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyLayer.h"
#include "hurricane/isobar/PyHorizontal.h"


namespace  Isobar {

using namespace Hurricane;


extern "C" {


# undef   ACCESS_OBJECT
# undef   ACCESS_CLASS
# define  ACCESS_OBJECT            _baseObject._baseObject._baseObject._object
# define  ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject._baseObject._baseObject)
# define  METHOD_HEAD(function)    GENERIC_METHOD_HEAD(Horizontal,horizontal,function)


// x=================================================================x
// |             "PyHorizontal" Python Module Code Part              |
// x=================================================================x

# if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |             "PyHorizontal" Attribute Methods                |
  // x-------------------------------------------------------------x


  DirectGetLongAttribute(PyHorizontal_getY       ,getY       ,PyHorizontal,Horizontal)
  DirectGetLongAttribute(PyHorizontal_getDxSource,getDxSource,PyHorizontal,Horizontal)
  DirectGetLongAttribute(PyHorizontal_getDxTarget,getDxTarget,PyHorizontal,Horizontal)
  DirectSetLongAttribute(PyHorizontal_setY       ,setY       ,PyHorizontal,Horizontal)
  DirectSetLongAttribute(PyHorizontal_setDxSource,setDxSource,PyHorizontal,Horizontal)
  DirectSetLongAttribute(PyHorizontal_setDxTarget,setDxTarget,PyHorizontal,Horizontal)

  // Standart destroy (Attribute).
  DBoDestroyAttribute(PyHorizontal_destroy, PyHorizontal)


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyHorizontal_create ()"

  static PyObject* PyHorizontal_create ( PyObject*, PyObject *args ) {
    trace << "PyHorizontal_create()" << endl;

    PyObject* arg0;
    PyObject* arg1;
    PyObject* arg2;
    PyObject* arg3;
    PyObject* arg4;
    PyObject* arg5;
    PyObject* arg6;

    Horizontal*   horizontal = NULL;

    HTRY

    __cs.init ("Horizontal.create");
    if ( ! PyArg_ParseTuple(args,"O&O&O&|O&O&O&O&:Horizontal.create"
                           ,Converter,&arg0
                           ,Converter,&arg1
                           ,Converter,&arg2
                           ,Converter,&arg3
                           ,Converter,&arg4
                           ,Converter,&arg5
                           ,Converter,&arg6
                           )) {
        PyErr_SetString ( ConstructorError, "invalid number of parameters for Horizontal constructor." );
        return NULL;
    }

    if      ( __cs.getObjectIds() == NET_LAYER_INT_ARG )
      horizontal = Horizontal::create ( PYNET_O(arg0)
                                      , PYLAYER_O(arg1)
                                      , PyAny_AsLong(arg2) );
    else if ( __cs.getObjectIds() == NET_LAYER_INTS2_ARG )
      horizontal = Horizontal::create ( PYNET_O(arg0)
                                      , PYLAYER_O(arg1)
                                      , PyAny_AsLong(arg2)
                                      , PyAny_AsLong(arg3) );
    else if ( __cs.getObjectIds() == COMPS2_LAYER_INT_ARG )
      horizontal = Horizontal::create ( ComponentCast(arg0)
                                      , ComponentCast(arg1)
                                      , PYLAYER_O(arg2)
                                      , PyAny_AsLong(arg3) );
    else if ( __cs.getObjectIds() == NET_LAYER_INTS3_ARG )
      horizontal = Horizontal::create ( PYNET_O(arg0)
                                      , PYLAYER_O(arg1)
                                      , PyAny_AsLong(arg2)
                                      , PyAny_AsLong(arg3)
                                      , PyAny_AsLong(arg4) );
    else if ( __cs.getObjectIds() == COMPS2_LAYER_INTS2_ARG )
      horizontal = Horizontal::create ( ComponentCast(arg0)
                                      , ComponentCast(arg1)
                                      , PYLAYER_O(arg2)
                                      , PyAny_AsLong(arg3)
                                      , PyAny_AsLong(arg4) );
    else if ( __cs.getObjectIds() == NET_LAYER_INTS4_ARG )
      horizontal = Horizontal::create ( PYNET_O(arg0)
                                      , PYLAYER_O(arg1)
                                      , PyAny_AsLong(arg2)
                                      , PyAny_AsLong(arg3)
                                      , PyAny_AsLong(arg4)
                                      , PyAny_AsLong(arg5) );
    else if ( __cs.getObjectIds() == COMPS2_LAYER_INTS3_ARG )
      horizontal = Horizontal::create ( ComponentCast(arg0)
                                      , ComponentCast(arg1)
                                      , PYLAYER_O(arg2)
                                      , PyAny_AsLong(arg3)
                                      , PyAny_AsLong(arg4)
                                      , PyAny_AsLong(arg5) );
    else if ( __cs.getObjectIds() == COMPS2_LAYER_INTS4_ARG )
      horizontal = Horizontal::create ( ComponentCast(arg0)
                                      , ComponentCast(arg1)
                                      , PYLAYER_O(arg2)
                                      , PyAny_AsLong(arg3)
                                      , PyAny_AsLong(arg4)
                                      , PyAny_AsLong(arg5)
                                      , PyAny_AsLong(arg6) );
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Horizontal constructor." );
      return NULL;
    }

    HCATCH

    return PyHorizontal_Link ( horizontal );
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyHorizontal_translate ()"

  static PyObject* PyHorizontal_translate ( PyHorizontal *self, PyObject* args ) {
    trace << "PyHorizontal_translate ()" << endl;
    
    HTRY
    METHOD_HEAD ( "Horizontal.translate()" )
    DbU::Unit dx=0, dy=0;
    if (PyArg_ParseTuple(args,"ll:Horizontal.translate", &dx, &dy)) {
        horizontal->translate(dx, dy);
    } else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Horizontal.translate()" );
      return NULL;
    }
    HCATCH

    Py_RETURN_NONE;
  }


  // ---------------------------------------------------------------
  // PyHorizontal Attribute Method table.

  PyMethodDef PyHorizontal_Methods[] =
    { { "create"     , (PyCFunction)PyHorizontal_create     , METH_VARARGS|METH_STATIC
                     , "Create a new Horizontal." }
    , { "getY"       , (PyCFunction)PyHorizontal_getY       , METH_NOARGS , "Get the segment Y position." }
    , { "getDxSource", (PyCFunction)PyHorizontal_getDxSource, METH_NOARGS , "Get the segment source X offset." }
    , { "getDxTarget", (PyCFunction)PyHorizontal_getDxTarget, METH_NOARGS , "Get the segment target X offset." }
    , { "setY"       , (PyCFunction)PyHorizontal_setY       , METH_VARARGS, "Modify the segment Y position." }
    , { "setDxSource", (PyCFunction)PyHorizontal_setDxSource, METH_VARARGS, "Modify the segment source X offset." }
    , { "setDxTarget", (PyCFunction)PyHorizontal_setDxTarget, METH_VARARGS, "Modify the segment target X offset." }
    , { "translate"  , (PyCFunction)PyHorizontal_translate  , METH_VARARGS, "Translates the Horizontal segment of dx and dy." }
    , { "destroy"    , (PyCFunction)PyHorizontal_destroy    , METH_NOARGS
                     , "destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };




  // x-------------------------------------------------------------x
  // |               "PyHorizontal" Object Methods                 |
  // x-------------------------------------------------------------x


  DBoDeleteMethod(Horizontal)
  PyTypeObjectLinkPyType(Horizontal)


#else  // End of Python Module Code Part.


// x=================================================================x
// |            "PyHorizontal" Shared Library Code Part              |
// x=================================================================x



  // Link/Creation Method.
  DBoLinkCreateMethod(Horizontal)


  // ---------------------------------------------------------------
  // PyHorizontal Object Definitions.

  PyTypeInheritedObjectDefinitions(Horizontal, Segment)

#endif  // End of Shared Library Code Part.


}  // End of extern "C".




}  // End of Isobar namespace.
 
