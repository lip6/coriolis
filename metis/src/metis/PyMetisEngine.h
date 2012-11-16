
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2012-2012, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |         M E T I S  -  Wrapper around hMetis                     |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./metis/PyMetisEngine.h"                       |
// +-----------------------------------------------------------------+


#ifndef __PY_METIS_ENGINE__
#define __PY_METIS_ENGINE__

#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/PyToolEngine.h"
#include "metis/MetisEngine.h"


namespace  Metis {

extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PyMetisEngine".

  typedef struct {
      CRL::PyToolEngine  _baseObject;
  } PyMetisEngine;


// -------------------------------------------------------------------
// Functions & Types exported to "PyMetis.ccp".

  extern  PyTypeObject  PyTypeMetisEngine;
  extern  PyMethodDef   PyMetisEngine_Methods[];

  extern  PyObject* PyMetisEngine_Link       ( Metis::MetisEngine* );
  extern  void      PyMetisEngine_LinkPyType ();


#define IsPyMetisEngine(v)    ( (v)->ob_type == &PyTypeMetisEngine )
#define PY_METIS_ENGINE(v)    ( (PyMetisEngine*)(v) )
#define PY_METIS_ENGINE_O(v)  ( PY_METIS_ENGINE(v)->_baseObject._object )


}  // extern "C".

}  // Metis namespace.

#endif  // __PY_METIS_ENGINE__
