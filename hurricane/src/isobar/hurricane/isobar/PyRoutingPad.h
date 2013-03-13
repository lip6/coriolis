
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2013-2013, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                   Jean-Paul Chaput               |
// |  E-mail      :           Jean-Paul.Chaput@lip6.fr               |
// | =============================================================== |
// |  C++ Header  :       "./isobar/PyRoutingPad.h"                  |
// +-----------------------------------------------------------------+


#ifndef ISOBAR_PYROUTINGPAD_H
#define ISOBAR_PYROUTINGPAD_H

#include "hurricane/isobar/PyComponent.h"
#include "hurricane/RoutingPad.h"


namespace  Isobar {

extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyRoutingPad".

  typedef struct {
      PyComponent  _baseObject;
  } PyRoutingPad;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

  extern  PyTypeObject  PyTypeRoutingPad;
  extern  PyMethodDef   PyRoutingPad_Methods[];

  extern  PyObject* PyRoutingPad_Link           ( Hurricane::RoutingPad* object );
  extern  void      PyRoutingPad_LinkPyType     ();
  extern  void      PyRoutingPad_postModuleInit ();


#define IsPyRoutingPad(v)    ( (v)->ob_type == &PyTypeRoutingPad )
#define PYROUTINGPAD(v)      ( (PyRoutingPad*)(v) )
#define PYROUTINGPAD_O(v)    ( PYROUTINGPAD(v)->_baseObject._baseObject._object )


}  // extern "C".

}  // Isobar namespace.

#endif  // ISOBAR_ROUTINGPAD_H
