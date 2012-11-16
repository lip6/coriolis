
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
// |  C++ Header  :  "./crlcore/PyCellGauge.h"                       |
// +-----------------------------------------------------------------+


#ifndef __CRL_PY_CELLGAUGE__
#define __CRL_PY_CELLGAUGE__

#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/CellGauge.h"


namespace  CRL {


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyCellGauge".

  typedef struct {
      PyObject_HEAD
      CellGauge* _object;
  } PyCellGauge;


// -------------------------------------------------------------------
// Functions & Types exported to "PyCRL.ccp".

  extern  PyTypeObject  PyTypeCellGauge;
  extern  PyMethodDef   PyCellGauge_Methods[];

  extern  PyObject* PyCellGauge_Link           ( CellGauge* );
  extern  void      PyCellGauge_LinkPyType     ();
  extern  void      PyCellGauge_postModuleInit ();


#define IsPyCellGauge(v)   ( (v)->ob_type == &PyTypeCellGauge )
#define PYCELLGAUGE(v)     ( (PyCellGauge*)(v) )
#define PYCELLGAUGE_O(v)   ( PYCELLGAUGE(v)->_object )


}  // extern "C".


}  // Hurricane namespace.


#endif  // __CRL_PY_CELLGAUGE__
