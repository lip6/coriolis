
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
// |  C++ Header  :  "./crlcore/PyDefImport.h"                       |
// +-----------------------------------------------------------------+


#ifndef CRL_PY_DEF_IMPORT_H
#define CRL_PY_DEF_IMPORT_H

#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/DefImport.h"


namespace  CRL {

extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyDefImport".

  typedef struct {
      PyObject_HEAD
  } PyDefImport;


// -------------------------------------------------------------------
// Functions & Types exported to "PyCRL.ccp".

  extern  PyTypeObject  PyTypeDefImport;
  extern  PyMethodDef   PyDefImport_Methods[];

  extern  void          PyDefImport_LinkPyType();


#define IsPyDefImport(v)    ( (v)->ob_type == &PyTypeDefImport )
#define PY_DEFIMPORT(v)     ( (PyDefImport*)(v) )


}  // extern "C".

}  // CRL namespace.

#endif  // CRL_PY_DEF_IMPORT_H
