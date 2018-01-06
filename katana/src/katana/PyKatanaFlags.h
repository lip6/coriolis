// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2017-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |      K i t e  -  D e t a i l e d   R o u t e r                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./katana/PyKatanaFlags.h"                      |
// +-----------------------------------------------------------------+


#ifndef PY_KATANA_FLAGS_H
#define PY_KATANA_FLAGS_H


#include "hurricane/isobar/PyHurricane.h"
#include "katana/Constants.h"


namespace  Katana {

  extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyTransformation".

    typedef struct {
        PyObject_HEAD
        Katana::Flags* _object;
    } PyKatanaFlags;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

    extern PyTypeObject  PyTypeKatanaFlags;
    extern PyMethodDef   PyKatanaFlags_Methods[];

    extern PyObject* PyKatanaFlags_Link           ( Katana::Flags* );
    extern void      PyKatanaFlags_LinkPyType ();
    extern void      PyKatanaFlags_postModuleInit ();


# define IsPyKatanaFlags(v)     ( (v)->ob_type == &PyTypeKatanaFlags )
# define PYKATANAFLAGS(v)       ( (PyKatanaFlags*)(v) )
# define PYKATANAFLAGS_O(v)     ( PYKATANAFLAGS(v)->_object )


  }  // extern "C".

}  // Katana namespace.
 
#endif  // PY_KATANA_FLAGS_H
