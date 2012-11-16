
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
// |  C++ Header  :  "./crlcore/PyRoutingLayerGauge.h"               |
// +-----------------------------------------------------------------+


#ifndef __CRL_PY_ROUTINGLAYERGAUGE__
#define __CRL_PY_ROUTINGLAYERGAUGE__

#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/RoutingLayerGauge.h"


namespace  CRL {


extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyRoutingLayerGauge".

  typedef struct {
      PyObject_HEAD
      RoutingLayerGauge* _object;
  } PyRoutingLayerGauge;


// -------------------------------------------------------------------
// Functions & Types exported to "PyCRL.ccp".

  extern  PyTypeObject  PyTypeRoutingLayerGauge;
  extern  PyMethodDef   PyRoutingLayerGauge_Methods[];

  extern  PyObject* PyRoutingLayerGauge_Link           ( RoutingLayerGauge* );
  extern  void      PyRoutingLayerGauge_LinkPyType     ();
  extern  void      PyRoutingLayerGauge_postModuleInit ();


#define IsPyRoutingLayerGauge(v)   ( (v)->ob_type == &PyTypeRoutingLayerGauge )
#define PYROUTINGLAYERGAUGE(v)     ( (PyRoutingLayerGauge*)(v) )
#define PYROUTINGLAYERGAUGE_O(v)   ( PYROUTINGLAYERGAUGE(v)->_object )


}  // extern "C".


}  // Hurricane namespace.


#endif  // __CRL_PY_ROUTINGLAYERGAUGE__
