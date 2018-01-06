// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2008-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Module  :       "./PyLibrary.cpp"                          |
// +-----------------------------------------------------------------+


#ifndef  PY_LIBRARY_H
#define  PY_LIBRARY_H

#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/Library.h"


namespace  Isobar {

  using namespace Hurricane;


  extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyLibrary".

    typedef struct {
        PyObject_HEAD
        Library* _object;
    } PyLibrary;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

    extern  PyTypeObject  PyTypeLibrary;
    extern  PyMethodDef   PyLibrary_Methods[];

    extern  PyObject* PyLibrary_Link       ( Hurricane::Library* lib );
    extern  void      PyLibrary_LinkPyType ();


#define IsPyLibrary(v) ( (v)->ob_type == &PyTypeLibrary )
#define PYLIBRARY(v)   ( (PyLibrary*)(v) )
#define PYLIBRARY_O(v) ( PYLIBRARY(v)->_object )


}  // extern "C".

}  // Isobar namespace.
 
# endif  // PY_LIBRARY_H
