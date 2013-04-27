
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2013-2013, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/isobar/PyHook.h"                   |
// +-----------------------------------------------------------------+


#ifndef ISOBAR_PYHOOK_H
#define ISOBAR_PYHOOK_H

#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/Hook.h"

namespace  Isobar {

using namespace Hurricane;

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyHook".

  typedef struct {
      PyObject_HEAD
      Hook* _object;
  } PyHook;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

  extern  PyTypeObject  PyTypeHook;
  extern  PyMethodDef   PyHook_Methods[];

  extern  void      PyHook_LinkPyType     ();

#define IsPyHook(v)    ( (v)->ob_type == &PyTypeHook )
#define PYHOOK(v)      ( (PyHook*)(v) )
#define PYHOOK_O(v)    ( PYHOOK(v)->_object )


}  // extern "C".

}  // Isobar namespace.
 
#endif
