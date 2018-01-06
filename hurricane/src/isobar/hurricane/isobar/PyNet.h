// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2006-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                     Sophie BELLOEIL              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/isobar/PyNet.h"                    |
// +-----------------------------------------------------------------+


#ifndef PY_NET_H
#define PY_NET_H

#include "hurricane/isobar/PyEntity.h"
#include "hurricane/Net.h"


namespace  Isobar {

  extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyNet".

    typedef struct {
        PyEntity  _baseObject;
    } PyNet;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

    extern  PyTypeObject  PyTypeNet;
    extern  PyMethodDef   PyNet_Methods[];

    extern  PyObject* PyNet_Link           ( Hurricane::Net* object );
    extern  void      PyNet_LinkPyType     ();
    extern  void      PyNet_postModuleInit ();



#define IsPyNet(v) ((v)->ob_type == &PyTypeNet)
#define PYNET(v)   ((PyNet*)(v))
#define PYNET_O(v) (dynamic_cast<Net*>(PYNET(v)->_baseObject._object))


  }  // extern "C".

}  // Isobar namespace.

#endif  // PY_NET_H
