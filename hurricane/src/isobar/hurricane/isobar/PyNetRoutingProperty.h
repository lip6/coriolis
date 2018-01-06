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
// |  C++ Header  :  "./hurricane/isobar/PyNetRoutingProperty.h"     |
// +-----------------------------------------------------------------+


#ifndef PY_NET_ROUTING_PROPERTY_H
#define PY_NET_ROUTING_PROPERTY_H


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/NetRoutingProperty.h"


namespace  Isobar {

  extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyNetRoutingExtension".

    typedef struct {
        PyObject_HEAD
        Hurricane::NetRoutingExtension* _object;
    } PyNetRoutingExtension;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

    extern PyTypeObject  PyTypeNetRoutingExtension;
    extern PyMethodDef   PyNetRoutingExtension_Methods[];

    extern PyObject* PyNetRoutingExtension_Link           ( Hurricane::NetRoutingExtension* );
    extern void      PyNetRoutingExtension_LinkPyType     ();
    extern void      PyNetRoutingExtension_postModuleInit ();


# define IsPyNetRoutingExtension(v)   ( (v)->ob_type == &PyTypeNetRoutingExtension )
# define PYNETROUTINGEXTENSION(v)     ( (PyNetRoutingExtension*)(v) )
# define PYNETROUTINGEXTENSION_O(v)   ( PYNETROUTINGEXTENSION(v)->_object )


  }  // extern "C".

}  // Isobar namespace.
 
#endif  // PY_NET_ROUTING_PROPERTY_H
