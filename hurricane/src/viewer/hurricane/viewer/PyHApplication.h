
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2012-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/viewer/PyHApplication.h"           |
// +-----------------------------------------------------------------+


#ifndef __VIEWER_PY_HAPPLICATION__
#define __VIEWER_PY_HAPPLICATION__

#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/viewer/HApplication.h"


namespace  Hurricane {

extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyHApplication".

  typedef struct {
      PyObject_HEAD
      HApplication* _object;
  } PyHApplication;


// -------------------------------------------------------------------
// Functions & Types exported to "PyViewer.ccp".

  extern  PyTypeObject  PyTypeHApplication;
  extern  PyMethodDef   PyHApplication_Methods[];

  extern  PyObject* PyHApplication_Link       ( HApplication* );
  extern  void      PyHApplication_LinkPyType ();


#define IsPyHApplication(v)   ( (v)->ob_type == &PyTypeHApplication )
#define PY_HAPPLICATION(v)    ( (PyHApplication*)(v) )
#define PY_HAPPLICATION_O(v)  ( PY_HAPPLICATION(v)->_object )


}  // extern "C".

}  // Hurricane namespace.

#endif  // __VIEWER_PY_HAPPLICATION__
