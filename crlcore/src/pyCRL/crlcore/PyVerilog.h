
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU 2021-2023, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Verilog / Hurricane  Interface                         |
// |                                                                 |
// |  Authors     :          Jean-Paul CHAPUT, Serge Rabyking        |
// |  E-mail      :          Jean-Paul.Chaput@lip6.fr                |
// | =============================================================== |
// |  C++ Header  :  "./crlcore/PyVerilog.h"                         |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/Verilog.h"


namespace  CRL {

extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyVerilog".

  typedef struct {
      PyObject_HEAD
  } PyVerilog;


// -------------------------------------------------------------------
// Functions & Types exported to "PyCRL.ccp".

  extern  PyTypeObject  PyTypeVerilog;
  extern  PyMethodDef   PyVerilog_Methods[];

  extern  void          PyVerilog_LinkPyType     ();
  extern  void          PyVerilog_postModuleInit ();

#define IsPyVerilog(v)    ( (v)->ob_type == &PyTypeVerilog )
#define PY_VERILOG(v)     ( (PyVerilog*)(v) )

}  // extern "C".

}  // Hurricane namespace.
