
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
// |  C++ Header  :  "./crlcore/PyAcmSigda.h"                        |
// +-----------------------------------------------------------------+


#ifndef __CRL_PY_ACMSIGDA__
#define __CRL_PY_ACMSIGDA__

#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/AcmSigda.h"


namespace  CRL {

extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyAcmSigda".

  typedef struct {
      PyObject_HEAD
  } PyAcmSigda;


// -------------------------------------------------------------------
// Functions & Types exported to "PyCRL.ccp".

  extern  PyTypeObject  PyTypeAcmSigda;
  extern  PyMethodDef   PyAcmSigda_Methods[];

  extern  void          PyAcmSigda_LinkPyType();


#define IsPyAcmSigda(v)     ( (v)->ob_type == &PyTypeAcmSigda )
#define PY_ACM_SIGDA(v)     ( (PyAcmSigda*)(v) )


}  // extern "C".

}  // Hurricane namespace.

#endif  // __CRL_PY_ACMSIGDA__
