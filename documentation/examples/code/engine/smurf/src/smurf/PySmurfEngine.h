
// -*- C++ -*-
//
// This file is part of the Coriolis Software.
// Copyright (c) UPMC/LIP6 2014-2018, All Rights Reserved
//
// +-----------------------------------------------------------------+ 
// |                   C O R I O L I S                               |
// |   S m u r f  -  A   D e m o   T o o l E n g i n e               |
// |                                                                 |
// |  Author      :                    Jean-Paul CHAPUT              |
// |  E-mail      :       Jean-Paul.Chaput@asim.lip6.fr              |
// | =============================================================== |
// |  C++ Header  :  "./smurf/PySmurfEngine.cpp"                     |
// +-----------------------------------------------------------------+


#ifndef PY_SMURF_ENGINE_H
#define PY_SMURF_ENGINE_H

#include "hurricane/isobar/PyHurricane.h"
#include "crlcore/PyToolEngine.h"
#include "smurf/SmurfEngine.h"


namespace  Smurf {

extern "C" {


// -------------------------------------------------------------------
// Python Object  :  "PySmurfEngine".

  typedef struct {
      CRL::PyToolEngine  _baseObject;
  } PySmurfEngine;


// -------------------------------------------------------------------
// Functions & Types exported to "PySmurf.ccp".

  extern  PyTypeObject  PyTypeSmurfEngine;
  extern  PyMethodDef   PySmurfEngine_Methods[];

  extern  PyObject* PySmurfEngine_Link       ( Smurf::SmurfEngine* );
  extern  void      PySmurfEngine_LinkPyType ();


#define IsPySmurfEngine(v)  ( (v)->ob_type == &PyTypeSmurfEngine )
#define PYSMURFENGINE(v)    ( (PySmurfEngine*)(v) )
#define PYSMURFENGINE_O(v)  ( PYSMURFENGINE(v)->_baseObject._object )


}  // extern "C".

}  // Smurf namespace.

#endif  // PY_SMURF_ENGINE_H
