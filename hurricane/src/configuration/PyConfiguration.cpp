// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU/LIP6 2020-2020, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyConfiguration.cpp"                    |
// +-----------------------------------------------------------------+


#include "hurricane/configuration/PyConfiguration.h"


namespace  Cfg2 {

using namespace Hurricane;

extern "C" {


//#define  METHOD_HEAD(function)  GENERIC_METHOD_HEAD(Configuration,cw,function)


// +=================================================================+
// |            "PyConfiguration" Python Module Code Part            |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  // Standart Destroy (Attribute).
  // DirectDestroyAttribute(PyConfiguration_destroy, PyConfiguration)


  // static PyObject* PyConfiguration_hasMenu ( PyConfiguration* self, PyObject* args )
  // {
  //   cdebug_log(20,0) << "PyConfiguration_hasMenu()" << endl;

  //   HTRY
  //   METHOD_HEAD("Configuration.hasMenu()")

  //   char* path = NULL;
  //   if (not PyArg_ParseTuple(args,"s:Configuration.hasMenu()", &path)) {
  //     PyErr_SetString ( ConstructorError, "Configuration.hasMenu(): Takes exactly one argument." );
  //     return NULL;
  //   }

  //   if (cw->hasMenu( path )) Py_RETURN_TRUE;
  //   HCATCH

  //   Py_RETURN_FALSE;
  // }


  // ---------------------------------------------------------------
  // PyConfiguration Attribute Method table.

  PyMethodDef PyConfiguration_Methods[] =
    {
    //   { "hasMenu"             , (PyCFunction)PyConfiguration_hasMenu             , METH_VARARGS
    //                           , "Return true if the menu at \"path\" exists." }
      {NULL, NULL, 0, NULL}   /* sentinel */
    };


  // PythonOnlyDeleteMethod(Configuration)
  // PyTypeObjectLinkPyType(Configuration)


#else  // Python Module Code Part.


// +=================================================================+
// |           "PyConfiguration" Shared Library Code Part            |
// +=================================================================+


  // LinkCreateMethod(Configuration)
  // PyTypeRootObjectDefinitions(Configuration)


  // static void  ConfigurationLoadConstants ( PyObject* dictionnary ) {
  //   PyObject* constant;

  //   LoadObjectConstant( dictionnary, Configuration::NoFlags, "NoFlags" )
  //   LoadObjectConstant( dictionnary, Configuration::TopMenu, "TopMenu" )
  // }


  // extern  void  PyConfiguration_postModuleInit ()
  // {
  //   ConfigurationLoadConstants(PyTypeConfiguration.tp_dict);
  // }

# endif  // Shared Library Code Part.


}  // extern "C".

}  // Cfg namespace.
