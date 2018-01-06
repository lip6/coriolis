// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2014-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/isobar/PyNetDirection.h"           |
// +-----------------------------------------------------------------+


#ifndef PY_NET_DIRECTION_H
#define PY_NET_DIRECTION_H


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/Net.h"


namespace  Isobar {

  extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyNetDirection".

    typedef struct {
        PyObject_HEAD
        Hurricane::Net::Direction* _object;
    } PyNetDirection;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

    extern PyTypeObject  PyTypeNetDirection;
    extern PyMethodDef   PyNetDirection_Methods[];

    extern PyObject* PyNetDirection_Link           ( Hurricane::Net::Direction* );
    extern void      PyNetDirection_LinkPyType ();
    extern void      PyNetDirection_postModuleInit ();


# define IsPyNetDirection(v)    ( (v)->ob_type == &PyTypeNetDirection )
# define PYNETDIRECTION(v)      ( (PyNetDirection*)(v) )
# define PYNETDIRECTION_O(v)    ( PYNETDIRECTION(v)->_object )


  }  // extern "C".

}  // Isobar namespace.
 
#endif  // PY_NET_DIRECTION_H
