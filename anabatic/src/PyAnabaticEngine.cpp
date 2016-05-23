// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2016, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |     A n a b a t i c  -  Global Routing Toolbox                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyAnabaticEngine.cpp"                        |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyCell.h"
#include "hurricane/viewer/PyCellViewer.h"
#include "hurricane/viewer/ExceptionWidget.h"
#include "hurricane/Cell.h"
#include "anabatic/PyAnabaticEngine.h"
#include <functional>

# undef   ACCESS_OBJECT
# undef   ACCESS_CLASS
# define  ACCESS_OBJECT            _baseObject._object
# define  ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define   METHOD_HEAD(function)    GENERIC_METHOD_HEAD(AnabaticEngine,engine,function)


namespace  Anabatic {

  using std::cerr;
  using std::endl;
  using std::hex;
  using std::ostringstream;
  using Hurricane::tab;
  using Hurricane::Exception;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Hurricane::ExceptionWidget;
  using Isobar::ProxyProperty;
  using Isobar::ProxyError;
  using Isobar::ConstructorError;
  using Isobar::HurricaneError;
  using Isobar::HurricaneWarning;
  using Isobar::ParseOneArg;
  using Isobar::ParseTwoArg;
  using Isobar::PyCell;
  using Isobar::PyCell_Link;
  using Isobar::PyCellViewer;
  using Isobar::PyTypeCellViewer;
  using CRL::PyToolEngine;


extern "C" {

#if defined(__PYTHON_MODULE__)


#define DirectVoidToolMethod(SELF_TYPE, SELF_OBJECT, FUNC_NAME)         \
  static PyObject* Py##SELF_TYPE##_##FUNC_NAME(Py##SELF_TYPE* self)     \
  {                                                                     \
      trace << "Py" #SELF_TYPE "_" #FUNC_NAME "()" << endl;             \
      HTRY                                                              \
        METHOD_HEAD(#SELF_TYPE "." #FUNC_NAME "()")                     \
        if (SELF_OBJECT->getViewer()) {                                 \
          if (ExceptionWidget::catchAllWrapper( std::bind(&AnabaticEngine::FUNC_NAME,SELF_OBJECT) )) { \
            PyErr_SetString( HurricaneError, #FUNC_NAME "() has thrown an exception (C++)." );     \
            return NULL;                                                \
          }                                                             \
        } else {                                                        \
          SELF_OBJECT->FUNC_NAME();                                     \
        }                                                               \
      HCATCH                                                            \
      Py_RETURN_NONE;                                                   \
  }


// +=================================================================+
// |          "PyAnabaticEngine" Python Module Code Part             |
// +=================================================================+


  static PyObject* PyAnabaticEngine_get ( PyObject*, PyObject* args )
  {
    cdebug.log(32) << "PyAnabaticEngine_get()" << endl;

    AnabaticEngine* engine = NULL;
    
    HTRY
      PyObject* arg0;

      if (not ParseOneArg("Anabatic.get", args, CELL_ARG, &arg0)) return NULL;
      engine = AnabaticEngine::get(PYCELL_O(arg0));
    HCATCH

    return PyAnabaticEngine_Link(engine);
  }


  static PyObject* PyAnabaticEngine_create ( PyObject*, PyObject* args )
  {
    cdebug.log(32) << "PyAnabaticEngine_create()" << endl;

    AnabaticEngine* engine = NULL;
    
    HTRY
      PyObject* arg0;
  
      if (not ParseOneArg("Anabatic.get", args, CELL_ARG, &arg0)) return NULL;
  
      Cell* cell = PYCELL_O(arg0);
      engine = AnabaticEngine::get(cell);
  
      if (engine == NULL) {
        engine = AnabaticEngine::create(cell);
      } else
        cerr << Warning("%s already has a Anabatic engine.",getString(cell).c_str()) << endl;
    HCATCH

    return PyAnabaticEngine_Link(engine);
  }


  static PyObject* PyAnabaticEngine_setViewer ( PyAnabaticEngine* self, PyObject* args )
  {
    cdebug.log(32) << "PyAnabaticEngine_setViewer ()" << endl;

    HTRY
      METHOD_HEAD( "AnabaticEngine.setViewer()" )
  
      PyObject* pyViewer = NULL;
      if (not PyArg_ParseTuple(args,"O:EtesianEngine.setViewer()",&pyViewer)) {
        PyErr_SetString( ConstructorError, "Bad parameters given to EtesianEngine.setViewer()." );
        return NULL;
      }
      if (IsPyCellViewer(pyViewer)) {
        engine->setViewer( PYCELLVIEWER_O(pyViewer) );
      }
    HCATCH

    Py_RETURN_NONE;
  }


  PyObject* PyAnabaticEngine_runTest ( PyAnabaticEngine* self, PyObject* args )
  {
    cdebug.log(32) << "PyAnabaticEngine_runTest()" << endl;

    HTRY
      METHOD_HEAD("AnabaticEngine.runTest()")
      unsigned int flags = 0;
      if (PyArg_ParseTuple(args,"I:AnabaticEngine.runTest", &flags)) {
        if (engine->getViewer()) {
          if (ExceptionWidget::catchAllWrapper( std::bind(&AnabaticEngine::_runTest,engine) )) {
            PyErr_SetString( HurricaneError, "AnabaticEngine::runTest() has thrown an exception (C++)." );
            return NULL;
          }
        } else {
          engine->_runTest();
        }
      } else {
        PyErr_SetString(ConstructorError, "AnabaticEngine.runGlobalRouter(): Invalid number/bad type of parameter.");
        return NULL;
      }
    HCATCH

    Py_RETURN_NONE;
  }


  // Standart Accessors (Attributes).

  // Standart Destroy (Attribute).
  DBoDestroyAttribute(PyAnabaticEngine_destroy,PyAnabaticEngine)


  PyMethodDef PyAnabaticEngine_Methods[] =
    { { "get"                  , (PyCFunction)PyAnabaticEngine_get                  , METH_VARARGS|METH_STATIC
                               , "Returns the Anabatic engine attached to the Cell, None if there isnt't." }
    , { "create"               , (PyCFunction)PyAnabaticEngine_create               , METH_VARARGS|METH_STATIC
                               , "Create a Anabatic engine on this cell." }
    , { "setViewer"            , (PyCFunction)PyAnabaticEngine_setViewer            , METH_VARARGS
                               , "Associate a Viewer to this AnabaticEngine." }
    , { "runTest"              , (PyCFunction)PyAnabaticEngine_runTest              , METH_VARARGS
                               , "Run the test procedure." }
    , { "destroy"              , (PyCFunction)PyAnabaticEngine_destroy              , METH_NOARGS
                               , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL}    /* sentinel */
    };


  DBoDeleteMethod(AnabaticEngine)
  PyTypeObjectLinkPyType(AnabaticEngine)


#else  // End of Python Module Code Part.


// +=================================================================+
// |           "PyAnabaticEngine" Shared Library Code Part           |
// +=================================================================+


  // Link/Creation Method.
  PyTypeInheritedObjectDefinitions(AnabaticEngine,PyToolEngine)
  DBoLinkCreateMethod(AnabaticEngine)


#endif  // Shared Library Code Part.

}  // extern "C".

}  // Anabatic namespace.
 
