
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2010-2010, All Rights Reserved
//
// ===================================================================
//
// $Id$
//
// x-----------------------------------------------------------------x 
// |                                                                 |
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :       "./PyCatalog.cpp"                          |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef __PY_CRL_CATALOG__
#define __PY_CRL_CATALOG__

#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/Catalog.h"


namespace  CRL {


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyCatalog".

  typedef struct {
      PyObject_HEAD
      CRL::Catalog* _object;
  } PyCatalog;


// -------------------------------------------------------------------
// Functions & Types exported to "PyCRL.ccp".

  extern  PyTypeObject  PyTypeCatalog;
  extern  PyMethodDef   PyCatalog_Methods[];

  extern  PyObject* PyCatalog_Link           ( CRL::Catalog* );
  extern  void      PyCatalog_LinkPyType     ();
  extern  void      PyCatalog_postModuleInit ();


#define IsPyCatalog(v)  ( (v)->ob_type == &PyTypeCatalog )
#define PYCATALOG(v)    ( (PyCatalog*)(v) )
#define PYCATALOG_O(v)  ( PYCATALOG(v)->_object )


}  // End of extern "C".


}  // End of CRL namespace.


#endif  // __PY_CRL_CATALOG__
