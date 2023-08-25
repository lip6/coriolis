// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2007-2023, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |       T r a m o n t a n a  -  Extractor & LVX                   |
// |                                                                 |
// |  Algorithm    :                   Christian MASSON              |
// |  First impl.  :                           Yifei WU              |
// |  Second impl. :                   Jean-Paul CHAPUT              |
// |  E-mail       :           Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module   :  "./PyTramontanaEngine.cpp"                     |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/isobar/PyCellViewer.h"
#include "hurricane/viewer/ExceptionWidget.h"
#include "hurricane/Cell.h"
#include "crlcore/Utilities.h"
#include "tramontana/PyTramontanaEngine.h"
#include <functional>

# undef   ACCESS_OBJECT
# undef   ACCESS_CLASS
# define  ACCESS_OBJECT            _baseObject._object
# define  ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define   METHOD_HEAD(function)    GENERIC_METHOD_HEAD(TramontanaEngine,tramontana,function)


namespace  Tramontana {

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
  using Isobar::__cs;
  using Isobar::Converter;
  using Isobar::ProxyProperty;
  using Isobar::ProxyError;
  using Isobar::ConstructorError;
  using Isobar::HurricaneError;
  using Isobar::HurricaneWarning;
  using Isobar::getPyHash;
  using Isobar::ParseOneArg;
  using Isobar::ParseTwoArg;
  using Isobar::PyNet;
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
      cdebug_log(40,0) << "Py" #SELF_TYPE "_" #FUNC_NAME "()" << endl;  \
      HTRY                                                              \
        METHOD_HEAD(#SELF_TYPE "." #FUNC_NAME "()")                     \
        if (SELF_OBJECT->getViewer()) {                                 \
          if (ExceptionWidget::catchAllWrapper( std::bind(&TramontanaEngine::FUNC_NAME,SELF_OBJECT) )) { \
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
// |         "PyTramontanaEngine" Python Module Code Part            |
// +=================================================================+


  static PyObject* PyTramontanaEngine_get ( PyObject*, PyObject* args )
  {
    cdebug_log(40,0) << "PyTramontanaEngine_get()" << endl;
    TramontanaEngine* tramontana = NULL;
    HTRY
      PyObject* arg0;
      if (not ParseOneArg("Tramontana.get", args, CELL_ARG, &arg0)) return NULL;
      tramontana = TramontanaEngine::get(PYCELL_O(arg0));
      HCATCH
    return PyTramontanaEngine_Link(tramontana);
  }


  static PyObject* PyTramontanaEngine_create ( PyObject*, PyObject* args )
  {
    cdebug_log(40,0) << "PyTramontanaEngine_create()" << endl;
    TramontanaEngine* tramontana = NULL;
    HTRY
      PyObject* arg0;
      if (not ParseOneArg("Tramontana.get", args, CELL_ARG, &arg0)) return NULL;
      Cell* cell = PYCELL_O(arg0);
      tramontana = TramontanaEngine::get(cell);
      if (tramontana == NULL) {
        tramontana = TramontanaEngine::create(cell);
      } else
        cerr << Warning("%s already has a Tramontana engine.",getString(cell).c_str()) << endl;
    HCATCH
    return PyTramontanaEngine_Link(tramontana);
  }


  static PyObject* PyTramontanaEngine_setViewer ( PyTramontanaEngine* self, PyObject* args )
  {
    cdebug_log(40,0) << "PyTramontanaEngine_setViewer ()" << endl;
    HTRY
      METHOD_HEAD( "TramontanaEngine.setViewer()" )
      PyObject* pyViewer = NULL;
      if (not PyArg_ParseTuple(args,"O:TramontanaEngine.setViewer()",&pyViewer)) {
        PyErr_SetString( ConstructorError, "Bad parameters given to TramontanaEngine.setViewer()." );
        return NULL;
      }
      if (IsPyCellViewer(pyViewer)) {
        tramontana->setViewer( PYCELLVIEWER_O(pyViewer) );
      }
    HCATCH
    Py_RETURN_NONE;
  }


  static PyObject* PyTramontanaEngine_extract ( PyTramontanaEngine* self )
  {
    cdebug_log(40,0) << "PyTramontanaEngine_extract()" << endl;
    HTRY
    METHOD_HEAD("TramontanaEngine.extract()")
    if (tramontana->getViewer()) {
      if (ExceptionWidget::catchAllWrapper( std::bind(&TramontanaEngine::extract,tramontana) )) {
        PyErr_SetString( HurricaneError, "TramontanaEngine::extract() has thrown an exception (C++)." );
        return NULL;
      }
    } else {
      tramontana->extract();
    }
    HCATCH
    Py_RETURN_NONE;
  }


  // Standart Accessors (Attributes).

  // Standart Destroy (Attribute).
  DBoDestroyAttribute(PyTramontanaEngine_destroy,PyTramontanaEngine)


  PyMethodDef PyTramontanaEngine_Methods[] =
    { { "get"                      , (PyCFunction)PyTramontanaEngine_get                     , METH_VARARGS|METH_STATIC
                                   , "Returns the Tramontana engine attached to the Cell, None if there isnt't." }
    , { "create"                   , (PyCFunction)PyTramontanaEngine_create                  , METH_VARARGS|METH_STATIC
                                   , "Create a Tramontana engine on this cell." }
    , { "destroy"                  , (PyCFunction)PyTramontanaEngine_destroy                 , METH_NOARGS
                                   , "Destroy a Tramontana engine." }
    , { "setViewer"                , (PyCFunction)PyTramontanaEngine_setViewer               , METH_VARARGS
                                   , "Associate a Viewer to this TramontanaEngine." }
    , { "extract"                  , (PyCFunction)PyTramontanaEngine_extract                 , METH_NOARGS
                                   , "Perform the layout extraction." }
    , {NULL, NULL, 0, NULL}        /* sentinel */
    };


  DBoDeleteMethod(TramontanaEngine)
  PyTypeObjectLinkPyType(TramontanaEngine)


#else  // End of Python Module Code Part.


// +=================================================================+
// |          "PyTramontanaEngine" Shared Library Code Part          |
// +=================================================================+


  // Link/Creation Method.
  PyTypeInheritedObjectDefinitions(TramontanaEngine,PyToolEngine)
  DBoLinkCreateMethod(TramontanaEngine)


  // extern  void  PyTramontanaEngine_postModuleInit ()
  // {
  //   PyTramontanaFlags_postModuleInit();

  //   PyDict_SetItemString( PyTypeTramontanaEngine.tp_dict, "Flags", (PyObject*)&PyTypeTramontanaFlags );

  //   PyObject* constant = NULL;
  //   LoadObjectConstant( PyTypeTramontanaEngine.tp_dict, TramontanaEngine::GlobalRoutingSuccess  , "GlobalRoutingSuccess"   )
  //   LoadObjectConstant( PyTypeTramontanaEngine.tp_dict, TramontanaEngine::DetailedRoutingSuccess, "DetailedRoutingSuccess" )
  // }


#endif  // Shared Library Code Part.

}  // extern "C".

}  // Tramontana namespace.
 
