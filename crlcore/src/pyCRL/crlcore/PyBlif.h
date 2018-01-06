
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./crlcore/PyBlif.h"                            |
// +-----------------------------------------------------------------+


#ifndef CRL_PY_BLIF_H
#define CRL_PY_BLIF_H

#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/Blif.h"


namespace  CRL {

extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyBlif".

  typedef struct {
      PyObject_HEAD
  } PyBlif;


// -------------------------------------------------------------------
// Functions & Types exported to "PyCRL.ccp".

  extern  PyTypeObject  PyTypeBlif;
  extern  PyMethodDef   PyBlif_Methods[];

  extern  void          PyBlif_LinkPyType();


#define IsPyBlif(v)    ( (v)->ob_type == &PyTypeBlif )
#define PY_BLIF(v)     ( (PyBlif*)(v) )


}  // extern "C".

}  // Hurricane namespace.

#endif  // CRL_PY_BLIF_H
