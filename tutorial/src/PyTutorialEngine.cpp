// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2017-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |        T o o l E n g i n e   T u t o r i a l                    |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyTutorialEngine.cpp"                        |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyCell.h"
#include "hurricane/viewer/PyCellViewer.h"
#include "hurricane/viewer/ExceptionWidget.h"
#include "hurricane/Cell.h"
#include "crlcore/Utilities.h"
#include "tutorial/PyTutorialEngine.h"
#include <functional>

# undef   ACCESS_OBJECT
# undef   ACCESS_CLASS
# define  ACCESS_OBJECT            _baseObject._object
# define  ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define   METHOD_HEAD(function)    GENERIC_METHOD_HEAD(TutorialEngine,tutorial,function)


namespace  Tutorial {

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
  using Isobar::getPyHash;
  using Isobar::ParseOneArg;
  using Isobar::ParseTwoArg;
  using Isobar::PyCell;
  using Isobar::PyCell_Link;
  using Isobar::PyCellViewer;
  using Isobar::PyTypeCellViewer;
  using CRL::PyToolEngine;


extern "C" {

#if defined(__PYTHON_MODULE__)


// +=================================================================+
// |          "PyTutorialEngine" Python Module Code Part             |
// +=================================================================+


  static PyObject* PyTutorialEngine_get ( PyObject*, PyObject* args )
  {
    cdebug_log(40,0) << "PyTutorialEngine_get()" << endl;

    TutorialEngine* tutorial = NULL;
    
    HTRY
    PyObject* arg0;

    if (not ParseOneArg("Tutorial.get", args, CELL_ARG, &arg0)) return NULL;
    tutorial = TutorialEngine::get(PYCELL_O(arg0));
    HCATCH

    return PyTutorialEngine_Link(tutorial);
  }


  static PyObject* PyTutorialEngine_create ( PyObject*, PyObject* args )
  {
    cdebug_log(40,0) << "PyTutorialEngine_create()" << endl;

    TutorialEngine* tutorial = NULL;
    
    HTRY
    PyObject* arg0;

    if (not ParseOneArg("Tutorial.get", args, CELL_ARG, &arg0)) return NULL;

    Cell* cell = PYCELL_O(arg0);
    tutorial = TutorialEngine::get(cell);

    if (tutorial == NULL) {
      tutorial = TutorialEngine::create(cell);
    } else
      cerr << Warning("%s already has a Tutorial engine.",getString(cell).c_str()) << endl;
    HCATCH

    return PyTutorialEngine_Link(tutorial);
  }


  static PyObject* PyTutorialEngine_setViewer ( PyTutorialEngine* self, PyObject* args )
  {
    cdebug_log(40,0) << "PyTutorialEngine_setViewer ()" << endl;

    HTRY
      METHOD_HEAD( "TutorialEngine.setViewer()" )
  
      PyObject* pyViewer = NULL;
      if (not PyArg_ParseTuple(args,"O:EtesianEngine.setViewer()",&pyViewer)) {
        PyErr_SetString( ConstructorError, "Bad parameters given to EtesianEngine.setViewer()." );
        return NULL;
      }
      if (IsPyCellViewer(pyViewer)) {
        tutorial->setViewer( PYCELLVIEWER_O(pyViewer) );
      }
    HCATCH

    Py_RETURN_NONE;
  }


  PyObject* PyTutorialEngine_runDemoPart1 ( PyTutorialEngine* self )
  {
    cdebug_log(40,0) << "PyTutorialEngine_runDemoPart1()" << endl;

    Cell* cell = NULL;

    HTRY
      METHOD_HEAD("TutorialEngine.runDemoPart1()")
      if (tutorial->getViewer()) {
        if (ExceptionWidget::catchAllWrapper( std::bind(&TutorialEngine::runDemoPart1,tutorial) )) {
          PyErr_SetString( HurricaneError, "TutorialEngine::runDemoPart1() has thrown an exception (C++)." );
          return NULL;
        }
        cell = tutorial->getCell();
      } else {
        cell = tutorial->runDemoPart1();
      }
    HCATCH

    return PyCell_Link( cell );
  }


  PyObject* PyTutorialEngine_runDemoPart2 ( PyTutorialEngine* self )
  {
    cdebug_log(40,0) << "PyTutorialEngine_runDemoPart2()" << endl;

    HTRY
      METHOD_HEAD("TutorialEngine.runDemoPart2()")
      if (tutorial->getViewer()) {
        if (ExceptionWidget::catchAllWrapper( std::bind(&TutorialEngine::runDemoPart1,tutorial) )) {
          PyErr_SetString( HurricaneError, "TutorialEngine::runDemoPart2() has thrown an exception (C++)." );
          return NULL;
        }
      } else {
        tutorial->runDemoPart2();
      }
    HCATCH

    Py_RETURN_NONE;
  }


  // Standart Accessors (Attributes).

  // Standart Destroy (Attribute).
  DBoDestroyAttribute(PyTutorialEngine_destroy,PyTutorialEngine)


  PyMethodDef PyTutorialEngine_Methods[] =
    { { "get"                  , (PyCFunction)PyTutorialEngine_get                  , METH_VARARGS|METH_STATIC
                               , "Returns the Tutorial engine attached to the Cell, None if there isnt't." }
    , { "create"               , (PyCFunction)PyTutorialEngine_create               , METH_VARARGS|METH_STATIC
                               , "Create a Tutorial engine on this cell." }
    , { "setViewer"            , (PyCFunction)PyTutorialEngine_setViewer            , METH_VARARGS
                               , "Associate a Viewer to this TutorialEngine." }
    , { "runDemoPart1"         , (PyCFunction)PyTutorialEngine_runDemoPart1         , METH_NOARGS
                               , "Run the first part of the demo." }
    , { "runDemoPart2"         , (PyCFunction)PyTutorialEngine_runDemoPart2         , METH_NOARGS
                               , "Run the second part of the demo." }
    , { "destroy"              , (PyCFunction)PyTutorialEngine_destroy              , METH_NOARGS
                               , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL}    /* sentinel */
    };


  DBoDeleteMethod(TutorialEngine)
  PyTypeObjectLinkPyType(TutorialEngine)


#else  // End of Python Module Code Part.


// +=================================================================+
// |           "PyTutorialEngine" Shared Library Code Part           |
// +=================================================================+


  // Link/Creation Method.
  PyTypeInheritedObjectDefinitions(TutorialEngine,PyToolEngine)
  DBoLinkCreateMethod(TutorialEngine)


#endif  // Shared Library Code Part.

}  // extern "C".

}  // Tutorial namespace.
 
