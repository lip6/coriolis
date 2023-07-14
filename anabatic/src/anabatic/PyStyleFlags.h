// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2022-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |     A n a b a t i c  -  Global Routing Toolbox                  |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/isobar/PyStyleFlags.h"             |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/isobar/PyHurricane.h"
#include "anabatic/Constants.h"


namespace  Anabatic {

  extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyStyleFlags".

    typedef struct {
        PyObject_HEAD
        StyleFlags* _object;
    } PyStyleFlags;


    extern PyTypeObject  PyTypeStyleFlags;
    extern PyMethodDef   PyStyleFlags_Methods[];

    extern PyObject*     PyStyleFlags_Link           ( StyleFlags* );
    extern void          PyStyleFlags_LinkPyType     ();
    extern void          PyStyleFlags_postModuleInit ();

#define IsPyStyleFlags(v)  ( (v)->ob_type == &PyTypeStyleFlags )
#define PYSTYLEFLAGS(v)    ( (PyStyleFlags*)(v) )
#define PYSTYLEFLAGS_O(v)  ( PYSTYLEFLAGS(v)->_object )

  }  // extern "C".


  extern Anabatic::StyleFlags  PyInt_AsStyleFlags ( PyObject* );


}  // Anabatic namespace.
