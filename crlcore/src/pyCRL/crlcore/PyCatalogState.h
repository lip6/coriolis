
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
// |  C++ Header  :       "./PyCatalogState.cpp"                     |
// | *************************************************************** |
// |  U p d a t e s                                                  |
// |                                                                 |
// x-----------------------------------------------------------------x


#ifndef __PY_CRL_CATALOG_STATE__
#define __PY_CRL_CATALOG_STATE__

#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/Catalog.h"


namespace  CRL {


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyCatalogState".

  typedef struct {
      PyObject_HEAD
      CRL::Catalog::State* _object;
  } PyCatalogState;


// -------------------------------------------------------------------
// Functions & Types exported to "PyCRL.ccp".

  extern  PyTypeObject  PyTypeCatalogState;
  extern  PyMethodDef   PyCatalogState_Methods[];

  extern  PyObject* PyCatalogState_Link           ( CRL::Catalog::State* );
  extern  void      PyCatalogState_LinkPyType     ();
  extern  void      PyCatalogState_postModuleInit ();


#define IsPyCatalogState(v)  ( (v)->ob_type == &PyTypeCatalogState )
#define PYCATALOGSTATE(v)    ( (PyCatalogState*)(v) )
#define PYCATALOGSTATE_O(v)  ( PYCATALOGSTATE(v)->_object )


}  // End of extern "C".


}  // End of CRL namespace.


#endif  // __PY_CRL_CATALOG_STATE__
