
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2013-2013, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyHook.cpp"                                  |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyEntity.h"
#include "hurricane/isobar/PyComponent.h"
#include "hurricane/isobar/PyHook.h"
#include "hurricane/isobar/PyHookCollection.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


#define  METHOD_HEAD(function)   GENERIC_METHOD_HEAD(Hook,hook,function)


// +=================================================================+
// |                "PyHook" Python Module Code Part                 |
// +=================================================================+

#if defined(__PYTHON_MODULE__)

  accessorHook(getNextHook          ,PyHook,Hook)
  accessorHook(getPreviousHook      ,PyHook,Hook)
  accessorHook(getMasterHook        ,PyHook,Hook)
  accessorHook(getNextMasterHook    ,PyHook,Hook)
  accessorHook(getPreviousMasterHook,PyHook,Hook)
  DirectGetBoolAttribute(PyHook_isMaster  ,isMaster  ,PyHook,Hook)
  DirectGetBoolAttribute(PyHook_isAttached,isAttached,PyHook,Hook)


  static PyObject* PyHook_getComponent ( PyHook *self )
  {
    cdebug.log(20) << "PyHook_getComponent()" << endl;
    METHOD_HEAD ( "Hook.getComponent()" )

    Component* component = NULL;
    HTRY
    component = hook->getComponent();
    HCATCH
      
    return (PyObject*)PyEntity_NEW(component);
  }
  

  static PyObject* PyHook_getHooks ( PyHook *self )
  {
    cdebug.log(20) << "PyHook_getHooks()" << endl;

    METHOD_HEAD( "Hook.getHooks()" )

    PyHookCollection* pyHookCollection = NULL;

    HTRY
    Hooks* hooks = new Hooks( hook->getHooks() );

    pyHookCollection = PyObject_NEW( PyHookCollection, &PyTypeHookCollection );
    if (pyHookCollection == NULL) return NULL;

    pyHookCollection->_object = hooks;
    HCATCH
    
    return (PyObject*)pyHookCollection;
  }
  

  static PyObject* PyHook_getSlaveHooks ( PyHook *self )
  {
    cdebug.log(20) << "PyHook_getSlaveHooks()" << endl;

    METHOD_HEAD( "Hook.getSlaveHooks()" )

    PyHookCollection* pyHookCollection = NULL;

    HTRY
    Hooks* hooks = new Hooks( hook->getSlaveHooks() );

    pyHookCollection = PyObject_NEW( PyHookCollection, &PyTypeHookCollection );
    if (pyHookCollection == NULL) return NULL;

    pyHookCollection->_object = hooks;
    HCATCH
    
    return (PyObject*)pyHookCollection;
  }


  static PyObject* PyHook_detach ( PyHook *self )
  {
    cdebug.log(20) << "PyHook_detach()" << endl;
    METHOD_HEAD ( "Hook.detach()" )

    PyHook* pyReturnHook = PyObject_NEW ( PyHook, &PyTypeHook );
    if (pyReturnHook == NULL) return NULL;

    HTRY
    pyReturnHook->_object = hook->detach();
    HCATCH
      
    return (PyObject*)pyReturnHook;
  }


  static PyObject* PyHook_attach ( PyHook *self, PyObject* args )
  {
    cdebug.log(20) << "PyHook_attach()" << endl;
    METHOD_HEAD ( "Hook.attach()" )

    PyHook* pyReturnHook = PyObject_NEW ( PyHook, &PyTypeHook );
    if (pyReturnHook == NULL) return NULL;

    PyObject* pyMasterHook = NULL;
    if (not PyArg_ParseTuple(args,"O:Hook.attach", &pyMasterHook))
      return NULL;

    Hook* masterHook = PYHOOK_O(pyMasterHook);
    if (masterHook == NULL) {
      PyErr_SetString ( ConstructorError
                      , "Hook.attach(): Parameter is not of Hook type" );
      return NULL;
    }

    HTRY
      pyReturnHook->_object = hook->attach( masterHook );
    HCATCH
      
    return (PyObject*)pyReturnHook;
  }


  static PyObject* PyHook_merge ( PyHook *self, PyObject* args )
  {
    cdebug.log(20) << "PyHook_merge()" << endl;
    METHOD_HEAD ( "Hook.merge()" )

    PyHook* pyReturnHook = PyObject_NEW ( PyHook, &PyTypeHook );
    if (pyReturnHook == NULL) return NULL;

    PyObject* pyMasterHook = NULL;
    if (not PyArg_ParseTuple(args,"O:Hook.merge", &pyMasterHook))
      return NULL;

    Hook* masterHook = PYHOOK_O(pyMasterHook);
    if (masterHook == NULL) {
      PyErr_SetString ( ConstructorError
                      , "Hook.merge(): Parameter is not of Hook type" );
      return NULL;
    }

    HTRY
      pyReturnHook->_object = hook->merge( masterHook );
    HCATCH
      
    return (PyObject*)pyReturnHook;
  }


  // Standart destroy (Attribute).


  PyMethodDef PyHook_Methods[] =
    { { "getComponent"         , (PyCFunction)PyHook_getComponent         , METH_NOARGS
                               , "Return the Component into which the Hook is nested." }
    , { "getNextHook"          , (PyCFunction)PyHook_getNextHook          , METH_NOARGS
                               , "Return the the next Hook in the ring." }
    , { "getPreviousHook"      , (PyCFunction)PyHook_getPreviousHook      , METH_NOARGS
                               , "Return the the previous Hook in the ring (walk trough the whole ring)." }
    , { "getMasterHook"        , (PyCFunction)PyHook_getMasterHook        , METH_NOARGS
                               , "Return the master of this Hook." }
    , { "getNextMasterHook"    , (PyCFunction)PyHook_getNextMasterHook    , METH_NOARGS
                               , "Return the master hook next to the master of this Hook." }
    , { "getPreviousMasterHook", (PyCFunction)PyHook_getPreviousMasterHook, METH_NOARGS
                               , "Return the master hook previous to the master of this Hook (walk trough the whole ring)." }
    , { "getHooks"             , (PyCFunction)PyHook_getHooks             , METH_NOARGS
                               , "Return the Collection of all hooks part of that ring." }
    , { "getSlaveHooks"        , (PyCFunction)PyHook_getSlaveHooks        , METH_NOARGS
                               , "Return the Collection of all hooks slave to this one." }
    , { "isMaster"             , (PyCFunction)PyHook_isMaster             , METH_NOARGS
                               , "Tells if this Hook is of master kind" }
    , { "isAttached"           , (PyCFunction)PyHook_isAttached           , METH_NOARGS
                               , "Tells if this Hook is inserted in a ring" }
    , { "detach"               , (PyCFunction)PyHook_detach               , METH_NOARGS
                               , "Detach a slave from it's ring or detach the whole ring of a master." }
    , { "attach"               , (PyCFunction)PyHook_attach               , METH_VARARGS
                               , "Insert this hook (slave) or ring (master) into another ring." }
    , { "merge"                , (PyCFunction)PyHook_merge                , METH_VARARGS
                               , "Merge the the two rings of thoses masters." }
    , {NULL, NULL, 0, NULL}    /* sentinel */
    };


  // ---------------------------------------------------------------
  // PyHook Type Methods.


  PlugDeleteMethod(PyHook_DeAlloc,PyHook)
  PyTypeObjectLinkPyType(Hook)


#else  // End of Python Module Code Part.


// +=================================================================+
// |               "PyHook" Shared Library Code Part                 |
// +=================================================================+

  PyTypeRootObjectDefinitions(Hook)


#endif  // End of Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace. 
