// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2014-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/isobar/PyPlacementStatus.h"        |
// +-----------------------------------------------------------------+


#ifndef PY_PLACEMENTSTATUS_H
#define PY_PLACEMENTSTATUS_H

#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/Instance.h"


namespace  Isobar {

  extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyPlacementStatus".

    typedef struct {
        PyObject_HEAD
        Hurricane::Instance::PlacementStatus* _object;
    } PyPlacementStatus;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

    extern PyTypeObject  PyTypePlacementStatus;
    extern PyMethodDef   PyPlacementStatus_Methods[];

    extern PyObject*                             PyPlacementStatus_Link           ( Hurricane::Instance::PlacementStatus* );
    extern void                                  PyPlacementStatus_LinkPyType     ();
    extern void                                  PyPlacementStatus_postModuleInit ();


#define IsPyPlacementStatus(v)  ( (v)->ob_type == &PyTypePlacementStatus )
#define PYPLACEMENTSTATUS(v)    ( (PyPlacementStatus*)(v) )
#define PYPLACEMENTSTATUS_O(v)  ( PYPLACEMENTSTATUS(v)->_object )


  }  // End of extern "C".


  extern Hurricane::Instance::PlacementStatus  PyInt_AsPlacementStatus          ( PyObject* );


}  // End of Isobar namespace.

#endif  // PY_PLACEMENTSTATUS_H
