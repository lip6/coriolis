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
// |  C++ Module  :  "./PyPad.cpp"                                   |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyLayer.h"
#include "hurricane/isobar/PyBox.h"
#include "hurricane/isobar/PyPad.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject._baseObject)
#define METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Pad,pad,function)


// x=================================================================x
// |              "PyPad" Python Module Code Part                |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  // x-------------------------------------------------------------x
  // |                 "PyPad" Attribute Methods                   |
  // x-------------------------------------------------------------x


  // Standard Accessors (Attributes).
  DirectGetLongAttribute(PyPad_getX, getX, PyPad, Pad)
  DirectGetLongAttribute(PyPad_getY, getY, PyPad, Pad)

  // Standard Destroy (Attribute).
  DBoDestroyAttribute(PyPad_destroy, PyPad)

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPad_create ()"

  static PyObject* PyPad_create ( PyObject*, PyObject *args ) {
    cdebug.log(20) << "PyPad_create()" << endl;

    PyObject* arg0;
    PyObject* arg1;
    PyObject* arg2;
    Pad* pad = NULL;

    HTRY
    __cs.init ("Pad.create");
    if (!PyArg_ParseTuple(args,"O&O&O&:Pad.create"
                           ,Converter,&arg0
                           ,Converter,&arg1
                           ,Converter,&arg2
                           )) {
        PyErr_SetString ( ConstructorError, "invalid number of parameters for Pad constructor." );
        return NULL;
    }

    if ( __cs.getObjectIds() == ":ent:layer:box") {
      pad = Pad::create(PYNET_O(arg0), PYLAYER_O(arg1), *PYBOX_O(arg2));
    } else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Pad constructor." );
      return NULL;
    }

    HCATCH

    return PyPad_Link(pad);
  }

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPad_getBoundingBox ()"

  static PyObject* PyPad_getBoundingBox( PyPad *self ) {
    cdebug.log(20) << "PyPad_getBoundingBox()" << endl;

    METHOD_HEAD ( "Pad.BoundingBox()" )

    PyBox* pyBox = PyObject_NEW ( PyBox, &PyTypeBox );
    if (pyBox == NULL) { return NULL; }
    
    HTRY
    pyBox->_object = new Box ( pad->getBoundingBox() );
    HCATCH    

    return ( (PyObject*)pyBox );
  }

  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPad_setBoudingBox()"

  static PyObject* PyPad_setBoundingBox(PyPad *self, PyObject* args) {
    cdebug.log(20) << "Pad.setBoudingBox()" << endl;

    HTRY
    METHOD_HEAD ( "Pad.setBoudingBox()" )

    PyBox* box;
    if (!ParseOneArg("Pad.setBoundingBox", args, BOX_ARG, (PyObject**)&box)) {
        return NULL;
    }
    pad->setBoundingBox(*PYBOX_O(box));
    HCATCH

    Py_RETURN_NONE;
  }

  
  // ---------------------------------------------------------------
  // Attribute Method  :  "PyPad_translate ()"

  static PyObject* PyPad_translate ( PyPad *self, PyObject* args ) {
    cdebug.log(20) << "PyPad_translate ()" << endl;
    
    HTRY
    METHOD_HEAD ( "Pad.translate()" )
    DbU::Unit dx=0, dy=0;
    if (PyArg_ParseTuple(args,"ll:Pad.translate", &dx, &dy)) {
        pad->translate(dx, dy);
    } else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Pad.translate()" );
      return NULL;
    }
    HCATCH

    Py_RETURN_NONE;
  }


  // ---------------------------------------------------------------
  // PyPad Attribute Method table.

  PyMethodDef PyPad_Methods[] =
    { { "create"        , (PyCFunction)PyPad_create        , METH_VARARGS|METH_STATIC
                        , "Create a new Pad." }
    , { "getX"          , (PyCFunction)PyPad_getX          , METH_NOARGS, "Return the Pad X value." }
    , { "getY"          , (PyCFunction)PyPad_getY          , METH_NOARGS, "Return the Pad Y value." }
    , { "getBoundingBox", (PyCFunction)PyPad_getBoundingBox, METH_NOARGS, "Return the Pad Bounding Box." }
    , { "setBoundingBox", (PyCFunction)PyPad_setBoundingBox, METH_VARARGS, "Sets the Pad Bounding Box." }
    , { "translate"     , (PyCFunction)PyPad_translate     , METH_VARARGS, "Translates the Pad of dx and dy." }
    , { "destroy"       , (PyCFunction)PyPad_destroy       , METH_NOARGS
                        , "Destroy associated hurricane object, the python object remains." }
    , {NULL, NULL, 0, NULL}  /* sentinel */
    };


  // x-------------------------------------------------------------x
  // |                "PyPad" Object Methods                       |
  // x-------------------------------------------------------------x


  DBoDeleteMethod(Pad)
  PyTypeObjectLinkPyType(Pad)


#else  // End of Python Module Code Part.


// x=================================================================x
// |             "PyPad" Shared Library Code Part                    |
// x=================================================================x


  // Link/Creation Method.
  DBoLinkCreateMethod(Pad)
  PyTypeInheritedObjectDefinitions(Pad, Component)

#endif  // Shared Library Code Part.

}  // extern "C".




}  // End of Isobar namespace.
