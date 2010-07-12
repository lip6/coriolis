
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2010-2010, All Rights Reserved
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyNetExternalComponents.cpp"            |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyComponent.h"
#include "hurricane/isobar/PyComponentCollection.h"
#include "hurricane/isobar/PyNetExternalComponents.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


#define  METHOD_HEAD(function)  GENERIC_METHOD_HEAD(NetExternalComponents,property,function)


// x=================================================================x
// |        "PyNetExternalComponents" Python Module Code Part        |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  static void PyNetExternalComponents_DeAlloc ( PyNetExternalComponents* self )
  {
    trace << "PySingletonObject_DeAlloc(" << hex << self << ")" << endl;
  }
  

  static PyObject* PyNetExternalComponents_isExternal ( PyObject*, PyObject *args )
  {
    trace << "PyNetExternalComponents_isExternal()" << endl;

    bool isExternal = false;

    HTRY
    PyObject* arg0;
    if ( not ParseOneArg ( "NetExternalComponents.isExternal()", args, COMP_ARG, &arg0) ) return NULL;
    
    isExternal = NetExternalComponents::isExternal(PYCOMPONENT_O(arg0));
    HCATCH

    if ( isExternal ) Py_RETURN_TRUE;
    Py_RETURN_FALSE;
  }


  static PyObject* PyNetExternalComponents_get ( PyObject*, PyObject* args )
  {
    trace << "PyNetExternalComponents_getExternalComponents()" << endl;

    PyObject* arg0;

    PyComponentCollection* pyComponentCollection = NULL;

    HTRY

    if ( not ParseOneArg ( "getExternalComponents", args, ":ent", &arg0) ) return NULL;
    Components* components = new Components(NetExternalComponents::get(PYNET_O(arg0)));

    pyComponentCollection = PyObject_NEW(PyComponentCollection, &PyTypeComponentCollection);
    if (pyComponentCollection == NULL) { return NULL; }

    pyComponentCollection->_object = components;

    HCATCH

    return (PyObject*)pyComponentCollection;
  }


  PyMethodDef PyNetExternalComponents_Methods[] =
    { { "isExternal", (PyCFunction)PyNetExternalComponents_isExternal, METH_VARARGS|METH_CLASS
                    , "Tells if Component belong to the externals of the Net." }
    , { "get"       , (PyCFunction)PyNetExternalComponents_get       , METH_VARARGS|METH_CLASS
                    , "Returns the Collection of external components of the Net." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };


  PyTypeObjectLinkPyTypeWithoutObject(NetExternalComponents,NetExternalComponents)


  // extern void  PyNetExternalComponents_LinkPyType()
  // {
  //   trace << "PyNetExternalComponents_LinkType()" << endl;

  //   PyTypeNetExternalComponents.tp_new     = (newfunc)   PyType_GenericNew;
  //   PyTypeNetExternalComponents.tp_dealloc = (destructor)PyNetExternalComponents_DeAlloc;
  //   PyTypeNetExternalComponents.tp_methods = PyNetExternalComponents_Methods;
  // }


#else  // End of Python Module Code Part.


// x=================================================================x
// |       "PyNetExternalComponents" Shared Library Code Part        |
// x=================================================================x


  PyTypeObjectDefinitions(NetExternalComponents)


# endif  // End of Shared Library Code Part.


}  // End of extern "C".


}  // End of Isobar namespace.
 
