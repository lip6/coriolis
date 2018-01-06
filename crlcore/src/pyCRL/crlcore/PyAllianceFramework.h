// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2010-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./crlcore/PyAllianceFramework.h"               |
// +-----------------------------------------------------------------+


#ifndef __CRL_PY_ALLIANCE_FRAMEWORK__
#define __CRL_PY_ALLIANCE_FRAMEWORK__

#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/AllianceFramework.h"


namespace  CRL {


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyAllianceFramework".

  typedef struct {
      PyObject_HEAD
      CRL::AllianceFramework* _object;
  } PyAllianceFramework;


// -------------------------------------------------------------------
// Functions & Types exported to "PyCRL.ccp".

  extern  PyTypeObject  PyTypeAllianceFramework;
  extern  PyMethodDef   PyAllianceFramework_Methods[];

  extern  PyObject* PyAllianceFramework_Link           ( CRL::AllianceFramework* );
  extern  void      PyAllianceFramework_LinkPyType     ();
  extern  void      PyAllianceFramework_postModuleInit ();


#define IsPyAllianceFramework(v)  ( (v)->ob_type == &PyTypeAllianceFramework )
#define PYALLIANCEFRAMEWORK(v)    ( (PyAllianceFramework*)(v) )
#define PYALLIANCEFRAMEWORK_O(v)  ( PYALLIANCEFRAMEWORK(v)->_object )


}  // extern "C".


}  // CRL namespace.


#endif  // __CRL_PY_ALLIANCE_FRAMEWORK__
