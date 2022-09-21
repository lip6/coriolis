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
// |  C++ Module  :       "./PyDagExtension.cpp"                     |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyEntity.h"
#include "hurricane/isobar/PyInstance.h"
#include "foehn/PyDagExtension.h"
#include "foehn/DagProperty.h"


namespace Foehn {

  using namespace Hurricane;
  using Isobar::ConstructorError;
  using Isobar::HurricaneError;
  using Isobar::HurricaneWarning;
  using Isobar::PyInstance_Link;
  using Isobar::PyTypeInstance;
  using Isobar::PyInstance;

  extern "C" {


#define  METHOD_HEAD(function)  GENERIC_METHOD_HEAD(DagExtension,property,function)


// +=================================================================+
// |            "PyDagExtension" Python Module Code Part             |
// +=================================================================+

#if defined(__PYTHON_MODULE__)

#define  ExtensionGetBoolFunction(FUNC_NAME,SELF_TYPE)                          \
  static PyObject* Py##SELF_TYPE##_##FUNC_NAME ( PyObject*, PyObject* args )    \
  {                                                                             \
    cdebug_log(20,0) << "Py"#FUNC_NAME"()" << endl;                             \
    bool flag = false;                                                          \
    HTRY                                                                        \
      PyObject* pyDBo;                                                          \
      if (not PyArg_ParseTuple(args,"O:"#SELF_TYPE"."#FUNC_NAME"()", &pyDBo)) { \
        PyErr_SetString( ConstructorError, #SELF_TYPE"."#FUNC_NAME": Takes exactly one arguments." ); \
        return NULL;                                                            \
      }                                                                         \
      Entity* entity = Isobar::EntityCast( pyDBo );                             \
      if (not entity) {                                                         \
        PyErr_SetString( ConstructorError, #SELF_TYPE"."#FUNC_NAME": Argument is not an Entity." ); \
        return NULL;                                                            \
      }                                                                         \
      flag = SELF_TYPE::FUNC_NAME( entity );                                    \
    HCATCH                                                                      \
    if (flag) Py_RETURN_TRUE;                                                   \
    Py_RETURN_FALSE;                                                            \
  }


#define  ExtensionGetInt32Function(FUNC_NAME,SELF_TYPE)                         \
  static PyObject* Py##SELF_TYPE##_##FUNC_NAME ( PyObject*, PyObject* args )    \
  {                                                                             \
    cdebug_log(20,0) << "Py"#FUNC_NAME"()" << endl;                             \
    int32_t value = 0;                                                          \
    HTRY                                                                        \
      PyObject* pyDBo;                                                          \
      if (not PyArg_ParseTuple(args,"O:"#SELF_TYPE"."#FUNC_NAME"()", &pyDBo)) { \
        PyErr_SetString( ConstructorError, #SELF_TYPE"."#FUNC_NAME": Takes exactly one arguments." ); \
        return NULL;                                                            \
      }                                                                         \
      Entity* entity = Isobar::EntityCast( pyDBo );                             \
      if (not entity) {                                                         \
        PyErr_SetString( ConstructorError, #SELF_TYPE"."#FUNC_NAME": Argument is not an Entity." ); \
        return NULL;                                                            \
      }                                                                         \
      value = SELF_TYPE::FUNC_NAME( entity );                                   \
    HCATCH                                                                      \
    return Py_BuildValue( "i", value );                                         \
  }


#define  ExtensionGetUInt64Function(FUNC_NAME,SELF_TYPE)                        \
  static PyObject* Py##SELF_TYPE##_##FUNC_NAME ( PyObject*, PyObject* args )    \
  {                                                                             \
    cdebug_log(20,0) << "Py"#FUNC_NAME"()" << endl;                             \
    uint64_t value = 0;                                                         \
    HTRY                                                                        \
      PyObject* pyDBo;                                                          \
      if (not PyArg_ParseTuple(args,"O:"#SELF_TYPE"."#FUNC_NAME"()", &pyDBo)) { \
        PyErr_SetString( ConstructorError, #SELF_TYPE"."#FUNC_NAME": Takes exactly one arguments." ); \
        return NULL;                                                            \
      }                                                                         \
      Entity* entity = Isobar::EntityCast( pyDBo );                             \
      if (not entity) {                                                         \
        PyErr_SetString( ConstructorError, #SELF_TYPE"."#FUNC_NAME": Argument is not an Entity." ); \
        return NULL;                                                            \
      }                                                                         \
      value = SELF_TYPE::FUNC_NAME( entity );                                   \
    HCATCH                                                                      \
    return Py_BuildValue( "k", value );                                         \
  }


#define  ExtensionSetInt32Function(FUNC_NAME,SELF_TYPE)                       \
  static PyObject* Py##SELF_TYPE##_##FUNC_NAME ( PyObject*, PyObject* args )  \
  {                                                                           \
    cdebug_log(20,0) << "Py"#FUNC_NAME"()" << endl;                           \
    HTRY                                                                      \
      PyObject* pyDBo = NULL;                                                 \
      long      value = 0;                                                    \
      if (not PyArg_ParseTuple(args,"OL:"#SELF_TYPE"."#FUNC_NAME"()", &pyDBo, &value)) { \
        PyErr_SetString( ConstructorError, #SELF_TYPE"."#FUNC_NAME": Takes exactly two arguments." ); \
        return NULL;                                                          \
      }                                                                       \
      Entity* entity = Isobar::EntityCast( pyDBo );                           \
      if (not entity) {                                                       \
        PyErr_SetString( ConstructorError, #SELF_TYPE"."#FUNC_NAME": First argument is not an Entity." ); \
        return NULL;                                                          \
      }                                                                       \
      SELF_TYPE::FUNC_NAME( entity, value );                                  \
    HCATCH                                                                    \
    Py_RETURN_NONE;                                                           \
  }


#define  ExtensionSetUInt64Function(FUNC_NAME,SELF_TYPE)                      \
  static PyObject* Py##SELF_TYPE##_##FUNC_NAME ( PyObject*, PyObject* args )  \
  {                                                                           \
    cdebug_log(20,0) << "Py"#FUNC_NAME"()" << endl;                           \
    HTRY                                                                      \
      PyObject*      pyDBo = NULL;                                            \
      unsigned long  value = 0;                                               \
      if (not PyArg_ParseTuple(args,"Ok:"#SELF_TYPE"."#FUNC_NAME"()", &pyDBo, &value)) { \
        PyErr_SetString( ConstructorError, #SELF_TYPE"."#FUNC_NAME": Takes exactly two arguments." ); \
        return NULL;                                                          \
      }                                                                       \
      Entity* entity = Isobar::EntityCast( pyDBo );                           \
      if (not entity) {                                                       \
        PyErr_SetString( ConstructorError, #SELF_TYPE"."#FUNC_NAME": First argument is not an Entity." ); \
        return NULL;                                                          \
      }                                                                       \
      SELF_TYPE::FUNC_NAME( entity, value );                                  \
    HCATCH                                                                    \
    Py_RETURN_NONE;                                                           \
  }


  static void PyDagExtension_DeAlloc ( PyDagExtension* self )
  {
    cdebug_log(20,0) << "PyDagExtension_DeAlloc(" << hex << self << ")" << endl;
  }


  ExtensionGetBoolFunction  (isPresent      ,DagExtension)
  ExtensionGetBoolFunction  (isNetOwned     ,DagExtension)
  ExtensionGetBoolFunction  (isInstanceOwned,DagExtension)
  ExtensionGetUInt64Function(getFlags       ,DagExtension)
  ExtensionSetUInt64Function(setFlags       ,DagExtension)
  ExtensionSetUInt64Function(resetFlags     ,DagExtension)
  ExtensionGetInt32Function (getMinDepth    ,DagExtension)
  ExtensionGetInt32Function (getMaxDepth    ,DagExtension)
  ExtensionSetInt32Function (setMinDepth    ,DagExtension)
  ExtensionSetInt32Function (setMaxDepth    ,DagExtension)


  static PyObject* PyDagExtension_getDriver ( PyObject*, PyObject* args )
  {
    cdebug_log(20,0) << "PyDagExtension_getDriver()" << endl;
    Instance* driver = nullptr;
    HTRY
      PyObject* pyDBo = NULL;
      if (not PyArg_ParseTuple(args,"O:DagExtension.getDriver()", &pyDBo)) {
        PyErr_SetString( ConstructorError, "DagExtension.getDriver(): Takes exactly one arguments." );
        return NULL;
      }
      Entity* entity = Isobar::EntityCast( pyDBo );
      if (not entity) {
        PyErr_SetString( ConstructorError, "DagExtension.getDriver(): Argument is not an Entity." );
        return NULL;
      }
      driver = DagExtension::getDriver( entity );
    HCATCH
    return PyInstance_Link( driver );
  }


  static PyObject* PyDagExtension_setDriver ( PyObject*, PyObject* args )
  {
    cdebug_log(20,0) << "PyDagExtension_setDriver()" << endl;
    HTRY
      PyObject* pyDBo    = nullptr;
      PyObject* pyDriver = nullptr;
      if (not PyArg_ParseTuple(args,"OO:DagExtension.setDriver()", &pyDBo, &pyDriver)) {
        PyErr_SetString( ConstructorError, "DagExtension.setDriver(): Takes exactly two arguments." );
        return NULL;
      }
      Entity* entity = Isobar::EntityCast( pyDBo );
      if (not entity) {
        PyErr_SetString( ConstructorError, "DagExtension.setDriver: First argument is not an Entity." );
        return NULL;
      }
      if (not IsPyInstance(pyDriver)) {
        PyErr_SetString( ConstructorError, "DagExtension.setDriver: Second argument is not an Instance." );
        return NULL;
      }
      DagExtension::setDriver( entity, PYINSTANCE_O(pyDriver) );
    HCATCH
    Py_RETURN_NONE;
  }


  PyMethodDef PyDagExtension_Methods[] =
    { { "isPresent"      , (PyCFunction)PyDagExtension_isPresent        , METH_VARARGS|METH_CLASS
                          , "Tells if a DBo has a DagProperty." }
    , { "isNetOwned"      , (PyCFunction)PyDagExtension_isNetOwned      , METH_VARARGS|METH_CLASS
                          , "Tells if the DagProperty is owned by a Net." }
    , { "isInstanceOwned" , (PyCFunction)PyDagExtension_isInstanceOwned , METH_VARARGS|METH_CLASS
                          , "Tells if the DagProperty is owned by an Instance." }
    , { "getFlags"        , (PyCFunction)PyDagExtension_getFlags        , METH_VARARGS|METH_CLASS
                          , "Returns the flags of the DAG associated property." }
    , { "getDriver"       , (PyCFunction)PyDagExtension_getDriver       , METH_VARARGS|METH_CLASS
                          , "Returns the driver (Instance) from the DAG property of a Net." }
    , { "setFlags"        , (PyCFunction)PyDagExtension_setFlags        , METH_VARARGS|METH_CLASS
                          , "Set flags of the DAG associated property." }
    , { "resetFlags"      , (PyCFunction)PyDagExtension_resetFlags      , METH_VARARGS|METH_CLASS
                          , "Reset flags of the DAG associated property." }
    , { "getMinDepth"     , (PyCFunction)PyDagExtension_getMinDepth     , METH_VARARGS|METH_CLASS
                          , "Returns the minimum depth of the object in the DAG." }
    , { "getMaxDepth"     , (PyCFunction)PyDagExtension_getMaxDepth     , METH_VARARGS|METH_CLASS
                          , "Returns the maximum depth of the object in the DAG." }
    , { "setMinDepth"     , (PyCFunction)PyDagExtension_setMinDepth     , METH_VARARGS|METH_CLASS
                          , "Sets the minimum depth of the object in the DAG." }
    , { "setMaxDepth"     , (PyCFunction)PyDagExtension_setMaxDepth     , METH_VARARGS|METH_CLASS
                          , "Sets the maximum depth of the object in the DAG." }
    , { "setDriver"       , (PyCFunction)PyDagExtension_setDriver       , METH_VARARGS|METH_CLASS
                          , "Sets the driver (Instance) on the DAG property of a Net." }
    , {NULL, NULL, 0, NULL}  /* sentinel */
    };


  PyTypeObjectLinkPyTypeWithoutObject(DagExtension,DagExtension)


#else  // End of Python Module Code Part.


// +=================================================================+
// |            "PyDagExtension" Shared Library Code Part            |
// +=================================================================+


  PyTypeObjectDefinitions(DagExtension)


  extern  void  PyDagExtension_postModuleInit ()
  {
    PyObject* constant;

    LoadObjectConstant(PyTypeDagExtension.tp_dict,DagExtension::Ignore ,"Ignore" );
    LoadObjectConstant(PyTypeDagExtension.tp_dict,DagExtension::Reached,"Reached");
  }



# endif  // End of Shared Library Code Part.


}  // End of extern "C".

}  // Foehn namespace.
 
