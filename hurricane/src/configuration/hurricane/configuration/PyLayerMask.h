// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2020-2021, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    C o n f i g u r a t i o n   D a t a - B a s e                |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/configuration/PyLayerMask.h"       |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/configuration/PyTypeManager.h"
#include "hurricane/Layer.h"


namespace  Isobar3 {


extern "C" {


  extern PyMethodDef      PyLayerMask_Methods[];
  extern PyNumberMethods  PyLayerMask_NumberMethods;
  extern PyObject*    PyLayerMask_NEW        ( PyTypeObject* pyType, PyObject* args, PyObject* kwargs );
  extern int          PyLayerMask_Init       ( PyObject* self, PyObject* args, PyObject* kwargs );
  extern PyObject*    tpRichCompareLayerMask ( PyObject *self, PyObject* other, int op );


}  // extern "C".

}  // Isobar3 namespace.
