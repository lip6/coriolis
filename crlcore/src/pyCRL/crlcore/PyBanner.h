
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2012-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./crlcore/PyBanner.h"                          |
// +-----------------------------------------------------------------+


#ifndef __CRL_PY_BANNER__
#define __CRL_PY_BANNER__

#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/Banner.h"


namespace  CRL {


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyBanner".

  typedef struct {
      PyObject_HEAD
      Banner* _object;
  } PyBanner;


// -------------------------------------------------------------------
// Functions & Types exported to "PyCRL.ccp".

  extern  PyTypeObject  PyTypeBanner;
  extern  PyMethodDef   PyBanner_Methods[];

  extern  PyObject* PyBanner_Link       ( Banner* );
  extern  void      PyBanner_LinkPyType ();


#define IsPyBanner(v)   ( (v)->ob_type == &PyTypeBanner )
#define PY_BANNER(v)    ( (PyBanner*)(v) )
#define PY_BANNER_O(v)  ( PYBANNER(v)->_object )


}  // extern "C".

}  // Hurricane namespace.

#endif  // __CRL_PY_BANNER__
