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
// |  C++ Module  :  "./PyCell.cpp"                                  |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyLayer.h"
#include "hurricane/isobar/PyVertical.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT            _baseObject._baseObject._baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject._baseObject._baseObject)
#define METHOD_HEAD(function)    GENERIC_METHOD_HEAD(Vertical,vertical,function)

// x=================================================================x
// |              "PyVertical" Python Module Code Part               |
// x=================================================================x

#if defined(__PYTHON_MODULE__)

  
  // x-------------------------------------------------------------x
  // |              "PyVertical" Attribute Methods                 |
  // x-------------------------------------------------------------x


  DirectGetLongAttribute(PyVertical_getX       ,getX       ,PyVertical,Vertical)
  DirectGetLongAttribute(PyVertical_getDySource,getDySource,PyVertical,Vertical)
  DirectGetLongAttribute(PyVertical_getDyTarget,getDyTarget,PyVertical,Vertical)
  DirectSetLongAttribute(PyVertical_setX,setX,"Vertical.setX",PyVertical,Vertical)
  DirectSetLongAttribute(PyVertical_setDySource,setDySource,"Vertical.setDySource",PyVertical,Vertical)
  DirectSetLongAttribute(PyVertical_setDyTarget,setDyTarget,"Vertical.setDyTarget",PyVertical,Vertical)

  // Standard destroy (Attribute).
  DBoDestroyAttribute(PyVertical_destroy, PyVertical)


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyVertical_create ()"

  static PyObject* PyVertical_create ( PyObject*, PyObject *args ) {
    trace << "PyVertical_create()" << endl;

    PyObject* arg0;
    PyObject* arg1;
    PyObject* arg2;
    PyObject* arg3;
    PyObject* arg4;
    PyObject* arg5;
    PyObject* arg6;
    Vertical* vertical = NULL;

    HTRY
    __cs.init ("Vertical.create");
    if (!PyArg_ParseTuple(args,"O&O&O&|O&O&O&O&:Vertical.create"
                           ,Converter,&arg0
                           ,Converter,&arg1
                           ,Converter,&arg2
                           ,Converter,&arg3
                           ,Converter,&arg4
                           ,Converter,&arg5
                           ,Converter,&arg6
                           )) {
        PyErr_SetString ( ConstructorError, "invalid number of parameters for Vertical constructor." );
        return NULL;
    }


    if      ( __cs.getObjectIds() == NET_LAYER_INT_ARG )
      vertical = Vertical::create ( PYNET_O(arg0)
                                  , PYLAYER_O(arg1)
                                  , PyAny_AsLong(arg2) );
    else if ( __cs.getObjectIds() == NET_LAYER_INTS2_ARG )
      vertical = Vertical::create ( PYNET_O(arg0)
                                  , PYLAYER_O(arg1)
                                  , PyAny_AsLong(arg2)
                                  , PyAny_AsLong(arg3) );
    else if ( __cs.getObjectIds() == COMPS2_LAYER_INT_ARG )
      vertical = Vertical::create ( ComponentCast(arg0)
                                  , ComponentCast(arg1)
                                  , PYLAYER_O(arg2)
                                  , PyAny_AsLong(arg3) );
    else if ( __cs.getObjectIds() == NET_LAYER_INTS3_ARG )
      vertical = Vertical::create ( PYNET_O(arg0)
                                  , PYLAYER_O(arg1)
                                  , PyAny_AsLong(arg2)
                                  , PyAny_AsLong(arg3)
                                  , PyAny_AsLong(arg4) );
    else if ( __cs.getObjectIds() == COMPS2_LAYER_INTS2_ARG )
      vertical = Vertical::create ( ComponentCast(arg0)
                                  , ComponentCast(arg1)
                                  , PYLAYER_O(arg2)
                                  , PyAny_AsLong(arg3)
                                  , PyAny_AsLong(arg4) );
    else if ( __cs.getObjectIds() == NET_LAYER_INTS4_ARG )
      vertical = Vertical::create ( PYNET_O(arg0)
                                  , PYLAYER_O(arg1)
                                  , PyAny_AsLong(arg2)
                                  , PyAny_AsLong(arg3)
                                  , PyAny_AsLong(arg4)
                                  , PyAny_AsLong(arg5) );
    else if ( __cs.getObjectIds() == COMPS2_LAYER_INTS3_ARG )
      vertical = Vertical::create ( ComponentCast(arg0)
                                  , ComponentCast(arg1)
                                  , PYLAYER_O(arg2)
                                  , PyAny_AsLong(arg3)
                                  , PyAny_AsLong(arg4)
                                  , PyAny_AsLong(arg5) );
    else if ( __cs.getObjectIds() == COMPS2_LAYER_INTS4_ARG )
      vertical = Vertical::create ( ComponentCast(arg0)
                                  , ComponentCast(arg1)
                                  , PYLAYER_O(arg2)
                                  , PyAny_AsLong(arg3)
                                  , PyAny_AsLong(arg4)
                                  , PyAny_AsLong(arg5)
                                  , PyAny_AsLong(arg6) );
    else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Vertical constructor." );
      return NULL;
    }

    HCATCH

    return PyVertical_Link(vertical);
  }


  // ---------------------------------------------------------------
  // Attribute Method  :  "PyVertical_translate ()"

  static PyObject* PyVertical_translate ( PyVertical *self, PyObject* args ) {
    trace << "PyVertical_translate ()" << endl;
    
    HTRY
    METHOD_HEAD ( "Vertical.translate()" )
    DbU::Unit dx=0, dy=0;
    if (PyArg_ParseTuple(args,"ll:Vertical.translate", &dx, &dy)) {
        vertical->translate(dx, dy);
    } else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Vertical.translate()" );
      return NULL;
    }
    HCATCH

    Py_RETURN_NONE;
  }


  // ---------------------------------------------------------------
  // PyVertical Attribute Method table.

  PyMethodDef PyVertical_Methods[] =
    { { "create"     , (PyCFunction)PyVertical_create     , METH_VARARGS|METH_STATIC
                     , "Create a new Vertical." }
    , { "getX"       , (PyCFunction)PyVertical_getX       , METH_NOARGS , "Get the segment X position." }
    , { "getDySource", (PyCFunction)PyVertical_getDySource, METH_NOARGS , "Get the segment source Y offset." }
    , { "getDyTarget", (PyCFunction)PyVertical_getDyTarget, METH_NOARGS , "Get the segment target Y offset." }
    , { "setX"       , (PyCFunction)PyVertical_setX       , METH_VARARGS, "Modify the segment X position." }
    , { "setDySource", (PyCFunction)PyVertical_setDySource, METH_VARARGS, "Modify the segment source Y offset." }
    , { "setDyTarget", (PyCFunction)PyVertical_setDyTarget, METH_VARARGS, "Modify the segment target Y offset." }
    , { "translate"  , (PyCFunction)PyVertical_translate  , METH_VARARGS, "Translates the Vertical segment of dx and dy." }
    , { "destroy"    , (PyCFunction)PyVertical_destroy    , METH_NOARGS
                     , "Destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };




  // x-------------------------------------------------------------x
  // |                "PyVertical" Object Methods                  |
  // x-------------------------------------------------------------x

  DBoDeleteMethod(Vertical)
  PyTypeObjectLinkPyType(Vertical)


#else  // End of Python Module Code Part.


// x=================================================================x
// |             "PyVertical" Shared Library Code Part               |
// x=================================================================x



  // Link/Creation Method.
  DBoLinkCreateMethod(Vertical)

  // ---------------------------------------------------------------
  // PyVertical Object Definitions.

  PyTypeInheritedObjectDefinitions(Vertical, Segment)

#endif  // End of Shared Library Code Part.


}  // End of extern "C".




}  // End of Isobar namespace.
 
