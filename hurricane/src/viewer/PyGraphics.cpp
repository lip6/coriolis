
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2010-2010, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyGraphics.cpp"                              |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/viewer/PyGraphics.h"
#include "hurricane/viewer/PyHSVr.h"
#include "hurricane/viewer/PyDisplayStyle.h"


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


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(Graphics,graphics,function)


#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |             "PyGraphics" Python Module Code Part                |
// +=================================================================+


  static PyObject* PyGraphics_get ( PyObject* )
  {
    cdebug_log(20,0) << "PyGraphics_get()" << endl;

    Graphics*   graphics   = NULL;
    PyGraphics* pyGraphics = NULL;
    
    HTRY
    graphics = Graphics::getGraphics ();

    pyGraphics = PyObject_NEW ( PyGraphics, &PyTypeGraphics );
    if ( graphics == NULL ) return NULL;

    pyGraphics->_object = graphics;
    HCATCH

    return (PyObject*)pyGraphics;
  }


  PyObject* PyGraphics_isEnabled ( PyGraphics* )
  {
    cdebug_log(20,0) << "PyGraphics_isEnabled()" << endl;

    bool isEnabled = false;
    HTRY
      Graphics* graphics = Graphics::getGraphics ();
      isEnabled = graphics->isEnabled();
    HCATCH

    if (isEnabled) Py_RETURN_TRUE;
    Py_RETURN_FALSE;
  }


  PyObject* PyGraphics_isHighDpi ( PyGraphics* )
  {
    cdebug_log(20,0) << "PyGraphics_isHighDpi()" << endl;

    bool isHighDpi = false;
    HTRY
      Graphics* graphics = Graphics::getGraphics ();
      isHighDpi = graphics->isHighDpi();
    HCATCH

    if (isHighDpi) Py_RETURN_TRUE;
    Py_RETURN_FALSE;
  }


  static PyObject* PyGraphics_setStyle ( PyObject*, PyObject* args )
  {
    cdebug_log(20,0) << "PyGraphics_setStyle()" << endl;

    HTRY
    Graphics* graphics = Graphics::getGraphics ();

    char* name = NULL;
    if (PyArg_ParseTuple(args,"s:Graphics.setStyle", &name)) {
      graphics->setStyle(Name(name));
    } else {
      PyErr_SetString ( ConstructorError, "Bad parameters given to Graphics.setStyle()." );
      return NULL;
    }
    HCATCH

    Py_RETURN_NONE;
  }


  PyObject* PyGraphics_getPattern ( PyGraphics* , PyObject* args )
  {
    cdebug_log(20,0) << "PyGraphics_getPattern()" << endl;

    const char* pattern = NULL;

    HTRY
    Graphics* graphics = Graphics::getGraphics ();
    char*     name     = NULL;
    if (PyArg_ParseTuple(args,"s:Graphics.getPattern", &name)) {
      pattern = graphics->getPattern (Name(name)).c_str();
    } else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Graphics.getPattern." );
      return NULL;
    }
    HCATCH

    return PyString_FromString(pattern);
  }


  PyObject* PyGraphics_getStyle ( PyGraphics* , PyObject* args )
  {
    cdebug_log(20,0) << "PyGraphics_getStyle()" << endl;

    DisplayStyle* style = NULL;

    HTRY
    Graphics* graphics = Graphics::getGraphics ();
    char*     name     = NULL;
    if (PyArg_ParseTuple(args,"|s:Graphics.getStyle", &name)) {
      if (name != NULL) style = graphics->getStyle(Name(name));
      else              style = graphics->getStyle();
    } else {
      PyErr_SetString ( ConstructorError, "Graphics.getStyle(): Invalid/bad type parameters." );
      return NULL;
    }
    HCATCH

    return PyDisplayStyle_Link(style);
  }


  PyObject* PyGraphics_getGroup ( PyGraphics* , PyObject* args )
  {
    cdebug_log(20,0) << "PyGraphics_getGroup()" << endl;

    const char* group = "NoGroup";

    HTRY
    Graphics* graphics = Graphics::getGraphics ();
    char*     name     = NULL;
    if (PyArg_ParseTuple(args,"s:Graphics.getGroup", &name)) {
      group = getString(graphics->getGroup(Name(name))).c_str();
    } else {
      PyErr_SetString ( ConstructorError, "Graphics.getGroup(): Invalid/bad type parameters." );
      return NULL;
    }
    HCATCH

    return Py_BuildValue("s",group);
  }


  PyObject* PyGraphics_getThreshold ( PyGraphics* , PyObject* args )
  {
    cdebug_log(20,0) << "PyGraphics_getThreshold()" << endl;

    float threshold = 0.0;

    HTRY
    Graphics* graphics = Graphics::getGraphics ();
    char*     name     = NULL;
    if (PyArg_ParseTuple(args,"s:Graphics.getThreshold", &name)) {
      threshold = graphics->getThreshold(Name(name));
    } else {
      PyErr_SetString ( ConstructorError, "Graphics.getThreshold(): Invalid/bad type parameters." );
      return NULL;
    }
    HCATCH

    return Py_BuildValue("f",threshold);
  }


  PyObject* PyGraphics_getDarkening ( PyGraphics* , PyObject* args )
  {
    cdebug_log(20,0) << "PyGraphics_getDarkening()" << endl;

    DisplayStyle::HSVr* darkening = new DisplayStyle::HSVr();

    HTRY
    Graphics* graphics = Graphics::getGraphics ();
    *darkening = graphics->getDarkening();
    HCATCH

    return PyHSVr_Link(darkening);
  }


  PyObject* PyGraphics_toHtml ( PyGraphics* , PyObject* args )
  {
    cdebug_log(20,0) << "PyGraphics_toHtml()" << endl;

    string htmlText = "";

    HTRY
    Graphics* graphics = Graphics::getGraphics ();
    char*     name     = NULL;
    if (PyArg_ParseTuple(args,"s:Graphics.toHtml", &name)) {
      htmlText = graphics->toHtml(name);
    } else {
      PyErr_SetString ( ConstructorError, "Graphics.toHtml(): Invalid/bad type parameters." );
      return NULL;
    }
    HCATCH

    return Py_BuildValue("s",htmlText.c_str());
  }


  PyObject* PyGraphics_breakpointStopCb ( PyGraphics* , PyObject* args )
  {
    cdebug_log(20,0) << "PyGraphics_breakpointStopCb()" << endl;

    bool rvalue = false;

    HTRY
    Graphics* graphics = Graphics::getGraphics ();
    char*     message  = NULL;
    if (PyArg_ParseTuple(args,"s:Graphics.breakpointStopCb", &message)) {
      rvalue = graphics->breakpointStopCb(message);
    } else {
      PyErr_SetString ( ConstructorError, "Graphics.breakpointStopCb(): Invalid/bad type parameters." );
      return NULL;
    }
    HCATCH

    if (rvalue) Py_RETURN_TRUE;
    Py_RETURN_FALSE;
  }


  static PyObject* PyGraphics_addStyle ( PyObject*, PyObject* args )
  {
    cdebug_log(20,0) << "PyGraphics_addStyle()" << endl;

    HTRY
    Graphics* graphics = Graphics::getGraphics ();

    PyObject* ds;
    if (!ParseOneArg("Graphics.addStyle", args, ":displaySty", (PyObject**)&ds)) {
      PyErr_SetString ( ConstructorError, "Bad parameters given to Graphics.addStyle()." );
      return NULL;
    }
    graphics->addStyle(PYDISPLAY_STYLE_O(ds));
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyGraphics_enable ( PyObject*, PyObject* args )
  {
    cdebug_log(20,0) << "PyGraphics_enable()" << endl;

    HTRY
    Graphics::getGraphics()->enable();
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyGraphics_getStyles ( PyObject* )
  {
    cdebug_log(20,0) << "PyGraphics_getStyles()" << endl;

    PyDisplayStyleVector* pyVector = NULL;

    HTRY
    pyVector = PyObject_NEW(PyDisplayStyleVector,&PyTypeDisplayStyleVector);
    if (pyVector == NULL) return NULL;
    pyVector->_container = &Graphics::getGraphics()->getStyles();
    HCATCH

    return (PyObject*)pyVector;
  }


  // Standart Destroy (Attribute).
  // DBoDestroyAttribute(PyGraphics_destroy,PyGraphics)


  PyMethodDef PyGraphics_Methods[] =
    { { "isEnabled"           , (PyCFunction)PyGraphics_isEnabled       , METH_NOARGS |METH_STATIC
                              , "Tells if the Qt part of Graphics has been activated." }
    , { "isHighDpi"           , (PyCFunction)PyGraphics_isHighDpi       , METH_NOARGS |METH_STATIC
                              , "Tells if we are running with a Hi-DPI screen." }
    , { "get"                 , (PyCFunction)PyGraphics_get             , METH_NOARGS |METH_STATIC
                              , "Gets the Viewer Graphic Configuration." }
    , { "getStyle"            , (PyCFunction)PyGraphics_getStyle        , METH_VARARGS|METH_STATIC
                              , "Get a display style (by name or the current active one)." }
    , { "getGroup"            , (PyCFunction)PyGraphics_getGroup        , METH_VARARGS|METH_STATIC
                              , "Returns the group to which the key (DrawingStyle) belongs (on active style)." }
    , { "getPattern"          , (PyCFunction)PyGraphics_getPattern      , METH_VARARGS|METH_STATIC
                              , "Gets a pattern, by key name (on active style)." }
    , { "getThreshold"        , (PyCFunction)PyGraphics_getThreshold    , METH_VARARGS|METH_STATIC
                              , "Gets a threshold, by key name (on active style)." }
    , { "getDarkening"        , (PyCFunction)PyGraphics_getDarkening    , METH_NOARGS|METH_STATIC
                              , "Return the darkening of the active style." }
    , { "toHtml"              , (PyCFunction)PyGraphics_toHtml          , METH_VARARGS|METH_STATIC
                              , "Translate a string so it can be rendered in an HTML device." }
    , { "breakpointStopCb"    , (PyCFunction)PyGraphics_breakpointStopCb, METH_VARARGS|METH_STATIC
                              , "Run the breakpoint utility, with a message." }
    , { "addStyle"            , (PyCFunction)PyGraphics_addStyle        , METH_VARARGS|METH_STATIC
                              , "Adds a new display style." }
    , { "setStyle"            , (PyCFunction)PyGraphics_setStyle        , METH_VARARGS|METH_STATIC
                              , "Select the display style." }
    , { "getStyles"           , (PyCFunction)PyGraphics_getStyles       , METH_NOARGS|METH_STATIC
                              , "Return the list of all availables styles (only iterable)." }
    , { "enable"              , (PyCFunction)PyGraphics_enable          , METH_NOARGS|METH_STATIC
                              , "Bind to Qt, effectively calling Qt and allocating all objects." }
  //, { "destroy"             , (PyCFunction)PyGraphics_destroy         , METH_NOARGS
  //                          , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  PythonOnlyDeleteMethod(Graphics)
  PyTypeObjectLinkPyType(Graphics)


#else  // End of Python Module Code Part.

// +=================================================================+
// |              "PyGraphics" Shared Library Code Part              |
// +=================================================================+

  // Link/Creation Method.
  PyTypeObjectDefinitions(Graphics)


#endif  // End of Shared Library Code Part.


}  // extern "C".


}  // Hurricane namespace.
 
