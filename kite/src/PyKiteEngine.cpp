// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2010-2014, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyKiteEngine.cpp"                            |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyCell.h"
#include "hurricane/viewer/CellViewer.h"
#include "hurricane/viewer/ExceptionWidget.h"
#include "hurricane/Cell.h"
#include "kite/PyKiteEngine.h"
#include <functional>

# undef   ACCESS_OBJECT
# undef   ACCESS_CLASS
# define  ACCESS_OBJECT            _baseObject._object
# define  ACCESS_CLASS(_pyObject)  &(_pyObject->_baseObject)
#define   METHOD_HEAD(function)    GENERIC_METHOD_HEAD(KiteEngine,kite,function)


namespace  Kite {

  using std::cerr;
  using std::endl;
  using std::hex;
  using std::ostringstream;
  using Hurricane::tab;
  using Hurricane::in_trace;
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
  using CRL::PyToolEngine;


extern "C" {

#if defined(__PYTHON_MODULE__)


#define DirectVoidToolMethod(SELF_TYPE, SELF_OBJECT, FUNC_NAME)         \
  static PyObject* Py##SELF_TYPE##_##FUNC_NAME(Py##SELF_TYPE* self)     \
  {                                                                     \
      trace << "Py" #SELF_TYPE "_" #FUNC_NAME "()" << endl;             \
      HTRY                                                              \
      METHOD_HEAD(#SELF_TYPE "." #FUNC_NAME "()")                       \
      if (SELF_OBJECT->getViewer()) {                                   \
        ExceptionWidget::catchAllWrapper( std::bind(&KiteEngine::FUNC_NAME,SELF_OBJECT) ); \
      } else {                                                          \
        SELF_OBJECT->FUNC_NAME();                                       \
      }                                                                 \
      HCATCH                                                            \
      Py_RETURN_NONE;                                                   \
  }


// +=================================================================+
// |            "PyKiteEngine" Python Module Code Part               |
// +=================================================================+


  static PyObject* PyKiteEngine_get ( PyObject*, PyObject* args )
  {
    trace << "PyKiteEngine_get()" << endl;

    KiteEngine* kite = NULL;
    
    HTRY
    PyObject* arg0;

    if (not ParseOneArg("Kite.get", args, CELL_ARG, &arg0)) return NULL;
    kite = KiteEngine::get(PYCELL_O(arg0));
    HCATCH

    return PyKiteEngine_Link(kite);
  }


  static PyObject* PyKiteEngine_create ( PyObject*, PyObject* args )
  {
    trace << "PyKiteEngine_create()" << endl;

    KiteEngine* kite = NULL;
    
    HTRY
    PyObject* arg0;

    if (not ParseOneArg("Kite.get", args, CELL_ARG, &arg0)) return NULL;

    Cell* cell = PYCELL_O(arg0);
    kite = KiteEngine::get(cell);

    if (kite == NULL) {
      kite = KiteEngine::create(cell);
      if (cmess1.enabled())
        kite->getKiteConfiguration()->print(cell);
    } else
      cerr << Warning("%s already has a Kite engine.",getString(cell).c_str()) << endl;
    HCATCH

    return PyKiteEngine_Link(kite);
  }


  PyObject* PyKiteEngine_runGlobalRouter ( PyKiteEngine* self, PyObject* args )
  {
    trace << "PyKiteEngine_runGlobalRouter()" << endl;

    HTRY
    METHOD_HEAD("KiteEngine.runGlobalRouter()")
    unsigned int flags = 0;
    if (PyArg_ParseTuple(args,"I:KiteEngine.runGlobalRouter", &flags)) {
      if (kite->getViewer()) {
        ExceptionWidget::catchAllWrapper( std::bind(&KiteEngine::runGlobalRouter,kite,flags) );
      } else {
        kite->runGlobalRouter(flags);
      }
    } else {
      PyErr_SetString(ConstructorError, "KiteEngine.runGlobalRouter(): Invalid number/bad type of parameter.");
      return NULL;
    }
    HCATCH

    Py_RETURN_NONE;
  }


  PyObject* PyKiteEngine_loadGlobalRouting ( PyKiteEngine* self, PyObject* args )
  {
    trace << "PyKiteEngine_loadGlobalRouting()" << endl;

    HTRY
    METHOD_HEAD("KiteEngine.loadGlobalRouting()")
    unsigned int  flags = 0;
    PyObject*     pyRoutingNets = NULL;
    if (PyArg_ParseTuple(args,"IO:KiteEngine.loadGlobalRouting", &flags, &pyRoutingNets)) {
    // This is a MEMORY LEAK. Router should copy & clean.
      KatabaticEngine::NetSet* routingNets = new KatabaticEngine::NetSet();

      if (PyObject_IsTrue(pyRoutingNets)) {
        if (not PyIter_Check(pyRoutingNets)) {
          PyErr_SetString(ConstructorError, "KiteEngine.loadGlobalRouting(): Second parameter do not support iterator protocol.");
          return NULL;
        }

        Cell*     cell      = kite->getCell();
        PyObject* iterator  = PyObject_GetIter(pyRoutingNets);
        PyObject* pyNetName = NULL;
        while( (pyNetName = PyIter_Next(iterator)) ) {
          if (not PyString_Check(pyNetName)) {
            PyErr_SetString(ConstructorError, "KiteEngine.loadGlobalRouting(): The second argument must be a container of strings.");
            return NULL;
          }
          Net* net = cell->getNet(Name(PyString_AsString(pyNetName)));
          routingNets->insert(net);
          Py_DECREF(pyNetName);
        }
        Py_DECREF(iterator);
      }

      if (kite->getViewer()) {
        ExceptionWidget::catchAllWrapper( std::bind(&KiteEngine::loadGlobalRouting,kite,flags/*,*routingNets*/) );
      } else {
        kite->loadGlobalRouting(flags/*,*routingNets*/);
      }
    } else {
      PyErr_SetString(ConstructorError, "KiteEngine.loadGlobalRouting(): Invalid number/bad type of parameter.");
      return NULL;
    }
    HCATCH

    Py_RETURN_NONE;
  }


  PyObject* PyKiteEngine_layerAssign ( PyKiteEngine* self, PyObject* args )
  {
    trace << "PyKiteEngine_layerAssign()" << endl;

    HTRY
    METHOD_HEAD("KiteEngine.layerAssign()")
    unsigned int flags = 0;
    if (PyArg_ParseTuple(args,"I:KiteEngine.layerAssign", &flags)) {

      if (kite->getViewer()) {
        ExceptionWidget::catchAllWrapper( std::bind(&KiteEngine::balanceGlobalDensity,kite) );
        ExceptionWidget::catchAllWrapper( std::bind(&KiteEngine::layerAssign         ,kite,flags) );
      } else {
        kite->balanceGlobalDensity();
        kite->layerAssign         (flags);
      }
    } else {
      PyErr_SetString(ConstructorError, "KiteEngine.layerAssign(): Invalid number/bad type of parameter.");
      return NULL;
    }
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyKiteEngine_runNegociatePreRouted ( PyKiteEngine* self )
  {
    trace << "PyKiteEngine_runNegociatePreRouted()" << endl;
    HTRY
    METHOD_HEAD("KiteEngine.runNegociatePreRouted()")
    if (kite->getViewer()) {
      ExceptionWidget::catchAllWrapper( std::bind(&KiteEngine::runNegociate,kite,Kite::KtPreRoutedStage) );
    } else {
      kite->runNegociate( Kite::KtPreRoutedStage );
    }
    HCATCH
    Py_RETURN_NONE;
  }


  static PyObject* PyKiteEngine_runNegociate ( PyKiteEngine* self )
  {
    trace << "PyKiteEngine_runNegociate()" << endl;
    HTRY
    METHOD_HEAD("KiteEngine.runNegociate()")
    if (kite->getViewer()) {
      ExceptionWidget::catchAllWrapper( std::bind(&KiteEngine::runNegociate,kite,0) );
    } else {
      kite->runNegociate();
    }
    HCATCH
    Py_RETURN_NONE;
  }


  // Standart Accessors (Attributes).
  DirectVoidToolMethod(KiteEngine,kite,printConfiguration)
  DirectVoidToolMethod(KiteEngine,kite,saveGlobalSolution)
  DirectVoidToolMethod(KiteEngine,kite,finalizeLayout)
  DirectVoidMethod(KiteEngine,kite,dumpMeasures)
  DirectGetBoolAttribute(PyKiteEngine_getToolSuccess,getToolSuccess,PyKiteEngine,KiteEngine)

  // Standart Destroy (Attribute).
  DBoDestroyAttribute(PyKiteEngine_destroy,PyKiteEngine)


  PyMethodDef PyKiteEngine_Methods[] =
    { { "get"                  , (PyCFunction)PyKiteEngine_get                  , METH_VARARGS|METH_STATIC
                               , "Returns the Kite engine attached to the Cell, None if there isnt't." }
    , { "create"               , (PyCFunction)PyKiteEngine_create               , METH_VARARGS|METH_STATIC
                               , "Create a Kite engine on this cell." }
    , { "printConfiguration"   , (PyCFunction)PyKiteEngine_printConfiguration   , METH_NOARGS
                               , "Display on the console the configuration of Kite." }
    , { "saveGlobalSolution"   , (PyCFunction)PyKiteEngine_saveGlobalSolution   , METH_NOARGS
                               , "Save the global routing solution on disk." }
    , { "getToolSuccess"       , (PyCFunction)PyKiteEngine_getToolSuccess       , METH_NOARGS
                               , "Returns True if the detailed routing has been successful." }
    , { "loadGlobalRouting"    , (PyCFunction)PyKiteEngine_loadGlobalRouting    , METH_VARARGS
                               , "Read/load the global routing and build topologies for Kite." }
    , { "runGlobalRouter"      , (PyCFunction)PyKiteEngine_runGlobalRouter      , METH_VARARGS
                               , "Run the global router (Knik)." }
    , { "layerAssign"          , (PyCFunction)PyKiteEngine_layerAssign          , METH_VARARGS
                               , "Run the layer assigment stage." }
    , { "runNegociatePreRouted", (PyCFunction)PyKiteEngine_runNegociatePreRouted, METH_NOARGS
                               , "Run the negociation stage for pre-routed of the detailed router." }
    , { "runNegociate"         , (PyCFunction)PyKiteEngine_runNegociate         , METH_NOARGS
                               , "Run the negociation stage of the detailed router." }
    , { "finalizeLayout"       , (PyCFunction)PyKiteEngine_finalizeLayout       , METH_NOARGS
                               , "Revert to a pure Hurricane database, remove router's additionnal data structures." }
    , { "dumpMeasures"         , (PyCFunction)PyKiteEngine_dumpMeasures         , METH_NOARGS
                               , "Dump to disk lots of statistical informations about the routing." }
    , { "destroy"              , (PyCFunction)PyKiteEngine_destroy              , METH_NOARGS
                               , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL}    /* sentinel */
    };


  DBoDeleteMethod(KiteEngine)
  PyTypeObjectLinkPyType(KiteEngine)


#else  // End of Python Module Code Part.


// +=================================================================+
// |             "PyKiteEngine" Shared Library Code Part             |
// +=================================================================+


  // Link/Creation Method.
  PyTypeInheritedObjectDefinitions(KiteEngine,PyToolEngine)
  DBoLinkCreateMethod(KiteEngine)


#endif  // Shared Library Code Part.

}  // extern "C".

}  // Kite namespace.
 
