
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) Sorbonne Université 2017-2022, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./crlcore/PyDefExport.h"                       |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/DefExport.h"


namespace  CRL {

extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyDefExport".

  typedef struct {
      PyObject_HEAD
  } PyDefExport;


// -------------------------------------------------------------------
// Functions & Types exported to "PyCRL.ccp".

  extern  PyTypeObject  PyTypeDefExport;
  extern  PyMethodDef   PyDefExport_Methods[];

  extern  void          PyDefExport_LinkPyType ();
  extern  void          PyDefExport_postModuleInit ();


#define IsPyDefExport(v)    ( (v)->ob_type == &PyTypeDefExport )
#define PY_DEFEXPORT(v)     ( (PyDefExport*)(v) )


}  // extern "C".

}  // CRL namespace.
