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
// |  C++ Header  :  "./hurricane/isobar/PyNetType.h"                |
// +-----------------------------------------------------------------+


#ifndef PY_NET_TYPE_H
#define PY_NET_TYPE_H


#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/Net.h"


namespace  Isobar {

  extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyTransformation".

    typedef struct {
        PyObject_HEAD
        Hurricane::Net::Type* _object;
    } PyNetType;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

    extern PyTypeObject  PyTypeNetType;
    extern PyMethodDef   PyNetType_Methods[];

    extern PyObject* PyNetType_Link           ( Hurricane::Net::Type* );
    extern void      PyNetType_LinkPyType ();
    extern void      PyNetType_postModuleInit ();


# define IsPyNetType(v)     ( (v)->ob_type == &PyTypeNetType )
# define PYNETTYPE(v)       ( (PyNetType*)(v) )
# define PYNETTYPE_O(v)     ( PYNETTYPE(v)->_object )


  }  // extern "C".

}  // Isobar namespace.
 
#endif  // PY_NET_TYPE_H
