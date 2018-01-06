
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
// |  C++ Header  :  "./crlcore/PyLefImport.h"                       |
// +-----------------------------------------------------------------+


#ifndef CRL_PY_LEF_IMPORT_H
#define CRL_PY_LEF_IMPORT_H

#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/LefImport.h"


namespace  CRL {

extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyLefImport".

  typedef struct {
      PyObject_HEAD
  } PyLefImport;


// -------------------------------------------------------------------
// Functions & Types exported to "PyCRL.ccp".

  extern  PyTypeObject  PyTypeLefImport;
  extern  PyMethodDef   PyLefImport_Methods[];

  extern  void          PyLefImport_LinkPyType();


#define IsPyLefImport(v)    ( (v)->ob_type == &PyTypeLefImport )
#define PY_LEFIMPORT(v)     ( (PyLefImport*)(v) )


}  // extern "C".

}  // CRL namespace.

#endif  // CRL_PY_LEF_IMPORT_H
