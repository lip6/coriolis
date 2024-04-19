
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2015-2024, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Roselyne CHOTIN               |
// |  E-mail      :            roselyne.chotin@lip6.fr               |
// | =============================================================== |
// |  C++ Header  :  "./crlcore/PyMeasures.h"                        |
// +-----------------------------------------------------------------+


#ifndef CRL_PY_MEASURES_H
#define CRL_PY_MEASURES_H

#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/Measures.h"

namespace  CRL {

extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyMeasures".

  typedef struct {
      PyObject_HEAD
  } PyMeasures;


// -------------------------------------------------------------------
// Functions & Types exported to "PyCRL.ccp".

  extern  PyTypeObject  PyTypeMeasures;
  extern  PyMethodDef   PyMeasures_Methods[];

  extern  void          PyMeasures_LinkPyType();


#define IsPyMeasures(v)    ( (v)->ob_type == &PyTypeMeasures )
#define PY_MEASURES(v)     ( (PyMeasures*)(v) )


}  // extern "C".

}  // Hurricane namespace.

#endif  // CRL_PY_MEASURES_H
