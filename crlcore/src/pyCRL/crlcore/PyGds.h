
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2018-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./crlcore/PyGds.h"                             |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/Gds.h"


namespace  CRL {

extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyGds".

  typedef struct {
      PyObject_HEAD
  } PyGds;


// -------------------------------------------------------------------
// Functions & Types exported to "PyCRL.ccp".

  extern  PyTypeObject  PyTypeGds;
  extern  PyMethodDef   PyGds_Methods[];

  extern  void          PyGds_LinkPyType     ();
  extern  void          PyGds_postModuleInit ();


#define IsPyGds(v)    ( (v)->ob_type == &PyTypeGds )
#define PY_GDS(v)     ( (PyGds*)(v) )


}  // extern "C".

}  // Hurricane namespace.
