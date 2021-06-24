
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU 2021-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./crlcore/PySpice.h"                           |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/Spice.h"


namespace  CRL {

extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PySpice".

  typedef struct {
      PyObject_HEAD
  } PySpice;


// -------------------------------------------------------------------
// Functions & Types exported to "PyCRL.ccp".

  extern  PyTypeObject  PyTypeSpice;
  extern  PyMethodDef   PySpice_Methods[];

  extern  void          PySpice_LinkPyType     ();
  extern  void          PySpice_postModuleInit ();


#define IsPySpice(v)    ( (v)->ob_type == &PyTypeSpice )
#define PY_SPICE(v)     ( (PySpice*)(v) )


}  // extern "C".

}  // Hurricane namespace.
