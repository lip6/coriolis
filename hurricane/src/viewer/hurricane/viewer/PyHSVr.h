
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
// |  C++ Header  :  "./hurricane/viewer/PyHSVr.h"                   |
// +-----------------------------------------------------------------+


#ifndef __VIEWER_PY_HSVR__
#define __VIEWER_PY_HSVR__

#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/viewer/DisplayStyle.h"


namespace  Hurricane {


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyHSVr".

  typedef struct {
      PyObject_HEAD
      DisplayStyle::HSVr* _object;
  } PyHSVr;


// -------------------------------------------------------------------
// Functions & Types exported to "PyViewer.ccp".

  extern  PyTypeObject  PyTypeHSVr;
  extern  PyMethodDef   PyHSVr_Methods[];

  extern  PyObject* PyHSVr_Link       ( DisplayStyle::HSVr* );
  extern  void      PyHSVr_LinkPyType ();


#define IsPyHSVr(v)  ( (v)->ob_type == &PyTypeHSVr )
#define PYHSVR(v)    ( (PyHSVr*)(v) )
#define PYHSVR_O(v)  ( PYHSVR(v)->_object )


}  // extern "C".


}  // Hurricane namespace.


#endif  // __VIEWER_PY_HSVR__
