// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2017-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyNetRoutingProperty.cpp"                    |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyNetRoutingProperty.h"
#include "hurricane/isobar/PyNetRoutingState.h"
#include "hurricane/isobar/PyNet.h"


namespace  Isobar {

  using std::cerr;
  using std::endl;
  using std::hex;
  using std::ostringstream;
  using Hurricane::tab;
  using Hurricane::Error;
  using Hurricane::Warning;


extern "C" {


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(NetRoutingExtension,extension,function)


// +=================================================================+
// |         "PyNetRoutingExtension" Python Module Code Part         |
// +=================================================================+

#if defined(__PYTHON_MODULE__)

# define  ExtensionGetBoolFunction(FUNC_NAME,SELF_TYPE)                       \
  static PyObject* Py##SELF_TYPE##_##FUNC_NAME ( PyObject*, PyObject* args )  \
  {                                                                           \
    cdebug_log(20,0) << "Py"#FUNC_NAME"()" << endl;                           \
    bool flag = false;                                                        \
    HTRY                                                                      \
      PyObject* arg0;                                                         \
      if (not ParseOneArg(#SELF_TYPE"."#FUNC_NAME"()", args, NET_ARG, &arg0)) return NULL;  \
      flag = SELF_TYPE::FUNC_NAME( PYNET_O(arg0) );                           \
    HCATCH                                                                    \
    if (flag) Py_RETURN_TRUE;                                                 \
    Py_RETURN_FALSE;                                                          \
  }

# define  ExtensionGetUIntFunction(FUNC_NAME,SELF_TYPE)                       \
  static PyObject* Py##SELF_TYPE##_##FUNC_NAME ( PyObject*, PyObject* args )  \
  {                                                                           \
    cdebug_log(20,0) << "Py"#FUNC_NAME"()" << endl;                           \
    uint32_t value = 0;                                                       \
    HTRY                                                                      \
      PyObject* arg0;                                                         \
      if (not ParseOneArg(#SELF_TYPE"."#FUNC_NAME"()", args, NET_ARG, &arg0)) return NULL;  \
      value = SELF_TYPE::FUNC_NAME( PYNET_O(arg0) );                          \
    HCATCH                                                                    \
    return Py_BuildValue( "I", value );                                       \
  }

# define  ExtensionGetLongFunction(FUNC_NAME,SELF_TYPE)                       \
  static PyObject* Py##SELF_TYPE##_##FUNC_NAME ( PyObject*, PyObject* args )  \
  {                                                                           \
    cdebug_log(20,0) << "Py"#FUNC_NAME"()" << endl;                           \
    long value = 0;                                                           \
    HTRY                                                                      \
      PyObject* arg0;                                                         \
      if (not ParseOneArg(#SELF_TYPE"."#FUNC_NAME"()", args, NET_ARG, &arg0)) return NULL;  \
      value = SELF_TYPE::FUNC_NAME( PYNET_O(arg0) );                          \
    HCATCH                                                                    \
    return Py_BuildValue("L",value);                                          \
  }

# define  ExtensionSetUIntFunction(FUNC_NAME,SELF_TYPE)                       \
  static PyObject* Py##SELF_TYPE##_##FUNC_NAME ( PyObject*, PyObject* args )  \
  {                                                                           \
    cdebug_log(20,0) << "Py"#FUNC_NAME"()" << endl;                           \
    HTRY                                                                      \
      PyObject* arg0  = NULL;                                                 \
      PyObject* pyNet = NULL;                                                 \
      if (not PyArg_ParseTuple(args,"OO:"#SELF_TYPE"."#FUNC_NAME"()", &pyNet, &arg0)) return NULL; \
      if (not IsPyNet(pyNet)) return NULL;                                    \
      SELF_TYPE::FUNC_NAME( PYNET_O(pyNet), Isobar::PyAny_AsUInt32(arg0) );   \
    HCATCH                                                                    \
    Py_RETURN_NONE;                                                           \
  }

# define  ExtensionSetLongFunction(FUNC_NAME,SELF_TYPE)                       \
  static PyObject* Py##SELF_TYPE##_##FUNC_NAME ( PyObject*, PyObject* args )  \
  {                                                                           \
    cdebug_log(20,0) << "Py"#FUNC_NAME"()" << endl;                           \
    HTRY                                                                      \
      PyObject* arg0  = NULL;                                                 \
      PyObject* pyNet = NULL;                                                 \
      if (not PyArg_ParseTuple(args,"OO:"#SELF_TYPE"."#FUNC_NAME"()", &pyNet, &arg0)) return NULL; \
      if (not IsPyNet(pyNet)) return NULL;                                    \
      SELF_TYPE::FUNC_NAME( PYNET_O(pyNet), Isobar::PyAny_AsLong(arg0) );     \
    HCATCH                                                                    \
    Py_RETURN_NONE;                                                           \
  }


  static void PyNetRoutingExtension_DeAlloc ( PyNetRoutingExtension* self )
  {
    cdebug_log(20,0) << "PySingletonObject_DeAlloc(" << hex << self << ")" << endl;
  }


//ExtensionGetBoolFunction(isExcluded            ,NetRoutingExtension)
  ExtensionGetBoolFunction(isFixed               ,NetRoutingExtension)
  ExtensionGetBoolFunction(isUnconnected         ,NetRoutingExtension)
  ExtensionGetBoolFunction(isManualGlobalRoute   ,NetRoutingExtension)
  ExtensionGetBoolFunction(isManualDetailRoute   ,NetRoutingExtension)
  ExtensionGetBoolFunction(isAutomaticGlobalRoute,NetRoutingExtension)
  ExtensionGetBoolFunction(isMixedPreRoute       ,NetRoutingExtension)
  ExtensionGetBoolFunction(isSymmetric           ,NetRoutingExtension)
  ExtensionGetBoolFunction(isSymHorizontal       ,NetRoutingExtension)
  ExtensionGetBoolFunction(isSymVertical         ,NetRoutingExtension)
  ExtensionGetBoolFunction(isSymMaster           ,NetRoutingExtension)
//ExtensionGetBoolFunction(isSymSlave            ,NetRoutingExtension)
//ExtensionGetBoolFunction(isSelfSym             ,NetRoutingExtension)
  ExtensionGetBoolFunction(isAnalog              ,NetRoutingExtension)
  ExtensionGetUIntFunction(getFlags              ,NetRoutingExtension)
  ExtensionGetLongFunction(getSymAxis            ,NetRoutingExtension)

  ExtensionSetUIntFunction(setFlags              ,NetRoutingExtension)
  ExtensionSetUIntFunction(unsetFlags            ,NetRoutingExtension)
  ExtensionSetLongFunction(setSymAxis            ,NetRoutingExtension)
  ExtensionGetUIntFunction(getWPitch             ,NetRoutingExtension)
  ExtensionSetUIntFunction(setWPitch             ,NetRoutingExtension)


  static PyObject* PyNetRoutingExtension_getSymNet ( PyObject*, PyObject* args )
  {
    cdebug_log(20,0) << "PyNetRoutingExtension_getSymNet()" << endl;
    Net* symNet = NULL;
    HTRY
      PyObject* arg0;
      if (not ParseOneArg("NetRoutingExtension.getSymNet()", args, NET_ARG, &arg0)) return NULL;
      symNet = NetRoutingExtension::getSymNet( PYNET_O(arg0) );
    HCATCH
    return PyNet_Link(symNet);
  }


  static PyObject* PyNetRoutingExtension_get ( PyObject*, PyObject* args )
  {
    cdebug_log(20,0) << "PyNetRoutingExtension_get()" << endl;

    NetRoutingState* state = NULL;
    PyObject*        arg0  = NULL;
    HTRY
      if (not ParseOneArg("NetRoutingExtension.get", args, ":ent", &arg0)) return NULL;
      state = NetRoutingExtension::get( PYNET_O(arg0) );
    HCATCH
    return PyNetRoutingState_Link( state );
  }


  static PyObject* PyNetRoutingExtension_create ( PyObject*, PyObject* args )
  {
    cdebug_log(20,0) << "PyNetRoutingExtension_create()" << endl;

    NetRoutingState* state = NULL;
    HTRY
      PyObject* pyNet   = NULL;
      PyObject* pyFlags = NULL;
      __cs.init( "NetRoutingExtension.create" );
      
      if (not PyArg_ParseTuple(args,"O&|O&:NetRoutingExtension.create",
                               Converter, &pyNet,
                               Converter, &pyFlags)) 
        return NULL;

      if      (__cs.getObjectIds() == ":ent"    ) { state = NetRoutingExtension::create( PYNET_O(pyNet) ); }
      else if (__cs.getObjectIds() == ":ent:int") { state = NetRoutingExtension::create( PYNET_O(pyNet), PyAny_AsLong(pyFlags) ); }
      else {
        PyErr_SetString( ConstructorError, "invalid/bad number of parameters for NetRoutingExtension.create()." );
        return NULL;
      }
    HCATCH
    return PyNetRoutingState_Link( state );
  }


  PyMethodDef PyNetRoutingExtension_Methods[] =
    { { "isFixed"               , (PyCFunction)PyNetRoutingExtension_isFixed               , METH_NOARGS |METH_CLASS , "To be documented." }
    , { "isUnconnected"         , (PyCFunction)PyNetRoutingExtension_isUnconnected         , METH_NOARGS |METH_CLASS , "To be documented." }
    , { "isManualGlobalRoute"   , (PyCFunction)PyNetRoutingExtension_isManualGlobalRoute   , METH_NOARGS |METH_CLASS , "To be documented." }
    , { "isManualDetailRoute"   , (PyCFunction)PyNetRoutingExtension_isManualDetailRoute   , METH_NOARGS |METH_CLASS , "To be documented." }
    , { "isAutomaticGlobalRoute", (PyCFunction)PyNetRoutingExtension_isAutomaticGlobalRoute, METH_NOARGS |METH_CLASS , "To be documented." }
    , { "isMixedPreRoute"       , (PyCFunction)PyNetRoutingExtension_isMixedPreRoute       , METH_NOARGS |METH_CLASS , "To be documented." }
    , { "isSymmetric"           , (PyCFunction)PyNetRoutingExtension_isSymmetric           , METH_NOARGS |METH_CLASS , "To be documented." }
    , { "isSymHorizontal"       , (PyCFunction)PyNetRoutingExtension_isSymHorizontal       , METH_NOARGS |METH_CLASS , "To be documented." }
    , { "isSymVertical"         , (PyCFunction)PyNetRoutingExtension_isSymVertical         , METH_NOARGS |METH_CLASS , "To be documented." }
    , { "isSymMaster"           , (PyCFunction)PyNetRoutingExtension_isSymMaster           , METH_NOARGS |METH_CLASS , "To be documented." }
  //, { "isSymSlave"            , (PyCFunction)PyNetRoutingExtension_isSymSlave            , METH_NOARGS |METH_CLASS , "To be documented." }
  //, { "isSelfSym"             , (PyCFunction)PyNetRoutingExtension_isSelfSym             , METH_NOARGS |METH_CLASS , "To be documented." }
    , { "isAnalog"              , (PyCFunction)PyNetRoutingExtension_isAnalog              , METH_NOARGS |METH_CLASS , "To be documented." }
    , { "getFlags"              , (PyCFunction)PyNetRoutingExtension_getFlags              , METH_NOARGS |METH_CLASS , "To be documented." }
    , { "getSymAxis"            , (PyCFunction)PyNetRoutingExtension_getSymAxis            , METH_NOARGS |METH_CLASS , "To be documented." }
    , { "setFlags"              , (PyCFunction)PyNetRoutingExtension_setFlags              , METH_VARARGS|METH_CLASS , "To be documented." }
    , { "unsetFlags"            , (PyCFunction)PyNetRoutingExtension_unsetFlags            , METH_VARARGS|METH_CLASS , "To be documented." }
    , { "setSymAxis"            , (PyCFunction)PyNetRoutingExtension_setSymAxis            , METH_VARARGS|METH_CLASS , "To be documented." }
    , { "getSymNet"             , (PyCFunction)PyNetRoutingExtension_getSymNet             , METH_VARARGS|METH_CLASS , "To be documented." }
    , { "get"                   , (PyCFunction)PyNetRoutingExtension_get                   , METH_VARARGS|METH_CLASS
                                , "Returns the NetRoutingState, or None has not been created yet." }
    , { "create"                , (PyCFunction)PyNetRoutingExtension_create                , METH_VARARGS|METH_CLASS
                                , "Returns the NetRoutingState, create it if needs be." }
    , { "setWPitch"             , (PyCFunction)PyNetRoutingExtension_setWPitch             , METH_VARARGS|METH_CLASS , "To be documented." }
    , { "getWPitch"             , (PyCFunction)PyNetRoutingExtension_getWPitch             , METH_NOARGS |METH_CLASS , "To be documented." }
    , {NULL, NULL, 0, NULL}     /* sentinel */
    };


  PyTypeObjectLinkPyTypeWithoutObject(NetRoutingExtension,NetRoutingExtension)


#else  // End of Python Module Code Part.


// +=================================================================+
// |         "PyNetRoutingExtension" Shared Library Code Part        |
// +=================================================================+

  // Link/Creation Method.
  PyTypeObjectDefinitions(NetRoutingExtension)


#endif  // Shared Library Code Part.

}  // extern "C".

}  // Isobar namespace.
 
