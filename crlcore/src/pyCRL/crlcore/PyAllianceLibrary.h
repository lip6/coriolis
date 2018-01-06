// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./crlcore/PyAllianceLibrary.h"                 |
// +-----------------------------------------------------------------+


#ifndef CRL_PY_ALLIANCE_LIBRARY_H
#define CRL_PY_ALLIANCE_LIBRARY_H

#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/AllianceLibrary.h"


namespace  CRL {


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyAllianceLibrary".

  typedef struct {
      PyObject_HEAD
      AllianceLibrary* _object;
  } PyAllianceLibrary;


// -------------------------------------------------------------------
// Functions & Types exported to "PyCRL.ccp".

  extern  PyTypeObject  PyTypeAllianceLibrary;
  extern  PyMethodDef   PyAllianceLibrary_Methods[];

  extern  PyObject* PyAllianceLibrary_Link           ( AllianceLibrary* );
  extern  void      PyAllianceLibrary_LinkPyType     ();


#define IsPyAllianceLibrary(v)    ( (v)->ob_type == &PyTypeAllianceLibrary )
#define PYALLIANCE_LIBRARY(v)     ( (PyAllianceLibrary*)(v) )
#define PYALLIANCE_LIBRARY_O(v)   ( PYALLIANCE_LIBRARY(v)->_object )


}  // extern "C".

}  // Hurricane namespace.


#endif  // __CRL_PY_ALLIANCE_LIBRARY__
