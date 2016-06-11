
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2012-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          U n i c o r n  -  M a i n   G U I                      |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyUnicornGui.cpp"                            |
// +-----------------------------------------------------------------+


#include "unicorn/PyUnicornGui.h"
#include "hurricane/viewer/CellWidget.h"
#include "crlcore/PyBanner.h"
#include "crlcore/PyGraphicToolEngine.h"


#undef  ACCESS_OBJECT
#undef  ACCESS_CLASS
#define ACCESS_OBJECT           _baseObject._object
#define ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define METHOD_HEAD(function)   GENERIC_METHOD_HEAD(UnicornGui,gui,function)


namespace  Unicorn {

using namespace Hurricane;
using namespace Isobar;
using namespace CRL;

extern "C" {


#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |             "PyUnicornGui" Python Module Code Part              |
// +=================================================================+

  static PyObject* PyUnicornGui_create ( PyObject*, PyObject* args )
  {
    cdebug_log(46,0) << "PyUnicornGui_create()" << endl;

    UnicornGui*   gui   = NULL;
    PyUnicornGui* pyGui = NULL;
    
    HTRY
    gui = UnicornGui::create();

    pyGui = PyObject_NEW ( PyUnicornGui, &PyTypeUnicornGui );
    if ( pyGui == NULL ) {
      gui->destroy();
      return NULL;
    }

    pyGui->ACCESS_OBJECT = gui;
    HCATCH

    return (PyObject*)pyGui;
  }


  static PyObject* PyUnicornGui_registerTool ( PyUnicornGui* self, PyObject* args )
  {
    cdebug_log(46,0) << "PyUnicornGui_registerTool()" << endl;
    
    HTRY
    METHOD_HEAD("UnicornGui.registerTool()")
    PyObject* pyGraphicTool = NULL;

    if (PyArg_ParseTuple(args, "O:UnicornGui.registerTool()", &pyGraphicTool)) {
      if (not PyObject_IsInstance(pyGraphicTool, (PyObject*)&PyTypeGraphicTool)) {
        PyErr_SetString ( ConstructorError, "UnicornGui.registerTool(): Argument is not of type GraphicTool." );
        return NULL;
      }
      gui->registerTool( PY_GRAPHIC_TOOL_O(pyGraphicTool) );
    } else {
      PyErr_SetString ( ConstructorError, "Bad parameters given to UnicornGui.registerTool()." );
      return NULL;
    }
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyUnicornGui_show ( PyUnicornGui* self )
  {
    cdebug_log(46,0) << "PyUnicornGui_show()" << endl;
    
    HTRY
    METHOD_HEAD("UnicornGui.show()")
    gui->show();
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyUnicornGui_getBanner ( PyUnicornGui* self )
  {
    cdebug_log(46,0) << "PyUnicornGui_getBanner()" << endl;

    Banner* banner = NULL;
    
    HTRY
    METHOD_HEAD("UnicornGui.getBanner()")
    banner = &gui->getBanner();
    HCATCH

    return PyBanner_Link(banner);
  }


  // Standart Destroy (Attribute).
  DirectDestroyAttribute(PyUnicornGui_destroy, PyUnicornGui)


  // ---------------------------------------------------------------
  // PyUnicornGui Attribute Method table.

  PyMethodDef PyUnicornGui_Methods[] =
    { { "create"             , (PyCFunction)PyUnicornGui_create         , METH_NOARGS|METH_STATIC
                             , "Creates a new Unicorn GUI." }
    , { "getBanner"          , (PyCFunction)PyUnicornGui_getBanner      , METH_NOARGS
                             , "Returns the tool's banner." }
    , { "registerTool"       , (PyCFunction)PyUnicornGui_registerTool   , METH_VARARGS
                             , "Register a new Graphic tool in the Unicorn GUI." }
    , { "show"               , (PyCFunction)PyUnicornGui_show           , METH_NOARGS
                             , "Perform the Qt show on the GUI." }
    , { "destroy"            , (PyCFunction)PyUnicornGui_destroy        , METH_NOARGS
                             , "Destroy the C++ Unicorn GUI object. The python object remains." }
    , {NULL, NULL, 0, NULL}  /* sentinel */
    };


  PythonOnlyDeleteMethod(UnicornGui)
  PyTypeObjectLinkPyType(UnicornGui)


#else  // End of Python Module Code Part.


// +=================================================================+
// |            "PyUnicornGui" Shared Library Code Part              |
// +=================================================================+


  PyTypeInheritedObjectDefinitions(UnicornGui, CellViewer)


# endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Unicorn namespace.
