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
// |  C++ Module  :       "./PyParameter.cpp"                        |
// +-----------------------------------------------------------------+


#include "hurricane/configuration/PyParameter.h"


namespace  Cfg2 {

using namespace Hurricane;

extern "C" {


//#define  METHOD_HEAD(function)  GENERIC_METHOD_HEAD(Parameter,cw,function)


// +=================================================================+
// |              "PyParameter" Python Module Code Part              |
// +=================================================================+

#if defined(__PYTHON_MODULE__)


  // Standart Destroy (Attribute).
  // DirectDestroyAttribute(PyParameter_destroy, PyParameter)


  // static PyObject* PyParameter_hasMenu ( PyParameter* self, PyObject* args )
  // {
  //   cdebug_log(20,0) << "PyParameter_hasMenu()" << endl;

  //   HTRY
  //   METHOD_HEAD("Parameter.hasMenu()")

  //   char* path = NULL;
  //   if (not PyArg_ParseTuple(args,"s:Parameter.hasMenu()", &path)) {
  //     PyErr_SetString ( ConstructorError, "Parameter.hasMenu(): Takes exactly one argument." );
  //     return NULL;
  //   }

  //   if (cw->hasMenu( path )) Py_RETURN_TRUE;
  //   HCATCH

  //   Py_RETURN_FALSE;
  // }


  // ---------------------------------------------------------------
  // PyParameter Attribute Method table.

  PyMethodDef PyParameter_Methods[] =
    {
    //   { "hasMenu"             , (PyCFunction)PyParameter_hasMenu             , METH_VARARGS
    //                           , "Return true if the menu at \"path\" exists." }
      {NULL, NULL, 0, NULL}   /* sentinel */
    };


  // PythonOnlyDeleteMethod(Parameter)
  // PyTypeObjectLinkPyType(Parameter)


#else  // Python Module Code Part.


// +=================================================================+
// |             "PyParameter" Shared Library Code Part              |
// +=================================================================+


  // LinkCreateMethod(Parameter)
  // PyTypeRootObjectDefinitions(Parameter)


  // static void  ParameterLoadConstants ( PyObject* dictionnary ) {
  //   PyObject* constant;

  //   LoadObjectConstant( dictionnary, Parameter::NoFlags, "NoFlags" )
  //   LoadObjectConstant( dictionnary, Parameter::TopMenu, "TopMenu" )
  // }


  // extern  void  PyParameter_postModuleInit ()
  // {
  //   ParameterLoadConstants(PyTypeParameter.tp_dict);
  // }

# endif  // Shared Library Code Part.


}  // extern "C".

}  // Cfg namespace.
