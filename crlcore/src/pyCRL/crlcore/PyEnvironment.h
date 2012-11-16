
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
// |  C++ Header  :  "./crlcore/PyEnvironment.h"                     |
// +-----------------------------------------------------------------+


#ifndef __CRL_PY_ENVIRONMENT__
#define __CRL_PY_ENVIRONMENT__

#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/Environment.h"


namespace  CRL {


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyEnvironment".

  typedef struct {
      PyObject_HEAD
      Environment* _object;
  } PyEnvironment;


// -------------------------------------------------------------------
// Functions & Types exported to "PyCRL.ccp".

  extern  PyTypeObject  PyTypeEnvironment;
  extern  PyMethodDef   PyEnvironment_Methods[];

  extern  PyObject* PyEnvironment_Link           ( Environment* );
  extern  void      PyEnvironment_LinkPyType     ();
  extern  void      PyEnvironment_postModuleInit ();


#define IsPyEnvironment(v)   ( (v)->ob_type == &PyTypeEnvironment )
#define PYENVIRONMENT(v)     ( (PyEnvironment*)(v) )
#define PYENVIRONMENT_O(v)   ( PYENVIRONMENT(v)->_object )


}  // extern "C".


}  // Hurricane namespace.


#endif  // __CRL_PY_ENVIRONMENT__
