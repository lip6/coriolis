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
// |  C++ Header  :  "./hurricane/configuration/PyPoint.h"           |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/configuration/PyTypeManager.h"
#include "hurricane/Point.h"


namespace  Isobar3 {


extern "C" {


  extern PyMethodDef   PyPoint_Methods[];
  extern PyObject*     PyPoint_NEW ( PyTypeObject* pyType, PyObject* args, PyObject* kwargs );
  extern int           PyPoint_Init ( PyObject* self, PyObject* args, PyObject* kwargs );


}  // extern "C".

}  // Isobar3 namespace.
