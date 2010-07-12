
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
// |  C++ Module  :       "./PyBreakpoint.cpp"                       |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#include "hurricane/isobar/PyNet.h"
#include "hurricane/isobar/PyComponent.h"
#include "hurricane/isobar/PyComponentCollection.h"
#include "hurricane/isobar/PyBreakpoint.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


#define  METHOD_HEAD(function)  GENERIC_METHOD_HEAD(Breakpoint,bp,function)


// x=================================================================x
// |             "PyBreakpoint" Python Module Code Part              |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  static void PyBreakpoint_DeAlloc ( PyBreakpoint* self )
  {
    trace << "PySingletonObject_DeAlloc(" << hex << self << ")" << endl;
  }
  

  static PyObject* PyBreakpoint_stop ( PyObject*, PyObject *args )
  {
    trace << "PyBreakpoint_stop()" << endl;

    bool result = false;

    HTRY
    PyObject* arg0;
    PyObject* arg1;
    if ( not ParseTwoArg ( "Breakpoint::stop()", args, ":int:string", &arg0, &arg1) ) return NULL;
    
    result = Breakpoint::stop( (unsigned int)PyInt_AsLong     (arg0)
                             ,               PyString_AsString(arg1)
                             );
    HCATCH

    if ( result ) Py_RETURN_TRUE;
    Py_RETURN_FALSE;
  }


  static PyObject* PyBreakpoint_setStopLevel ( PyObject*, PyObject* args )
  {
    trace << "PyBreakpoint_setStopLevel()" << endl;


    HTRY

    PyObject* arg0;
    if ( not ParseOneArg ( "Breakpoint::setStopLevel()", args, ":int", &arg0) ) return NULL;

    Breakpoint::setStopLevel ( (unsigned int)PyInt_AsLong(arg0) );

    HCATCH

    Py_RETURN_NONE;
  }


  static PyObject* PyBreakpoint_getStopLevel ( PyObject* )
  {
    trace << "PyBreakpoint_getStopLevel()" << endl;

    return Py_BuildValue ( "i", Breakpoint::getStopLevel() );

  }


  PyMethodDef PyBreakpoint_Methods[] =
    { { "stop"        , (PyCFunction)PyBreakpoint_stop, METH_VARARGS|METH_CLASS
                      , "Sets a breakpoint of the given level." }
    , { "setStopLevel", (PyCFunction)PyBreakpoint_setStopLevel, METH_VARARGS|METH_CLASS
                      , "Sets the level below which breakpoints will be ignoreds." }
    , { "getStopLevel", (PyCFunction)PyBreakpoint_getStopLevel, METH_NOARGS|METH_CLASS
                      , "Returns the level below which breakpoints will be ignoreds." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
    };


  PyTypeObjectLinkPyTypeWithoutObject(Breakpoint,Breakpoint)


  // extern void  PyBreakpoint_LinkPyType()
  // {
  //   trace << "PyBreakpoint_LinkType()" << endl;

  //   PyTypeBreakpoint.tp_new     = (newfunc)   PyType_GenericNew;
  //   PyTypeBreakpoint.tp_dealloc = (destructor)PyBreakpoint_DeAlloc;
  //   PyTypeBreakpoint.tp_methods = PyBreakpoint_Methods;
  // }


#else  // End of Python Module Code Part.


// x=================================================================x
// |            "PyBreakpoint" Shared Library Code Part              |
// x=================================================================x


  PyTypeObjectDefinitions(Breakpoint)


# endif  // End of Shared Library Code Part.


}  // End of extern "C".


}  // End of Isobar namespace.
 
