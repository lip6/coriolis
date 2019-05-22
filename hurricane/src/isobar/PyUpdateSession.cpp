
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2010-2010, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyUpdateSession.cpp"                    |
// +-----------------------------------------------------------------+


#include "hurricane/isobar/PyUpdateSession.h"


namespace  Isobar {

using namespace Hurricane;

extern "C" {


#define  METHOD_HEAD(function)  GENERIC_METHOD_HEAD(UpdateSession,session,function)


// x=================================================================x
// |            "PyUpdateSession" Python Module Code Part            |
// x=================================================================x

#if defined(__PYTHON_MODULE__)


  static void PyUpdateSession_DeAlloc ( PyUpdateSession* self )
  {
    cdebug_log(20,0) << "PyUpdateSession_DeAlloc(" << hex << self << ")" << endl;
  }
  

  static PyObject* PyUpdateSession_open ( PyObject* )
  {
    cdebug_log(20,0) << "PyUpdateSession_open()" << endl;

    HTRY
    UpdateSession::open ();
    HCATCH

    Py_RETURN_NONE;
  }
  

  static PyObject* PyUpdateSession_close ( PyObject* )
  {
    cdebug_log(20,0) << "PyUpdateSession_close()" << endl;

    HTRY
    UpdateSession::close ();
    HCATCH

    Py_RETURN_NONE;
  }
  

  static PyObject* PyUpdateSession_getStackSize ( PyObject* )
  {
    cdebug_log(20,0) << "PyUpdateSession_getStackSize()" << endl;

    size_t stackSize = 0;
    HTRY
      stackSize = UpdateSession::getStackSize();
    HCATCH

    return Py_BuildValue ("I",stackSize);
  }


  PyMethodDef PyUpdateSession_Methods[] =
    { { "open"        , (PyCFunction)PyUpdateSession_open, METH_NOARGS|METH_CLASS
                      , "Opens a new Update Session." }
    , { "close"       , (PyCFunction)PyUpdateSession_close, METH_NOARGS|METH_CLASS
                      , "Closes an Update Session." }
    , { "getStackSize", (PyCFunction)PyUpdateSession_getStackSize, METH_NOARGS|METH_CLASS
                      , "Return the number of currently opened Update Sessions." }
    , {NULL, NULL, 0, NULL}  /* sentinel */
    };


  PyTypeObjectLinkPyTypeWithoutObject(UpdateSession,UpdateSession)


#else  // End of Python Module Code Part.


// x=================================================================x
// |           "PyUpdateSession" Shared Library Code Part            |
// x=================================================================x


  PyTypeObjectDefinitions(UpdateSession)


# endif  // End of Shared Library Code Part.


}  // End of extern "C".


}  // End of Isobar namespace.
 
