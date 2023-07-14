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
// |  C++ Module  :  "./PyDag.cpp"                                   |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyEntity.h"
#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyCell.h"
#include "hurricane/isobar/PyInstance.h"
#include "hurricane/viewer/ExceptionWidget.h"
#include "hurricane/Cell.h"
#include "crlcore/Utilities.h"
#include "foehn/PyDag.h"
#include "foehn/FoehnEngine.h"
#include <functional>


#define   METHOD_HEAD(function)    GENERIC_METHOD_HEAD(Dag,dag,function)


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
  using Isobar::PyTypeNet;
  using Isobar::PyTypeInstance;


extern "C" {

#if defined(__PYTHON_MODULE__)


// +=================================================================+
// |                "PyDag" Python Module Code Part                  |
// +=================================================================+


  static PyObject* PyDag_setDffRe ( PyDag* self, PyObject* args )
  {
    cdebug_log(40,0) << "PyDag_setDffRe ()" << endl;
    HTRY
      METHOD_HEAD( "Dag.setDffRe()" )
      char* dffRe = NULL;
      if (not PyArg_ParseTuple(args, "s:Dag.setDffRe", &dffRe)) {
        PyErr_SetString( ConstructorError, "Dag.setDffRe(): Invalid number of parameters." );
        return NULL;
      }
      dag->setDffRe( dffRe );
    HCATCH
    Py_RETURN_NONE;
  }


  static PyObject* PyDag_setIgnoredNetRe ( PyDag* self, PyObject* args )
  {
    cdebug_log(40,0) << "PyDag_setIgnoredNetRe ()" << endl;
    HTRY
      METHOD_HEAD( "Dag.setIgnoredNetRe()" )
      char* re = NULL;
      if (not PyArg_ParseTuple(args, "s:Dag.setIgnoredNetRe", &re)) {
        PyErr_SetString( ConstructorError, "Dag.setIgnoredNetRe(): Invalid number of parameters." );
        return NULL;
      }
      dag->setIgnoredNetRe( re );
    HCATCH
    Py_RETURN_NONE;
  }


  static PyObject* PyDag_setIgnoredMasterNetRe ( PyDag* self, PyObject* args )
  {
    cdebug_log(40,0) << "PyDag_setIgnoredMasterNetRe ()" << endl;
    HTRY
      METHOD_HEAD( "Dag.setIgnoredMasterNetRe()" )
      char* re = NULL;
      if (not PyArg_ParseTuple(args, "s:Dag.setIgnoredMasterNetRe", &re)) {
        PyErr_SetString( ConstructorError, "Dag.setIgnoredMasterNetRe(): Invalid number of parameters." );
        return NULL;
      }
      dag->setIgnoredMasterNetRe( re );
    HCATCH
    Py_RETURN_NONE;
  }


  PyObject* PyDag_dpropagate ( PyDag* self )
  {
    cdebug_log(40,0) << "PyDag_dpropagate()" << endl;
    HTRY
      METHOD_HEAD("Dag.dpropagate()")
      if (dag->getFoehn()->getViewer()) {
        if (ExceptionWidget::catchAllWrapper( std::bind(&Dag::dpropagate,dag) )) {
          PyErr_SetString( HurricaneError, "Dag::dpropagate() has thrown an exception (C++)." );
          return NULL;
        }
      } else {
        dag->dpropagate();
      }
    HCATCH
    Py_RETURN_NONE;
  }


  PyObject* PyDag_resetDepths ( PyDag* self )
  {
    cdebug_log(40,0) << "PyDag_resetDepths()" << endl;
    HTRY
      METHOD_HEAD("Dag.resetDepths()")
      if (dag->getFoehn()->getViewer()) {
        if (ExceptionWidget::catchAllWrapper( std::bind(&Dag::resetDepths,dag) )) {
          PyErr_SetString( HurricaneError, "Dag::resetDepths() has thrown an exception (C++)." );
          return NULL;
        }
      } else {
        dag->resetDepths();
      }
    HCATCH
    Py_RETURN_NONE;
  }


  static PyObject* PyDag_addDStart ( PyDag* self, PyObject* args )
  {
    cdebug_log(40,0) << "PyDag_addDStart ()" << endl;
    HTRY
      METHOD_HEAD( "Dag.addDStart()" )
      PyObject* pyDBo = NULL;
      if (not PyArg_ParseTuple(args, "O:Dag.addDStart", &pyDBo)) {
        PyErr_SetString( ConstructorError, "Dag.addDStart(): Invalid number of parameters." );
        return NULL;
      }
      if      (IsPyInstance(pyDBo)) dag->addDStart( PYINSTANCE_O(pyDBo) );
      else if (IsPyNet     (pyDBo)) dag->addDStart( PYNET_O     (pyDBo) );
      else {
        PyErr_SetString( ConstructorError, "Dag.addDStart(): First parameter is *not* an Instance." );
        return NULL;
      }
    HCATCH
    Py_RETURN_NONE;
  }


  // Standart Accessors (Attributes).
  accessorVectorFromVoid(getDOrder,PyDag,Dag,Entity)


  PyMethodDef PyDag_Methods[] =
    { { "setDffRe"                 , (PyCFunction)PyDag_setDffRe                , METH_VARARGS
                                   , "Set the pattern to match the memory elements (D Flip-Flop)." }
    , { "setIgnoredNetRe"          , (PyCFunction)PyDag_setIgnoredNetRe         , METH_VARARGS
                                   , "Set the pattern for ignored nets in propagation." }
    , { "setIgnoredMasterNetRe"    , (PyCFunction)PyDag_setIgnoredMasterNetRe   , METH_VARARGS
                                   , "Set the pattern for ignored *master* net in propagation." }
    , { "addDStart"                , (PyCFunction)PyDag_addDStart               , METH_VARARGS
                                   , "Add a starting instance for the direct propagation." }
    , { "dpropagate"               , (PyCFunction)PyDag_dpropagate              , METH_NOARGS
                                   , "Compute the Instance & Net direct ordering." }
    , { "resetDepths"              , (PyCFunction)PyDag_resetDepths              , METH_NOARGS
                                   , "Reset depths." }
    , { "getDOrder"                , (PyCFunction)PyDag_getDOrder               , METH_NOARGS
                                   , "Return the direct ordering of Instances & Nets." }
    , {NULL, NULL, 0, NULL}        /* sentinel */
    };


  PythonOnlyDeleteMethod(Dag)
  PyTypeObjectLinkPyType(Dag) 



#else  // End of Python Module Code Part.


// +=================================================================+
// |                "PyDag" Shared Library Code Part                 |
// +=================================================================+


  // Link/Creation Method.
  PyTypeObjectDefinitions(Dag)
  LinkCreateMethod(Dag)


#endif  // Shared Library Code Part.

}  // extern "C".

}  // Foehn namespace.
 
