// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |  B o r a  -  A n a l o g   S l i c i n g   T r e e              |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./bora/PyStepParameterRange.h"                 |
// +-----------------------------------------------------------------+


#ifndef  BORA_PY_STEP_PARAMETER_RANGE_H
#define  BORA_PY_STEP_PARAMETER_RANGE_H

#include "bora/PyParameterRange.h"


namespace  Bora {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyStepParameterRange".

  typedef struct {
      PyParameterRange  _baseObject;
  } PyStepParameterRange;


// -------------------------------------------------------------------
// Functions & Types exported to "PyBora.cpp".

  extern PyTypeObject  PyTypeStepParameterRange;
  extern PyMethodDef   PyStepParameterRange_Methods[];

  extern PyObject* PyStepParameterRange_Link       ( StepParameterRange* node );
  extern void      PyStepParameterRange_LinkPyType ();


#define IsPyStepParameterRange(v)    ( (v)->ob_type == &PyTypeStepParameterRange )
#define PYSTEPPARAMETERRANGE(v)      ( (PyStepParameterRange*)(v) )
#define PYSTEPPARAMETERRANGE_O(v)    ( dynamic_cast<StepParameterRange*>(PYSTEPPARAMETERRANGE(v)->_baseObject._object) )


}  // extern "C".

}  // Bora namespace.

#endif  // BORA_PY_STEP_PARAMETER_RANGE_H
