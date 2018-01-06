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
// |  C++ Header  :  "./hurricane/isobar/PyPinPlacementStatus.h"     |
// +-----------------------------------------------------------------+


#ifndef PY_PIN_PLACEMENTSTATUS_H
#define PY_PIN_PLACEMENTSTATUS_H


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/Pin.h"


namespace  Isobar {

  extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyPinPlacementStatus".

    typedef struct {
        PyObject_HEAD
        Hurricane::Pin::PlacementStatus* _object;
    } PyPinPlacementStatus;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

    extern PyTypeObject  PyTypePinPlacementStatus;
    extern PyMethodDef   PyPinPlacementStatus_Methods[];

    extern PyObject* PyPinPlacementStatus_Link           ( Hurricane::Pin::PlacementStatus* );
    extern void      PyPinPlacementStatus_LinkPyType ();
    extern void      PyPinPlacementStatus_postModuleInit ();


# define IsPyPinPlacementStatus(v)    ( (v)->ob_type == &PyTypePinPlacementStatus )
# define PYPINPLACEMENTSTATUS(v)      ( (PyPinPlacementStatus*)(v) )
# define PYPINPLACEMENTSTATUS_O(v)    ( PYPINPLACEMENTSTATUS(v)->_object )


  }  // extern "C".

}  // Isobar namespace.
 
#endif  // PY_PIN_PLACEMENTSTATUS_H
