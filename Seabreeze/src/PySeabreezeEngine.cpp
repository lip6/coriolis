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
// |  C++ Module  :  "./PySeabreezeEngine.cpp"                       |
// +-----------------------------------------------------------------+


#include <functional>
#include "hurricane/isobar/PyCell.h"
#include "hurricane/viewer/PyCellViewer.h"
#include "hurricane/isobar/PyNet.h"
#include "hurricane/viewer/ExceptionWidget.h"
#include "hurricane/Cell.h"
#include "crlcore/Utilities.h"
#include "seabreeze/PySeabreezeEngine.h"

# undef   ACCESS_OBJECT
# undef   ACCESS_CLASS
# define  ACCESS_OBJECT            _baseObject._object
# define  ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define   METHOD_HEAD(function)    GENERIC_METHOD_HEAD(SeabreezeEngine,seabreeze,function)


namespace  Seabreeze {

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
  using Isobar::PyNet;
  using CRL::PyToolEngine;


extern "C" {

#if defined(__PYTHON_MODULE__)


// +=================================================================+
// |          "PySeabreezeEngine" Python Module Code Part             |
// +=================================================================+


  static PyObject* PySeabreezeEngine_get ( PyObject*, PyObject* args )
  {
    cdebug_log(40,0) << "PySeabreezeEngine_get()" << endl;

    SeabreezeEngine* seabreeze = NULL;
    
    HTRY
    PyObject* arg0;

    if (not ParseOneArg("Seabreeze.get", args, CELL_ARG, &arg0)) return NULL;
    seabreeze = SeabreezeEngine::get(PYCELL_O(arg0));
    HCATCH

    return PySeabreezeEngine_Link(seabreeze);
  }


  static PyObject* PySeabreezeEngine_create ( PyObject*, PyObject* args )
  {
    cdebug_log(40,0) << "PySeabreezeEngine_create()" << endl;

    SeabreezeEngine* seabreeze = NULL;
    
    HTRY
    PyObject* arg0;

    if (not ParseOneArg("Seabreeze.get", args, CELL_ARG, &arg0)) return NULL;

    Cell* cell = PYCELL_O(arg0);
    seabreeze = SeabreezeEngine::get(cell);

    if (seabreeze == NULL) {
      seabreeze = SeabreezeEngine::create(cell);
    } else
      cerr << Warning("%s already has a Seabreeze engine.",getString(cell).c_str()) << endl;
    HCATCH

    return PySeabreezeEngine_Link(seabreeze);
  }


  static PyObject* PySeabreezeEngine_setViewer ( PySeabreezeEngine* self, PyObject* args )
  {
    cdebug_log(40,0) << "PySeabreezeEngine_setViewer ()" << endl;

    HTRY
      METHOD_HEAD( "SeabreezeEngine.setViewer()" )
  
      PyObject* pyViewer = NULL;
      if (not PyArg_ParseTuple(args,"O:EtesianEngine.setViewer()",&pyViewer)) {
        PyErr_SetString( ConstructorError, "Bad parameters given to EtesianEngine.setViewer()." );
        return NULL;
      }
      if (IsPyCellViewer(pyViewer)) {
        seabreeze->setViewer( PYCELLVIEWER_O(pyViewer) );
      }
    HCATCH

    Py_RETURN_NONE;
  }

/*
  PyObject* PySeabreezeEngine_runTool ( PySeabreezeEngine* self )
  {
    cdebug_log(40,0) << "PySeabreezeEngine_runTool()" << endl;

    Cell* cell = NULL;

    HTRY
      METHOD_HEAD("SeabreezeEngine.runTool()")
      if (seabreeze->getViewer()) {
        if (ExceptionWidget::catchAllWrapper( std::bind(&SeabreezeEngine::runTool,seabreeze) )) {
          PyErr_SetString( HurricaneError, "SeabreezeEngine::runTool() has thrown an exception (C++)." );
          return NULL;
        }
        cell = seabreeze->getCell();
      } else {
        cell = seabreeze->runTool();
      }
    HCATCH

    return PyCell_Link( cell );
  }
*/

  static PyObject* PySeabreezeEngine_buildElmore ( PySeabreezeEngine* self, PyObject* args )
  {
    cdebug_log(40,0) << "PySeabreezeEngine_buildElmore()" << endl;
    HTRY
      PyObject* arg0 = NULL;
      METHOD_HEAD("SeabreezeEngine.buildElmore()")
      if (not ParseOneArg("Seabreeze.buildElmore()", args, NET_ARG, &arg0)) return NULL;
      seabreeze->buildElmore(PYNET_O(arg0));
    HCATCH

    Py_RETURN_NONE;
  }


  // Standart Accessors (Attributes).

  // Standart Destroy (Attribute).
  DBoDestroyAttribute(PySeabreezeEngine_destroy,PySeabreezeEngine)


  PyMethodDef PySeabreezeEngine_Methods[] =
    { { "get"                  , (PyCFunction)PySeabreezeEngine_get                  , METH_VARARGS|METH_STATIC
                               , "Returns the Seabreeze engine attached to the Cell, None if there isnt't." }
    , { "create"               , (PyCFunction)PySeabreezeEngine_create               , METH_VARARGS|METH_STATIC
                               , "Create a Seabreeze engine on this cell." }
    , { "setViewer"            , (PyCFunction)PySeabreezeEngine_setViewer            , METH_VARARGS
                               , "Associate a Viewer to this SeabreezeEngine." }
  //, { "runDemoPart1"         , (PyCFunction)PySeabreezeEngine_runDemoPart1         , METH_NOARGS
  //                           , "Run the first part of the demo." }
    , { "buildElmore"          , (PyCFunction)PySeabreezeEngine_buildElmore          , METH_VARARGS
                               , "Run the Seabreeze tool." }
    , { "destroy"              , (PyCFunction)PySeabreezeEngine_destroy              , METH_NOARGS
                               , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL}    /* sentinel */
    };


  DBoDeleteMethod(SeabreezeEngine)
  PyTypeObjectLinkPyType(SeabreezeEngine)


#else  // End of Python Module Code Part.


// +=================================================================+
// |           "PySeabreezeEngine" Shared Library Code Part           |
// +=================================================================+


  // Link/Creation Method.
  PyTypeInheritedObjectDefinitions(SeabreezeEngine,PyToolEngine)
  DBoLinkCreateMethod(SeabreezeEngine)


#endif  // Shared Library Code Part.

}  // extern "C".

}  // Seabreeze namespace.
 
