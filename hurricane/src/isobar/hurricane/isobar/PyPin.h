// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2006-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                       Damien DUPUIS              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/isobar/PyPin.h"                    |
// +-----------------------------------------------------------------+


#ifndef PY_PIN_H
#define PY_PIN_H

#include "hurricane/isobar/PyContact.h"
#include "hurricane/Pin.h"


namespace  Isobar {
    
  extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyPin".

    typedef struct {
        PyContact _baseObject;
    } PyPin;

  
// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

    extern  PyTypeObject  PyTypePin;
    extern  PyMethodDef   PyPin_Methods[];

    extern  PyObject* PyPin_Link           (Hurricane::Pin* object );
    extern  void      PyPin_LinkPyType     ();
    extern  void      PyPin_postModuleInit ();


# define IsPyPin(v)    ( (v)->ob_type == &PyTypePin )
# define PYPIN(v)      ( (PyPin*)(v) )
# define PYPIN_O(v)    ( PYPIN(v)->_baseObject._baseObject._baseObject._object )


  }  // extern "C".

}  // Isobar namespace.

#endif  // PY_PIN_H
