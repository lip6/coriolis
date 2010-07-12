
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
// |  C++ Header  :       "./PyBreakpoint.h"                         |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x





# ifndef  __PY_BREAKPOINT__
#   define  __PY_BREAKPOINT__


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/Breakpoint.h"


namespace  Isobar {


extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyBreakpoint".

  typedef struct {
      PyObject_HEAD
  } PyBreakpoint;




// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.cpp".

  extern PyTypeObject  PyTypeBreakpoint;
  extern PyMethodDef   PyBreakpoint_Methods[];

  extern void  PyBreakpoint_LinkPyType  ();


#define IsPyBreakpoint(v)   ( (v)->ob_type == &PyTypeBreakpoint )
#define PYBREAKPOINT(v)     ( (PyBreakpoint*)(v) )
#define PYBREAKPOINT_O(v)   ( PY_BREAKPOINT(v)->_object )


}  // End of extern "C".



}  // End of Isobar namespace.
 



# endif
