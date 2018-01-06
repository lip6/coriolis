// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2010-2018, All Rights Reserved
//
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                       Damien DUPUIS              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/isobar/PyQuery.h"                  |
// +-----------------------------------------------------------------+


#ifndef  HURRICANE_PYQUERY_H
#define  HURRICANE_PYQUERY_H

#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/Query.h"

namespace  Isobar {

  using namespace Hurricane;


  extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyQuery".

    typedef struct {
        PyObject_HEAD
        Query* _object;
    } PyQuery;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

    extern  PyTypeObject  PyTypeQuery;
    extern  PyMethodDef   PyQuery_Methods[];

    extern  PyObject* PyQuery_Link           ( Query* object );
    extern  void      PyQuery_LinkPyType     ();
    extern  void      PyQuery_postModuleInit ();

#define IsPyQuery(v)    ( (v)->ob_type == &PyTypeQuery )
#define PYQUERY(v)      ( (PyQuery*)(v) )
#define PYQUERY_O(v)    ( PYQUERY(v)->_object )


  }  // extern "C".

}  // Isobar namespace.
 
#endif
