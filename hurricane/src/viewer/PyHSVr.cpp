
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2010-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyHSVr.cpp"                                  |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/viewer/PyGraphics.h"
#include "hurricane/viewer/PyHSVr.h"
#include <sstream>


namespace  Hurricane {

  using std::cerr;
  using std::endl;
  using std::hex;
  using std::ostringstream;
  using Isobar::ProxyProperty;
  using Isobar::ProxyError;
  using Isobar::ConstructorError;
  using Isobar::HurricaneError;
  using Isobar::HurricaneWarning;
  using Isobar::ParseOneArg;
  using Isobar::ParseTwoArg;


extern "C" {


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(DisplayStyle::HSVr,hsvr,function)


#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |           "PyHSVr" Python Module Code Part              |
// +=================================================================+


  static PyObject* PyHSVr_new ( PyTypeObject* type, PyObject* args, PyObject* kwds )
  {
    cdebug_log(20,0) << "PyHSVr_new()" << endl;

    DisplayStyle::HSVr* hsvr   = NULL;
    PyHSVr*             pyHSVr = (PyHSVr*)type->tp_alloc(type,0);

    HTRY
    if ( pyHSVr ) {
      float hue        = 1.0;
      float saturation = 1.0;
      float value      = 1.0;
      if (PyArg_ParseTuple(args,"fff:HSVr.new", &hue, &saturation, &value)) {
        hsvr = new DisplayStyle::HSVr(hue,saturation,value);
      } else {
        PyErr_SetString ( ConstructorError, "invalid number of parameters for HSVr()." );
        return NULL;
      }
      pyHSVr->_object = hsvr;
    }
    HCATCH

    return (PyObject*)pyHSVr;
  }


  static PyObject* PyHSVr_Repr ( PyHSVr* self )
  {
    DisplayStyle::HSVr* hsvr = self->_object;

    if ( hsvr == NULL )
      return PyString_FromString("<PyObject unbound>");

    ostringstream s;
    s << "<HSVr " << hsvr->getHue()
      << " "      << hsvr->getSaturation()
      << " "      << hsvr->getValue()
      << ">";
    return PyString_FromString(s.str().c_str());
  }

  
  // Standart Accessors (Attributes).
  DirectGetDoubleAttribute(PyHSVr_GetHue       ,getHue       ,PyHSVr,DisplayStyle::HSVr)
  DirectGetDoubleAttribute(PyHSVr_GetSaturation,getSaturation,PyHSVr,DisplayStyle::HSVr)
  DirectGetDoubleAttribute(PyHSVr_GetValue     ,getValue     ,PyHSVr,DisplayStyle::HSVr)
  DirectSetDoubleAttribute(PyHSVr_SetHue       ,setHue       ,PyHSVr,DisplayStyle::HSVr)
  DirectSetDoubleAttribute(PyHSVr_SetSaturation,setSaturation,PyHSVr,DisplayStyle::HSVr)
  DirectSetDoubleAttribute(PyHSVr_SetValue     ,setValue     ,PyHSVr,DisplayStyle::HSVr)

  // Standart Predicates (Attributes).
  DirectGetBoolAttribute(PyHSVr_IsId,isId,PyHSVr,DisplayStyle::HSVr)


  PyMethodDef PyHSVr_Methods[] =
    { { "isId"         , (PyCFunction)PyHSVr_IsId         , METH_NOARGS , "Tells if the HSV is ident." }
    , { "getHue"       , (PyCFunction)PyHSVr_GetHue       , METH_NOARGS , "Return the HSV hue." }
    , { "getSaturation", (PyCFunction)PyHSVr_GetSaturation, METH_NOARGS , "Return the HSV saturation." }
    , { "getValue"     , (PyCFunction)PyHSVr_GetValue     , METH_NOARGS , "Return the HSV value." }
    , { "setHue"       , (PyCFunction)PyHSVr_SetHue       , METH_VARARGS, "Modify the HSV hue." }
    , { "setSaturation", (PyCFunction)PyHSVr_SetSaturation, METH_VARARGS, "Modify the HSV saturation." }
    , { "setValue"     , (PyCFunction)PyHSVr_SetValue     , METH_VARARGS, "Modify the HSV value." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  DirectDeleteMethod(PyHSVr_DeAlloc,PyHSVr)
  DirectHashMethod(PyHSVr_Hash, PyHSVr)

  extern void  PyHSVr_LinkPyType() {
    cdebug_log(20,0) << "PyHSVr_LinkType()" << endl;

    PyTypeHSVr.tp_new     =              PyHSVr_new;
    PyTypeHSVr.tp_dealloc = (destructor) PyHSVr_DeAlloc;
    PyTypeHSVr.tp_repr    = (reprfunc)   PyHSVr_Repr;
    PyTypeHSVr.tp_str     = (reprfunc)   PyHSVr_Repr;
    PyTypeHSVr.tp_hash    = (hashfunc)   PyHSVr_Hash;
    PyTypeHSVr.tp_methods = PyHSVr_Methods;
  }


#else  // End of Python Module Code Part.

// +=================================================================+
// |                "PyHSVr" Shared Library Code Part                |
// +=================================================================+

  // Link/Creation Method.
  PyObject* PyHSVr_Link ( DisplayStyle::HSVr* object )
  {
    if ( object == NULL ) Py_RETURN_NONE;

    PyHSVr* pyObject = NULL;
    HTRY
    pyObject = PyObject_NEW(PyHSVr, &PyTypeHSVr);
    if (pyObject == NULL) return NULL;

    pyObject->_object = object;
    HCATCH

    return (PyObject*)pyObject;
  }

  PyTypeObjectDefinitions(HSVr)


#endif  // End of Shared Library Code Part.


}  // extern "C".


}  // Hurricane namespace.
 
