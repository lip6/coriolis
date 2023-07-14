// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2022-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |              F o e h n  -  DAG Toolbox                          |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./foehn/PyDagExtension.h"                 |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/isobar/PyHurricane.h"


namespace  Foehn {

  extern "C" {


    typedef struct {
        PyObject_HEAD
    } PyDagExtension;


    extern PyTypeObject  PyTypeDagExtension;
    extern PyMethodDef   PyDagExtension_Methods[];
    extern void          PyDagExtension_LinkPyType ();
    extern void          PyDagExtension_postModuleInit ();


#define IsPyDagExtension(v)   ( (v)->ob_type == &PyTypeDagExtension )
#define PYDAGEXTENSION(v)     ( (PyDagExtension*)(v) )


  }  // extern "C".

}  // Foehn namespace.
