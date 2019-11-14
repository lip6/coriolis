// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2012-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyDrawingStyle.cpp"                          |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/viewer/PyDrawingStyle.h"


namespace  Hurricane {

  using std::cerr;
  using std::endl;
  using std::hex;
  using Isobar::ProxyProperty;
  using Isobar::ProxyError;
  using Isobar::ConstructorError;
  using Isobar::HurricaneError;
  using Isobar::HurricaneWarning;
  using Isobar::ParseOneArg;
  using Isobar::ParseTwoArg;
  using Isobar::getPyHash;


extern "C" {


// #undef  GENERIC_METHOD_HEAD
// #define GENERIC_METHOD_HEAD(SELF_TYPE,SELF_OBJECT,function)                
//   if ( self->ACCESS_OBJECT.get() == NULL ) {                               
//       PyErr_SetString ( ProxyError, "Attempt to call " function " on an unbound hurricane object" ); 
//       return NULL;                                                         
//     }                                                                      
//     SELF_TYPE SELF_OBJECT = self->ACCESS_OBJECT;
//
// #define METHOD_HEAD(function)  GENERIC_METHOD_HEAD(DrawingStyle,ds,function)

#define METHOD_HEAD(function)  GENERIC_METHOD_HEAD(RawDrawingStyle,ds,function)


#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |           "PyDrawingStyle" Python Module Code Part              |
// +=================================================================+


  PyObject* PyRawDrawingStyle_getPattern ( PyRawDrawingStyle* self, PyObject* args )
  {
    cdebug_log(20,0) << "PyDrawingStyle_getPattern()" << endl;

    const char* pattern = NULL;

    HTRY
    METHOD_HEAD("DrawingStyle.getPattern()")
    pattern = ds->getPattern().c_str();
    HCATCH

    return PyString_FromString(pattern);
  }


  PyObject* PyRawDrawingStyle_getThreshold ( PyRawDrawingStyle* self, PyObject* args )
  {
    cdebug_log(20,0) << "PyRawDrawingStyle_getThreshold()" << endl;

    float threshold = 0.0;

    HTRY
    METHOD_HEAD("DrawingStyle.getThreshold()")
    threshold = ds->getThreshold();
    HCATCH

    return Py_BuildValue("f",threshold);
  }

  
  // Standart Accessors (Attributes).
  GetNameMethod(RawDrawingStyle,ds)
  DirectGetBoolAttribute(PyRawDrawingStyle_isGoMatched,isGoMatched,PyRawDrawingStyle,RawDrawingStyle)


  // Standart Destroy (Attribute).
  // DBoDestroyAttribute(PyRawDrawingStyle_destroy,PyRawDrawingStyle)


  PyMethodDef PyRawDrawingStyle_Methods[] =
    { { "isGoMatched"         , (PyCFunction)PyRawDrawingStyle_isGoMatched , METH_NOARGS
                              , "Tells if this style is associated with a GO (Graphical Object)." }
    , { "getName"             , (PyCFunction)PyRawDrawingStyle_getName     , METH_NOARGS
                              , "Return the name of the DrawingStyle." }
    , { "getPattern"          , (PyCFunction)PyRawDrawingStyle_getPattern  , METH_NOARGS
                              , "Return the pattern of the DrawingStyle." }
    , { "getThreshold"        , (PyCFunction)PyRawDrawingStyle_getThreshold, METH_NOARGS
                              , "Returns the display threshold." }
  //, { "destroy"             , (PyCFunction)PyRawDrawingStyle_destroy     , METH_NOARGS
  //                          , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  PythonOnlyDeleteMethod(RawDrawingStyle)
  PyTypeObjectLinkPyType(RawDrawingStyle)

  
  static PyObject* PyDrawingStyleIteratorNext(PyDrawingStyleVectorIterator* pyIterator)
  {
      HTRY
        if (pyIterator->_iterator != pyIterator->_pyContainer->_container->end()) {
          DrawingStyle  object = *(pyIterator->_iterator);
          ++(pyIterator->_iterator);
          return PyRawDrawingStyle_Link(object);
        }
      HCATCH
      return NULL;
  }

  VectorMethods(DrawingStyle)


#else  // End of Python Module Code Part.


// +=================================================================+
// |            "PyDrawingStyle" Shared Library Code Part            |
// +=================================================================+

  // Link/Creation Method.
  PyObject* PyRawDrawingStyle_Link ( DrawingStyle ds )
  {
    if ( ds.get() == NULL ) Py_RETURN_NONE;

  //   PyDrawingStyle* pyDs = NULL;
  //   HTRY
  //   pyDs = PyObject_NEW(PyDrawingStyle, &PyTypeDrawingStyle);
  //   if (pyDs == NULL) return NULL;
 
  // // Here is a very dirty hack:
  // //   The Python object is a C struct, so the construtor of _object,
  // // which is a shared_ptr<> never gets called (but memory is allocated).
  // // So we end up with a shared_ptr<> with an internal state all garbled.
  // // To set it right, we perform a direct memory copy from a fresh one
  // // we allocate dynamically (so it doesn't get destructed at the function's
  // // exit).
  //   DrawingStyle* empty = new DrawingStyle();
  //   memcpy((void*)&(pyDs->_object),(void*)empty,sizeof(shared_ptr<RawDrawingStyle>));

  // //cerr << "ptr:"        << pyDs->_object.get()
  // //     << " use_count:" << pyDs->_object.use_count() << endl;

  //   pyDs->_object.swap(ds);
  //   HCATCH

    PyRawDrawingStyle* pyDs = NULL;
    HTRY
    pyDs = PyObject_NEW(PyRawDrawingStyle, &PyTypeRawDrawingStyle);
    if (pyDs == NULL) return NULL;
    pyDs->_object = ds.get();
    HCATCH

    return (PyObject*)pyDs;
  }


  PyTypeObjectDefinitions(RawDrawingStyle)

  PyTypeVectorObjectDefinitions(DrawingStyleVector)
  PyTypeVectorObjectDefinitions(DrawingStyleVectorIterator)


#endif  // End of Shared Library Code Part.


}  // extern "C".


}  // Hurricane namespace.
 
