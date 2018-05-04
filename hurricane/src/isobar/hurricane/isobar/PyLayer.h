// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC 2006-2018, All Rights Reserved
//
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |    I s o b a r  -  Hurricane / Python Interface                 |
// |                                                                 |
// |  Author      :                       Damien DUPUIS              |
// |  E-mail      :            Jean-Paul.Chaput@lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./hurricane/isobar/PyLayer.h"                  |
// +-----------------------------------------------------------------+


# ifndef  ISOBAR_PYLAYER_H
# define  ISOBAR_PYLAYER_H

#include "hurricane/isobar/PyHurricane.h"
#include "hurricane/Layer.h"

namespace  Isobar {

using namespace Hurricane;

extern "C" {

// -------------------------------------------------------------------
// Python Object  :  "PyLayer".

  typedef struct {
      PyObject_HEAD
      Layer* _object;
  } PyLayer;


// -------------------------------------------------------------------
// Functions & Types exported to "PyHurricane.ccp".

  extern  PyTypeObject  PyTypeLayer;
  extern  PyMethodDef   PyLayer_Methods[];

  extern  PyObject* PyLayer_Link           ( Hurricane::Layer* object );
  extern  PyObject* PyLayer_LinkDerived    ( Hurricane::Layer* object );
  extern  void      PyLayer_LinkPyType     ();
  extern  void      PyLayer_postModuleInit ();
  extern  Layer*    PYDERIVEDLAYER_O       ( PyObject* );
  extern  bool      IsPyDerivedLayer       ( PyObject* );

# define IsPyLayer(v)    ( (v)->ob_type == &PyTypeLayer )
# define PYLAYER(v)      ( (PyLayer*)(v) )
# define PYLAYER_O(v)    ( PYLAYER(v)->_object )


}  // extern "C".

}  // Isobar namespace.
 
#endif  // ISOBAR_PYLAYER_H
