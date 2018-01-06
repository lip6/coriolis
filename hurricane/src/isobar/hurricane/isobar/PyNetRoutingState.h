// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2017-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/isobar/PyNetRoutingState.h"        |
// +-----------------------------------------------------------------+


#ifndef PY_NET_ROUTING_STATE_H
#define PY_NET_ROUTING_STATE_H


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/NetRoutingProperty.h"


namespace  Isobar {

  extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyNetRoutingState".

    typedef struct {
        PyObject_HEAD
        Hurricane::NetRoutingState* _object;
    } PyNetRoutingState;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

    extern PyTypeObject  PyTypeNetRoutingState;
    extern PyMethodDef   PyNetRoutingState_Methods[];

    extern PyObject* PyNetRoutingState_Link           ( Hurricane::NetRoutingState* );
    extern void      PyNetRoutingState_LinkPyType     ();
    extern void      PyNetRoutingState_postModuleInit ();


# define IsPyNetRoutingState(v)   ( (v)->ob_type == &PyTypeNetRoutingState )
# define PYNETROUTINGSTATE(v)     ( (PyNetRoutingState*)(v) )
# define PYNETROUTINGSTATE_O(v)   ( PYNETROUTINGSTATE(v)->_object )


  }  // extern "C".

}  // Isobar namespace.
 
#endif  // PY_NET_ROUTING_STATE_H
