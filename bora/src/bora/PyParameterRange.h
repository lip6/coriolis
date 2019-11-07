// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2016-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |  B o r a  -  A n a l o g   S l i c i n g   T r e e              |
// |                                                                 |
// |  Authors     :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./bora/PyParameterRange.h"                     |
// +-----------------------------------------------------------------+


#ifndef BORA_PY_PARAMETER_RANGE_H
#define BORA_PY_PARAMETER_RANGE_H

#include "hurricane/isobar/PyHurricane.h"
#include "bora/ParameterRange.h"


namespace  Bora {

  extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyParameterRange".

    typedef struct {
        PyObject_HEAD
        ParameterRange* _object;
    } PyParameterRange;


// -------------------------------------------------------------------
// Functions & Types exported to "PyBora.ccp".

    extern  PyObject* PyParameterRange_NEW            ( ParameterRange* range );
    extern  void      PyParameterRange_LinkPyType     ();
    extern  void      PyParameterRange_postModuleInit ();

    extern  PyTypeObject  PyTypeParameterRange;
    extern  PyMethodDef   PyParameterRange_Methods[];


#define IsPyParameterRange(v)   ( (v)->ob_type == &PyTypeParameterRange )
#define PYPARAMETERRANGE(v)     ( (PyParameterRange*)(v) )
#define PYPARAMETERRANGE_O(v)   ( PYPARAMETERRANGE(v)->_object )


  }  // extern "C".


  ParameterRange*  ParameterRangeCast ( PyObject* derivedObject );


}  // Bora namespace.

#endif  // BORA_PY_PARAMETER_RANGE_H
