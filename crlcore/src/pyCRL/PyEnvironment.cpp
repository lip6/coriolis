// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2012-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :  "./PyEnvironment.cpp"                           |
// +-----------------------------------------------------------------+


#include "crlcore/PyEnvironment.h"
#include <string>
#include <sstream>


namespace  CRL {

  using std::cerr;
  using std::endl;
  using std::hex;
  using std::string;
  using std::ostringstream;
  using Hurricane::tab;
  using Hurricane::Exception;
  using Hurricane::Bug;
  using Hurricane::Error;
  using Hurricane::Warning;
  using Isobar::ProxyProperty;
  using Isobar::ProxyError;
  using Isobar::ConstructorError;
  using Isobar::HurricaneError;
  using Isobar::HurricaneWarning;
  using Isobar::getPyHash;
  using Isobar::ParseOneArg;
  using Isobar::ParseTwoArg;
  using Isobar::__cs;


extern "C" {


#define METHOD_HEAD(function) GENERIC_METHOD_HEAD(Environment,env,function)


#if defined(__PYTHON_MODULE__)

// +=================================================================+
// |            "PyEnvironment" Python Module Code Part              |
// +=================================================================+


  static PyObject* PyEnvironment_validate ( PyEnvironment *self )
  {
    HTRY
    METHOD_HEAD("Environment.validate()")
    env->validate();
    HCATCH

    Py_RETURN_NONE;
  }


  PyObject* PyEnvironment_addSYSTEM_LIBRARY ( PyEnvironment* self, PyObject* args, PyObject* kwArgs )
  {
    cdebug_log(30,0) << "PyEnvironment_addSYSTEM_LIBRARY()" << endl;

    HTRY
    METHOD_HEAD("Environment.addSYSTEM_LIBRARY()")
    char* library = NULL;
    char* libName = NULL;
    int   addMode = Environment::Append;

    static char* keywords[] = { (char*)"library", (char*)"libName", (char*)"mode", NULL };

    if (PyArg_ParseTupleAndKeywords( args
                                   , kwArgs
                                   , "s|si:Environment.addSYSTEM_LIBRARY"
                                   , keywords
                                   , &library
                                   , &libName
                                   , &addMode )) {
      
      string slibrary (library);
      string slibName;

      switch(addMode) {
        case Environment::Append:
        case Environment::Prepend:
        case Environment::Replace: break;
        default:
          PyErr_SetString ( ConstructorError, "invalid mode value passed to Environment.addSYSTEM_LIBRARY." );
          return NULL;
      }
      
      if ( libName != NULL ) slibName = libName;
      else                   slibName = SearchPath::extractLibName(library);

      env->addSYSTEM_LIBRARY(slibrary.c_str(), slibName.c_str(), addMode);
    } else {
      PyErr_SetString ( ConstructorError, "invalid number of parameters for Environment.addSYSTEM_LIBRARY." );
      return NULL;
    }
    HCATCH

    Py_RETURN_NONE;
  }


  PyObject* PyEnvironment_getLIBRARYPath ( PyEnvironment* self, PyObject* args )
  {
    cdebug_log(30,0) << "PyEnvironment_getLIBRARYPath()" << endl;

    HTRY
    METHOD_HEAD("Environment.getLIBRARYPath()")
    int  index = 0;

    if (PyArg_ParseTuple( args, "i:Environment.addLIBRARYPath", &index )) {
      string path = env->getLIBRARYPath( index );
      if (path.size()) return Py_BuildValue( "s", path.c_str() );
    } else {
      PyErr_SetString( ConstructorError, "invalid number of parameters for Environment.getLIBRARYPath." );
      return NULL;
    }
    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyEnvironment_Repr ( PyEnvironment* self )
  {
    if ( self->ACCESS_OBJECT == NULL )
      return PyString_FromString("<PyObject unbound>");

    ostringstream s;
    s << "<Alliance Environment " << (void*)self->ACCESS_OBJECT << ">";
    return PyString_FromString(s.str().c_str());
  }

  
  // Standart Accessors (Attributes).
  DirectGetStringAttribute(PyEnvironment_getDisplayStyle,getDisplayStyle,PyEnvironment,Environment)
  DirectGetLongAttribute  (PyEnvironment_getSCALE_X     ,getSCALE_X     ,PyEnvironment,Environment)
  DirectGetStringAttribute(PyEnvironment_getIN_LO       ,getIN_LO       ,PyEnvironment,Environment)
  DirectGetStringAttribute(PyEnvironment_getIN_PH       ,getIN_PH       ,PyEnvironment,Environment)
  DirectGetStringAttribute(PyEnvironment_getOUT_LO      ,getOUT_LO      ,PyEnvironment,Environment)
  DirectGetStringAttribute(PyEnvironment_getOUT_PH      ,getOUT_PH      ,PyEnvironment,Environment)
  DirectGetStringAttribute(PyEnvironment_getPOWER       ,getPOWER       ,PyEnvironment,Environment)
  DirectGetStringAttribute(PyEnvironment_getGROUND      ,getGROUND      ,PyEnvironment,Environment)
  DirectGetStringAttribute(PyEnvironment_getCLOCK       ,getCLOCK       ,PyEnvironment,Environment)
  DirectGetStringAttribute(PyEnvironment_getBLOCKAGE    ,getBLOCKAGE    ,PyEnvironment,Environment)
  DirectGetStringAttribute(PyEnvironment_getPad         ,getPad         ,PyEnvironment,Environment)
  DirectGetStringAttribute(PyEnvironment_getRegister    ,getRegister    ,PyEnvironment,Environment)
  DirectGetStringAttribute(PyEnvironment_getCATALOG     ,getCATALOG     ,PyEnvironment,Environment)
  DirectGetStringAttribute(PyEnvironment_getPrint       ,getPrint       ,PyEnvironment,Environment)

  DirectIsAFromCStringAttribute(PyEnvironment_isPOWER   ,isPOWER   ,PyEnvironment,Environment)
  DirectIsAFromCStringAttribute(PyEnvironment_isGROUND  ,isGROUND  ,PyEnvironment,Environment)
  DirectIsAFromCStringAttribute(PyEnvironment_isCLOCK   ,isCLOCK   ,PyEnvironment,Environment)
  DirectIsAFromCStringAttribute(PyEnvironment_isBLOCKAGE,isBLOCKAGE,PyEnvironment,Environment)
  DirectIsAFromCStringAttribute(PyEnvironment_isPad     ,isPad     ,PyEnvironment,Environment)
  DirectIsAFromCStringAttribute(PyEnvironment_isRegister,isRegister,PyEnvironment,Environment)

  // Standart Mutators (Attributes).
  DirectSetCStringAttribute(PyEnvironment_setDisplayStyle   ,setDisplayStyle   ,PyEnvironment,Environment)
  DirectSetLongAttribute   (PyEnvironment_setSCALE_X        ,setSCALE_X        ,PyEnvironment,Environment)
  DirectSetCStringAttribute(PyEnvironment_setIN_LO          ,setIN_LO          ,PyEnvironment,Environment)
  DirectSetCStringAttribute(PyEnvironment_setIN_PH          ,setIN_PH          ,PyEnvironment,Environment)
  DirectSetCStringAttribute(PyEnvironment_setOUT_LO         ,setOUT_LO         ,PyEnvironment,Environment)
  DirectSetCStringAttribute(PyEnvironment_setOUT_PH         ,setOUT_PH         ,PyEnvironment,Environment)
  DirectSetCStringAttribute(PyEnvironment_setPOWER          ,setPOWER          ,PyEnvironment,Environment)
  DirectSetCStringAttribute(PyEnvironment_setGROUND         ,setGROUND         ,PyEnvironment,Environment)
  DirectSetCStringAttribute(PyEnvironment_setCLOCK          ,setCLOCK          ,PyEnvironment,Environment)
  DirectSetCStringAttribute(PyEnvironment_setBLOCKAGE       ,setBLOCKAGE       ,PyEnvironment,Environment)
  DirectSetCStringAttribute(PyEnvironment_setPad            ,setPad            ,PyEnvironment,Environment)
  DirectSetCStringAttribute(PyEnvironment_setRegister       ,setRegister       ,PyEnvironment,Environment)
  DirectSetCStringAttribute(PyEnvironment_setCATALOG        ,setCATALOG        ,PyEnvironment,Environment)
  DirectSetCStringAttribute(PyEnvironment_setWORKING_LIBRARY,setWORKING_LIBRARY,PyEnvironment,Environment)


  // Standart Destroy (Attribute).
  // DBoDestroyAttribute(PyEnvironment_destroy,PyEnvironment)


  PyMethodDef PyEnvironment_Methods[] =
    { { "getDisplayStyle"       , (PyCFunction)PyEnvironment_getDisplayStyle       , METH_NOARGS
                                , "Gets the name of the display style currently in use." }
    , { "getSCALE_X"            , (PyCFunction)PyEnvironment_getSCALE_X            , METH_NOARGS
                                , "Gets the SCALE_X factor (for fixed point computation)." }
    , { "getIN_LO"              , (PyCFunction)PyEnvironment_getIN_LO              , METH_NOARGS
                                , "Gets the netlist input format." }
    , { "getIN_PH"              , (PyCFunction)PyEnvironment_getIN_PH              , METH_NOARGS
                                , "Gets the layout input format." }
    , { "getOUT_LO"             , (PyCFunction)PyEnvironment_getOUT_LO             , METH_NOARGS
                                , "Gets the netlist output format." }
    , { "getOUT_PH"             , (PyCFunction)PyEnvironment_getOUT_PH             , METH_NOARGS
                                , "Gets the layout output format." }
    , { "getPOWER"              , (PyCFunction)PyEnvironment_getPOWER              , METH_NOARGS
                                , "Gets the power net recognition regular expression." }
    , { "getGROUND"             , (PyCFunction)PyEnvironment_getGROUND             , METH_NOARGS
                                , "Gets the ground net recognition regular expression." }
    , { "getCLOCK"              , (PyCFunction)PyEnvironment_getCLOCK              , METH_NOARGS
                                , "Gets the clock net recognition regular expression." }
    , { "getBLOCKAGE"           , (PyCFunction)PyEnvironment_getBLOCKAGE           , METH_NOARGS
                                , "Gets the bockage net recognition regular expression." }
    , { "getPad"                , (PyCFunction)PyEnvironment_getPad                , METH_NOARGS
                                , "Gets the pad cell recognition regular expression." }
    , { "getCATALOG"            , (PyCFunction)PyEnvironment_getCATALOG            , METH_NOARGS
                                , "Gets the catalog file name." }
    , { "getLIBRARYPath"        , (PyCFunction)PyEnvironment_getLIBRARYPath        , METH_VARARGS
                                , "Gets the path of the library at the given index." }
    , { "getPrint"              , (PyCFunction)PyEnvironment_getPrint              , METH_NOARGS
                                , "Display the environment in a terminal fashion formating." }
    , { "isPOWER"               , (PyCFunction)PyEnvironment_isPOWER               , METH_VARARGS
                                , "Checks if a name is a power net name." }
    , { "isGROUND"              , (PyCFunction)PyEnvironment_isGROUND              , METH_VARARGS
                                , "Checks if a name is a ground net name." }
    , { "isCLOCK"               , (PyCFunction)PyEnvironment_isCLOCK               , METH_VARARGS
                                , "Checks if a name is a clock net name." }
    , { "isBLOCKAGE"            , (PyCFunction)PyEnvironment_isBLOCKAGE            , METH_VARARGS
                                , "Checks if a name is a blockage net name." }
    , { "isPad"                 , (PyCFunction)PyEnvironment_isPad                 , METH_VARARGS
                                , "Checks if a name is a pad cell name." }
    , { "isRegister"            , (PyCFunction)PyEnvironment_isRegister            , METH_VARARGS
                                , "Checks if a name is a register cell name." }
    , { "validate"              , (PyCFunction)PyEnvironment_validate              , METH_NOARGS
                                , "Validate the coherency of the settings (raise an exception)." }
    , { "setDisplayStyle"       , (PyCFunction)PyEnvironment_setDisplayStyle       , METH_VARARGS
                                , "Sets the name of the display style to be used." }
    , { "setSCALE_X"            , (PyCFunction)PyEnvironment_setSCALE_X            , METH_VARARGS
                                , "Sets the SCALE_X factor (for fixed point computation)." }
    , { "setIN_LO"              , (PyCFunction)PyEnvironment_setIN_LO              , METH_VARARGS
                                , "Sets the netlist input format." }
    , { "setIN_PH"              , (PyCFunction)PyEnvironment_setIN_PH              , METH_VARARGS
                                , "Sets the layout input format." }
    , { "setOUT_LO"             , (PyCFunction)PyEnvironment_setOUT_LO             , METH_VARARGS
                                , "Sets the netlist output format." }
    , { "setOUT_PH"             , (PyCFunction)PyEnvironment_setOUT_PH             , METH_VARARGS
                                , "Sets the layout output format." }
    , { "setPOWER"              , (PyCFunction)PyEnvironment_setPOWER              , METH_VARARGS
                                , "Sets the power net recognition regular expression." }
    , { "setGROUND"             , (PyCFunction)PyEnvironment_setGROUND             , METH_VARARGS
                                , "Sets the ground net recognition regular expression." }
    , { "setCLOCK"              , (PyCFunction)PyEnvironment_setCLOCK              , METH_VARARGS
                                , "Sets the clock net recognition regular expression." }
    , { "setBLOCKAGE"           , (PyCFunction)PyEnvironment_setBLOCKAGE           , METH_VARARGS
                                , "Sets the blockage net recognition regular expression." }
    , { "setPad"                , (PyCFunction)PyEnvironment_setPad                , METH_VARARGS
                                , "Sets the pad cell recognition regular expression." }
    , { "setRegister"           , (PyCFunction)PyEnvironment_setRegister           , METH_VARARGS
                                , "Sets the register cell recognition regular expression." }
    , { "setCATALOG"            , (PyCFunction)PyEnvironment_setCATALOG            , METH_VARARGS
                                , "Sets the name of the per library catalog file." }
    , { "setWORKING_LIBRARY"    , (PyCFunction)PyEnvironment_setWORKING_LIBRARY    , METH_VARARGS
                                , "Sets the working library path." }
    , { "addSYSTEM_LIBRARY"     , (PyCFunction)PyEnvironment_addSYSTEM_LIBRARY     , METH_VARARGS|METH_KEYWORDS
                                , "Add/replace a library in the system search path." }
  //, { "destroy"               , (PyCFunction)PyEnvironment_destroy               , METH_VARARGS
  //                            , "Destroy the associated hurricane object. The python object remains." }
    , {NULL, NULL, 0, NULL}   /* sentinel */
    };


  PythonOnlyDeleteMethod(Environment)
  DirectHashMethod(PyEnvironment_Hash, Environment)

  extern void  PyEnvironment_LinkPyType() {
    cdebug_log(30,0) << "PyEnvironment_LinkType()" << endl;

    PyTypeEnvironment.tp_dealloc = (destructor) PyEnvironment_DeAlloc;
    PyTypeEnvironment.tp_repr    = (reprfunc)   PyEnvironment_Repr;
    PyTypeEnvironment.tp_str     = (reprfunc)   PyEnvironment_Repr;
    PyTypeEnvironment.tp_hash    = (hashfunc)   PyEnvironment_Hash;
    PyTypeEnvironment.tp_methods = PyEnvironment_Methods;
  }


#else  // End of Python Module Code Part.

// +=================================================================+
// |             "PyEnvironment" Shared Library Code Part            |
// +=================================================================+

  // Type Definition.
  PyTypeObjectDefinitionsOfModule(CRL,Environment)

  // Link/Creation Method.
  LinkCreateMethod(Environment)


  extern  void  PyEnvironment_postModuleInit ()
  {
    PyObject* constant;

    LoadObjectConstant(PyTypeEnvironment.tp_dict,Environment::Append ,"Append" );
    LoadObjectConstant(PyTypeEnvironment.tp_dict,Environment::Prepend,"Prepend");
    LoadObjectConstant(PyTypeEnvironment.tp_dict,Environment::Replace,"Replace");
  }


#endif  // End of Shared Library Code Part.


}  // extern "C".


}  // CRL namespace.
