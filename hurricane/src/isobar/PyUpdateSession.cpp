
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
// |  C++ Module  :       "./PyUpdateSession.cpp"                    |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


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


  PyMethodDef PyUpdateSession_Methods[] =
    { { "open"  , (PyCFunction)PyUpdateSession_open, METH_NOARGS|METH_CLASS
                , "Opens a new Update Session." }
    , { "close" , (PyCFunction)PyUpdateSession_close, METH_NOARGS|METH_CLASS
                , "Closes an Update Session." }
    , {NULL, NULL, 0, NULL}           /* sentinel */
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
 
