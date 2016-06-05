
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
// |  C++ Module  :  "./PyDisplayStyle.cpp"                          |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/viewer/PyGraphics.h"
#include "hurricane/viewer/PyDrawingStyle.h"
#include "hurricane/viewer/PyDrawingGroup.h"
#include "hurricane/viewer/PyDisplayStyle.h"
#include "hurricane/viewer/PyHSVr.h"
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


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(DisplayStyle,ds,function)


#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |           "PyDisplayStyle" Python Module Code Part              |
// +=================================================================+


  static PyObject* PyDisplayStyle_new ( PyTypeObject* type, PyObject* args, PyObject* kwds )
  {
    cdebug.log(20) << "PyDisplayStyle_new()" << endl;

    DisplayStyle*   ds   = NULL;
    PyDisplayStyle* pyDs = (PyDisplayStyle*)type->tp_alloc(type,0);

    HTRY
    if ( pyDs ) {
      char* name = NULL;
      if (PyArg_ParseTuple(args,"s:DisplayStyle.new", &name)) {
        ds = new DisplayStyle(Name(name));
      } else {
        PyErr_SetString ( ConstructorError, "invalid number of parameters for DisplayStyle()." );
        return NULL;
      }
      pyDs->_object = ds;
    }
    HCATCH

    return (PyObject*)pyDs;
  }


  PyObject* PyDisplayStyle_getDarkening ( PyDisplayStyle* self, PyObject* args )
  {
    cdebug.log(20) << "PyDisplayStyle_getDarkening()" << endl;

    DisplayStyle::HSVr* darkening = new DisplayStyle::HSVr();

    HTRY
    METHOD_HEAD("DisplayStyle.getDarkening()")
    *darkening = ds->getDarkening();
    HCATCH

    return PyHSVr_Link(darkening);
  }


  PyObject* PyDisplayStyle_getGroup ( PyDisplayStyle* self, PyObject* args )
  {
    cdebug.log(20) << "PyDisplayStyle_getGroup()" << endl;

    const char* group = NULL;

    HTRY
    METHOD_HEAD("DisplayStyle.getGroup()")
    char* name = NULL;
    if (PyArg_ParseTuple(args,"s:DisplayStyle.getGroup", &name)) {
      group = getString( ds->getPattern(Name(name)) ).c_str();
    } else {
      PyErr_SetString ( ConstructorError, "DisplayStyle.getGroup(): Invalid/bad type parameters." );
      return NULL;
    }
    HCATCH

    return PyString_FromString(group);
  }


  PyObject* PyDisplayStyle_getPattern ( PyDisplayStyle* self, PyObject* args )
  {
    cdebug.log(20) << "PyDisplayStyle_getPattern()" << endl;

    const char* pattern = NULL;

    HTRY
    METHOD_HEAD("DisplayStyle.getPattern()")
    char* name = NULL;
    if (PyArg_ParseTuple(args,"s:DisplayStyle.getPattern", &name)) {
      pattern = ds->getPattern (Name(name)).c_str();
    } else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for getPattern." );
      return NULL;
    }
    HCATCH

    return PyString_FromString(pattern);
  }


  PyObject* PyDisplayStyle_getThreshold ( PyDisplayStyle* self, PyObject* args )
  {
    cdebug.log(20) << "PyDisplayStyle_getThreshold()" << endl;

    float threshold = 0.0;

    HTRY
    METHOD_HEAD("DisplayStyle.getThreshold()")
    char* name = NULL;
    if (PyArg_ParseTuple(args,"s:DisplayStyle.getThreshold", &name)) {
      threshold = ds->getThreshold(Name(name));
    } else {
      PyErr_SetString ( ConstructorError, "DisplayStyle.getThreshold(): Invalid/bad type parameters." );
      return NULL;
    }
    HCATCH

    return Py_BuildValue("f",threshold);
  }


  PyObject* PyDisplayStyle_find ( PyDisplayStyle* self, PyObject* args )
  {
    cdebug.log(20) << "PyDisplayStyle_find()" << endl;

    DrawingStyle drawingStyle;

    HTRY
    METHOD_HEAD("DisplayStyle.find()")
    char* name = NULL;
    if (PyArg_ParseTuple(args,"s:DisplayStyle.find", &name)) {
      drawingStyle = ds->find(Name(name));
    } else {
      PyErr_SetString ( ConstructorError, "DisplayStyle.find(): Invalid/bad type parameters." );
      return NULL;
    }
    HCATCH

    return PyRawDrawingStyle_Link(drawingStyle);
  }


  PyObject* PyDisplayStyle_setDescription ( PyDisplayStyle* self, PyObject* args )
  {
    cdebug.log(20) << "PyDisplayStyle_setDescription()" << endl;

    HTRY
    METHOD_HEAD("DisplayStyle.setDescription()")
    char* name = NULL;
    if (PyArg_ParseTuple(args,"s:DisplayStyle.setDescription", &name)) {
      ds->setDescription(name);
    } else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for setDescription." );
      return NULL;
    }
    HCATCH

    Py_RETURN_NONE;
  }


  PyObject* PyDisplayStyle_setDarkening ( PyDisplayStyle* self, PyObject* args )
  {
    cdebug.log(20) << "PyDisplayStyle_setDarkening()" << endl;

    HTRY
    METHOD_HEAD("DisplayStyle.setDarkening()")

    PyObject* darkening;
    if (!ParseOneArg("DisplayStyle.setDarkening", args, ":hsvr", &darkening)) {
      PyErr_SetString ( ConstructorError, "Bad parameters given to DisplayStyle.setDarkening()." );
      return NULL;
    }
    ds->setDarkening(*PYHSVR_O(darkening));
    HCATCH

    Py_RETURN_NONE;
  }


  PyObject* PyDisplayStyle_inheritFrom ( PyDisplayStyle* self, PyObject* args )
  {
    cdebug.log(20) << "PyDisplayStyle_inheritFrom()" << endl;

    HTRY
    METHOD_HEAD("DisplayStyle.inheritFrom()")
    char* name = NULL;
    if (PyArg_ParseTuple(args,"s:DisplayStyle.inheritFrom", &name)) {
      DisplayStyle* baseStyle = Graphics::getStyle ( name );
      if ( !baseStyle ) {
        ostringstream error;
        error << "Base style <" << name << "> of <" << ds->getName() <<  "> doesn't exist (yet?).";
        PyErr_SetString ( ConstructorError, error.str().c_str() );
        return NULL;
      }
      ds->inheritFrom(baseStyle);
    } else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for DisplayStyle.inheritFrom." );
      return NULL;
    }
    HCATCH

    Py_RETURN_NONE;
  }


  PyObject* PyDisplayStyle_addDrawingStyle ( PyDisplayStyle* self, PyObject* args, PyObject* kwArgs )
  {
    cdebug.log(20) << "PyDisplayStyle_addDrawingStyle()" << endl;

    HTRY
    METHOD_HEAD("DisplayStyle.addDrawingStyle()")
    char*     argGroupKey    = (char*)"Not part of any group (error)";
    char*     argKey         = (char*)"No key defined (error)";
    char*     argPattern     = (char*)"FFFFFFFFFFFFFFFF";
    char*     argColor       = (char*)"255,255,255";
    int       borderWidth    = 0;
    float     threshold      = 1.0;
    PyObject* argGoMatched   = NULL;

    static char* keywords[] = { (char*)"group"
                              , (char*)"name"
                              , (char*)"color"
                              , (char*)"pattern"
                              , (char*)"border"
                              , (char*)"threshold"
                              , (char*)"goMatched"
                              , NULL };

    if (PyArg_ParseTupleAndKeywords( args
                                   , kwArgs
                                   , "ss|ssifO:DisplayStyle.addDrawingStyle"
                                   , keywords
                                   , &argGroupKey
                                   , &argKey
                                   , &argColor
                                   , &argPattern
                                   , &borderWidth
                                   , &threshold
                                   , &argGoMatched )) {
      int    red         = 255;
      int    green       = 255;
      int    blue        = 255;
      bool   goMatched   = (argGoMatched == NULL) or (PyObject_IsTrue(argGoMatched));

    //cerr << "Adding DrawingStyle" << argGroupKey << " "<< argKey << " " << argPattern << endl;

      parseRgbColor( argColor, red, green, blue );
      ds->addDrawingStyle( Name(argGroupKey)
                         , Name(argKey)
                         , argPattern
                         , red
                         , green
                         , blue
                         , borderWidth
                         , threshold
                         , goMatched
                         );
    } else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for DisplayStyle.addDrawingStyle." );
      return NULL;
    }
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyDisplayStyle_Repr ( PyDisplayStyle* self )
  {
    if ( self->ACCESS_OBJECT == NULL )
      return PyString_FromString("<PyObject unbound>");

    string s = "<DisplayStyle " + getString(self->_object->getName()) + ">";
    return PyString_FromString(s.c_str());
  }

  
  // Standart Accessors (Attributes).
  GetNameMethod(DisplayStyle,ds)
  DirectGetStringAttribute(PyDisplayStyle_getDescription,getDescription,PyDisplayStyle,DisplayStyle)
  accessorVectorFromVoid(getDrawingGroups,PyDisplayStyle,DisplayStyle,DrawingGroup)


  // Standart Destroy (Attribute).
  // DBoDestroyAttribute(PyDisplayStyle_destroy,PyDisplayStyle)


  PyMethodDef PyDisplayStyle_Methods[] =
    { { "getName"             , (PyCFunction)PyDisplayStyle_getName         , METH_NOARGS
                              , "Returns the name (id) of the DisplayStyle." }
    , { "getDescription"      , (PyCFunction)PyDisplayStyle_getDescription  , METH_NOARGS
                              , "Returns the description of the DisplayStyle (this is not the id)." }
    , { "getDarkening"        , (PyCFunction)PyDisplayStyle_getDarkening    , METH_NOARGS
                              , "Returns the darkening factor, applied on unselected items." }
    , { "getGroup"            , (PyCFunction)PyDisplayStyle_getGroup        , METH_VARARGS
                              , "Returns the first group containing this DrawingStyle." }
    , { "getPattern"          , (PyCFunction)PyDisplayStyle_getPattern      , METH_VARARGS
                              , "Gets a pattern, by key name." }
    , { "getThreshold"        , (PyCFunction)PyDisplayStyle_getThreshold    , METH_VARARGS
                              , "Returns the display threshold of the given DrawingStyle." }
    , { "getDrawingGroups"    , (PyCFunction)PyDisplayStyle_getDrawingGroups, METH_NOARGS
                              , "Returns the list of DrawingGroups (iterable only)." }
    , { "find"                , (PyCFunction)PyDisplayStyle_find            , METH_VARARGS
                              , "Find a DrawingStyle, by key name." }
    , { "setDescription"      , (PyCFunction)PyDisplayStyle_setDescription  , METH_VARARGS
                              , "Set the Display Style informational text." }
    , { "inheritFrom"         , (PyCFunction)PyDisplayStyle_inheritFrom     , METH_VARARGS
                              , "Set the base style (erase current and clone)." }
    , { "setDarkening"        , (PyCFunction)PyDisplayStyle_setDarkening    , METH_VARARGS
                              , "Set the Display Style darkning (unselected background)." }
    , { "addDrawingStyle"     , (PyCFunction)PyDisplayStyle_addDrawingStyle , METH_VARARGS|METH_KEYWORDS
                              , "Adds a new Drawing Style, or overwrite an older." }
  //, { "destroy"             , (PyCFunction)PyDisplayStyle_destroy         , METH_NOARGS
  //                          , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  PythonOnlyDeleteMethod(DisplayStyle)
  DirectHashMethod(PyDisplayStyle_Hash, PyDisplayStyle)

  extern void  PyDisplayStyle_LinkPyType() {
    cdebug.log(20) << "PyDisplayStyle_LinkType()" << endl;

    PyTypeDisplayStyle.tp_new     =              PyDisplayStyle_new;
    PyTypeDisplayStyle.tp_dealloc = (destructor) PyDisplayStyle_DeAlloc;
    PyTypeDisplayStyle.tp_repr    = (reprfunc)   PyDisplayStyle_Repr;
    PyTypeDisplayStyle.tp_str     = (reprfunc)   PyDisplayStyle_Repr;
    PyTypeDisplayStyle.tp_hash    = (hashfunc)   PyDisplayStyle_Hash;
    PyTypeDisplayStyle.tp_methods = PyDisplayStyle_Methods;
  }

  
  IteratorNextMethod(DisplayStyle)
  VectorMethods     (DisplayStyle)


#else  // End of Python Module Code Part.

// +=================================================================+
// |            "PyDisplayStyle" Shared Library Code Part            |
// +=================================================================+

  // Link/Creation Method.
  LinkCreateMethod(DisplayStyle)
  PyTypeObjectDefinitionsOfModule(Viewer,DisplayStyle)
  PyTypeVectorObjectDefinitions  (DisplayStyleVector)
  PyTypeVectorObjectDefinitions  (DisplayStyleVectorIterator)


  extern  void  PyDisplayStyle_postModuleInit ()
  {
    PyDict_SetItemString ( PyTypeDisplayStyle.tp_dict, "HSVr", (PyObject*)&PyTypeHSVr );
  }



#endif  // End of Shared Library Code Part.


}  // extern "C".


}  // Hurricane namespace.
 
