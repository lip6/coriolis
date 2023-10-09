// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2010-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyKatanaEngine.cpp"                          |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/viewer/ExceptionWidget.h"
#include "hurricane/viewer/PyCellViewer.h"
#include "hurricane/Cell.h"
#include "katana/PyKatanaEngine.h"
#include "katana/PyKatanaFlags.h"
#include <functional>

# undef   ACCESS_OBJECT
# undef   ACCESS_CLASS
# define  ACCESS_OBJECT            _baseObject._object
# define  ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define   METHOD_HEAD(function)    GENERIC_METHOD_HEAD(KatanaEngine,katana,function)


namespace  Katana {

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
          if (ExceptionWidget::catchAllWrapper( std::bind(&KatanaEngine::FUNC_NAME,SELF_OBJECT) )) { \
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
// |           "PyKatanaEngine" Python Module Code Part              |
// +=================================================================+


  static PyObject* PyKatanaEngine_get ( PyObject*, PyObject* args )
  {
    cdebug_log(40,0) << "PyKatanaEngine_get()" << endl;

    KatanaEngine* katana = NULL;
    
    HTRY
    PyObject* arg0;

    if (not ParseOneArg("Katana.get", args, CELL_ARG, &arg0)) return NULL;
    katana = KatanaEngine::get(PYCELL_O(arg0));
    HCATCH

    return PyKatanaEngine_Link(katana);
  }


  static PyObject* PyKatanaEngine_create ( PyObject*, PyObject* args )
  {
    cdebug_log(40,0) << "PyKatanaEngine_create()" << endl;

    KatanaEngine* katana = NULL;
    
    HTRY
    PyObject* arg0;

    if (not ParseOneArg("Katana.get", args, CELL_ARG, &arg0)) return NULL;

    Cell* cell = PYCELL_O(arg0);
    katana = KatanaEngine::get(cell);

    if (katana == NULL) {
      katana = KatanaEngine::create(cell);
      if (cmess1.enabled())
        katana->getConfiguration()->print(cell);
    } else
      cerr << Warning("%s already has a Katana engine.",getString(cell).c_str()) << endl;
    HCATCH

    return PyKatanaEngine_Link(katana);
  }


  static PyObject* PyKatanaEngine_setViewer ( PyKatanaEngine* self, PyObject* args )
  {
    cdebug_log(40,0) << "PyKatanaEngine_setViewer ()" << endl;

    HTRY
      METHOD_HEAD( "KatanaEngine.setViewer()" )
  
      PyObject* pyViewer = NULL;
      if (not PyArg_ParseTuple(args,"O:KatanaEngine.setViewer()",&pyViewer)) {
        PyErr_SetString( ConstructorError, "Bad parameters given to KatanaEngine.setViewer()." );
        return NULL;
      }
      if (IsPyCellViewer(pyViewer)) {
        katana->setViewer( PYCELLVIEWER_O(pyViewer) );
      }
    HCATCH

    Py_RETURN_NONE;
  }


  PyObject* PyKatanaEngine_digitalInit ( PyKatanaEngine* self )
  {
    cdebug_log(40,0) << "PyKatanaEngine_digitalInit()" << endl;

    HTRY
      METHOD_HEAD("KatanaEngine.digitalInit()")
      if (katana->getViewer()) {
        if (ExceptionWidget::catchAllWrapper( std::bind(&KatanaEngine::digitalInit,katana) )) {
          PyErr_SetString( HurricaneError, "KatanaEngine::digitalInit() has thrown an exception (C++)." );
          return NULL;
        }
      } else {
        katana->digitalInit();
      }
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyKatanaEngine_exclude ( PyKatanaEngine* self, PyObject* args )
  {
    cdebug_log(40,0) << "PyKatanaEngine_exclude ()" << endl;

    HTRY
      METHOD_HEAD( "KatanaEngine.exclude()" )

      PyObject* arg0;
      __cs.init ("KatanaEngine.exclude");

      if (not PyArg_ParseTuple(args, "O&:KatanaEngine.exclude", Converter, &arg0)) {
        PyErr_SetString( ConstructorError, "Bad parameters given to KatanaEngine.exclude()." );
        return NULL;
      }

      if      (__cs.getObjectIds() == STRING_ARG) { katana->exclude( PyString_AsString(arg0) ); }
      else if (__cs.getObjectIds() == NET_ARG   ) { katana->exclude( PYNET_O(arg0) ); }
    HCATCH

    Py_RETURN_NONE;
  }


  PyObject* PyKatanaEngine_runGlobalRouter ( PyKatanaEngine* self, PyObject* args )
  {
    cdebug_log(40,0) << "PyKatanaEngine_runGlobalRouter()" << endl;

    HTRY
      METHOD_HEAD("KatanaEngine.runGlobalRouter()")
      uint64_t  flags = 0;
      if (PyArg_ParseTuple(args,"L:KatanaEngine.runGlobalRouter", &flags)) {
        if (katana->getViewer()) {
          if (ExceptionWidget::catchAllWrapper( std::bind(&KatanaEngine::runGlobalRouter,katana,flags) )) {
            PyErr_SetString( HurricaneError, "KatanaEngine::runGlobalrouter() has thrown an exception (C++)." );
            return NULL;
          }
        } else {
          katana->runGlobalRouter( flags );
        }
      } else {
        PyErr_SetString(ConstructorError, "KatanaEngine.runGlobalRouter(): Invalid number/bad type of parameter.");
        return NULL;
      }
    HCATCH

    Py_RETURN_NONE;
  }


  PyObject* PyKatanaEngine_loadGlobalRouting ( PyKatanaEngine* self, PyObject* args )
  {
    cdebug_log(40,0) << "PyKatanaEngine_loadGlobalRouting()" << endl;

    HTRY
    METHOD_HEAD("KatanaEngine.loadGlobalRouting()")
    unsigned int  flags = 0;
    if (PyArg_ParseTuple(args,"I:KatanaEngine.loadGlobalRouting", &flags)) {
      if (katana->getViewer()) {
        if (ExceptionWidget::catchAllWrapper( std::bind(&KatanaEngine::loadGlobalRouting,katana,flags) )) {
          PyErr_SetString( HurricaneError, "KatanaEngine::loadGlobalrouting() has thrown an exception (C++)." );
          return NULL;
        }
      } else {
        katana->loadGlobalRouting(flags);
      }
    } else {
      PyErr_SetString(ConstructorError, "KatanaEngine.loadGlobalRouting(): Invalid number/bad type of parameter.");
      return NULL;
    }
    HCATCH

    Py_RETURN_NONE;
  }


  PyObject* PyKatanaEngine_layerAssign ( PyKatanaEngine* self, PyObject* args )
  {
    cdebug_log(40,0) << "PyKatanaEngine_layerAssign()" << endl;

    HTRY
    METHOD_HEAD("KatanaEngine.layerAssign()")
    unsigned int flags = 0;
    if (PyArg_ParseTuple(args,"I:KatanaEngine.layerAssign", &flags)) {

      if (katana->getViewer()) {
        bool failure = false;
        //= ExceptionWidget::catchAllWrapper( std::bind(&KatanaEngine::balanceGlobalDensity,katana) );
        if (not failure)
          failure = ExceptionWidget::catchAllWrapper( std::bind(&KatanaEngine::layerAssign,katana,flags) );

        if (failure) {
          PyErr_SetString( HurricaneError, "KatanaEngine::layerAssign() has thrown an exception (C++)." );
          return NULL;
        }
      } else {
      //katana->balanceGlobalDensity();
        katana->layerAssign         (flags);
      }
    } else {
      PyErr_SetString(ConstructorError, "KatanaEngine.layerAssign(): Invalid number/bad type of parameter.");
      return NULL;
    }
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyKatanaEngine_runNegociatePreRouted ( PyKatanaEngine* self )
  {
    cdebug_log(40,0) << "PyKatanaEngine_runNegociatePreRouted()" << endl;
    HTRY
    METHOD_HEAD("KatanaEngine.runNegociatePreRouted()")
    if (katana->getViewer()) {
      if (ExceptionWidget::catchAllWrapper( std::bind(&KatanaEngine::runNegociate,katana,Flags::PreRoutedStage) )) {
        PyErr_SetString( HurricaneError, "KatanaEngine::runNegociatePreRouted() has thrown an exception (C++)." );
        return NULL;
      }
    } else {
      katana->runNegociate( Flags::PreRoutedStage );
    }
    HCATCH
    Py_RETURN_NONE;
  }


  static PyObject* PyKatanaEngine_runNegociate ( PyKatanaEngine* self, PyObject* args )
  {
    cdebug_log(40,0) << "PyKatanaEngine_runNegociate()" << endl;

    unsigned int flags = 0;
    HTRY
      METHOD_HEAD("KatanaEngine.runNegociate()")
      if (PyArg_ParseTuple(args,"I:KatanaEngine.runNegociate", &flags)) {
        if (katana->getViewer()) {
          if (ExceptionWidget::catchAllWrapper( std::bind(&KatanaEngine::runNegociate,katana,flags) )) {
            PyErr_SetString( HurricaneError, "KatanaEngine::runNegociate() has thrown an exception (C++)." );
            return NULL;
          }
        } else {
          katana->runNegociate( flags );
        }
      } else {
        PyErr_SetString( ConstructorError, "KatanaEngine.runNegociate(): Invalid number/bad type of parameter." );
        return NULL;
      }
    HCATCH
    Py_RETURN_NONE;
  }


  // Standart Accessors (Attributes).
  DirectVoidToolMethod  (KatanaEngine,katana,printConfiguration)
  DirectVoidToolMethod  (KatanaEngine,katana,finalizeLayout)
  DirectVoidToolMethod  (KatanaEngine,katana,resetRouting)
  DirectVoidMethod      (KatanaEngine,katana,dumpMeasures)
  DirectGetBoolAttribute(PyKatanaEngine_isGlobalRoutingSuccess,isGlobalRoutingSuccess,PyKatanaEngine,KatanaEngine)
  DirectGetBoolAttribute(PyKatanaEngine_isDetailedRoutingSuccess,isDetailedRoutingSuccess,PyKatanaEngine,KatanaEngine)
  DirectGetUIntAttribute(PyKatanaEngine_getSuccessState,getSuccessState,PyKatanaEngine,KatanaEngine)

  // Standart Destroy (Attribute).
  DBoDestroyAttribute(PyKatanaEngine_destroy,PyKatanaEngine)


  PyMethodDef PyKatanaEngine_Methods[] =
    { { "get"                      , (PyCFunction)PyKatanaEngine_get                     , METH_VARARGS|METH_STATIC
                                   , "Returns the Katana engine attached to the Cell, None if there isnt't." }
    , { "create"                   , (PyCFunction)PyKatanaEngine_create                  , METH_VARARGS|METH_STATIC
                                   , "Create a Katana engine on this cell." }
    , { "setViewer"                , (PyCFunction)PyKatanaEngine_setViewer               , METH_VARARGS
                                   , "Associate a Viewer to this KatanaEngine." }
    , { "digitalInit"              , (PyCFunction)PyKatanaEngine_digitalInit             , METH_NOARGS
                                   , "Setup Katana for digital routing." }
    , { "exclude"                  , (PyCFunction)PyKatanaEngine_exclude                 , METH_VARARGS
                                   , "Exclude a net from routing." }
    , { "printConfiguration"       , (PyCFunction)PyKatanaEngine_printConfiguration      , METH_NOARGS
                                   , "Display on the console the configuration of Katana." }
    , { "getSuccessState"          , (PyCFunction)PyKatanaEngine_getSuccessState         , METH_NOARGS
                                   , "Returns the success statuses of both global & detailed routing." }
    , { "isGlobalRoutingSuccess"   , (PyCFunction)PyKatanaEngine_isGlobalRoutingSuccess  , METH_NOARGS
                                   , "Returns True if the global routing has been successful." }
    , { "isDetailedRoutingSuccess" , (PyCFunction)PyKatanaEngine_isDetailedRoutingSuccess, METH_NOARGS
                                   , "Returns True if the detailed routing has been successful." }
    , { "runGlobalRouter"          , (PyCFunction)PyKatanaEngine_runGlobalRouter         , METH_VARARGS
                                   , "Run the global router (Katana)." }
    , { "loadGlobalRouting"        , (PyCFunction)PyKatanaEngine_loadGlobalRouting       , METH_VARARGS
                                   , "Load global routing into the detailed router." }
    , { "layerAssign"              , (PyCFunction)PyKatanaEngine_layerAssign             , METH_VARARGS
                                   , "Run the layer assigment stage." }
    , { "runNegociatePreRouted"    , (PyCFunction)PyKatanaEngine_runNegociatePreRouted   , METH_NOARGS
                                   , "Run the negociation stage for pre-routed of the detailed router." }
    , { "runNegociate"             , (PyCFunction)PyKatanaEngine_runNegociate            , METH_VARARGS
                                   , "Run the negociation stage of the detailed router." }
    , { "finalizeLayout"           , (PyCFunction)PyKatanaEngine_finalizeLayout          , METH_NOARGS
                                   , "Revert to a pure Hurricane database, remove router's additionnal data structures." }
    , { "resetRouting"             , (PyCFunction)PyKatanaEngine_resetRouting            , METH_NOARGS
                                   , "Remove all router's work, revert to placed only design." }
    , { "dumpMeasures"             , (PyCFunction)PyKatanaEngine_dumpMeasures            , METH_NOARGS
                                   , "Dump to disk lots of statistical informations about the routing." }
    , { "destroy"                  , (PyCFunction)PyKatanaEngine_destroy                 , METH_NOARGS
                                   , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL}        /* sentinel */
    };


  DBoDeleteMethod(KatanaEngine)
  PyTypeObjectLinkPyType(KatanaEngine)


#else  // End of Python Module Code Part.


// +=================================================================+
// |            "PyKatanaEngine" Shared Library Code Part            |
// +=================================================================+


  // Link/Creation Method.
  PyTypeInheritedObjectDefinitions(KatanaEngine,PyToolEngine)
  DBoLinkCreateMethod(KatanaEngine)


  extern  void  PyKatanaEngine_postModuleInit ()
  {
    PyKatanaFlags_postModuleInit();

    PyDict_SetItemString( PyTypeKatanaEngine.tp_dict, "Flags", (PyObject*)&PyTypeKatanaFlags );

    PyObject* constant = NULL;
    LoadObjectConstant( PyTypeKatanaEngine.tp_dict, KatanaEngine::GlobalRoutingSuccess  , "GlobalRoutingSuccess"   )
    LoadObjectConstant( PyTypeKatanaEngine.tp_dict, KatanaEngine::DetailedRoutingSuccess, "DetailedRoutingSuccess" )
  }


#endif  // Shared Library Code Part.

}  // extern "C".

}  // Katana namespace.
 
