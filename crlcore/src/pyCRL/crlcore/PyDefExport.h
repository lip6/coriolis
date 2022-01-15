
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2017-2018, All Rights Reserved
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


#ifndef CRL_PY_DEF_EXPORT_H
#define CRL_PY_DEF_EXPORT_H

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

  extern  void          PyDefExport_LinkPyType();


#define IsPyDefExport(v)    ( (v)->ob_type == &PyTypeDefExport )
#define PY_DEFEXPORT(v)     ( (PyDefExport*)(v) )


}  // extern "C".

}  // CRL namespace.

#endif  // CRL_PY_DEF_EXPORT_H
