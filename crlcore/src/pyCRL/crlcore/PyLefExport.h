
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
// |  C++ Header  :  "./crlcore/PyLefExport.h"                       |
// +-----------------------------------------------------------------+


#ifndef CRL_PY_LEF_EXPORT_H
#define CRL_PY_LEF_EXPORT_H

#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/LefExport.h"


namespace  CRL {

extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyLefExport".

  typedef struct {
      PyObject_HEAD
  } PyLefExport;


// -------------------------------------------------------------------
// Functions & Types exported to "PyCRL.ccp".

  extern  PyTypeObject  PyTypeLefExport;
  extern  PyMethodDef   PyLefExport_Methods[];

  extern  void          PyLefExport_LinkPyType();


#define IsPyLefExport(v)    ( (v)->ob_type == &PyTypeLefExport )
#define PY_LEFEXPORT(v)     ( (PyLefExport*)(v) )


}  // extern "C".

}  // CRL namespace.

#endif  // CRL_PY_LEF_EXPORT_H
