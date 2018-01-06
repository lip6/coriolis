// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2012-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./crlcore/PyIspd05.h"                          |
// +-----------------------------------------------------------------+


#ifndef CRL_PY_ACMSIGDA_H
#define CRL_PY_ACMSIGDA_H

#include "hurricane/isobar/PyHurricane.h"


namespace  CRL {

extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyIspd05".

  typedef struct {
      PyObject_HEAD
  } PyIspd05;


// -------------------------------------------------------------------
// Functions & Types exported to "PyCRL.ccp".

  extern  PyTypeObject  PyTypeIspd05;
  extern  PyMethodDef   PyIspd05_Methods[];

  extern  void          PyIspd05_LinkPyType();


#define IsPyIspd05(v)     ( (v)->ob_type == &PyTypeIspd05 )
#define PY_ISPD05(v)      ( (PyIspd05*)(v) )


}  // extern "C".

}  // Hurricane namespace.

#endif  // CRL_PY_ACMSIGDA_H
