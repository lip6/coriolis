
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2013-2013, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul Chaput              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/isobar/PyHookCollection.h"         |
// +-----------------------------------------------------------------+


#ifndef ISOBAR_PYSEGMENT_COLLECTION_H
#define ISOBAR_PYSEGMENT_COLLECTION_H

#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/Hook.h"
#include "hurricane/Hooks.h"


namespace Isobar {

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyHookCollection".

  typedef struct {
      PyObject_HEAD
      Hurricane::Hooks* _object;
  } PyHookCollection;

  typedef struct {
      PyObject_HEAD
      Hurricane::Locator<Hurricane::Hook*>* _object;
      PyHookCollection* _collection;
  } PyHookCollectionLocator;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

  extern PyTypeObject PyTypeHookCollection;
  extern PyTypeObject PyTypeHookCollectionLocator;

  extern void PyHookCollection_LinkPyType();
  extern void PyHookCollectionLocator_LinkPyType();

}  // extern "C".

}  // Isobar namespace.
 
#endif
