
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2008-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                       Damien DUPUIS              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./crlcore/PyToolEngine.h"                      |
// +-----------------------------------------------------------------+


#ifndef __CRL_PY_TOOLENGINE_COLLECTION__
#define __CRL_PY_TOOLENGINE_COLLECTION__

#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/ToolEngine.h"
#include "crlcore/ToolEngines.h"


namespace CRL {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyToolEngineCollection".

  typedef struct {
      PyObject_HEAD
      CRL::ToolEngines* _object;
  } PyToolEngineCollection;


  typedef struct {
      PyObject_HEAD
      Hurricane::Locator<CRL::ToolEngine*>* _object;
      PyToolEngineCollection* _collection;
  } PyToolEngineCollectionLocator;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

  extern PyTypeObject PyTypeToolEngineCollection;
  extern PyTypeObject PyTypeToolEngineCollectionLocator;

  extern void PyToolEngineCollection_LinkPyType();


}  // extern "C".

}  // CRL namespace.
 
#endif
