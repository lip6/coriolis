
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) SU/LIP6 2020-2020, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                   Jean-Paul Chaput               |
// |  E-mail      :           Jean-Paul.Chaput@lip6.fr               |
// | =============================================================== |
// |  C++ Header  :       "./isobar/PyRoutingPadCollection.h"        |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/RoutingPad.h"
#include "hurricane/RoutingPads.h"


namespace Isobar {

  extern "C" {


    typedef struct {
        PyObject_HEAD
        Hurricane::RoutingPads* _object;
    } PyRoutingPadCollection;

    typedef struct {
        PyObject_HEAD
        Hurricane::Locator<Hurricane::RoutingPad*>* _object;
        PyRoutingPadCollection*                     _collection;
    } PyRoutingPadCollectionLocator;


    extern PyTypeObject PyTypeRoutingPadCollection;
    extern PyTypeObject PyTypeRoutingPadCollectionLocator;

    extern void PyRoutingPadCollection_LinkPyType();
    extern void PyRoutingPadCollectionLocator_LinkPyType();


  }  // End of extern "C".

}  // Isobar namespace.
