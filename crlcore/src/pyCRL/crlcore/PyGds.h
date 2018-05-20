
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2018-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./crlcore/PyGds.h"                            |
// +-----------------------------------------------------------------+


#ifndef CRL_PY_GDS_H
#define CRL_PY_GDS_H

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

  extern  void          PyGds_LinkPyType();


#define IsPyGds(v)    ( (v)->ob_type == &PyTypeGds )
#define PY_GDS(v)     ( (PyGds*)(v) )


}  // extern "C".

}  // Hurricane namespace.

#endif  // CRL_PY_GDS_H
