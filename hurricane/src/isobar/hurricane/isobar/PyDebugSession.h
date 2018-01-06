// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2014-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./PyDebugSession.h"                       |
// +-----------------------------------------------------------------+


#ifndef  ISOBAR_PY_DEBUG_SESSION_H
#define  ISOBAR_PY_DEBUG_SESSION_H

#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/DebugSession.h"


namespace  Isobar {

  extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyDebugSession".

    typedef struct {
        PyObject_HEAD
    } PyDebugSession;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

    extern PyTypeObject  PyTypeDebugSession;
    extern PyMethodDef   PyDebugSession_Methods[];

    extern void  PyDebugSession_LinkPyType  ();


#define IsPyDebugSession(v)   ( (v)->ob_type == &PyTypeDebugSession )
#define PYDEBUGSESSION(v)     ( (PyDebugSession*)(v) )
#define PYDEBUGSESSION_O(v)   ( PY_UPDATE_SESSION(v)->_object )


  }  // extern "C".

}  // Isobar namespace.

# endif
