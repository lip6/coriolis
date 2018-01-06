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
// |  C++ Header  :  "./hurricane/isobar/PyPinDirection.h"           |
// +-----------------------------------------------------------------+


#ifndef PY_PIN_DIRECTION_H
#define PY_PIN_DIRECTION_H


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/Pin.h"


namespace  Isobar {

  extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyPinDirection".

    typedef struct {
        PyObject_HEAD
        Hurricane::Pin::AccessDirection* _object;
    } PyPinDirection;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

    extern PyTypeObject  PyTypePinDirection;
    extern PyMethodDef   PyPinDirection_Methods[];

    extern PyObject* PyPinDirection_Link           ( Hurricane::Pin::AccessDirection* );
    extern void      PyPinDirection_LinkPyType     ();
    extern void      PyPinDirection_postModuleInit ();


# define IsPyPinDirection(v)    ( (v)->ob_type == &PyTypePinDirection )
# define PYPINDIRECTION(v)      ( (PyPinDirection*)(v) )
# define PYPINDIRECTION_O(v)    ( PYPINDIRECTION(v)->_object )


  }  // extern "C".

}  // Isobar namespace.
 
#endif  // PY_PIN_DIRECTION_H
