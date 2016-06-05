
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2012-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyDrawingGroup.cpp"                          |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/viewer/PyDrawingStyle.h"
#include "hurricane/viewer/PyDrawingGroup.h"
#include <sstream>


namespace {

  using std::istringstream;


  // void  parseInt ( const char* s, int& value )
  // { istringstream is ( s ); is >> value; }


  // void  parseFloat ( const char* s, float& value )
  // { istringstream is ( s ); is >> value; }


  void  parseRgbColor ( const char* color, int& red, int& green, int& blue )
  {
    char colon1, colon2;

    istringstream is ( color );
    is >> red >> colon1 >> green >> colon2 >> blue;
  }



} // Anonymous namespace.


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


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(DrawingGroup,dg,function)


#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |           "PyDrawingGroup" Python Module Code Part              |
// +=================================================================+


  PyObject* PyDrawingGroup_find ( PyDrawingGroup* self, PyObject* args )
  {
    cdebug.log(20) << "PyDrawingGroup_find()" << endl;

    DrawingStyle drawingStyle;

    HTRY
    METHOD_HEAD("DrawingGroup.find()")
    char* name = NULL;
    if (PyArg_ParseTuple(args,"s:DrawingGroup.find", &name)) {
      drawingStyle = dg->find(Name(name));
    } else {
      PyErr_SetString ( ConstructorError, "DrawingGroup.find(): Invalid/bad type parameters." );
      return NULL;
    }
    HCATCH

    return PyRawDrawingStyle_Link(drawingStyle);
  }


  PyObject* PyDrawingGroup_addDrawingStyle ( PyDrawingGroup* self, PyObject* args, PyObject* kwArgs )
  {
    cdebug.log(20) << "PyDrawingGroup_addDrawingStyle()" << endl;

    HTRY
    METHOD_HEAD("DrawingGroup.addDrawingStyle()")
    char*     argKey         = (char*)"No key defined (error)";
    char*     argPattern     = (char*)"FFFFFFFFFFFFFFFF";
    char*     argColor       = (char*)"255,255,255";
    int       borderWidth    = 0;
    float     threshold      = 1.0;
    char*     argGoMatched   = (char*)"true";

    static char* keywords[] = { (char*)"name"
                              , (char*)"color"
                              , (char*)"pattern"
                              , (char*)"border"
                              , (char*)"threshold"
                              , (char*)"goMatched"
                              , NULL };

    if (PyArg_ParseTupleAndKeywords( args
                                   , kwArgs
                                   , "ss|ssifs:DrawingGroup.addDrawingStyle"
                                   , keywords
                                   , &argKey
                                   , &argColor
                                   , &argPattern
                                   , &borderWidth
                                   , &threshold
                                   , &argGoMatched )) {
      int    red         = 255;
      int    green       = 255;
      int    blue        = 255;
      bool   goMatched   = (string(argGoMatched) == "true");

    //cerr << "Adding DrawingStyle" << argGroupKey << " "<< argKey << " " << argPattern << endl;

      parseRgbColor( argColor, red, green, blue );
      dg->addDrawingStyle( Name(argKey)
                         , argPattern
                         , red
                         , green
                         , blue
                         , borderWidth
                         , threshold
                         , goMatched
                         );
    } else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for DrawingGroup.addDrawingStyle." );
      return NULL;
    }
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyDrawingGroup_Repr ( PyDrawingGroup* self )
  {
    if ( self->ACCESS_OBJECT == NULL )
      return PyString_FromString("<PyObject unbound>");

    string s = "<DrawingGroup " + getString(self->_object->getName()) + ">";
    return PyString_FromString(s.c_str());
  }

  
  // Standart Accessors (Attributes).
  GetNameMethod(DrawingGroup,dg)
  accessorVectorFromVoid(getDrawingStyles,PyDrawingGroup,DrawingGroup,DrawingStyle)


  // Standart Destroy (Attribute).
  // DBoDestroyAttribute(PyDrawingGroup_destroy,PyDrawingGroup)


  PyMethodDef PyDrawingGroup_Methods[] =
    { { "getName"             , (PyCFunction)PyDrawingGroup_getName         , METH_NOARGS
                              , "Adds a new Drawing Style, or overwrite an older." }
    , { "getDrawingStyles"    , (PyCFunction)PyDrawingGroup_getDrawingStyles, METH_NOARGS
                              , "Returns the list of associated DrawingStyle (iterable only)." }
    , { "find"                , (PyCFunction)PyDrawingGroup_find            , METH_VARARGS
                              , "Finds a DrawingStyle, by key name." }
    , { "addDrawingStyle"     , (PyCFunction)PyDrawingGroup_addDrawingStyle , METH_VARARGS|METH_KEYWORDS
                              , "Adds a new Drawing Style, or overwrite an older." }
  //, { "destroy"             , (PyCFunction)PyDrawingGroup_destroy         , METH_NOARGS
  //                          , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  PythonOnlyDeleteMethod(DrawingGroup)
  DirectHashMethod(PyDrawingGroup_Hash, PyDrawingGroup)

  extern void  PyDrawingGroup_LinkPyType() {
    cdebug.log(20) << "PyDrawingGroup_LinkType()" << endl;

    PyTypeDrawingGroup.tp_dealloc = (destructor) PyDrawingGroup_DeAlloc;
    PyTypeDrawingGroup.tp_repr    = (reprfunc)   PyDrawingGroup_Repr;
    PyTypeDrawingGroup.tp_str     = (reprfunc)   PyDrawingGroup_Repr;
    PyTypeDrawingGroup.tp_hash    = (hashfunc)   PyDrawingGroup_Hash;
    PyTypeDrawingGroup.tp_methods = PyDrawingGroup_Methods;
  }

  
  IteratorNextMethod(DrawingGroup)
  VectorMethods     (DrawingGroup)


#else  // End of Python Module Code Part.

// +=================================================================+
// |            "PyDrawingGroup" Shared Library Code Part            |
// +=================================================================+

  // Link/Creation Method.
  LinkCreateMethod(DrawingGroup)
  PyTypeObjectDefinitionsOfModule(Viewer,DrawingGroup)
  PyTypeVectorObjectDefinitions  (DrawingGroupVector)
  PyTypeVectorObjectDefinitions  (DrawingGroupVectorIterator)



#endif  // End of Shared Library Code Part.


}  // extern "C".


}  // Hurricane namespace.
 
