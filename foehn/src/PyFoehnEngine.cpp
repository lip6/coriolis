// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2022-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |              F o e h n  -  DAG Toolbox                          |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyFoehnEngine.cpp"                           |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyEntity.h"
#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/isobar/PyInstance.h"
#include "hurricane/isobar/PyCellViewer.h"
#include "hurricane/viewer/ExceptionWidget.h"
#include "hurricane/Cell.h"
#include "crlcore/Utilities.h"
#include "foehn/PyFoehnEngine.h"
#include "foehn/PyDag.h"
#include <functional>

# undef   ACCESS_OBJECT
# undef   ACCESS_CLASS
# define  ACCESS_OBJECT            _baseObject._object
# define  ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define   METHOD_HEAD(function)    GENERIC_METHOD_HEAD(FoehnEngine,foehn,function)


namespace  Foehn {

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
  using Isobar::PyEntity;
  using Isobar::PyEntityVector;
  using Isobar::PyTypeEntityVector;
  using Isobar::PyEntityVectorIterator;
  using Isobar::PyNet;
  using Isobar::PyCell;
  using Isobar::PyInstance;
  using Isobar::PyCell_Link;
  using Isobar::PyCellViewer;
  using Isobar::PyTypeNet;
  using Isobar::PyTypeInstance;
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
          if (ExceptionWidget::catchAllWrapper( std::bind(&FoehnEngine::FUNC_NAME,SELF_OBJECT) )) { \
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
// |            "PyFoehnEngine" Python Module Code Part              |
// +=================================================================+


  static PyObject* PyFoehnEngine_get ( PyObject*, PyObject* args )
  {
    cdebug_log(40,0) << "PyFoehnEngine_get()" << endl;
    FoehnEngine* foehn = NULL;
    HTRY
      PyObject* arg0;
      if (not ParseOneArg("Foehn.get", args, CELL_ARG, &arg0)) return NULL;
      foehn = FoehnEngine::get( PYCELL_O(arg0) );
    HCATCH
    return PyFoehnEngine_Link( foehn );
  }


  static PyObject* PyFoehnEngine_create ( PyObject*, PyObject* args )
  {
    cdebug_log(40,0) << "PyFoehnEngine_create()" << endl;
    FoehnEngine* foehn = NULL;
    HTRY
      PyObject* arg0;
      if (not ParseOneArg("Foehn.get", args, CELL_ARG, &arg0)) return NULL;
  
      Cell* cell = PYCELL_O( arg0 );
      foehn = FoehnEngine::get( cell );
      if (foehn == NULL) {
        foehn = FoehnEngine::create( cell );
        if (cmess1.enabled()) {
        //foehn->getFoehnConfiguration()->print(cell);
        }
      } else
        cerr << Warning( "%s already has a Foehn engine.", getString(cell).c_str() ) << endl;
    HCATCH
    return PyFoehnEngine_Link( foehn );
  }


  static PyObject* PyFoehnEngine_getDag ( PyFoehnEngine* self, PyObject* args )
  {
    cdebug_log(40,0) << "PyFoehnEngine_getDag ()" << endl;
    Dag* dag = NULL;
    HTRY
      METHOD_HEAD( "FoehnEngine.getDag()" )
      char* label = NULL;
      if (not PyArg_ParseTuple(args, "s:FoehnEngine.getDag", &label)) {
        PyErr_SetString( ConstructorError, "FoehnEngine.getDag(): Invalid number of parameters." );
        return NULL;
      }
      dag = foehn->getDag( label );
    HCATCH
    return PyDag_Link( dag );
  }


  static PyObject* PyFoehnEngine_newDag ( PyFoehnEngine* self, PyObject* args )
  {
    cdebug_log(40,0) << "PyFoehnEngine_newDag ()" << endl;
    Dag* dag = NULL;
    HTRY
      METHOD_HEAD( "FoehnEngine.newDag()" )
      char* label = NULL;
      if (not PyArg_ParseTuple(args, "s:FoehnEngine.newDag", &label)) {
        PyErr_SetString( ConstructorError, "FoehnEngine.newDag(): Invalid number of parameters." );
        return NULL;
      }
      dag = foehn->newDag( label );
    HCATCH
    return PyDag_Link( dag );
  }


  // Standart Accessors (Attributes).
  DirectVoidToolMethod  (FoehnEngine,foehn,clear)

  // Standart Destroy (Attribute).
  DBoDestroyAttribute(PyFoehnEngine_destroy,PyFoehnEngine)


  PyMethodDef PyFoehnEngine_Methods[] =
    { { "get"                      , (PyCFunction)PyFoehnEngine_get                     , METH_VARARGS|METH_STATIC
                                   , "Returns the Foehn engine attached to the Cell, None if there isnt't." }
    , { "create"                   , (PyCFunction)PyFoehnEngine_create                  , METH_VARARGS|METH_STATIC
                                   , "Create a Foehn engine on this cell." }
    , { "getDag"                   , (PyCFunction)PyFoehnEngine_getDag                , METH_VARARGS
                                   , "Get a DAG of the given label." }
    , { "newDag"                   , (PyCFunction)PyFoehnEngine_newDag         , METH_VARARGS
                                   , "Create a new dag named label." }
    , { "clear"                    , (PyCFunction)PyFoehnEngine_clear                   , METH_NOARGS
                                   , "Clear the previous order computed. The tool remains ready for another one." }
    , { "destroy"                  , (PyCFunction)PyFoehnEngine_destroy                 , METH_NOARGS
                                   , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL}        /* sentinel */
    };


  DBoDeleteMethod(FoehnEngine)
  PyTypeObjectLinkPyType(FoehnEngine)


#else  // End of Python Module Code Part.


// +=================================================================+
// |            "PyFoehnEngine" Shared Library Code Part             |
// +=================================================================+


  // Link/Creation Method.
  PyTypeInheritedObjectDefinitions(FoehnEngine,PyToolEngine)
  DBoLinkCreateMethod(FoehnEngine)


  extern  void  PyFoehnEngine_postModuleInit ()
  {
    // PyFoehnFlags_postModuleInit();

    // PyDict_SetItemString( PyTypeFoehnEngine.tp_dict, "Flags", (PyObject*)&PyTypeFoehnFlags );

    // PyObject* constant = NULL;
    // LoadObjectConstant( PyTypeFoehnEngine.tp_dict, FoehnEngine::GlobalRoutingSuccess  , "GlobalRoutingSuccess"   )
    // LoadObjectConstant( PyTypeFoehnEngine.tp_dict, FoehnEngine::DetailedRoutingSuccess, "DetailedRoutingSuccess" )
  }


#endif  // Shared Library Code Part.

}  // extern "C".

}  // Foehn namespace.
 
