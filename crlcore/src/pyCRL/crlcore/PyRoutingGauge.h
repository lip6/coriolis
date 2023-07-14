
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2012-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |          Alliance / Hurricane  Interface                        |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./crlcore/PyRoutingGauge.h"                    |
// +-----------------------------------------------------------------+


#pragma  once
#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/RoutingGauge.h"


namespace  CRL {

  class RoutingLayerGauge;


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyRoutingGauge".

  typedef struct {
      PyObject_HEAD
      RoutingGauge* _object;
  } PyRoutingGauge;


// -------------------------------------------------------------------
// Functions & Types exported to "PyCRL.ccp".

  extern  PyTypeObject  PyTypeRoutingGauge;
  extern  PyMethodDef   PyRoutingGauge_Methods[];

  extern  PyObject* PyRoutingGauge_Link           ( RoutingGauge* );
  extern  void      PyRoutingGauge_LinkPyType     ();
  extern  void      PyRoutingGauge_postModuleInit ();


#define IsPyRoutingGauge(v)   ( (v)->ob_type == &PyTypeRoutingGauge )
#define PYROUTINGGAUGE(v)     ( (PyRoutingGauge*)(v) )
#define PYROUTINGGAUGE_O(v)   ( PYROUTINGGAUGE(v)->_object )


// Vector of DrawingGroups.
declareVectorObject(RoutingLayerGauge);


}  // extern "C".


}  // Hurricane namespace.
