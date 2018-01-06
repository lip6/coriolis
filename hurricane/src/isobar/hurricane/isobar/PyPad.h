// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2006-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/isobar/PyPad.h"                    |
// +-----------------------------------------------------------------+


#ifndef PY_PAD_H
#define PY_PAD_H

#include "hurricane/isobar/PyComponent.h"
#include "hurricane/Pad.h"


namespace  Isobar {

  extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyPad".

    typedef struct {
        PyComponent  _baseObject;
    } PyPad;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

    extern  PyTypeObject  PyTypePad;
    extern  PyMethodDef   PyPad_Methods[];

    extern  PyObject* PyPad_Link       ( Hurricane::Pad* object );
    extern  void      PyPad_LinkPyType ();


#define IsPyPad(v)    ( (v)->ob_type == &PyTypePad )
#define PYPAD(v)      ( (PyPad*)(v) )
#define PYPAD_O(v)    ( PYPAD(v)->_baseObject._baseObject._object )


  }  // extern "C".

}  // Isobar namespace.

#endif // PY_PAD_H
